Usage:
------
1. Place scripts in subfolder static_codeanalysis
2. Load appropriate script in main wscript

    *NOTE:* If main wscript contains conf.autorecurse() in functions configure and options, this step is not necessary

    * hilscher_lint

    Sample:
    -------
      def options(ctx):
        ...
        ctx.load('hilscher_lint', tooldir='static_codeanalysis')

      def configure(conf):
        ...
        conf.load('hilscher_lint', tooldir='static_codeanalysis')
       

3. Make sure the appropriate static code analyser is in your path
4. run "waf configure" and check if the analyzer was found
    Sample output:
    --------------
      waf configure
        Checking for program cppcheck              : C:\Program Files (x86)\Cppcheck\cppcheck.exe
        Checking for program splint                : D:\splint-3.1.2\bin\splint.exe
        Checking for program lint-nt               : C:\Program Files (x86)\PC-lint 9.0\lint-nt.exe
5. Run appropriate checker via waf
    * waf pclint    (for PC-Lint)
    * waf splint    (for Splint)

    NOTE1: before checking the targets will be compiled. If compilation fails no check is done.
    NOTE2: You can check single targets using "waf --targets=targets <checker>"

    Sample:
    -------
    waf --targets=arm-none-eabi/4.5.2/rcX_V2.1/rcx pclint

6. Check the log files named "<targetname>_<tool>.log" under build/pclint directory
    Sample:
    ------
      arm-none-eabi/4.5.2/rcX_V2.1/rcx -> d:\Projects\rcX\OS\V2.1_trunk\build\rcx_cppcheck.log

Additional PC-Lint options:
---------------------------

  --pclint-warninglevel=<x>
  + set the warning level to x
  + range: 0 to 4
  + default: 2
  
  --pclint-passes=<x>
  + set the number of passes to x
  + range: 1 to 4
  + default: 2

  --static-analysis-console=<x>
  + configure reporting PClint findings to console
  + default: errors_else_warnings
  + following output options can be used:
    + all                   - all messages are output
    + errors_else_warnings  - only errors and warnings
    + off                   - no output
  + output: CDT build console

  --pclint-options=<x>
  + configure inclusion of options.lnt files located near wscript files
  + default: none
  + following inclusion options can be used:
     + global           - inclusion of options.lnt in the repository of the root-wscript file
     + global_and_local - inclusion of options.lnt in the repository of the root-wscript or target-/component-wscript file
     + local            - inclusion of options.lnt in the repository of the target-/component-wscript file
     + none             - no inclusion of options.lnt

  * You can place a file 'options.lnt' besides any wscript which is used for all targets defined in this wscript
      Example:
        Targets
          + - netX51
                + - wscript
                + - options.lnt -> this file will be included for all lint operations for targets defined in the above wscript

  * You can place a file 'options.lnt' besides the root-wscript which is used for all targets in this project (all sub-wscripts)
      Example:
        wscript
        options.lnt -> this file will be included for all lint operations for all targets in all sub-wscripts
        Targets
          + - netX51
                + - wscript

  --pclint-additional-rules=<x>
  + enable additional rule
  + multiple rules are not supported
  + following rules can be used:
    + MISRAC1998 - Enable MISRA C 1998 checks
    + MISRAC2004 - Enable MISRA C 2004 checks
    + MISRAC2012 - Enable MISRA C 2012 checks
    + BARREMBC   - Enable "The Barr Group's Bug-Killing Coding - Standard Rules for Embedded C" checks

  --pclint-jenkins-report
  + enables the generation of a jenkins report file suitable for use with jenkins plot plugin
  + output: .\build\pclint\jenkins_report.csv