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

#include "common.h"
#include <stdio.h>

#ifdef UNIT_TESTING
#include "../test/testing.h"
#endif

static const char *
flog_error_map[] = {
    [FLOG_ERROR_NONE]   = "none",
    [FLOG_ERROR_ALLOC]  = "allocation failed",
    [FLOG_ERROR_APPEND] = "unable to append log message to file",
    [FLOG_ERROR_LVL]    = "unknown log level",
    [FLOG_ERROR_MSG]    = "message string required",
    [FLOG_ERROR_SUBSYS] = "category option requires subsystem option to be set",
    [FLOG_ERROR_OPTS]   = "invalid options",
    [FLOG_ERROR_FILE]   = "file path too long",
    [FLOG_ERROR_STAT]   = "unable to determine state of stdin file descriptor",
};

const char *
flog_error_string(FlogError error) {
    return flog_error_map[error];
}

void
flog_print_error(FlogError error) {
    fprintf(stderr, "%s: %s\n", PROGRAM_NAME, flog_error_string(error));
}

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
        "    -a, --append <path>      Append the log message to a file (creating it if necessary)\n"
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
