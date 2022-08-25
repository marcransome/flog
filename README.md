<img alt="flog" src="images/trunk.png" width="180" align="right">

# flog

[![Issues](https://img.shields.io/github/issues/marcransome/flog)](https://github.com/marcransome/flog/issues) [![License](https://img.shields.io/badge/license-MIT-blue)](https://opensource.org/licenses/mit-license.php) [![macOS](https://img.shields.io/badge/macOS-11+-blue)](https://www.apple.com/macos/)

`flog` is a command-line tool for sending log messages to Apple's unified logging system and is primaily intended for use in scripts.

<hr>

## Getting started

### Installation

Install with [Homebrew](https://brew.sh):

```bash
brew install marcransome/tap/flog
```

### Logging messages

To log a message using the `default` log level:

```bash
flog '<message>'
```

Optionally specify a _subsystem_ and _category_ using the `-s|--subsystem` and `-c|--category` options:

```bash
flog -s uk.co.fidgetbox -c general 'informational message'
```

Override the default log level using the `-l|--level` option and one of the values `info`, `debug`, `error` or `fault`:

```bash
flog -l fault -s uk.co.fidgetbox -c general 'unrecoverable failure'
```

> **Warning**:
> Log message strings are _public_ by default and can be read using the `log(1)` command or [Console](https://support.apple.com/en-gb/guide/console/welcome/mac) app. To mark a message as private add the `-p|--private` option to the command. Doing so will redact the message string, which will be shown as `'<private>'` when accessed using the methods previously mentioned. [Device Management Profiles](https://developer.apple.com/documentation/devicemanagement) can be used to grant access to private log messages.

### Reading log messages

Refer to the `log(1)` man page provided on macOS-based systems for extensive documentation on how to access system wide log messages, or alternatively use the [Console](https://support.apple.com/en-gb/guide/console/welcome/mac) app.

Here's an example log stream in Console, filtered by subsystem name, showing messages generated by `flog`:

<img width="1004" alt="Console" src="https://user-images.githubusercontent.com/679401/185230991-25283ef6-8d90-4bec-9b1f-8235d436f1aa.png">

And here's a similar log stream viewed with Apple's `log(1)` command:

<img width="995" alt="log" src="https://user-images.githubusercontent.com/679401/186741971-7ae79199-c3a0-4390-a9cd-93e33e1ba649.png">


## Building

### Requirements

* macOS (`11.x` or later)
* A C17 compiler
* CMake (`3.22` or later)

### Building from source

To perform an out-of-source build from the project directory:

```bash
cmake -S . -B build
cmake --build build
```

## Acknowledgements

* Trunk icon made by [Freepik](https://www.flaticon.com/authors/freepik) from [www.flaticon.com](https://www.flaticon.com/)

## License

`flog` is provided under the terms of the [MIT License](https://opensource.org/licenses/mit-license.php).

## Contact

Email me at [marc.ransome@fidgetbox.co.uk](mailto:marc.ransome@fidgetbox.co.uk) or tweet [@marcransome](http://www.twitter.com/marcransome).
