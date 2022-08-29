// MIT License
//
// Copyright (c) 2022 Marc Ransome
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "utils.h"
#include "defs.h"
#include <stdio.h>

void
flog_usage(void) {
    printf(
        "%s %s\n"
        "\n"
        "Usage:\n"
        "    %s [options] message\n"
        "\n"
        "Help Options:\n"
        "    -h, --help       Show this help message\n"
        "    -v, --version    Print the version string\n"
        "\n"
        "Application Options:\n"
        "    -s, --subsystem <name>   Specify a subsystem name\n"
        "    -c, --category <name>    Specify a category name (requires subsystem option)\n"
        "    -l, --level <level>      Specify the log level ('default' if not provided)\n"
        "    -p, --private            Mark the log message as private\n"
        "\n"
        "Log Levels:\n"
        "    default, info, debug, error, fault\n"
        "\n",
        PROGRAM_NAME,
        PROGRAM_VERSION,
        PROGRAM_NAME
    );
}

void
flog_version(void) {
    printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
}
