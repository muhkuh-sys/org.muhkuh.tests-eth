#! /usr/bin/env python
# encoding: utf-8
########################################################################################
# Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
########################################################################################
# $Id:  $:
#
# Description:
# Distribution generation functions
########################################################################################
from waflib import Task, Logs, Context, Options, Node, Errors, Utils
from waflib.Configure import conf
from waflib.Build import BuildContext
from waflib.TaskGen import feature, after_method, before_method
from waflib.Context import WSCRIPT_FILE
import shutil
import time, os, re, sys, string

template_wscript_header = string.Template('''\
def build(bld):
''')

template_wscript_sdk = string.Template('''\
    bld(
        name            = '${NAME}',
        description     = '${DESCRIPTION}',
        displaygroup    = '${DISPLAYGROUP}',
        version         = '${VERSION}',

        SDK             = True,
        export_includes = [${INCLUDE_PATHS}],
        export_defines  = [${EXPORT_DEFINES}],
    )
''')

template_wscript_extlib = string.Template('''\
    bld.externalcomponent(
        name            = '${NAME}',
        description     = '${DESCRIPTION}',
        displaygroup    = '${DISPLAYGROUP}',
        version         = '${VERSION}',

        target          = '${TARGET}',
        path            = '${PATH}',
        export_includes = [${INCLUDE_PATHS}],
        export_defines  = [${EXPORT_DEFINES}],
    )
''')

@conf
def distribute_lib(bld, install_path, use , **kw):
    if not getattr(bld,'is_install', False):
        return

    kw.setdefault('wscript_name',        'wscript')
    kw.setdefault('include_folder_name', 'Includes')
    kw.setdefault('lib_folder_name',     'Lib')
    kw.setdefault('lib_ltd_folder_name', 'Lib_Ltd')

    kw.setdefault('dist_includes', [])

    # mandatory parameters:
    kw['use']          = Utils.to_list(use)[:]
    kw['install_path'] = install_path
    kw['target']       = install_path + '/' + kw['wscript_name']
    kw['name']         = kw['target']

    use_ltd =  Utils.to_list(kw.get('use_ltd', []))[:]
    use_usedlibs = Utils.to_list(use)[:] + use_ltd

    # generate distribution folder
    bld(features = ['distribute_lib'],
        hidden_from_list = True,
        **kw)

    # generate libsused file
    bld(name     = '%s/libsused' % install_path,
        target   = 'libsused.txt',
        features = 'libsused',
        use      = use_usedlibs,
        include_SDK = True,
        hidden_from_list = True,
        install_path = '%s/ReleaseNotes/' % install_path)

@conf
def distribute_firmware(bld, install_path, use , **kw):
    if not getattr(bld,'is_install', False):
        return

    kw.setdefault('include_folder_name', 'Includes')
    kw.setdefault('firmware_folder_name',     'Firmware')
    kw.setdefault('firmware_ltd_folder_name', 'Firmware_Ltd')

    kw.setdefault('dist_includes', [])

    # mandatory parameters:
    kw['use']          = use[:]
    kw['install_path'] = install_path
    kw['name']         = install_path

    use_ltd =  kw.get('usr_ltd', [])[:]
    use_usedlibs = list(x for x,p in (use + use_ltd))

    # generate distribution folder
    bld(features = ['distribute_firmware'],
        hidden_from_list = True,
        **kw)

    # generate libsused file
    bld(name     = '%s/libsused' % install_path,
        target   = 'libsused.txt',
        features = 'libsused',
        use      = use_usedlibs,
        hidden_from_list = True,
        install_path = '%s/ReleaseNotes/' % install_path)

@feature('distribute_lib', 'distribute_firmware')
@before_method('check_tgen_availability')
def distribute_generate_distribution(self):
    bld = self.bld

    install_path = Utils.split_path(self.install_path)

    include_dist_paths   = set()

    if 'distribute_lib' in self.features:
        name = 'generated_' + '_' .join(Utils.split_path(self.install_path) + [self.wscript_name])

        wscript_node = self.path.find_or_declare(name)
        tsk = self.wscript_task = self.create_task('distribute_generate_wscript', [], [wscript_node])
        tsk.install_dirs  = {}
        tsk.generators     = []

        bld.install_as('/'.join(install_path + [self.wscript_name]), wscript_node)
        tsk.install_dirs[wscript_node] = '/'.join(install_path)

        include_dist_paths.add(tuple(install_path[:]))

    use     = self.to_list(self.use)
    use_ltd = self.to_list(getattr(self,'use_ltd', []))

    include_search_paths = []

    for i, x in enumerate(use + use_ltd):
        ltd = i >= len(use)

        if isinstance(x,(tuple,list)):
            name, subdir_path = tuple(x)
        else:
            name, subdir_path  = x, None

        tgen = self.bld.get_tgen_by_name(name)

        if not getattr(tgen, 'posted', None):
            tgen.post()

        # build up include search paths
        include_tgen = [tgen]

        for y in Utils.to_list(getattr(tgen,'use',[])):
            include_tgen.append(self.bld.get_tgen_by_name(y))

        for y in include_tgen:
            include_search_paths.extend(getattr(y, "include_nodes", [])[:])

            for z in getattr(y, "export_includes", []):
                include_search_paths.append(y.path.find_node(z))

        parts    = tgen.name.split('/')

        if getattr(tgen,'SDK', False):
            dist_node = None
            dst_path_parts       = None
        elif set(tgen.features) & set(['fake_lib', 'shlib', 'stlib', 'cshlib', 'cstlib']):
            dist_node = tgen.link_task.outputs[0]

            if ltd:
                dst_path_parts = install_path + [self.lib_ltd_folder_name]
            else:
                dst_path_parts = install_path + [self.lib_folder_name]
        else:
            dist_node = tgen.path.find_or_declare(tgen.target)

            if ltd:
                dst_path_parts = install_path + [self.firmware_ltd_folder_name]
            else:
                dst_path_parts = install_path + [self.firmware_folder_name]

            include_dist_paths.add(tuple(dst_path_parts[:]))


        if 'fake_lib' in tgen.features:
            # an external library is to be distributed with this distribution, extract
            # original path
            prefix = ('%s/%s/' % (parts[0], parts[1])).lower()
            orig_path = '/'.join(Utils.split_path(dist_node.path_from(tgen.path)))

            i = orig_path.lower().find(prefix)

            if i >= 0:
                dst_path_parts.extend(Utils.split_path(prefix + orig_path[ i + len(prefix):]))
            else:
                Logs.warn(u'Unable to extract prefix for %s' % tgen.name)
                dst_path_parts.extend(Utils.split_path(orig_path))
        elif dist_node is not None:
            if subdir_path is not None:
                if subdir_path.endswith('/') or subdir_path.endswith('\\'):
                    # subdir_path is a directory:
                    dst_path_parts.extend(Utils.split_path(subdir_path) + [dist_node.name])
                else:
                    # subdir_path is a file
                    dst_path_parts.extend(Utils.split_path(subdir_path))

                    base, ext = os.path.splitext(dst_path_parts[-1].lower())

                    if ext not in '.nxf .nxo .rom'.split():
                        Logs.warn(u'Missing file extension for distribution %s. (Check for missing trailing slash sub-path?)' % ('/'.join(dst_path_parts)))

                # also install libsused file if available
                libsused_task = getattr(tgen, 'libsused_task', None)

                if libsused_task is not None:
                    bld.install_as('/'.join( dst_path_parts[0:-1] + ['%s_usedlibs.txt' % dst_path_parts[-1]]),
                                   libsused_task.outputs[0])

            elif 'distribute_lib' in self.features:
                toolchain, toolchain_version, dummy = bld.get_name_prefix(
                  toolchain = tgen.toolchain,platform  = tgen.platform).split('/', 2)

                if len(parts) not in (3,4):
                    bld.fatal(u'Invalid generator name %s' % tgen.name)

                if parts[0] != toolchain:
                    Logs.warn(u'Unexpected toolchain in generator %s' % tgen.name)

                if parts[1] != toolchain_version:
                    Logs.warn(u'Unexpected toolchain version in generator %s' % tgen.name)

                dst_path_parts.extend([toolchain, toolchain_version])

                if len(parts) == 4:
                    os_label = parts[2]
                    dst_path_parts.append(os_label)

                platform_label = tgen.platform.lower()

                if platform_label not in ('netx'):
                    dst_path_parts.append(platform_label)

                dst_path_parts.append(dist_node.name)
            else:
                bld.fatal(u'Unable to determine install path for %s' % tgen.name)

        if dist_node is not None:
            bld.install_as('/'.join(dst_path_parts), dist_node)

        if 'distribute_lib' in self.features:
            if dist_node is not None:
                tsk.set_inputs(dist_node)
                tsk.install_dirs[dist_node] = '/'.join(dst_path_parts[:-1])

            tsk.generators.append((tgen, dist_node))

    for x in self.dist_includes:
        if isinstance(x,(tuple,list)):
            src_path, subdir_path = x
        else:
            src_path, subdir_path = x, None

        for x in include_search_paths:
            include_node = x.find_node(src_path)

            if include_node:
                if subdir_path is None:
                    subdir_path = include_node.parent.path_from(x)
                break
        else:
            bld.fatal(u'Include file %s not found in use/use_ltd targets' % src_path)

        for y in include_dist_paths:
            dst_path_parts = list(y) +  [self.include_folder_name] + Utils.split_path(subdir_path)
            bld.install_files('/'.join(dst_path_parts), [include_node])

class distribute_generate_wscript(Task.Task):
    ''' Run objcopy on the target'''
    log_str   = "[WSCRIPT] $TARGET"
    color     = 'PINK'
    inst_to   = None
    cmdline   = None

    def run(self):
      output_node = self.outputs[0]

      output_node.parent.mkdir()

      fh = open(output_node.abspath(), 'wt')

      fh.write(template_wscript_header.substitute())
      separator = ''

      remove_prefix = self.install_dirs[output_node] + '/'
      for tgen, node in self.generators:
          dct = {
            'NAME'           : tgen.name,
            'DESCRIPTION'    : tgen.description,
            'DISPLAYGROUP'   : tgen.displaygroup,
            'VERSION'        : tgen.version,
            'INCLUDE_PATHS'  : "'%s'" % self.generator.include_folder_name,
          }

          if getattr(tgen, 'SDK', False):
              tmpl = template_wscript_sdk
          else:
              tmpl = template_wscript_extlib

              dct['TARGET'] = Utils.split_path(tgen.target)[-1]
              dct['PATH']   = self.install_dirs[node][len(remove_prefix):]

          # If a component defines public_defines we will use them,
          # otherwise fall back to export defines
          export_defines = Utils.to_list(getattr(tgen, 'public_defines', [])) +\
                           Utils.to_list(getattr(tgen, 'export_defines', []))

          indent = ' ' * 28
          dct['EXPORT_DEFINES'] = (',\n' + indent).join("'%s'" % x for x in export_defines)

          fh.write(separator + tmpl.substitute(dct))
          separator = '\n'
      fh.close()

