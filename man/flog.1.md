% flog(1) Version v1.6.4 | Flog User's Guide

NAME
====

**flog** - write log messages to the unified logging system

SYNOPSIS
========

| **flog** [*options*] _message_

DESCRIPTION
===========

*flog* is used to write log messages to the unified logging system. Log messages may include a _subsystem_ and _category_ name for the purposes of filtering, or to customise the logging behaviour of a subsystem; see log(1) for more information. Specify a log level with the **-l,** **\--level** option to override the 'default' level if necessary. Wrap the _message_ string in quotes to preserve spacing.

Options
-------

**-h,** **\--help**

:   Print brief help information

**-v,** **\--version**

:   Print the current version string

**-s,** **\--subsystem**

:   Specify an optional subsystem name. This should be the identifier of the subsystem in reverse DNS form.

**-c,** **\--category**

:   Specify an optional category name. This option requires the **-s,** **\--subsystem** option.

**-l,** **\--level** _name_

:   Set the log level. Supported values: default, info, debug, error, or fault. The default log level is 'default'.

**-a,** **\--append** _file_

:   Append the log message to a file after sending it to the unified logging system, creating the file if necessary.

**-p,** **\--private**

:   Mark the log message as private. Log message strings are public by default and can be viewed with the log(1) command or Console app. If the **-p,** **\--private** option is used the message string will be redacted and display as '\<private\>'. Device Management Profiles can be used to grant access to private log messages.

EXAMPLES
========

    flog 'batch.sh script success'
    flog -l fault -s uk.co.fidgetbox.scm -c config 'invalid configuration provided'


EXIT STATUS
===========

**flog** exits 0 on success, and >0 if an error occurs.

BUGS
====

See GitHub Issues: https://github.com/marcransome/flog/issues

AUTHOR
======

Marc Ransome <marc.ransome@fidgetbox.co.uk>

SEE ALSO
========

log(1), os\_log(3)
