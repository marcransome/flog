# flog

`flog` is a command-line tool for sending log messages to Apple's unified logging system and is primaily intended for use in scripts.


## Getting started

### Installation

`flog` is currenttly a _work-in-progress_ and no binary packages are provided at present. See the [Building](#building) section if you would like to use it before these are made available.

### Logging messages

To log a message using the default `info` log level:

```bash
flog -m '<message>'
```

Optionally specify a _subsystem_ and/or _category_ using the `-s|--subsystem` and `-c|--category` options:

```bash
flog -s uk.co.fidgetbox -c general -m 'informational message'
```

Override the default log level using the `-l|--level` option and one of the values `info`, `debug`, `error` or `fault`:

```bash
flog -l fault -s uk.co.fidgetbox -c general -m 'unrecoverable failure'
```

:warning: Log message strings are _public_ by default and can be accessed using the `log(1)` command or [Console](https://support.apple.com/en-gb/guide/console/welcome/mac) app. To mark a message as private add the `--private` option to the command. Doing so will redact the message string, and this will display as `'<private>'` when accessed using the previously mentioned methods. [Device Management Profiles](https://developer.apple.com/documentation/devicemanagement/systemlogging) can be used to grant access to private log messages.

### Reading log messages

Refer to the `log(1)` man page provided on macOS-based system for extensive documentation on how to access system wide log messages, or alternatively use the [Console](https://support.apple.com/en-gb/guide/console/welcome/mac) app.

## Building

### Dependencies

* A C17 compiler
* CMake (version 3.22 or later)

## Building from source

From the top-level project directory:

```shell
cd src
mkdir build
cd build
cmake ..
cmake --build .
```

## License

`flog` is provided under the terms of the [MIT License](https://opensource.org/licenses/mit-license.php).

## Contact

Email me at [marc.ransome@fidgetbox.co.uk](mailto:marc.ransome@fidgetbox.co.uk) or tweet [@marcransome](http://www.twitter.com/marcransome).
