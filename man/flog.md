% flog(1) Version 1.0.0 | Flog User's Guide

NAME
====

**flog** — write log messages to the unified logging system

SYNOPSIS
========

| **flog** [*options*] _message_

DESCRIPTION
===========

*flog* is used to write log messages to the unified logging system. Log messages may optionally include _subsystem_ and _category_ arguments.

Wrap the _message_ string in quotes to preserve spacing, otherwise multiple arguments will be separated by a single space character.

Options
-------

**-h,** **\--help**

:   Print brief usage information

**-v,** **\--version**

:   Print the current version string

**-s,** **\--subsystem**

:   Specify an optional subsystem argument. This should be the identifier of the subsystem in reverse DNS form.

**-c,** **\--category**

:   Specify an optional category within the subsystem, for differentiating by topic and settings.

**-l,** **\--level** _name_

:   Set the log level. Supported values: default, info, debug, error, or fault. Tbe default log level for messages is 'default'.

**-p,** **\--private**

:   Mark the log message as private. Log message strings are public by default and can be viewed with the log(1) command or Console app. If the **-p,** **--private** option is used the message string will be redacted and display as '<private>'. Device Management Profiles can be used to grant access to private log messages. 

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
