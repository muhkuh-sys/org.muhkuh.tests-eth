#! /usr/bin/env python
# encoding: utf-8

from waflib import Task, Utils
from waflib.Errors import WafError
from waflib.Configure import conf
from waflib.TaskGen import feature, after_method, before_method, taskgen_method
from waflib.Context import STDOUT, BOTH
from waflib import Logs
from hilscher_netx import aboot_header_elements, aboot_magiccookie, aboot_signature
import hilscher_toolchains
import re
import os.path
from netx_image_generator.builder     import NxoBuilder, nxupdate_fn
from collections import namedtuple

def display_findings(bld):
    findings = bld.findings

    separatorline = '{0:=^80}'.format

    if findings:
        Logs.warn(separatorline(' Findings '))

        for node in sorted(findings, key= lambda x : x.nice_path()):
            title = '%s' % (node.nice_path())

            Logs.warn(title)
            Logs.warn('-' * len(title))

            for msg in sorted(findings[node]):
                Logs.warn('- %s' % msg)

            Logs.warn('')

        Logs.warn(separatorline(''))

@taskgen_method
def report_finding(self, node, msg):
  bld = self.bld

  if display_findings not in getattr(bld, 'post_funs', []):
      bld.findings = {}
      bld.add_post_fun(display_findings)

  try:
      bld.findings[node].add(msg)
  except KeyError:
      bld.findings[node] = set([msg])


''' ------------------------------------------
   Generate information about used libraries
   ------------------------------------------ '''

@feature('libsused')
@after_method('apply_link','build_nxo','build_nxf')
def build_libsused(self):
    bld = self.bld

    inputs       = []
    input_tgens  = []

    def libsused_get_generators_recursive(generators, tg):
        stack = set(Utils.to_list(getattr(tg, 'use', [])))

        while stack:
            x = bld.get_tgen_by_name(stack.pop())

            if x not in generators:
                generators.add(x)
                stack.update(set(Utils.to_list(getattr(x, 'use', []))))

    generators = set()
    referenced_node = None

    use = self.to_list(getattr(self,'use',[]))
    if len(use) == 0:
      # Generate global libs_used file
      output     = self.path.find_or_declare(self.target)
      install_name = output.name

      for g in self.bld.groups:
        for tg in g:
            features = self.to_list(getattr(tg, 'features', ''))

            # Only add version information of libraries referenced in programs, or other libraries
            if 'cprogram' in features or 'cstlib' in features:
                libsused_get_generators_recursive(generators,tg)

            if 'cstlib' in features:
                generators.add(tg)

    else:
      if 'cprogram' in self.features:
          # The 'libsused' features was enabled on this component
          output = self.path.find_or_declare(self.target + '_usedlibs.txt')
          install_name = output.name

          libsused_get_generators_recursive(generators,self)
      else:
          if 'nxf' in self.features:
              output = self.path.find_or_declare(self.target + '_usedlibs.txt')
              install_name = output.name

              tg     = bld.get_tgen_by_name(use[0])
              referenced_node = self.nxf_task.outputs[0]
          elif 'nxo' in self.features:
              output = self.path.find_or_declare(self.target + '_usedlibs.txt')
              install_name = output.name

              tg = self
              referenced_node = self.nxobuilder_task.outputs[0]
          else:
              # The component being investigated is passed in use component
              # This is triggered by a call to bld.generate_libsused()

              #generate a unique name for generated libsused file
              unique_name = "_generated_" + ("_".join(Utils.split_path(self.name) + Utils.split_path(self.target)))

              output = self.path.find_or_declare(unique_name)
              install_name = Utils.split_path(self.target)[-1]
              tg     = bld.get_tgen_by_name(use[0])

              if not tg.posted:
                  tg.post()

              link_task = getattr(tg,'link_task',None)

              if link_task:
                  referenced_node = link_task.outputs[0]

              for x in use[1:]:
                  libsused_get_generators_recursive(generators,bld.get_tgen_by_name(x))

          libsused_get_generators_recursive(generators,tg)

    for tg in generators:
        if 'cprogram' in tg.features:
            # never include an already linked elf in usedl ibs.txt
            hide_from_usedlibs = True
        elif getattr(tg, 'hidden_from_list', False):
            if 'cstlib' in tg.features:
                hide_from_usedlibs = True
            elif 'cxxstlib' in tg.features:
                hide_from_usedlibs = True
            else:
                # Do hide external components and sdk's (they're marked with 'hidden_from_list' too)
                hide_from_usedlibs = False
        else:
            hide_from_usedlibs = False

        if not hide_from_usedlibs:
            if not getattr(tg, 'posted', False):
                tg.post()

            link_task = getattr(tg,'link_task',None)

            if link_task:
                inputs.append(link_task.outputs[0])
                input_tgens.append((tg, link_task.outputs[0]))
            elif getattr(tg,'SDK', False) and getattr(self,'include_SDK', False):
                input_tgens.append((tg, None))

    task = self.libsused_task = self.create_task('LibsusedTask', inputs, [output])
    task.input_tgens = input_tgens
    task.referenced_node   = referenced_node

    if getattr(self, 'install_path', None):
        self.bld.install_as( "/".join(Utils.split_path(self.install_path) + [install_name]),\
                           output)

@conf
def generate_libsused(bld, name, target, use = [], **kw):
  use = Utils.to_list(use)

  if use:
      #user passed use parameter, so we need to check it
      use = Utils.to_list(use)
      if len(use) > 1:
          # error, we only provide libsused generation for a single target
          bld.fatal("Libused generation only works for a single target, and multiple targets (%u)were given via use" % len(use))

      tg = bld.get_tgen_by_name(use[0])
      if not 'cprogram' in getattr(tg, 'features', []):
          # This is not a cprogram. So we try to check the used components
          # e.g. a firmware target will have the cprogram target in use
          for x in Utils.to_list(getattr(tg, 'use', [])):
              tg = bld.get_tgen_by_name(x)
              if 'cprogram' in getattr(tg, 'features', []):
                  # We've found the possible target, so override use parameter from function call
                  use = [tg.name]
                  break
          else:
              # No cprogram found in given use component
              bld.fatal("Libsused generation did not find any useable program for generation in use parameters (target=%r, use=%r)" % (name, use))

  bld(name             = name,
      target           = target,
      features         = 'libsused',
      use              = use,
      hidden_from_list = True,
      **kw)

class LibsusedTask(Task.Task):
    color   = 'PINK'
    log_str = '[LIBSUSED] $TARGETS'

    def extractVersionSection(self, tg, node):
        bld = self.generator.bld

        env = tg.env

        if not env['OBJDUMP']:
            toolchain = getattr(tg,'toolchain', None)

            if toolchain:
                env = bld.all_envs[toolchain]
            else:
                target_triple, sep, dummy = tg.name.partition('/')

                toolchains = {
                  'arm-none-eabi'     : ['codesourcery'],
                  'armv7r-none-eabi'  : ['gccarmemb'],
                  'armv7em-none-eabi' : ['gccarmemb'],
                  'arm-hitex-elf'     : ['hitex'],
                }.get(target_triple,['codesourcery', 'gccarmemb'])

                for x in toolchains:
                    try:
                        env = bld.all_envs['toolchain_%s' % x]
                    except KeyError:
                        pass
                    else:
                        if env['OBJDUMP']:
                            break

        objdump = env.get_flat('OBJDUMP')

        if not objdump:
            msg = 'Suitable objdump not found for %s' % node.abspath()
            Logs.debug('libsused: ' + msg)
            raise ValueError(msg)

        cmd = [objdump, '-sj', '.hilscher_version', node.abspath()]
        try:
            out,err = bld.cmd_and_log(cmd, output=BOTH, quiet=BOTH)
        except WafError, e:
            Logs.debug('libsused: Failed to run "%s"' % (' '.join(cmd)))
            err = 'Failed to run objdump'

        if len(err) > 0:
            msg = 'Failed to dump hilscher_version section from %s' % node.abspath()
            Logs.debug('libsused: ' + msg)
            raise ValueError(msg)
        else:
            dummy, sep, dump = out.partition("Contents of section .hilscher_version:")

            hexversion = ''
            match = re.compile(r'[0-9a-fA-F]+\s[0-9a-fA-F]+').match

            for line in dump.splitlines():
                line = line.strip()
                if line:
                    if match(line):
                        hex,sep,dummy = line.partition('  ')
                        hexnumbers  = hex.split()[1:]
                        hexversion += ''.join(hexnumbers)
                    else:
                        break

            if len(hexversion) % 2:
                Logs.error('unexpected version string length %d for target %s' % (len(hexversion), tg.name))
                hexversion = hexversion[:-1]

            version_string = hexversion.decode('hex').partition('\x00')[0].strip()

        return version_string

    def run(self):
        # define a nmed tuple to hold information used later when formating the file
        used_info = namedtuple('used_info', ['name', 'version', 'description'])

        bld = self.generator.bld
        cwd = bld.variant_dir

        usedlibs   = {}

        lengths = { "max_name_len" : 0, "max_version_len" : 0}

        # iterate over all input files and generate a version entry for them
        for tg, x in self.input_tgens:
            prefix, sep, name = tg.name.rpartition('/')

            version     = getattr(tg, 'version', None)

            if version:
                version = u'V%s' % version
            else:
                version = u'<Undefined version>'

            description = unicode(getattr(tg, 'description', None) or u'<No description>')

            if x is not None:
                name = x.name

                try:
                    version_string = self.extractVersionSection(tg, x)
                    Logs.debug(u'libsused: extracted version information from library %s' % (x.name))
                except ValueError:
                    self.generator.report_finding(x, u'no version information compiled into library')
                    Logs.debug(u'libsused: extracted version information from task generator %s' % (x.name))
                else:
                    if version and (version not in version_string.split()):
                        self.generator.report_finding(x,u'mismatch in version information extracted from library and task generator "%s" <> "%s"' % (version_string, version))

                    description = re.sub(r'\s?[Vv][0-9]+[.][0-9]+[.][0-9]+[.][0-9]+\s?', u' ', version_string)

            lengths['max_name_len']    = max(len(name), lengths['max_name_len'])
            lengths['max_version_len'] = max(len(version), lengths['max_version_len'])

            try:
                usedlibs[prefix].add(used_info(name,version,description))
            except KeyError:
                usedlibs[prefix] = set((used_info(name,version,description),))

        fmt_use = (u'{name:<%(max_name_len)u} {version:<%(max_version_len)u} {description}' % lengths).format
        output  = u''

        if self.referenced_node:
            target_node    = self.referenced_node
            target_version = getattr(self.generator,'version', None)

            if target_version:
                target_version = u'V%s' % target_version
            else:
                target_version = u'<Undefined version>'

            output += u'Target:  {0}\n'.format(target_node.name)
            output += u'Version: {0}\n'.format(target_version)
            output += u'\n'

            output += u'Used Libraries\n'
            output += u'==============\n'
        else:
            output += u'Used Libraries summary\n'
            output += u'======================\n'

        for prefix in sorted(usedlibs.keys()):
            output += u'\n%s\n' % prefix
            output += (u'-' * len(prefix)) + u'\n'

            output += u'\n'.join(fmt_use(**y._asdict()) for y in sorted(usedlibs[prefix],key = lambda x : x.name)) + u'\n'

        self.outputs[0].write(output)

''' ------------------------------------------
   NXO/Loadable Module
   ------------------------------------------ '''
@feature('remove_librcx')
@after_method('apply_link', 'process_use')
def process_removelibrcx(self):
  # features are also applied to Objects, so we will receive
  # a call for thumb_objs / arm_objs which don't have link task
  if not getattr(self, 'link_task', None):
      return

  tmp_list = list(self.link_task.env['STLIB'])
  for lib_to_remove in tmp_list:
      # find archives the name starts with 'rcx'
      # do not remove 'rcx_module*' and 'rcx_netxNNN_2portswitch'
      m = re.match('rcx(_(vol|bas|mid|midshm|hif_cifx|netx\d+(_physhif|_hif)?))?$', lib_to_remove)
      if m:
          # remove and warn, because there is a wrong use-component in wscript for this target
          Logs.pprint('YELLOW', "Warning: use-component '{0}' removed from build of target '{1}'".format(lib_to_remove, self.name))
          self.link_task.env['STLIB'].remove(lib_to_remove)

@conf
def module(bld, target, fileheader_source, platform = None, toolchain = None,**kw):
  kw['platform']  = platform
  kw['toolchain'] = toolchain

  name             = kw.pop('name', target)
  description      = kw.pop('description', '')
  use              = kw.pop('use',        [])
  source           = kw.pop('source',     [])
  source_arm       = kw.pop('source_arm', [])

  if not handle_toolchain(bld,kw):
    return

  tempelf_target = bld.path.find_or_declare(target)
  tempelf_target = str(tempelf_target.change_ext(''))
  tempelf_name   = target + '.nxo'

  install_path = kw.pop('install_path', None)

  features_program = Utils.to_list(kw.pop('features', [])) + ['remove_libc', 'remove_librcx', 'buildstamp']
  features_nxo     = []

  if 'libsused' in features_program:
      features_program.remove('libsused')
      features_nxo = ['libsused']

  # programm
  bld.program(
          target      = tempelf_target,
          name        = tempelf_name,
          features    = features_program,
          source      = source,
          source_arm  = source_arm,
          use         = use,
          linkflags   = Utils.to_list(kw.pop('linkflags', [])) + ['-Wl,-r'],
          hidden_from_list = True,
          **kw)

  nxo_sources = Utils.to_list(fileheader_source) + Utils.to_list(kw.get('taglist_source', []))

  bld(features      = features_nxo + ['c', 'nxo'],
      source        = nxo_sources,
      target        = target,
      name          = name,
      moduleelf     = tempelf_name,
      fileheader_source = fileheader_source,
      use           = Utils.to_list(use),
      description   = description,
      install_path  = install_path,
      **kw)

@feature('nxo')
@after_method('apply_link', 'process_source')
def build_nxo(self):
    '''
    Function to build a netX loadable module.
    '''
    if not getattr(self, 'use', None):
      self.bld.fatal("Building a NXO requires 'use' parameter")

    inputs =  [None,None,None]
    outputs = [self.path.find_or_declare(self.target)]

    # Search our elf file to convert
    try:
        tg = self.bld.get_tgen_by_name(self.moduleelf)
        # is already posted
        if not getattr(tg,'posted',None):
          tg.post()
    except self.bld.errors.WafError:
        Logs.pprint('YELLOW',"Skipping nxo-file \'%s\' because no matching tgen found. (Probably the source elf was not build because of missing toolchain)" % (target))
        return

    if getattr(tg, 'link_task', None):
      # Only use .elf files as inputs
      for x in tg.link_task.outputs:
          if x.suffix().lower() == '.elf':
              inputs[0] = x
    else:
        self.bld.fatal('Input tgen has no link task')

    fileheader_node = self.path.find_node(self.fileheader_source)

    if getattr(self, 'taglist_source', None):
        taglist_node = self.path.find_node(self.taglist_source)
    else:
        taglist_node = none

    for x in self.compiled_tasks:
        if (x.inputs[0] == fileheader_node):
            inputs[1] = x.outputs[0]
        elif (x.inputs[0] == taglist_node):
            inputs[2] = x.outputs[0]

    # create task and adapt its env
    self.nxobuilder_task = self.create_task('NxoBuilderTask', inputs,outputs)
    self.nxobuilder_task.env  = tg.env.derive()

    if getattr(self, 'install_path', None):
        self.bld.install_files(getattr(self, 'install_path', None), self.nxobuilder_task.outputs[0])

class NxoBuilderTask(Task.Task):
    color   = 'PINK'
    cmdline = None
    inst_to = None

    def run(self):
        inputfile  = self.inputs[0].abspath()
        outputfile = self.outputs[0].get_bld().abspath()
        fileheader = self.inputs[1]
        taglist    = self.inputs[2]

        if not NxoBuilder(self, outputfile, inputfile, fileheader, taglist):
          self.generator.bld.fatal("NXO building failed")

''' ------------------------------------------
   Firmware
   ------------------------------------------ '''
from hilscher_netx import  program,handle_toolchain

@conf
def firmware(bld, target, linkerscript,BOOTBLOCKERFLAGS, platform=None, toolchain=None,**kw):
  """
  Function to build a firmware (nxf/nxo[todo]) from sources and libraries.
  <b>  Mandatory parameters  </b>
  @param target    \b string: file name of the target (e.g. netX.nxf)
  @param platform  \b string: type of platform, that specified compiler parameter set. Values: \n\e netx - common parameters for all netx platforms \n\e netx500 - parameters specific for netX500 \n\e netx100 - parameters specific for netX100 \n\e netx50  - parameters specific for netX50 \n\e netx51  - parameters specific for netX51 \n\e netx10  - parameters specific for netX10
  @param toolchain \b string: identification of toolchain to use in build. \n For example: \e 'hitex' or \e 'codesourcery'. See 'hilscher_toolchains.py' for more toolchains or define therein a new one. \n
  @param linkerscript \b string: linker script
  @param BOOTBLOCKERFLAGS <b> list of strings: </b>parameters for bootblocker
  \n\n\n
  <b> Optional parameters: </b>\n\n
  @param description \b string: description of this target, will be listed in "waf list"
  @param sources       <b> list of strings: </b> list of source files (*.c/*.s) in absolute or relative path, that compiler translates in 'thumb' mode
  @param sources_thumb <b> list of strings: </b> (equivalent to \e sources) list of source files (*.c/*.s) in absolute or relative path, that compiler translates in 'thumb' mode
  @param sources_arm   <b> list of strings: </b> list of source files (*.c/*.s) in absolute or relative path, that compiler translates in 'arm' mode
  @param includes <b> list of strings: </b> list of include directories to include in build of source files
  @param use <b> list of strings: </b> list of libraries to use in compile stage (their 'export_defines' and 'export_includes' are used for build too)
  @param defines <b> list of strings: </b> list of define directives to use in build of source files
  \n\n\n
  <b> Additional parameters for compiler, assembler and linker: </b>\n\n
  @param stlib <b> list of strings: </b> used static libraries
  @param stlibpath <b> list of strings: </b> path to static libraries
  @param linkflags <b> list of strings: </b> options, forwarded to the linker
  @param cflags    <b> list of strings: </b> options, forwarded to the C-Compiler
  @param cxxflags  <b> list of strings: </b> options, forwarded to the C++-Compiler
  \n\n\n
  <b> Installation: </b>\n\n
  @param install_path \b string:  (relative) path to install current component while „waf install“
  \n\n\n
  """
  # mandatory parameters
  kw['platform']         = platform
  kw['toolchain']        = toolchain
  kw['linkerscript']     = linkerscript

  if not handle_toolchain(bld,kw):
    return

  tgt = bld.path.find_or_declare(target)

  nxf_target = target
  nxf_name   = target

  prog_target = str(tgt.change_ext(''))
  prog_name   = prog_target

  name = kw.pop('name', [])
  if name:
    nxf_name  = name
    prog_name = name + '.elf'

  install_path = kw.pop('install_path', None)

  features_program = Utils.to_list(kw.pop('features', [])) + ['buildstamp']
  features_nxf     = []

  if 'libsused' in features_program:
      features_program.remove('libsused')
      features_nxf = ['libsused']

  # programm
  bld.program(target           = prog_target,
              name             = prog_name,
              install_path     = [],
              hidden_from_list = True,
              features         = features_program,
              **kw)

  # nxf
  buildnxf(bld,
           target           = target,
           name             = nxf_name,
           use              = prog_name,
           features         = features_nxf,
           BOOTBLOCKERFLAGS = BOOTBLOCKERFLAGS,
           description      = kw.get('description',  " "),
           displaygroup     = kw.pop('displaygroup', 'default'),
           version          = kw.pop('version', None),
           install_path     = install_path)

@conf
def generate_hboot_image(bld, target, netx_type, hboot_xml, use, **kw):
  """
  """
  # mandatory parameters

  #check python version
  python_version = bld.cmd_and_log([bld.env['PYTHON'], '-c', 'import sys; sys.stdout.write(str(tuple(sys.version_info)))'], quiet=BOTH)
  python_version = eval(python_version, {})

  if python_version[0:3] < (2,7,9):
      Logs.pprint('YELLOW', "Warning: hboot compiler might not work with python version %d.%d.%d" % (python_version[0],python_version[1], python_version[2]))

  features = Utils.to_list(kw.pop('features', [])) + ['hboot']
  bld(target = target,
      features = features,
      netx_type = netx_type,
      hboot_xml = hboot_xml, use=use, **kw)


@conf
def generate_netx90_intflash2_image(bld, target, use, **kw):
  u""" build an application image for application flash 2 for netx90.
       The image must be compiled with proper linkerscript in order to be usefull
  """

  features = Utils.to_list(kw.pop('features', [])) + ['nx90_intflash2_image']
  bld(target = target,
      features = features,
      use=use, **kw)

''' ------------------------------------------
   NXF Update support
   ------------------------------------------ '''
@conf
def buildnxf(bld, target, *k, **kw):
    """
    Function to build a firmware (nxf) from executable (elf).
    <b>  Mandatory parameters  </b>
    @param target    \b string: file name of firmware
    @param use <b> list of strings: </b> target, from binary will be constructed
    @param BOOTBLOCKERFLAGS <b> list of strings: </b>parameters for bootblocker
    \n\n\n
    <b> Optional parameters: </b>\n\n
    @param description \b string: description of this target, will be listed in "waf list"
    \n\n\n
    <b> Installation: </b>\n\n
    @param install_path \b string:  (relative) path to install current component while „waf install“
    \n\n\n
    """
    if (not 'BOOTBLOCKERFLAGS' in kw) and (not 'BOOTBLOCKERDATA' in kw):
        bld.fatal("Bootblocker flags are missing")

    kw['target']    = target
    kw['name_patch']= '~'+target+'.bin'
    if not 'name' in kw:
      kw['name']    = target

    features = Utils.to_list(kw.pop('features', [])) + ['bootimage', 'nxf']

    bld(features = features, **kw)



class nxupdate(Task.Task):
    ''' Run objcopy on the target'''
    color     = 'PINK'
    inst_to   = None
    cmdline   = None

    def run(self):
      inputfile   = self.inputs[0].get_bld().abspath()
      outputfile  = self.outputs[0].get_bld().abspath()
      nxupdate_fn(inputfile, outputfile)

@feature('nxf')
@after_method('build_bootimage')
def build_nxf(self):
    '''
    Function to build a netX bootable image by performing an OBJCOPY and
    prepending a netX bootheader (64 byte).
    '''
    target = self.path.find_or_declare(self.target)
    #target = self.path.make_node(self.name)

    input = self.to_list(getattr(self, 'use', None))

    self.nxf_task = self.create_task('nxupdate')

    #target = self.path.find_or_declare(self.name_real)

    self.nxf_task.outputs     = [target]

    self.nxf_task.inputs.extend(self.bootblocker_task.outputs)
    self.nxf_task.set_run_after(self.bootblocker_task)

    if getattr(self, 'install_path', None):
      self.bld.install_files(getattr(self, 'install_path', None), self.nxf_task.outputs)
""" HELPER functions """

class hboot(Task.Task):
    ''' Run objcopy on the target'''
    color     = 'PINK'
    inst_to   = None
    cmdline   = None
    log_str   = '[HBOOT] $TARGETS'

    def run(self):
        tgen = self.generator
        bld = tgen.bld

        env = self.env

        cmd = [env.get_flat('PYTHON'), env.get_flat('HBOOT_IMAGE_COMPILER'), '-v']

        if not all(bool(x) for x in cmd):
            bld.fatal('Either Python or HBOOT image compiler not available')

        hboot_xml, patch_table_node = self.inputs[0:2]
        elf_inputs  = self.inputs[2:]

        # build hboot compiler commandline
        cmd.append('--include=%s'   % hboot_xml.parent.abspath())
        cmd.append('--netx-type=%s' % tgen.netx_type)
        cmd.append('--objdump=%s'   % env.get_flat('OBJDUMP'))
        cmd.append('--objcopy=%s'   % env.get_flat('OBJCOPY'))
        cmd.append('--readelf=%s'   % env.get_flat('READELF'))


        cmd.append('--patch-table=%s' % patch_table_node.abspath())

        sniplib_node = getattr(self,'sniplib_node', None)
        if sniplib_node is not None:
            cmd.append('--sniplib=%s'   % sniplib_node.abspath())

        for i,x in enumerate(elf_inputs):
            cmd.append('--alias=tElf%d=%s' % (i, x.abspath()))

        cmd.append(hboot_xml.abspath())
        cmd.append(self.outputs[0].abspath())

        # run boot image creator
        dct = dict(os.environ)
        dct['LANG']='C'

        out,err = bld.cmd_and_log(cmd, env = dct, output=BOTH, quiet=STDOUT)

        # hboot_image_compiler -v --netx-type 4000 --objcopy %GCC_ARM_PATH%/bin/arm-none-eabi-objcopy --objdump %GCC_ARM_PATH%/bin/arm-none-eabi-objdump --readelf %GCC_ARM_PATH%/bin/arm-none-eabi-readelf --alias tElfCR7=netx4000.elf CR7_DDR600.xml CR7_DDR600.bin

default_hboot_patch_tables = {
  'NETX56'          : 'hboot_netx56_patch_table.xml',
  'NETX90'          : 'hboot_netx90_patch_table.xml',
  'NETX90_MPW'      : 'hboot_netx90_mpw_patch_table.xml',
  'NETX90_MPW_APP'  : 'hboot_netx90_mpw_app_patch_table.xml',
  'NETX4000_RELAXED': 'hboot_netx4000_relaxed_patch_table.xml',
  'NETX4000'        : 'hboot_netx4000_patch_table.xml',
  'NETX4100'        : 'hboot_netx4000_patch_table.xml',
}

@feature('hboot')
@after_method('check_tgen_availability')
def build_hboot(self):
    '''
    Function to build a netX bootable image by performing an OBJCOPY and
    prepending a netX bootheader (64 byte).
    '''
    bld = self.bld
    target    = self.path.find_or_declare(self.target)
    hboot_xml = self.path.find_resource(self.hboot_xml)

    if not hboot_xml:
        bld.fatal('HBoot xml description file "%s" not found' % (self.path.nice_path() + os.path.sep + self.hboot_xml))

    elf_inputs = []
    platform   = ''
    toolchain  = ''

    for x in self.to_list(getattr(self, 'use', None)):
        t = bld.get_tgen_by_name(x)

        platform  = platform or  t.platform.lower()
        toolchain = toolchain or t.toolchain.lower()

        if toolchain != t.toolchain:
            bld.fatal('HBoot toolchain of all source elfs must be identical')

        if platform != t.platform:
            bld.fatal('HBoot platforms of all source elfs must be identical')

        if not getattr(t, 'posted', None):
            tgen.post()

        elf_inputs.append(t.link_task.outputs[0])

    if self.netx_type not in default_hboot_patch_tables:
        bld.fatal('Unsupported netx type %s when building hboot image' % self.netx_type)

    self.platform  = platform
    self.toolchain = toolchain

    # setup environment according to toolchain of use references
    self.env = self.bld.all_envs['toolchain_%s' % toolchain].derive()

    patch_table_path = self.env.get_flat('HBOOT_IMAGE_COMPILER') +\
                           '/patch_tables/%s' % default_hboot_patch_tables[self.netx_type]

    patch_table_node = bld.root.find_node(patch_table_path)

    if not patch_table_node:
        bld.fatal('Patch table %r not found' % patch_table_path)

    hboot_task = self.hboot_task = self.create_task('hboot',
        [hboot_xml, patch_table_node] + elf_inputs, [target])

    sniplib_node = None
    sniplib = getattr(self,'sniplib', None)

    if sniplib is not None:
        if isinstance(sniplib, bld.node_class):
            sniplib_node = sniplib
        else:
            sniplib_node = self.path.find_dir(sniplib)

        if not os.path.isdir(sniplib_node.abspath()):
            bld.fatal('Sniplib not found in %r' % sniplib)

        hboot_task.sniplib_node = sniplib_node

    if getattr(self, 'install_path', None):
      self.bld.install_files(getattr(self, 'install_path', None), hboot_task.outputs)

class objcopy(Task.Task):
    u''' Run objcopy on a file'''
    color   = 'PINK'
    log_str = '[OBJCOPY] $SOURCES $TARGETS'
    run_str = '${OBJCOPY} -O binary ${SRC} ${TGT}'

class appflash_update(Task.Task):
    u''' Update application binary flash file'''
    color     = 'PINK'
    inst_to   = None
    cmdline   = None
    log_str   = '[APPFLASH] $TARGETS'


    def run(self):
        tgen = self.generator
        bld = tgen.bld

        env = self.env
        cmd = [env.get_flat('PYTHON'), env.get_flat('HBOOT_APPFLASH_IMAGE'), '-v']

        if not all(bool(x) for x in cmd):
            bld.fatal('Either Python or HBOOT image compiler not available')

        cmd.append(self.inputs[0].abspath())
        cmd.append(self.outputs[0].abspath())

        out,err = bld.cmd_and_log(cmd, output=BOTH, quiet=STDOUT)

@feature('nx90_intflash2_image')
@after_method('check_tgen_availability')
def build_nx90_intflash2_image(self):
    '''
    Function to build a netX 90 APP bootable image for flash 2.
    '''
    bld = self.bld
    target     = self.path.find_or_declare(self.target)

    target_tmp = target.parent.find_or_declare(target.name + '.tmp')

    elf_inputs = []
    platform   = ''
    toolchain  = ''

    for x in self.to_list(getattr(self, 'use', None)):
        t = bld.get_tgen_by_name(x)

        platform  = platform or  t.platform.lower()
        toolchain = toolchain or t.toolchain.lower()

        if toolchain != t.toolchain:
            bld.fatal('Toolchain of all source elfs must be identical')

        if platform != t.platform:
            bld.fatal('Platforms of all source elfs must be identical')

        if not getattr(t, 'posted', None):
            tgen.post()

        elf_inputs.append(t.link_task.outputs[0])

    self.objcopy_task = self.create_task('objcopy', elf_inputs ,[target_tmp])
    self.image_task   = self.create_task('appflash_update', [target_tmp], [target])

@conf
def SpecFWFileName(self, DeviceFamily, DeviceForm, netXType,
                   PrimaryProtocol, SecondaryProtocol, HWCompInd):
    """
    Function constructs the name of firmware according to document "Specification Firmware File Names" (revision 3, 2013-04-03) [H:\Manual netX Architecture\System\Firmware Names]

    @param DeviceFamily \b string: device family
    @param DeviceForm \b string: device form
    @param netXType \b string: netX type
    @param PrimaryProtocol \b string: primary protocol on stack
    @param SecondaryProtocol \b string: secondary protocol on stack
    @param HWCompInd \b string: hardware compatibility index
    @return  \b string: Firmware file name
    """
    #Table 3:
    device_family = {'netbrick'          :{None :'B0', '' :'B0',},
                     'cifx'              :{None :'C0', '' :'C0',},
                     'netdimm'           :{None :'D0', '' :'D0',},
                     'eu5c'              :{None :'E0', '' :'E0',},
                     'nethost'           :{'t100' :'FT'},
                     'netsmart'          :{None :'G0', '' :'G0',},
                     'nethmi'            :{'j500' :'HJ', 'b500' :'HB',},
                     'netic'             :{None :'I0', '' :'I0',},
                     'netjack'           :{None :'J0', '' :'J0',},
                     'netlink'           :{'usb':'LU', 'ethernet':'LN'},
                     'comx'              :{None :'M0', '' :'M0',},
                     'netpac'            :{None :'P0', '' :'P0',},
                     'netrapid'          :{None :'R0', '' :'R0',},
                     'netplc'            :{'c100':'SC', 'd100':'SD', 'j100':'SJ', 'j500':'SJ', 'm100':'SM', 't100':'ST'},
                     'nettap'            :{None :'T0', '' :'T0',},
                     'netx'              :{None :'X0', '' :'X0',},
                     'rcx'               :{None :'X0', '' :'X0',},
                     'rcx base firmware' :{None :'X0', '' :'X0',},
                    }
    #Table 3, remark:
    file_extention = ".nxf"
    if DeviceFamily.lower() in ['rcx', 'rcx base firmware']:
        file_extention = ".nxo"
    # Table 4:
    netx_type    =  {'netx5'   : '4',
                     'netx6'   : '8',
                     'netx10'  : '5',
                     'netx50'  : '3',
                     'netx51'  : '6',
                     'netx52'  : '7',
                     'netx100' : '2',
                     'netx500' : '1',
                    }
    #Table 5:
    protocol_table = { None                                   : '00',
                ''                                      : '00',
                'rcx'                                   : '00',
                'profibus-dp master'                    : '01',
                'profibus dp master'                    : '01',
                'profibus-dp slave'                     : '02',
                'profibus dp slave'                     : '02',
                'profibus-mpi'                          : '03',
                'profibus mpi'                          : '03',
                'canopen master'                        : '04',
                'canopen slave'                         : '05',
                'devicenet master'                      : '06',
                'devicenet slave'                       : '07',
                'as-interface master'                   : '08',
                'cc-link slave'                         : '09',
                'componet slave'                        : '0A',
                'io-link master'                        : '0B',
                'profinet io controller'                : '0C',
                'pnm'                                   : '0C',
                'profinet io device'                    : '0D',
                'pns'                                   : '0D',
                'ethercat master'                       : '0E',
                'ecm'                                   : '0E',
                'ethercat slave'                        : '0F',
                'ecs'                                   : '0F',
                'ethernet/ip scanner/master'            : '0G',
                'ethernet/ip adapter/slave'             : '0H',
                'sercos iii master'                     : '0I',
                's3m'                                   : '0I',
                'sercos iii slave'                      : '0J',
                's3s'                                   : '0J',
                'powerlink controlled node'             : '0K',
                'open modbus/tcp'                       : '0L',
                'rfc 1006'                              : '0M',
                'df1'                                   : '0N',
                '3964r'                                 : '0P',
                'ascii'                                 : '0Q',
                'modbus rtu (master/slave)'             : '0R',
                'modbus rtu'                            : '0R',
                'netscript (programmable serial)'       : '0S',
                'netscript'                             : '0S',
                'varan client (server)'                 : '0T',
                'varan client'                          : '0T',
                'varan server'                          : '0T',
                'varan'                                 : '0T',
                'smartwire-dt master'                   : '0U',
                'marshaller'                            : '0V',
                'atvise (ethernet)'                     : '10',
                'atvise'                                : '10',
                'profibus-dp master and codesys'        : '11',
                'profibus dp master and codesys'        : '11',
                'devicenet master and codesys'          : '16',
                'profinet io controller and codesys'    : '1C',
                'pnm and codesys'                       : '1C',
                'ethernet/ip scanner/master and codesys': '1G',
                'eip and codesys'                       : '1G',
                'codesys and local i/o'                 : '1Z',
                'profibus-dp master and proconos eclr'  : '21',
                'profibus dp master and proconos eclr'  : '21',
                'canopen master and proconos eclr'      : '24',
                'devicenet master and proconos eclr'    : '26',
                'proconos eclr and local i/o'           : '2Z',
                'profibus-dp master and ibh s7'         : '31',
                'profibus dp master and ibh s7'         : '31',
                'canopen master and ibh s7'             : '34',
                'devicenet master and ibh s7'           : '36',
                'ibh s7 and local i/o'                  : '3Z',
                }

    def get_protocol_id(protocol):
        ltd = False

        if isinstance(protocol, str):
            protocol = protocol.lower()

            if protocol.endswith(" limited"):
                protocol = protocol[:-8]
                ltd = True

            id = protocol_table[protocol]
        else:
            id = protocol_table[protocol]

        if ltd:
            if id[0] == '0':
                id = 'Z' + id[1:]
            else:
                self.fatal("Can not generate limited firmware name for protocol {protocol}".format(**locals()))

        return id

    error_msg = "error: wrong parameter specified in \"SpecFWFileName\" on argument "
    out = ''
    # 1st letter
    try:
        loc_device_form = device_family[DeviceFamily.lower()]
    except KeyError:
        self.fatal(error_msg + "\"DeviceFamily = %s\"" % DeviceFamily)
    # 2nd letter
    try:
        if DeviceForm:
            DeviceForm = DeviceForm.lower()
        out = loc_device_form[ DeviceForm ]
    except KeyError:
        self.fatal(error_msg + "\"DeviceForm = %s\". Available \"DeviceForm\": %s" % (DeviceForm, ', '.join([str(var) for var,value in loc_device_form.iteritems()]) ) )
    #3rd letter
    try:
        out += netx_type[ netXType.replace(' ','').lower() ]
    except KeyError:
        self.fatal(error_msg + "\"netXType = %s\"" % netXType)
    #4th and 5th letters
    try:
        out += get_protocol_id (PrimaryProtocol)
    except KeyError:
        self.fatal(error_msg + "\"PrimaryProtocol = %s\"" % PrimaryProtocol)
    #6th and 7th letters
    try:
        out += get_protocol_id (SecondaryProtocol)
    except KeyError:
        self.fatal(error_msg + "\"SecondaryProtocol = %s\"" % SecondaryProtocol)
    #8th letter and firmware extention
    out += str(HWCompInd) + file_extention

    return out
