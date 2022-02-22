#! /usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################
# Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
########################################################################################
# $Id:  $:
#
# Description:
# This module implements waf buildsystem support for lint
########################################################################################
from waflib import Task, Context, Build, Options, Logs
from waflib.TaskGen import feature, after_method, before_method, taskgen_method, task_gen
from waflib import Node
import optparse
import os
import tempfile
import string
import re
import operator
import sys
from datetime import date
from docutils.parsers.rst.directives import path

logfile_txt = ''
today = date.today()
summary_list = []

module_path = os.path.dirname(os.path.abspath(__file__))

additional_rules = {
  'MISRAC1998' : 'au-misra1.lnt',  # MISRA C 1998
  'MISRAC2004' : 'au-misra2.lnt',  # MISRA C 2004
  'MISRAC2012' : 'au-misra3.lnt',  # MISRA C 2012
  'BARREMBC'   : 'au-barr10.lnt',  # The Barr Group's Bug-Killing Coding - Standard Rules for Embedded C
}

class Paths(object):
   splintrc_template_path         = os.path.join(module_path, 'splintrc.tmpl')
   pclintlnt_template_path        = os.path.join(module_path, 'pclint.lnt.tmpl')
   pclint_output_eclipse_lnt_path = os.path.join(module_path, 'pclint.cfg', 'eclipse-output.lnt')
   pclintreport_template_path     = os.path.join(module_path, 'pclint.log.tmpl')

stdout_sep = '{0:#^80}'.format(' OUT ')
stderr_sep = '{0:#^80}'.format(' ERR ')

def option_pclint_warninglevel(option, opt_str, value, parser):
    if not (0 <= value <= 4):
        raise optparse.OptionValueError(u'Invalid value %d for PClint warning level specified' % value)

    parser.values.pclint_warninglevel = value

def option_pclint_passes(option, opt_str, value, parser):
    if not (1 <= value <= 4):
        raise optparse.OptionValueError(u'Invalid value %d for PClint passes specified' % value)

    parser.values.pclint_passes = value
def options(opt):
    opt.add_option('--pclint-warninglevel', dest='pclint_warninglevel',
                   action = 'callback', callback = option_pclint_warninglevel,
                   type='int', help='Overide PC-Lint warning level set in project (-w<level>)')

    opt.add_option('--pclint-passes', dest='pclint_passes',
                   action = 'callback', callback = option_pclint_passes,
                   type='int', help='Set PC-Lint number of passes (-passes(<passes>)), default:2')

    opt.add_option('--static-analysis-console', dest='static_analysis_console', type='choice',
                   choices=['all', 'errors_else_warnings', 'off'], default='errors_else_warnings',
                   help='Output static analysis messages to console.')

    opt.add_option('--pclint-options', dest='pclint_options', type='choice',
                   choices=['global', 'global_and_local', 'local', 'none'], default='none',
                   help='Include options.lnt located at wscripts.')

    opt.add_option('--pclint-additional-rules', type='choice', action='append',
                   choices=list(sorted(additional_rules.keys())), default=[],
                   help='Enable additional rules')

    opt.add_option('--pclint-jenkins-report', action='store_true', default=False,
                   help='Generate a jenkins report file suitable for use with jenkins plot plugin')

def configure(conf):
    conf.setenv('')
    env = conf.env

    env.stash()
    try:
        conf.find_program('splint', var='SPLINT')
    except:
        env.revert()
    else:
        env['SPLINTCPPPATH_ST'] = '-I%s'
        env['SPLINTDEFINES_ST'] = '-D%s'

    env.stash()
    try:
        conf.find_program('lint-nt', var='PCLINT')
    except:
        env.revert()
    else:
        # The following variables are not used directly but we want the lint
        # task depend on them therefore the're added to environment and will be
        # referenced by the task
        env['PCLINT_OPT_WARNINGLEVEL'] = Options.options.pclint_warninglevel
        env['PCLINT_OPT_PASSES']       = Options.options.pclint_passes
        env['PCLINT_OPT_OPTIONS']      = Options.options.pclint_options
        env.append_value('PCLINT_OPT_ADDITIONALRULES', Options.options.pclint_additional_rules)

def build(bld):
    bld.setenv('')

    if bld.cmd == 'splint' and not bld.env['SPLINT']:
        bld.fatal(u'Program splint not found but command splint used')

    if bld.cmd == 'pclint' and not bld.env['PCLINT']:
        bld.fatal(u'Program pclint not found but command pclint used')

class PClintOpts(object):
    '''Container to hold pclint options'''
    pass

@feature('*')
@after_method('process_source')
def apply_lint(self):
    name = self.name
    bld  = self.bld

    if name.endswith('_arm_objs') or name.endswith('_thumb_objs'):
        return

    splintrc_template = bld.root.find_resource(Paths.splintrc_template_path)

    if not splintrc_template:
        self.bld.fatal('splint rc file template not found in %s' % Paths.splintrc_template_path)

    pclintlnt_template = bld.root.find_resource(Paths.pclintlnt_template_path)

    if not pclintlnt_template:
        self.bld.fatal('pclint lnt file template not found in %s' % Paths.pclintlnt_template_path)

    pclint_output_lnt = bld.root.find_resource(Paths.pclint_output_eclipse_lnt_path)

    if not pclint_output_lnt:
        self.bld.fatal('pclint output lnt file not found in %s' % Paths.pclint_output_eclipse_lnt_path)

    pclintreport_template = bld.root.find_resource(Paths.pclintreport_template_path)

    if not pclintreport_template:
        self.bld.fatal('pclint report template file not found in %s' % Paths.pclintreport_template_path)

    pclint_rules = []

    compiler_rules = {
        'gcc'       : ['co-gcc.lnt'],
        'clang'     : ['co-clang.lnt'],
        'xpic-llvm' : ['co-clang.lnt'],
    }.get(self.env.get_flat('CC_NAME'), [])

    target_rules = self.to_list(getattr(self,'pclint_rules', 'hilscher-options.lnt'))

    standard_rules = list(additional_rules[x] for x in Options.options.pclint_additional_rules)

    for x in compiler_rules + target_rules + standard_rules:
        path = os.path.join(module_path, 'pclint.cfg', x)
        node = bld.root.find_resource(path)

        if not node:
            self.bld.fatal('pclint lnt file %s not found' % path)

        pclint_rules.append(node)


    if Options.options.pclint_options in ('global', 'global_and_local'):
        node = bld.path.find_resource('options.lnt')

        if node:
            pclint_rules.append(node)

    if Options.options.pclint_options in ('local', 'global_and_local'):
        node = self.path.find_resource('options.lnt')

        if node:
            pclint_rules.append(node)

    if not all(pclint_rules):
        # this should not happen, the error shall be catched before
        self.bld.fatal('pclint lnt file not found')

    # make list of compiling tasks for which we want to lint
    # the source files
    compiled_tasks = getattr(self,'compiled_tasks', [])

    # handle source files of all sub task generators
    # which make the arm/thumb objects. This is default case
    # for hilscher
    for x in self.to_list(getattr(self,'use',[])):
        if x.startswith(self.name + '_') and x.endswith('_objs'):
            tgen = bld.get_tgen_by_name(x)

            if not getattr(tgen, 'posted', None):
                tgen.post()

            compiled_tasks.extend( x for x in getattr(tgen,'compiled_tasks',[]) if type(x).__name__ not in ('fake_o',))

    # when target is given on command line,
    # only lint this target
    if Options.options.targets:
        if name not in Options.options.targets:
            compiled_tasks = []

    source_files = list(x.inputs[0] for x in compiled_tasks)

    if source_files and bld.cmd == 'splint':
        inputs = [splintrc_template]

        for x in source_files:
            if not x.name.lower().endswith('.s'):
                inputs.append(x)

        splint_outdir  = bld.srcnode.make_node('build/splint')
        splint_outname = self.name.translate(string.maketrans("/.","__"))

        outputs= [
            self.path.get_bld().find_or_declare('%s_splintrc'   % self.target),
            # self.path.get_bld().find_or_declare('%s_splint.log' % self.target),
            splint_outdir.make_node('%s_splint.log' % splint_outname),
        ]

        task     = self.create_task("splint", inputs, outputs)
        task.env = self.env.derive()

        # lint shall be run after compilation success
        task.run_after |= set(compiled_tasks);

        if Options.options.static_analysis_console != 'off':
            try:
                splint_reports = bld.splint_reports
            except:
                splint_reports = bld.splint_reports = []
                bld.add_post_fun(splint_show_results)

            splint_reports.append(task.outputs[1])

    if source_files and bld.cmd == 'pclint':
        inputs = [pclintlnt_template, pclint_output_lnt] + pclint_rules

        for x in source_files:
            if not x.name.lower().endswith('.s'):
                inputs.append(x)

        pclint_outdir  = bld.srcnode.make_node('build/pclint')
        pclint_outname = self.name.translate(string.maketrans("/.","__"))

        pclint_lnt_node = pclint_outdir.make_node('%s.lnt' % pclint_outname)
        pclint_log_node = pclint_outdir.make_node('%s_pclint.log' % pclint_outname)

        outputs = [pclint_lnt_node, pclint_log_node]

        for node in outputs:
            if not os.path.isfile(node.abspath()):
                node.sig = None

        task     = self.create_task("pclint", inputs, outputs)
        task.env = self.env.derive()

        # lint shall be run after compilation success
        task.run_after |= set(compiled_tasks);

        task.opts = opts = PClintOpts()

        if Options.options.pclint_warninglevel is not None:
            opts.warninglevel = Options.options.pclint_warninglevel
        else:
            opts.warninglevel = getattr(self,"pclint_warninglevel", 2)

        if Options.options.pclint_passes is not None:
            opts.passes = Options.options.pclint_passes
        else:
            opts.passes = getattr(self,"pclint_passes", 2)

        try:
            log_task = bld.log_task
        except AttributeError:
            log_node = pclint_outdir.make_node('__pclint.log')

            if not os.path.isfile(log_node.abspath()):
                log_node.sig = None

            log_task = bld.log_task = self.create_task('pclint_log', [pclintreport_template], [log_node])

            if Options.options.static_analysis_console != 'off':
                bld.add_post_fun(pclint_show_results)

        log_task.set_inputs(pclint_lnt_node)
        log_task.set_inputs(pclint_log_node)

        if Options.options.pclint_jenkins_report:
            try:
                report_task = bld.report_task
            except AttributeError:
                report_node = pclint_outdir.make_node('jenkins_report.csv')

                if not os.path.isfile(report_node.abspath()):
                    report_node.sig = None

                report_task = bld.report_task = self.create_task('pclint_jenkins_report', [], [report_node])

            report_task.set_inputs(pclint_log_node)

def splint_show_results(bld):
    """Dump all splint logs after building to the console"""
    global stdout_sep, stderr_sep

    for report_node in bld.splint_reports:
        content = report_node.read().strip()

        a,dummy,err     = content.partition(stderr_sep)
        dummy,dummy,out = a.partition(stdout_sep)

        out = out.strip()
        err = err.strip()

        if out:
            x = report_node.nice_path()

            if len(x) > 72:
                x = " ..." + x[-69:] + " "
            else:
                x = " " + x + " "

            msg = "---{0:-^74}---".format(x)

            print('\n'.join([msg,content,msg,""]))

        # TODO: join all tables in err output into one table to show at the end


re_match_pclint_module  = re.compile(r'^--- Module:\s+(\S.+)\s+([(].+[)])$').match

'''Match a pc lint message according our message style'''
re_match_pclint_message      = re.compile(r'^(?P<Path>.+):(?P<Line>[0-9]+):(?P<Column>[0-9]+):\s*(?P<Type>\w+)\s+(?P<Number>[0-9]+)\s*:(?P<Message>.+)$').match
re_match_pclint_summary      = re.compile(r'^\s*(?P<Count>Count|[0-9]+)\s+(?P<Type>\w+)\s+(?P<Number>Number|[0-9]+)\s+(?P<Text>.*)$').match
re_match_pclint_version      = re.compile(r'^PC-lint .+ Gimpel Software .+$').match
re_match_pclint_passes       = re.compile(r'^-passes[(](\d+)[)]$').match
re_match_pclint_warninglevel = re.compile(r'^-w(\d+)$').match


def pclint_show_results(bld):
    """Dump all pclint logs after building to the console"""
    global stdout_sep, stderr_sep

    log_task = bld.log_task

    messages = log_task.messages
    summary  = log_task.summary_messages

    msg = ''
    # select the messages to print

    type_priority = 'Error Warning Info Note'.split()

    if Options.options.static_analysis_console == 'errors_else_warnings':
        for t in type_priority:
            msgs_to_print = messages.get(t,[])

            if msgs_to_print:
                msg = 'Printed only PC-lint %s messages' % t

                msgs_to_print = list(sorted(msgs_to_print))
                msgs_to_print.extend(['',msg])
                break
    else:
        msgs_to_print = list(sorted(reduce(operator.or_, messages.values(),set())))

    if Options.options.static_analysis_console != 'off':
        if log_task.lst_summary:
            msgs_to_print.extend([
            '',
            '{0:#^80}'.format(' PC-lint message summary '),
            '',
            ])

            msgs_to_print.extend(log_task.lst_summary)
            msgs_to_print.append('')

        if msgs_to_print:
            Logs.info('\n'.join(msgs_to_print))
            sys.stdout.flush()
    #build_summary()


class lint(object):
    re_match_define = re.compile(r'#define\s+(?P<name>\S+)\s+(?P<value>.+)').match

    def exec_command(self, cmd, **kw):
        """
        Wrapper for :py:meth:`waflib.Context.Context.exec_command` which sets a current working directory to ``build.variant_dir``

        :return: the return code
        :rtype: int
        """
        bld = self.generator.bld
        try:
          if not kw.get('cwd', None):
            kw['cwd'] = bld.cwd
        except AttributeError:
          bld.cwd = kw['cwd'] = bld.variant_dir

        kw['quiet'] = Context.BOTH
        kw['output'] = Context.BOTH

        if self.env['OSENVIRON']:
            kw['env'] = dict(os.environ, **self.env['OSENVIRON'])

        return bld.cmd_and_log(cmd, **kw)

    def extract_compiler_params(self):
        tgen   = self.generator
        env    = self.env
        inputs = self.inputs

        # at first run gcc to extract internal defines, search paths, etc
        env.stash()
        (fd, path) = tempfile.mkstemp(suffix='.c')
        try:
            os.close(fd)
            env['OSENVIRON'] = { 'LANG': 'C' }

            self.inputs = [tgen.bld.root.find_resource(path)]
            out,err = self.run_cc()
        finally:
            env.revert()
            self.inputs = inputs
            os.unlink(path)

        defines  = []
        includes = []

        re_match_define = self.re_match_define

        stack = out.splitlines()
        while stack:
            line = stack.pop(0).strip()

            m = re_match_define(line)

            if m:
                defines.append(m.group('name','value'))

        stack = err.splitlines()

        # skip initial stuff
        while stack:
            line = stack.pop(0).strip()

            if line.startswith('#include'):
                break;

        # extract the include search paths
        while stack:
            line = stack.pop(0).strip()

            line = re.sub(r'^/cygdrive/([a-z])/', r'\1:/', line)

            if os.path.isdir(line):
                includes.append(os.path.normpath(line))
            elif line.startswith('End of search list'):
                break

        return defines, includes

    def runnable_status(self):
        """
        Override :py:meth:`waflib.Task.TaskBase.runnable_status` to determine if the task is ready
        to be run (:py:attr:`waflib.Task.Task.run_after`)
        """
        #return 0 # benchmarking

        for t in self.run_after:
            if not t.hasrun:
                return Task.ASK_LATER

        bld = self.generator.bld

        # first compute the signature
        try:
            new_sig = self.signature()
        except Errors.TaskNotReady:
            return Task.ASK_LATER

        # compare the signature to a signature computed previously
        key = self.uid()
        try:
            prev_sig = bld.task_sigs[key]
        except KeyError:
            Logs.debug("task: task %r must run as it was never run before or the task code changed" % self)
            return Task.RUN_ME

        if new_sig != prev_sig:
            Logs.debug("task: task %r must run as the task sig does not match" % self)
            return Task.RUN_ME

        # compare the signatures of the outputs
        for node in self.outputs:
            if not os.path.isfile(node.abspath()):
                Logs.debug("task: task %r must run as the output nodes do not exist" % self)
                return Task.RUN_ME

        return Task.SKIP_ME

class splint(lint,Task.Task):
    color = 'CYAN'

    run_str_splint = '${SPLINT} ${SPLINTOPTS} ${SPLINTCPPPATH_ST:INCPATHS} ${SPLINTDEFINES_ST:DEFINES} ${SPLINTDUMPOPTS} ${SRC}'
    run_str_cc     = '${CC} ${ARCH_ST:ARCH} ${CFLAGS} ${CPPFLAGS} ${FRAMEWORKPATH_ST:FRAMEWORKPATH} ${CPPPATH_ST:INCPATHS} ${DEFINES_ST:DEFINES} -E -dM -dD -v ${CC_SRC_F}${SRC[0].abspath()}'

    (run_splint, splint_vars) = Task.compile_fun(run_str_splint)
    (run_cc, cc_vars)         = Task.compile_fun(run_str_cc)

    vars = splint_vars + cc_vars

    log_str = '[SPLINT] $name $out'

    @property
    def name(self):
        return self.generator.name

    @property
    def out(self):
        return self.outputs[1].nice_path()

    def run(self):
        global stdout_sep, stderr_sep

        tgen    = self.generator
        env     = self.env
        inputs  = self.inputs
        outputs = self.outputs

        for x in outputs:
            x.delete()

        splintrc_template_node = inputs[0]
        source_nodes           = inputs[1:]

        splintrc_node, output_node = outputs

        defines, includes = self.extract_compiler_params()

        # for some reason splint seems to completely ignore stdint.h from gcc
        # I have no idea why. Therefore we must supply our own stdint.h
        # in order to have splint run properly
        includes.insert(0, os.path.join(module_path, "splint_includes"))

        tgenincludes = set(x.abspath() for x in tgen.to_incnodes(getattr(tgen, 'includes', [])))

        sysincludes = list(x for x in includes if x not in tgenincludes)

        # splint seems to have problems with resolving some headers:
        # It seems to completely ignore stdint.h from compiler files.

        dct = dict()

        # the following does not work
        # splint does not support spaces in splintrc
        dct['INCPATHFLAGS'] = ''
        dct['SYSDIRSFLAG']  = ''

        #dct['INCPATHFLAGS'] = os.linesep.join('+I"%s"' % x.replace(' ', '\\\\ ') for x in gccincludes)
        #dct['SYSDIRSFLAG'] = '-sys-dirs %s' % os.pathsep.join(sysincludes)

        env.stash()
        try:
            template_string = string.Template(splintrc_template_node.read())
            splintrc_node.write(template_string.substitute(dct))

            env.append_value('SPLINTOPTS', ['-f',splintrc_node.abspath()])

            # the following two are required beccause auf splint problems
            # currently it seems not to work when they are specified via
            # splintrc
            if not dct['INCPATHFLAGS']:
                env['INCPATHS'] = includes

            if not dct['SYSDIRSFLAG']:
                env.append_value('SPLINTOPTS', ['-systemdirs', os.pathsep.join(sysincludes)])

            try:
                self.inputs = source_nodes
                out, err = self.run_splint()
            except Exception, e:
                out = getattr(e, 'stdout', None)
                err = getattr(e, 'stderr', None)
            finally:
                self.inputs = inputs
        finally:
            env.revert()

        output = [stdout_sep] + out.splitlines() + [stderr_sep] + err.splitlines()

        output_node.parent.mkdir()
        output_node.write('\n'.join(output))


class pclint(lint,Task.Task):
    color = 'CYAN'

    run_str_pclint = '${PCLINT} ${PCLINTOPTS} ${SRC}'
    run_str_cc     = '${CC} ${ARCH_ST:ARCH} ${CFLAGS} ${CPPFLAGS} ${FRAMEWORKPATH_ST:FRAMEWORKPATH} ${CPPPATH_ST:INCPATHS} ${DEFINES_ST:DEFINES} -E -dM -dD -v ${CC_SRC_F}${SRC[0].abspath()}'

    (run_pclint, pclint_vars) = Task.compile_fun(run_str_pclint)
    (run_cc, cc_vars)         = Task.compile_fun(run_str_cc)

    vars = pclint_vars + cc_vars + [
      'PCLINT_OPT_WARNINGLEVEL',
      'PCLINT_OPT_PASSES',
      'PCLINT_OPT_OPTIONS',
      'PCLINT_OPT_ADDITIONALRULES',
    ]

    log_str = '[PCLINT] $name $out'

    sizeflags = [
        ('__SIZEOF_INT__' ,        '-si'),
        ('__SIZEOF_LONG__',        '-sl'),
        ('__SIZEOF_LONG_LONG__',   '-sll'),
        ('__SIZEOF_SHORT__',       '-ss'),
        ('__SIZEOF_FLOAT__',       '-sf'),
        ('__SIZEOF_DOUBLE__',      '-sd'),
        ('__SIZEOF_LONG_DOUBLE__', '-sld'),
        ('__SIZEOF_WCHAR_T__',     '-sw'),
        ('__SIZEOF_POINTER__',     '-sp'),
    ]

    @property
    def name(self):
        return self.generator.name

    @property
    def out(self):
        return self.outputs[1].nice_path()

    def run(self):
        global stdout_sep, stderr_sep

        tgen    = self.generator
        bld     = tgen.bld
        env     = self.env
        inputs  = self.inputs
        outputs = self.outputs

        for x in outputs:
            x.delete()

        source_nodes = inputs[:]

        pclintlnt_template_node = inputs[0]
        pclint_output_lnt_node  = inputs[1]

        pclint_rules_lnt_nodes = [x for x in inputs[2:] if x.suffix().lower() == '.lnt']
        source_nodes           = [x for x in inputs[2:] if x.suffix().lower() != '.lnt']

        pclintlnt_node, output_node = outputs

        defines, includes = self.extract_compiler_params()

        # pclint specific header files
        includes.insert(0, os.path.join(module_path, "pclint_includes"))

        tgenincludes = set(x.abspath() for x in tgen.to_incnodes(getattr(tgen, 'includes', [])))

        dct = dict()

        dct['INCPATHFLAGS'] = '\n'.join('-"i%s"' % x for x in includes)
        dct['LIBPATHFLAGS'] = '\n'.join('-libdir("%s")' % x for x in tgenincludes)

        dct['DEFINEFLAGS'] = '\n'.join(('-d%s=%s' if v[0] in '"' else '-"d%s=%s"') % (n, v) for n, v in defines)

        dct['OUTPUTFORMATFLAGS'] = pclint_output_lnt_node.read()

        defines_map = dict(defines)

        if env['CC_NAME'] in ('gcc', 'clang', 'xpic-llvm'):
            if '__SIZEOF_INT__' in defines_map:
                dct['SIZEFLAGS'] = '\n'.join('%s%s' % (f, defines_map.get(d,'0')) for d,f in self.sizeflags)
            else:
                # probably an old gcc:
                compiler_32bit_sizes = (
                  ('-si',  4),
                  ('-sl',  4),
                  ('-sll', 8),
                  ('-ss',  2),
                  ('-sf',  4),
                  ('-sd',  8),
                  ('-sld', 8),
                  ('-sw',  4),
                  ('-sp',  4),
                )

                dct['SIZEFLAGS'] = '/* Unable to extract type sizes from compiler, using built in 32 bit type size */\n' +\
                                    '\n'.join('%s%d' % x for x in compiler_32bit_sizes)
        else:
            dct['SIZEFLAGS'] = '/* Unable to extract type sizes from compiler "%s" */\n' % env['CC_NAME']

        dct['WARNINGLEVELFLAGS'] = '-w%u' % self.opts.warninglevel
        dct['PASSESFLAGS'] = '-passes(%u)' % self.opts.passes

        sep = '/*{0:*^76}*/\n'
        dct['RULEFLAGS'] = '\n'.join((sep.format(' %s ' % x.nice_path()) + x.read() + '\n' + sep.format(' %s ' % x.nice_path())) for x in pclint_rules_lnt_nodes)

        dct['SOURCEFILES'] = '\n'.join('"%s"' % x.path_from(bld.bldnode) for x in source_nodes)

        env.stash()
        try:
            template_string = string.Template(pclintlnt_template_node.read())

            pclintlnt_node.parent.mkdir()
            pclintlnt_node.write(template_string.substitute(dct))

            try:
                self.inputs = [pclintlnt_node]
                out, err = self.run_pclint()
            except Exception, e:
                out = getattr(e, 'stdout', None)
                err = getattr(e, 'stderr', None)
            finally:
                self.inputs = inputs
        finally:
            env.revert()

        output = [stdout_sep] + out.splitlines() + [stderr_sep] + err.splitlines()

        output_node.parent.mkdir()
        output_node.write('\n'.join(output))

@Task.always_run
class pclint_log(Task.Task):
    '''Generate a pclint log file for the whole project'''
    color   = 'CYAN'
    log_str = '[PCLINTLOG] $out'

    @property
    def out(self):
        return self.outputs[0].nice_path()

    def run(self):
        tgen = self.generator
        bld  = tgen.bld

        inputs = self.inputs[:]

        messages = self.messages = {}
        summary_messages = self.summary_messages = {}
        paths_checked  = set()
        paths_messages = set()

        dct = {}
        version  = None

        passes       = set()
        warninglevel = set()

        class Message(object):
            text = None
            count = 0

        log_template_node = inputs.pop(0)

        try:
            cwd = bld.cwd
        except AttributeError:
            cwd = bld.variant_dir

        def tokenizer(content):
            sep = ''
            for x in content.splitlines():
                x, _, _ = x.partition('//')
                y = ''

                while len(x) > 0:
                    if sep == '/*':
                        _, sep, x = x.partition('*/')

                        if sep == '':
                            sep = '/*'
                        else:
                            sep = ''

                    if sep == '"':
                        y1, sep, x = x.partition('"')
                        y += y1+sep

                        if sep == '':
                            sep = '"'
                        else:
                            sep = ''

                    if sep == '':
                        y1, sep1, x1 = x.partition('/*')
                        y2, sep2, x2 = x.partition('"')

                        if len(y2) < len(y1):
                            y += y2 + sep2
                            sep,x = sep2,x2
                        else:
                            y += y1
                            sep,x = sep1,x1

                y = y.strip()

                if len(y) > 0:
                    yield y

        while inputs:
            lnt_node    = inputs.pop(0)
            report_node = inputs.pop(0)

            content = lnt_node.read().strip()

            for x in tokenizer(content):
                m = re_match_pclint_passes(x)

                if m:
                    passes.add(int(m.group(1),10))

                m = re_match_pclint_warninglevel(x)

                if m:
                    warninglevel.add(int(m.group(1),10))

                # skip all lines which do not look like a file path, this speeds up things a lot
                if x.startswith('-'):
                    continue

                if x.startswith('+'):
                    continue

                path = os.path.abspath(os.path.join(cwd, x.strip('"')))

                if path not in paths_checked:
                    if os.path.isfile(path):
                        paths_checked.add(path)

            content = report_node.read().strip()

            a,dummy,err     = content.partition(stderr_sep)
            dummy,dummy,out = a.partition(stdout_sep)

            out = out.strip()
            err = err.strip()

            stack = out.splitlines()

            # parse out all messages and strip of double messages using
            # set()
            while stack:
                x = stack.pop(0).strip()

                m = re_match_pclint_message(x)
                if m:
                    # Message pattern matched

                    # messages are structured by type in order to print them
                    # later on easily
                    mnumber = int(m.group('Number'),10)
                    mtype   = m.group('Type').strip()
                    mpath   = os.path.abspath(m.group('Path'))

                    paths_checked.add(mpath)
                    paths_messages.add(mpath)

                    try:
                        msgs = messages[mtype]
                    except:
                        msgs = messages[mtype] = set()

                    if x not in msgs:
                        msgs.add(x)

                        try:
                            y = summary_messages[mnumber]
                        except KeyError:
                            y = summary_messages[mnumber] = Message()
                            y.number = mnumber

                        y.count  = y.count + 1

                        b = m.group('Message').strip()
                        if y.text is None:
                            y.text = b
                        else:
                            a = y.text
                            x = 0
                            for x in xrange(min((len(a), len(b)))):
                                if a[x] != b[x]:
                                    break

                            y.text = a[:x]
                else:
                    m = re_match_pclint_summary(x)

                    if m:
                        # summary pattern matches
                        if m.groups() == ('Count', 'Type', 'Number', 'Text'):
                            break

            # parse out summary and further stuff
            while stack:
                x = stack.pop(0).strip()
                m = re_match_pclint_summary(x)

                if m:
                    # summary pattern matches
                    mnumber = int(m.group('Number'),10)

                    y = summary_messages.setdefault(mnumber, Message())
                    y.number = mnumber

                    y.type = m.group('Type').strip()
                    y.text = m.group('Text').strip()

            for x in err.splitlines():
                x = x.strip()
                m = re_match_pclint_version(x)

                if m:
                    if m.group(0) != (version or m.group(0)):
                        bld.fatal('PClint version mismatch detected')

                    version = m.group(0)

        dct['PCLINTVERSIONSTRING'] = version or 'Unkown'
        dct['PCLINTOPTIONSTRING'] = ''

        if len(warninglevel) > 1:
            dct['PCLINTOPTIONSTRING'] += 'Warning Level: %d to %d\n' % (min(warninglevel), max(warninglevel))
        else:
            dct['PCLINTOPTIONSTRING'] += 'Warning Level: %d\n' % (min(warninglevel))

        if len(passes) > 1:
            dct['PCLINTOPTIONSTRING'] += 'Passes: %d to %d\n' % (min(passes), max(passes))
        else:
            dct['PCLINTOPTIONSTRING'] += 'Passes: %d\n' % (min(passes))

        # these variables shall not be used anymore
        dct['PROJECT'] = getattr(Context.g_module,'APPNAME','')
        dct['VERSION'] = getattr(Context.g_module,'VERSION','')

        lst = self.lst_summary = []
        fmt = '{0:6}  {1:14} {2:6}  {3}'.format

        if summary_messages:
            lst.extend([
                fmt('Count', 'Type', 'Number', 'Text'),
                ''
            ])

            fmt = '{0:>6}  {1:14} {2:>6}  {3}'.format

            for number in sorted(summary_messages.keys()):
                x = summary_messages[number]

                #type  = x.type
                type  = self.get_warning_type(number)
                text  = x.text
                count = x.count

                lst.append(fmt(count,type,number,text))

        dct['PCLINTSUMMARY'] = u'\n'.join(lst)

        maxpath = 0

        details = []
        selectors = 'Path Line Column Type Number Message'.split()

        for x in reduce(operator.__or__, messages.values(), set()):
            m = re_match_pclint_message(x)

            z = Message()

            path, line, z.column, z.type, number, z.text = m.groups(selectors)

            z.line   = int(line,10)
            z.number = int(number,10)

            try:
                z.path = os.path.relpath(path, bld.srcnode.abspath())
            except ValueError:
                z.path = os.path.abspath(path)

            maxpath = max(maxpath, len(z.path))
            details.append(z)

        def cmp_details(a,b):
            r = cmp(a.path,b.path)
            r = r or cmp(a.line,b.line)
            r = r or cmp(a.column,b.column)
            r = r or cmp(a.number,b.number)
            return r

        details = sorted(details,cmp = cmp_details)

        lst = []

        fmt = (u'{0:<%d} {1:>5}:{2:<3} {3:14} {4:>6}  {5}' % (maxpath)).format
        lst.append(fmt(*u'Path Row Col Type Number Text'.split()))
        lst.append(u'')

        fmt = (u'{0:<%d} {1:>5}:{2:<3} {3:14} {4:>6}  {5}' % (maxpath)).format
        for x in details:
            lst.append(fmt(x.path, x.line, x.column, self.get_warning_type(x.number), x.number, x.text))

        dct['PCLINTDETAILS'] = u'\n'.join(lst)

        dct['PCLINTPASSED'] = u'\n'.join(sorted(paths_checked - paths_messages))

        template_string = string.Template(log_template_node.read())
        self.outputs[0].parent.mkdir()
        self.outputs[0].write(template_string.substitute(dct))

    def get_warning_type(self,number):
        if number < 200:
            return 'Syntax Error'
        elif number < 300:
            return 'Internal Error'
        elif number < 400:
            return 'Fatal Error'
        elif number < 700:
            return 'Warning'
        elif number < 900:
            return 'Informational'
        elif number < 1000:
            return 'Elective Note'
        elif number < 9000:
            return 'Unknown'
        elif number < 9199:
            return 'Elective Note'
        else:
            return 'Unknown'



class pclint_jenkins_report(Task.Task):
    '''Generate a report file for jenkins to generate a plot'''

    def run(self):
        messages = {}
        summary = {}

        for log_node in self.inputs:
            content = log_node.read().strip()

            a, dummy, err = content.partition(stderr_sep)
            dummy, dummy, out = a.partition(stdout_sep)

            out = out.strip()
            err = err.strip()

            stack = out.splitlines()

            # parse out all messages and strip of double messages using
            # set()
            while stack:
                x = stack.pop(0).strip()

                m = re_match_pclint_message(x)
                if m:
                    # Message pattern matched

                    # messages are structured by type in order to print them
                    # later on easily
                    try:
                        msgs = messages[m.group('Type')]
                    except:
                        msgs = messages[m.group('Type')] = set()

                    msgs.add(x)
                else:
                    m = re_match_pclint_summary(x)

                    if m:
                        # summary pattern matches
                        if m.groups() == ('Count', 'Type', 'Number', 'Text'):
                            break

            # parse out summary
            while stack:
                x = stack.pop(0).strip()
                m = re_match_pclint_summary(x)

                if m:
                    # summary pattern matches
                    key = m.group('Type').strip(), int(m.group('Number'), 10), m.group('Text').strip()
                    count = int(m.group('Count'), 10)

                    summary[key] = count + summary.get(key, 0)

        dct = { 'Error' : 0, 'Warning' : 0, 'Info' : 0, 'Note' : 0}

        for (typ, number,text), count in summary.items():
            dct[typ] = dct[typ] + count

        content = ""

        columns = sorted(dct.keys())

        content += ','.join(columns) + '\n'
        content += ','.join('%d' % dct[k] for k in columns) + '\n'

        self.outputs[0].write(content)

class SplintContext(Build.BuildContext):
  '''Build & run splint'''
  cmd = 'splint'

class PClintContext(Build.BuildContext):
  '''Build & run splint'''
  cmd = 'pclint'
