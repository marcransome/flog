project('flog', 'c',
  version : '1.5.0',
  default_options : ['warning_level=3'])

poptdep = dependency('popt', version : '>=1.19')

executable('flog',
           'utils.c',
           'flog.c',
           'main.c',
           'config.c',
           dependencies : poptdep,
           install : true)
