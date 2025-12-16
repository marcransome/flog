// MIT License
//
// Copyright (c) 2024 Marc Ransome
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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>

#include "common.h"

#define UNUSED(x) (void)(x)

#define ERROR_STRING_LEN 64
#define STDOUT_BUFF_SIZE 1024
#define STDERR_BUFF_SIZE 1024

static FILE *saved_stdout = NULL;
static FILE *saved_stderr = NULL;

static int
capture_stdout(void **state) {
    UNUSED(state);

    saved_stdout = stdout;

    char *stdout_buff = calloc(1, STDOUT_BUFF_SIZE);
    if (stdout_buff == NULL) {
        return 1;
    }

    stdout = fmemopen(stdout_buff, STDOUT_BUFF_SIZE, "w");
    if (stdout == NULL) {
        return 1;
    }

    *state = stdout_buff;

    return 0;
}

static int
restore_stdout(void **state) {
    UNUSED(state);

    fclose(stdout);
    stdout = saved_stdout;
    free(*state);

    return 0;
}

static int
capture_stderr(void **state) {
    UNUSED(state);

    saved_stderr = stderr;

    char *stderr_buff = calloc(1, STDERR_BUFF_SIZE);
    if (stderr_buff == NULL) {
        return 1;
    }

    stderr = fmemopen(stderr_buff, STDERR_BUFF_SIZE, "w");
    if (stderr == NULL) {
        return 1;
    }

    *state = stderr_buff;

    return 0;
}

static int
restore_stderr(void **state) {
    UNUSED(state);

    fclose(stderr);
    stderr = saved_stderr;
    free(*state);

    return 0;
}

static void
flog_usage_succeeds(void **state) {
    UNUSED(state);

    char expected_string[1024] = {0};
    sprintf(expected_string,
        "%s v%d.%d.%d\n"
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
        PROGRAM_VERSION_MAJOR,
        PROGRAM_VERSION_MINOR,
        PROGRAM_VERSION_PATCH,
        PROGRAM_NAME
    );

    flog_usage();

    assert_string_equal(*state, expected_string);
}

static void
flog_version_succeeds(void **state) {
    UNUSED(state);

    char expected_string[ERROR_STRING_LEN] = {0};
    sprintf(expected_string, "%s v%d.%d.%d\n",
        PROGRAM_NAME,
        PROGRAM_VERSION_MAJOR,
        PROGRAM_VERSION_MINOR,
        PROGRAM_VERSION_PATCH
    );

    flog_version();

    assert_string_equal(*state, expected_string);
}

static void
flog_error_string_none_succeeds(void **state) {
    UNUSED(state);

    const char *msg = flog_error_string(FLOG_ERROR_NONE);

    assert_string_equal(msg, "none");
}

static void
flog_error_string_alloc_succeeds(void **state) {
    UNUSED(state);

    const char *msg = flog_error_string(FLOG_ERROR_ALLOC);

    assert_string_equal(msg, "allocation failed");
}

static void
flog_error_string_append_succeeds(void **state) {
    UNUSED(state);

    const char *msg = flog_error_string(FLOG_ERROR_APPEND);

    assert_string_equal(msg, "unable to append log message to file");
}

static void
flog_error_string_lvl_succeeds(void **state) {
    UNUSED(state);

    const char *msg = flog_error_string(FLOG_ERROR_LVL);

    assert_string_equal(msg, "unknown log level");
}

static void
flog_error_string_msg_succeeds(void **state) {
    UNUSED(state);

    const char *msg = flog_error_string(FLOG_ERROR_MSG);

    assert_string_equal(msg, "message string required");
}

static void
flog_error_string_opts_succeeds(void **state) {
    UNUSED(state);

    const char *msg = flog_error_string(FLOG_ERROR_OPTS);

    assert_string_equal(msg, "invalid options");
}

static void
flog_error_string_subsys_succeeds(void **state) {
    UNUSED(state);

    const char *msg = flog_error_string(FLOG_ERROR_SUBSYS);

    assert_string_equal(msg, "category option requires subsystem option to be set");
}

static void
flog_error_string_file_succeeds(void **state) {
    UNUSED(state);

    const char *msg = flog_error_string(FLOG_ERROR_FILE);

    assert_string_equal(msg, "file path too long");
}

static void
flog_print_error_none_succeeds(void **state) {
    UNUSED(state);

    char expected_string[ERROR_STRING_LEN] = {0};
    sprintf(expected_string, "%s: none\n", PROGRAM_NAME);

    flog_print_error(FLOG_ERROR_NONE);

    assert_string_equal(*state, expected_string);
}

static void
flog_print_error_alloc_succeeds(void **state) {
    UNUSED(state);

    char expected_string[ERROR_STRING_LEN] = {0};
    sprintf(expected_string, "%s: allocation failed\n", PROGRAM_NAME);

    flog_print_error(FLOG_ERROR_ALLOC);

    assert_string_equal(*state, expected_string);
}

static void
flog_print_error_append_succeeds(void **state) {
    UNUSED(state);

    char expected_string[ERROR_STRING_LEN] = {0};
    sprintf(expected_string, "%s: unable to append log message to file\n", PROGRAM_NAME);

    flog_print_error(FLOG_ERROR_APPEND);

    assert_string_equal(*state, expected_string);
}

static void
flog_print_error_lvl_succeeds(void **state) {
    UNUSED(state);

    char expected_string[ERROR_STRING_LEN] = {0};
    sprintf(expected_string, "%s: unknown log level\n", PROGRAM_NAME);

    flog_print_error(FLOG_ERROR_LVL);

    assert_string_equal(*state, expected_string);
}

static void
flog_print_error_msg_succeeds(void **state) {
    UNUSED(state);

    char expected_string[ERROR_STRING_LEN] = {0};
    sprintf(expected_string, "%s: message string required\n", PROGRAM_NAME);

    flog_print_error(FLOG_ERROR_MSG);

    assert_string_equal(*state, expected_string);
}

static void
flog_print_error_opts_succeeds(void **state) {
    UNUSED(state);

    char expected_string[ERROR_STRING_LEN] = {0};
    sprintf(expected_string, "%s: invalid options\n", PROGRAM_NAME);

    flog_print_error(FLOG_ERROR_OPTS);

    assert_string_equal(*state, expected_string);
}

static void
flog_print_error_subsys_succeeds(void **state) {
    UNUSED(state);

    char expected_string[ERROR_STRING_LEN] = {0};
    sprintf(expected_string, "%s: category option requires subsystem option to be set\n", PROGRAM_NAME);

    flog_print_error(FLOG_ERROR_SUBSYS);

    assert_string_equal(*state, expected_string);
}

static void
flog_print_error_file_succeeds(void **state) {
    UNUSED(state);

    char expected_string[ERROR_STRING_LEN] = {0};
    sprintf(expected_string, "%s: file path too long\n", PROGRAM_NAME);

    flog_print_error(FLOG_ERROR_FILE);

    assert_string_equal(*state, expected_string);
}

int main(void) {
    cmocka_set_message_output(CM_OUTPUT_TAP);

    const struct CMUnitTest tests[] = {
        // flog_usage() success tests
        cmocka_unit_test_setup_teardown(flog_usage_succeeds, capture_stdout, restore_stdout),

        // flog_version() success tests
        cmocka_unit_test_setup_teardown(flog_version_succeeds, capture_stdout, restore_stdout),

        // flog_error_string() success tests
        cmocka_unit_test(flog_error_string_none_succeeds),
        cmocka_unit_test(flog_error_string_alloc_succeeds),
        cmocka_unit_test(flog_error_string_append_succeeds),
        cmocka_unit_test(flog_error_string_lvl_succeeds),
        cmocka_unit_test(flog_error_string_msg_succeeds),
        cmocka_unit_test(flog_error_string_opts_succeeds),
        cmocka_unit_test(flog_error_string_subsys_succeeds),
        cmocka_unit_test(flog_error_string_file_succeeds),

        // flog_print_error() success tests
        cmocka_unit_test_setup_teardown(flog_print_error_none_succeeds, capture_stderr, restore_stderr),
        cmocka_unit_test_setup_teardown(flog_print_error_alloc_succeeds, capture_stderr, restore_stderr),
        cmocka_unit_test_setup_teardown(flog_print_error_append_succeeds, capture_stderr, restore_stderr),
        cmocka_unit_test_setup_teardown(flog_print_error_lvl_succeeds, capture_stderr, restore_stderr),
        cmocka_unit_test_setup_teardown(flog_print_error_msg_succeeds, capture_stderr, restore_stderr),
        cmocka_unit_test_setup_teardown(flog_print_error_opts_succeeds, capture_stderr, restore_stderr),
        cmocka_unit_test_setup_teardown(flog_print_error_subsys_succeeds, capture_stderr, restore_stderr),
        cmocka_unit_test_setup_teardown(flog_print_error_file_succeeds, capture_stderr, restore_stderr),
    };

    return cmocka_run_group_tests_name("Common function tests", tests, NULL, NULL);
}
