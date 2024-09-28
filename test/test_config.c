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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <sys/syslimits.h>
#include "config.h"
#include "common.h"

#define TEST_PROGRAM_NAME "flog"
#define TEST_MESSAGE "test message"
#define TEST_CATEGORY "category"
#define TEST_SUBSYSTEM "subsystem"
#define TEST_OUTPUT_FILE "output_file"

#define TEST_OPTION_VERSION_SHORT "-v"
#define TEST_OPTION_VERSION_LONG "--version"

#define TEST_OPTION_HELP_SHORT "-h"
#define TEST_OPTION_HELP_LONG "--help"

#define TEST_OPTION_SUBSYSTEM_SHORT "-s"
#define TEST_OPTION_SUBSYSTEM_LONG "--subsystem"

#define TEST_OPTION_CATEGORY_SHORT "-c"
#define TEST_OPTION_CATEGORY_LONG "--category"

#define TEST_OPTION_LEVEL_SHORT "-l"
#define TEST_OPTION_LEVEL_LONG "--level"

#define TEST_OPTION_LEVEL_VALUE_DEFAULT "default"
#define TEST_OPTION_LEVEL_VALUE_INFO "info"
#define TEST_OPTION_LEVEL_VALUE_DEBUG "debug"
#define TEST_OPTION_LEVEL_VALUE_ERROR "error"
#define TEST_OPTION_LEVEL_VALUE_FAULT "fault"
#define TEST_OPTION_LEVEL_VALUE_UNKNOWN "unknown"

#define TEST_OPTION_APPEND_SHORT "-a"
#define TEST_OPTION_APPEND_LONG "--append"

#define TEST_OPTION_PRIVATE_SHORT "-p"
#define TEST_OPTION_PRIVATE_LONG "--private"

#define TEST_OPTION_INVALID_SHORT "-i"
#define TEST_OPTION_INVALID_LONG "--invalid"

#define TEST_PATH "/tmp/test-file"

#define TEST_ERROR 255

#define TEST_CHAR 'x'

#define MOCK_ARGS(...) \
    char *mock_argv[] = {__VA_ARGS__, NULL}; \
    int mock_argc = (sizeof(mock_argv) / sizeof(mock_argv[0]) - 1);

#define UNUSED(x) (void)(x)

static void
flog_config_new_with_zero_arg_count_fails(void **state) {
    UNUSED(state);

    FlogError error;        // should pass non-null assertion (&error)
    int mock_argc = 0;      // should fail >1 assertion
    char *mock_argv[] = {}; // should pass non-null assertion

    expect_assert_failure(flog_config_new(mock_argc, mock_argv, &error));
}

static void
flog_config_new_with_null_arg_values_fails(void **state) {
    UNUSED(state);

    FlogError error;         // should pass non-null assertion (&error)
    int mock_argc = 2;       // should pass >1 assertion
    char **mock_argv = NULL; // should fail non-null assertion

    expect_assert_failure(flog_config_new(mock_argc, mock_argv, &error));
}

static void test_new_config_with_no_error_ptr_calls_assert(void **state) {
    UNUSED(state);

    int mock_argc = 2;      // should pass >1 assertion
    char *mock_argv[] = {}; // should pass non-null assertion

    expect_assert_failure(flog_config_new(mock_argc, mock_argv, NULL));
}

static void
flog_config_new_with_short_invalid_opt_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_INVALID_SHORT,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_null(config);
    assert_int_equal(error, FLOG_ERROR_OPTS);
}

static void
flog_config_new_with_long_invalid_opt_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_INVALID_LONG,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_null(config);
    assert_int_equal(error, FLOG_ERROR_OPTS);
}

static void
flog_config_new_with_short_category_opt_and_no_subsystem_opt_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_CATEGORY_SHORT,
        TEST_CATEGORY,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_null(config);
    assert_int_equal(error, FLOG_ERROR_SUBSYS);
}

static void
flog_config_new_with_long_category_opt_and_no_subsystem_opt_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_CATEGORY_LONG,
        TEST_CATEGORY,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_null(config);
    assert_int_equal(error, FLOG_ERROR_SUBSYS);
}

static void flog_config_with_short_subsystem_opt_and_no_subsystem_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    int mock_argc = 2;
    char *mock_argv[] = {
        TEST_PROGRAM_NAME,
        TEST_OPTION_SUBSYSTEM_SHORT
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_null(config);
    assert_int_equal(error, FLOG_ERROR_OPTS);
}

static void flog_config_with_long_subsystem_opt_and_no_subsystem_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    int mock_argc = 2;
    char *mock_argv[] = {
        TEST_PROGRAM_NAME,
        TEST_OPTION_SUBSYSTEM_LONG
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_null(config);
    assert_int_equal(error, FLOG_ERROR_OPTS);
}

static void
flog_config_new_with_stdin_tty_and_no_message_args_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_SHORT,
        TEST_OPTION_LEVEL_VALUE_INFO
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_null(config);
    assert_int_equal(error, FLOG_ERROR_MSG);
}

static void
flog_config_new_with_short_level_opt_and_unknown_value_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_SHORT,
        TEST_OPTION_LEVEL_VALUE_UNKNOWN,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_null(config);
    assert_int_equal(error, FLOG_ERROR_LVL);
}

static void
flog_config_new_with_long_level_opt_and_unknown_value_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_LONG,
        TEST_OPTION_LEVEL_VALUE_UNKNOWN,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_null(config);
    assert_int_equal(error, FLOG_ERROR_LVL);
}

static void
flog_config_new_with_short_append_opt_and_long_path_fails(void **state) {
    UNUSED(state);

    char *path = malloc(PATH_MAX + 1);
    memset(path, TEST_CHAR, PATH_MAX);
    path[PATH_MAX] = '\0';

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_APPEND_SHORT,
        path,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_null(config);
    assert_int_equal(error, FLOG_ERROR_FILE);

    free(path);
}

static void
flog_config_new_with_long_append_opt_and_long_path_fails(void **state) {
    UNUSED(state);

    char *path = malloc(PATH_MAX + 1);
    memset(path, TEST_CHAR, PATH_MAX);
    path[PATH_MAX] = '\0';

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_APPEND_LONG,
        path,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_null(config);
    assert_int_equal(error, FLOG_ERROR_FILE);

    free(path);
}

static void
flog_config_new_with_message_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);

    flog_config_free(config);
}

static void
flog_config_new_with_short_subsystem_opt_and_message_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_SUBSYSTEM_SHORT,
        TEST_SUBSYSTEM,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);

    flog_config_free(config);
}

static void
flog_config_new_with_long_subsystem_opt_and_message_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_SUBSYSTEM_LONG,
        TEST_SUBSYSTEM,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);

    flog_config_free(config);
}


static void
flog_config_new_with_short_level_opt_and_default_value_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_SHORT,
        TEST_OPTION_LEVEL_VALUE_DEFAULT,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_level(config), LVL_DEFAULT);

    flog_config_free(config);
}

static void
flog_config_new_with_short_level_opt_and_info_value_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_SHORT,
        TEST_OPTION_LEVEL_VALUE_INFO,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_level(config), LVL_INFO);

    flog_config_free(config);
}

static void
flog_config_new_with_short_level_opt_and_debug_value_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_SHORT,
        TEST_OPTION_LEVEL_VALUE_DEBUG,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_level(config), LVL_DEBUG);

    flog_config_free(config);
}

static void
flog_config_new_with_short_level_opt_and_error_value_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_SHORT,
        TEST_OPTION_LEVEL_VALUE_ERROR,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_level(config), LVL_ERROR);

    flog_config_free(config);
}

static void
flog_config_new_with_short_level_opt_and_fault_value_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_SHORT,
        TEST_OPTION_LEVEL_VALUE_FAULT,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_level(config), LVL_FAULT);

    flog_config_free(config);
}

static void
flog_config_new_with_short_private_opt_and_message_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_PRIVATE_SHORT,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_message_type(config), MSG_PRIVATE);

    flog_config_free(config);
}

static void
flog_config_new_with_short_append_opt_and_path_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_APPEND_SHORT,
        TEST_PATH,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_string_equal(flog_config_get_output_file(config), TEST_PATH);

    flog_config_free(config);
}

static void
flog_config_new_with_long_level_opt_and_default_value_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_LONG,
        TEST_OPTION_LEVEL_VALUE_DEFAULT,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_level(config), LVL_DEFAULT);

    flog_config_free(config);
}

static void
flog_config_new_with_long_level_opt_and_info_value_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_LONG,
        TEST_OPTION_LEVEL_VALUE_INFO,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_level(config), LVL_INFO);

    flog_config_free(config);
}

static void
flog_config_new_with_long_level_opt_and_debug_value_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_LONG,
        TEST_OPTION_LEVEL_VALUE_DEBUG,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_level(config), LVL_DEBUG);

    flog_config_free(config);
}

static void
flog_config_new_with_long_level_opt_and_error_value_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_LONG,
        TEST_OPTION_LEVEL_VALUE_ERROR,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_level(config), LVL_ERROR);

    flog_config_free(config);
}

static void
flog_config_new_with_long_level_opt_and_fault_value_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_LEVEL_LONG,
        TEST_OPTION_LEVEL_VALUE_FAULT,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_level(config), LVL_FAULT);

    flog_config_free(config);
}
static void
flog_config_new_with_long_private_opt_and_message_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_PRIVATE_LONG,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_int_equal(flog_config_get_message_type(config), MSG_PRIVATE);

    flog_config_free(config);
}

static void
flog_config_new_with_short_version_opt_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_VERSION_SHORT
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_true(flog_config_get_version_flag(config));
    assert_int_equal(error, FLOG_ERROR_NONE);

    flog_config_free(config);
}

static void
flog_config_new_with_long_append_opt_and_path_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_APPEND_LONG,
        TEST_PATH,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_string_equal(flog_config_get_output_file(config), TEST_PATH);

    flog_config_free(config);
}

static void
flog_config_new_with_message_from_stream_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    char *message = "0123456789ABCDEF";

    FILE *saved_stdin = stdin;
    stdin = fmemopen(message, strlen(message), "r");

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    // Ensure stdin stream is restored before making assertions in order to avoid impacting
    // other tests if an assertion fails and leaves stdin pointing to the in-memory buffer
    fclose(stdin);
    stdin = saved_stdin;

    assert_non_null(config);
    assert_int_equal(error, FLOG_ERROR_NONE);
    assert_string_equal(flog_config_get_message(config), message);

    flog_config_free(config);
}

static void
flog_config_new_with_long_version_opt_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_VERSION_LONG
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_true(flog_config_get_version_flag(config));
    assert_int_equal(error, FLOG_ERROR_NONE);

    flog_config_free(config);
}

static void
flog_config_new_with_short_help_opt_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_HELP_SHORT
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_true(flog_config_get_help_flag(config));
    assert_int_equal(error, FLOG_ERROR_NONE);

    flog_config_free(config);
}

static void
flog_config_new_with_long_help_opt_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_OPTION_HELP_LONG
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_non_null(config);
    assert_true(flog_config_get_help_flag(config));
    assert_int_equal(error, FLOG_ERROR_NONE);

    flog_config_free(config);
}

static void
flog_config_get_subsystem_with_null_config_arg_fails(void **state) {
    UNUSED(state);
    expect_assert_failure(flog_config_get_subsystem(NULL));
}

static void
flog_config_set_subsystem_with_null_config_arg_fails(void **state) {
    UNUSED(state);
    expect_assert_failure(flog_config_set_subsystem(NULL, TEST_SUBSYSTEM));
}

static void
flog_config_set_subsystem_with_null_subsystem_arg_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    expect_assert_failure(flog_config_set_subsystem(config, NULL))

    flog_config_free(config);
 }

static void
flog_config_get_subsystem_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_string_equal(flog_config_get_subsystem(config), "");

    flog_config_free(config);
}

static void
flog_config_set_and_get_subsystem_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_subsystem(config, TEST_SUBSYSTEM);
    assert_string_equal(flog_config_get_subsystem(config), TEST_SUBSYSTEM);

    flog_config_free(config);
}

static void
flog_config_set_subsystem_with_long_subsystem_truncates(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    char *subsystem = malloc(subsystem_len + 1);
    memset(subsystem, TEST_CHAR, subsystem_len);
    subsystem[subsystem_len] = '\0';

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_subsystem(config, subsystem);
    assert_int_equal(strlen(flog_config_get_subsystem(config)), strlen(subsystem) - 1);

    flog_config_free(config);
    free(subsystem);
}

static void
flog_config_get_category_with_null_config_arg_fails(void **state) {
    UNUSED(state);
    expect_assert_failure(flog_config_get_category(NULL));
}

static void
flog_config_set_category_with_null_config_arg_fails(void **state) {
    UNUSED(state);
    expect_assert_failure(flog_config_set_category(NULL, TEST_CATEGORY));
}

static void
flog_config_set_category_with_null_category_arg_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    expect_assert_failure(flog_config_set_category(config, NULL));

    flog_config_free(config);
}

static void
flog_config_get_category_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_string_equal(flog_config_get_category(config), "");

    flog_config_free(config);
}

static void
flog_config_set_and_get_category_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_category(config, TEST_CATEGORY);
    assert_string_equal(flog_config_get_category(config), TEST_CATEGORY);

    flog_config_free(config);
}

static void
flog_config_set_category_with_long_category_truncates(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    char *category = malloc(category_len + 1);
    memset(category, TEST_CHAR, category_len);
    category[category_len] = '\0';

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_category(config, category);
    assert_int_equal(strlen(flog_config_get_category(config)), strlen(category) - 1);

    flog_config_free(config);
    free(category);
}

static void
flog_config_get_output_file_with_null_config_arg_fails(void **state) {
    UNUSED(state);
    expect_assert_failure(flog_config_get_output_file(NULL));
}

static void
flog_config_set_output_file_with_null_config_arg_fails(void **state) {
    UNUSED(state);
    expect_assert_failure(flog_config_set_output_file(NULL, TEST_OUTPUT_FILE));
}

static void
flog_config_set_output_file_with_long_path_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    char *path = malloc(PATH_MAX + 1);
    memset(path, TEST_CHAR, PATH_MAX);
    path[PATH_MAX] = '\0';

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_int_equal(flog_config_set_output_file(config, path), FLOG_ERROR_FILE);

    flog_config_free(config);
    free(path);
}

static void
flog_config_set_message_with_null_config_arg_fails(void **state) {
    UNUSED(state);
    expect_assert_failure(flog_config_set_message(NULL, TEST_MESSAGE));
}

static void
flog_config_set_message_with_null_message_arg_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    expect_assert_failure(flog_config_set_message(config, NULL));

    flog_config_free(config);
}

static void
flog_config_set_output_file_with_null_output_file_arg_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    expect_assert_failure(flog_config_set_output_file(config, NULL));

    flog_config_free(config);
}

static void
flog_config_get_output_file_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_string_equal(flog_config_get_output_file(config), "");

    flog_config_free(config);
}

static void
flog_config_set_and_get_output_file_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_output_file(config, TEST_OUTPUT_FILE);
    assert_string_equal(flog_config_get_output_file(config), TEST_OUTPUT_FILE);

    flog_config_free(config);
}

static void
flog_config_get_message_with_null_config_arg_fails(void **state) {
    UNUSED(state);
    expect_assert_failure(flog_config_get_message(NULL));
}


static void
flog_config_get_message_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_string_equal(flog_config_get_message(config), TEST_MESSAGE);

    flog_config_free(config);
}

static void
flog_config_set_and_get_message_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    const char message[] = "0123456789ABCDEF";

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_message(config, message);
    assert_string_equal(flog_config_get_message(config), message);

    flog_config_free(config);
}

static void
flog_config_set_message_with_long_message_truncates(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    char *message = malloc(message_len + 1);
    memset(message, TEST_CHAR, message_len);
    message[message_len] = '\0';

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_message(config, message);
    assert_int_equal(strlen(flog_config_get_message(config)), message_len - 1);

    flog_config_free(config);
    free(message);
}

static void
flog_config_get_message_type_with_null_config_arg_fails(void **state) {
    UNUSED(state);
    expect_assert_failure(flog_config_get_message_type(NULL));
}

static void
flog_config_set_message_type_with_null_config_arg_fails(void **state) {
    UNUSED(state);
    expect_assert_failure(flog_config_set_message_type(NULL, MSG_PRIVATE));
}

static void
flog_config_get_message_type_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    assert_int_equal(flog_config_get_message_type(config), MSG_PUBLIC);

    flog_config_free(config);
}

static void
flog_config_set_and_get_message_type_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_message_type(config, MSG_PRIVATE);
    assert_int_equal(flog_config_get_message_type(config), MSG_PRIVATE);

    flog_config_free(config);
}

static void
flog_config_set_message_from_stream_with_null_config_arg_fails(void **state) {
    UNUSED(state);

    FILE mock_stream = {};
    expect_assert_failure(flog_config_set_message_from_stream(NULL, &mock_stream));
}

static void
flog_config_set_message_from_stream_with_null_stream_arg_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    expect_assert_failure(flog_config_set_message_from_stream(config, NULL));

    flog_config_free(config);
}

static void
flog_config_set_and_get_message_from_stream_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    char *message = "0123456789ABCDEF";

    FILE *mock_stream = fmemopen(message, strlen(message), "r");
    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_message_from_stream(config, mock_stream);
    assert_int_equal(strlen(flog_config_get_message(config)), strlen(message));
    assert_string_equal(flog_config_get_message(config), message);

    fclose(mock_stream);
    flog_config_free(config);
}

static void
flog_config_set_message_from_stream_with_long_message_truncates(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    char *message = malloc(message_len + 1);
    memset(message, TEST_CHAR, message_len);
    message[message_len] = '\0';

    FILE *mock_stream = fmemopen(message, strlen(message), "r");
    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_message_from_stream(config, mock_stream);
    assert_int_equal(strlen(flog_config_get_message(config)), message_len - 1);

    fclose(mock_stream);
    flog_config_free(config);
    free(message);
}

static void
flog_config_set_message_from_args_with_null_config_arg_fails(void **state) {
    UNUSED(state);

    const char *mock_args[] = {
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    };

    expect_assert_failure(flog_config_set_message_from_args(NULL, mock_args));
}

static void
flog_config_set_message_from_args_with_null_args_fails(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    expect_assert_failure(flog_config_set_message_from_args(config, NULL));

    flog_config_free(config);
}

static void
flog_config_set_and_get_message_from_args_succeeds(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    const char *mock_args[] = {
        "abc",
        "def",
        "123",
        "456",
        NULL
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_message_from_args(config, mock_args);
    assert_string_equal(flog_config_get_message(config), "abc def 123 456");

    flog_config_free(config);
}

static void
flog_config_set_message_from_args_with_long_message_truncates(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    char *message = malloc(message_len + 1);
    memset(message, TEST_CHAR, message_len);
    message[message_len] = '\0';

    const char *mock_args[] = {
        message,
        NULL
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_message_from_args(config, mock_args);
    assert_int_equal(strlen(flog_config_get_message(config)), message_len - 1);

    flog_config_free(config);
    free(message);
}

static void
flog_config_set_message_from_args_truncates_when_appending_space(void **state) {
    UNUSED(state);

    FlogError error = TEST_ERROR;
    MOCK_ARGS(
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    )

    char *message = malloc(message_len);
    memset(message, TEST_CHAR, message_len);
    message[message_len - 1] = '\0';

    const char *mock_args[] = {
        message,
        "abcdef123456",
        NULL
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv, &error);

    flog_config_set_message_from_args(config, mock_args);
    assert_int_equal(strlen(flog_config_get_message(config)), message_len - 1);

    flog_config_free(config);
    free(message);
}

int main(void) {
    cmocka_set_message_output(CM_OUTPUT_TAP);

    const struct CMUnitTest tests[] = {
        // flog_config_new() precondition tests
        cmocka_unit_test(flog_config_new_with_zero_arg_count_fails),
        cmocka_unit_test(flog_config_new_with_null_arg_values_fails),

        // flog_config_new() failure tests
        cmocka_unit_test(flog_config_new_with_short_invalid_opt_fails),
        cmocka_unit_test(flog_config_new_with_long_invalid_opt_fails),
        cmocka_unit_test(flog_config_new_with_short_category_opt_and_no_subsystem_opt_fails),
        cmocka_unit_test(flog_config_new_with_long_category_opt_and_no_subsystem_opt_fails),
        cmocka_unit_test(flog_config_with_short_subsystem_opt_and_no_subsystem_fails),
        cmocka_unit_test(flog_config_with_long_subsystem_opt_and_no_subsystem_fails),
        cmocka_unit_test(flog_config_new_with_stdin_tty_and_no_message_args_fails),
        cmocka_unit_test(flog_config_new_with_short_level_opt_and_unknown_value_fails),
        cmocka_unit_test(flog_config_new_with_long_level_opt_and_unknown_value_fails),
        cmocka_unit_test(flog_config_new_with_short_append_opt_and_long_path_fails),
        cmocka_unit_test(flog_config_new_with_long_append_opt_and_long_path_fails),

        // flog_config_new() success tests
        cmocka_unit_test(flog_config_new_with_message_succeeds),
        cmocka_unit_test(flog_config_new_with_short_subsystem_opt_and_message_succeeds),
        cmocka_unit_test(flog_config_new_with_long_subsystem_opt_and_message_succeeds),
        cmocka_unit_test(flog_config_new_with_short_level_opt_and_default_value_succeeds),
        cmocka_unit_test(flog_config_new_with_short_level_opt_and_info_value_succeeds),
        cmocka_unit_test(flog_config_new_with_short_level_opt_and_debug_value_succeeds),
        cmocka_unit_test(flog_config_new_with_short_level_opt_and_error_value_succeeds),
        cmocka_unit_test(flog_config_new_with_short_level_opt_and_fault_value_succeeds),
        cmocka_unit_test(flog_config_new_with_short_private_opt_and_message_succeeds),
        cmocka_unit_test(flog_config_new_with_short_append_opt_and_path_succeeds),
        cmocka_unit_test(flog_config_new_with_long_level_opt_and_default_value_succeeds),
        cmocka_unit_test(flog_config_new_with_long_level_opt_and_info_value_succeeds),
        cmocka_unit_test(flog_config_new_with_long_level_opt_and_debug_value_succeeds),
        cmocka_unit_test(flog_config_new_with_long_level_opt_and_error_value_succeeds),
        cmocka_unit_test(flog_config_new_with_long_level_opt_and_fault_value_succeeds),
        cmocka_unit_test(flog_config_new_with_long_private_opt_and_message_succeeds),
        cmocka_unit_test(flog_config_new_with_long_append_opt_and_path_succeeds),
        cmocka_unit_test(flog_config_new_with_message_from_stream_succeeds),
        cmocka_unit_test(flog_config_new_with_short_version_opt_succeeds),
        cmocka_unit_test(flog_config_new_with_long_version_opt_succeeds),
        cmocka_unit_test(flog_config_new_with_short_help_opt_succeeds),
        cmocka_unit_test(flog_config_new_with_long_help_opt_succeeds),

        // flog_config_set_subsystem() and flog_config_get_subsystem() precondition tests
        cmocka_unit_test(flog_config_get_subsystem_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_subsystem_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_subsystem_with_null_subsystem_arg_fails),

        // flog_config_set_subsystem() and flog_config_get_subsystem() success tests
        cmocka_unit_test(flog_config_get_subsystem_succeeds),
        cmocka_unit_test(flog_config_set_and_get_subsystem_succeeds),

        // flog_config_set_subsystem() truncation tests
        cmocka_unit_test(flog_config_set_subsystem_with_long_subsystem_truncates),

        // flog_config_set_category() and flog_config_get_category() precondition tests
        cmocka_unit_test(flog_config_get_category_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_category_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_category_with_null_category_arg_fails),

        // flog_config_set_category() and flog_config_get_category() success tests
        cmocka_unit_test(flog_config_get_category_succeeds),
        cmocka_unit_test(flog_config_set_and_get_category_succeeds),

        // flog_config_set_category() truncation tests
        cmocka_unit_test(flog_config_set_category_with_long_category_truncates),

        // flog_config_set_output_file() and flog_config_get_output_file() precondition tests
        cmocka_unit_test(flog_config_get_output_file_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_output_file_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_output_file_with_null_output_file_arg_fails),

        // flog_config_set_output_file() and flog_config_get_output_file() success tests
        cmocka_unit_test(flog_config_get_output_file_succeeds),
        cmocka_unit_test(flog_config_set_and_get_output_file_succeeds),

        // flog_config_set_output_file() failure tests
        cmocka_unit_test(flog_config_set_output_file_with_long_path_fails),

        // flog_config_set_message() and flog_config_get_message() precondition tests
        cmocka_unit_test(flog_config_set_message_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_message_with_null_message_arg_fails),
        cmocka_unit_test(flog_config_get_message_with_null_config_arg_fails),

        // flog_config_set_message() and flog_config_get_message() success tests
        cmocka_unit_test(flog_config_get_message_succeeds),
        cmocka_unit_test(flog_config_set_and_get_message_succeeds),

        // flog_config_set_message() truncation tests
        cmocka_unit_test(flog_config_set_message_with_long_message_truncates),

        // flog_config_set_message_type() and flog_config_get_message_type() precondition tests
        cmocka_unit_test(flog_config_get_message_type_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_message_type_with_null_config_arg_fails),

        // flog_config_set_message_type() and flog_config_get_message_type() success tests
        cmocka_unit_test(flog_config_get_message_type_succeeds),
        cmocka_unit_test(flog_config_set_and_get_message_type_succeeds),

        // flog_config_set_message_from_stream() precondition tests
        cmocka_unit_test(flog_config_set_message_from_stream_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_message_from_stream_with_null_stream_arg_fails),

        // flog_config_set_message_from_stream() success tests
        cmocka_unit_test(flog_config_set_and_get_message_from_stream_succeeds),

        // flog_config_set_message_from_stream() truncation tests
        cmocka_unit_test(flog_config_set_message_from_stream_with_long_message_truncates),

        // flog_config_set_message_from_args() precondition tests
        cmocka_unit_test(flog_config_set_message_from_args_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_message_from_args_with_null_args_fails),

        // flog_config_set_message_from_args() success tests
        cmocka_unit_test(flog_config_set_and_get_message_from_args_succeeds),

        // flog_config_set_message_from_args() truncation tests
        cmocka_unit_test(flog_config_set_message_from_args_with_long_message_truncates),
        cmocka_unit_test(flog_config_set_message_from_args_truncates_when_appending_space)
    };

    return cmocka_run_group_tests_name("FlogConfig tests", tests, NULL, NULL);
}
