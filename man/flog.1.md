% flog(1) Version v1.7.0 | Flog User's Guide

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

OPTION ALIASING
===============

*flog* supports option aliasing via the `libpopt` library (see popt(3) for more information). An alias is an arbitrary option name which expands to one or more command-line options, making repeat operations less verbose and allowing for sets of options to be grouped contextually by name.

User-defined option aliases can be added to the files **/etc/popt** and **$HOME/.popt**, both of which can contain an arbitrary number of aliases. Each alias should be formatted as:

uk.co.fidgetbox.flog alias _alias-name_ _options_

Where _alias-name_ is the user-defined option alias name and _options_ is one or more command-line options.

_Example:_ The following entry creates an option alias **\--runtime-failure** which expands to the options **-l fault -s uk.co.fidgetbox.server -c runtime**:

    uk.co.fidgetbox.flog alias --runtime--failure -l fault -s uk.co.fidgetbox.server -c runtime

EXAMPLES
========

To log a message using the default log level:

    flog 'batch.sh script success'

To log a message with a specific log level, subsystem, and category:

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
