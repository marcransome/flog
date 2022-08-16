# flog

`flog` is a command-line tool for sending log messages to Apple's unified logging system and is primaily intended for use in scripts.


## Getting started

### Installation

`flog` is currently a _work-in-progress_, however a fully functional _pre-release_ of version `1.0.0` is available for [download](https://github.com/marcransome/flog/releases/tag/1.0.0-pre-release) as a universal macOS binary with support for both Intel `x86_64` and Apple `arm64e` architectures.

Please note that pre-release versions are _not_ code signed or notarized and will require the removal of the quarantine attribute before they can be run:

```bash
$ tar -xvzf flog-1.0.0-pre-release.tar.gz
$ xattr -d com.apple.quarantine flog
$ ./flog
...
```

### Logging messages

To log a message using `default` log level:

```bash
flog '<message>'
```

Optionally specify a _subsystem_ and/or _category_ using the `-s|--subsystem` and `-c|--category` options:

```bash
flog -s uk.co.fidgetbox -c general 'informational message'
```

Override the default log level using the `-l|--level` option and one of the values `info`, `debug`, `error` or `fault`:

```bash
flog -l fault -s uk.co.fidgetbox -c general 'unrecoverable failure'
```

:warning: Log message strings are _public_ by default and can be accessed using the `log(1)` command or [Console](https://support.apple.com/en-gb/guide/console/welcome/mac) app. To mark a message as private add the `--private` option to the command. Doing so will redact the message string, and this will display as `'<private>'` when accessed using the previously mentioned methods. [Device Management Profiles](https://developer.apple.com/documentation/devicemanagement/systemlogging) can be used to grant access to private log messages.

### Reading log messages

Refer to the `log(1)` man page provided on macOS-based system for extensive documentation on how to access system wide log messages, or alternatively use the [Console](https://support.apple.com/en-gb/guide/console/welcome/mac) app.

## Building

### Dependencies

* A C17 compiler
* CMake (version 3.22 or later)

### Building from source

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
