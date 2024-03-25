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

#define TEST_OPTION_APPEND_SHORT "-a"
#define TEST_OPTION_APPEND_LONG "--append"

#define TEST_OPTION_PRIVATE_SHORT "-p"
#define TEST_OPTION_PRIVATE_LONG "--private"

#define TEST_OPTION_INVALID_SHORT "-i"
#define TEST_OPTION_INVALID_LONG "--invalid"

#define TEST_ERROR 255

#define MOCK_ARGS(...) \
    char *mock_argv[] = {__VA_ARGS__, NULL}; \
    int mock_argc = (sizeof(mock_argv) / sizeof(mock_argv[0]) - 1);

void flog_usage() {}
void flog_version() {}

static void
flog_config_new_with_zero_arg_count_fails(void **state) {
    FlogError error;        // should pass non-null assertion (&error)
    int mock_argc = 0;      // should fail >0 assertion
    char *mock_argv[] = {}; // should pass non-null assertion

    expect_assert_failure(flog_config_new(mock_argc, mock_argv, &error));
}

static void
flog_config_new_with_null_arg_values_fails(void **state) {
    FlogError error;         // should pass non-null assertion (&error)
    int mock_argc = 1;       // should pass >0 assertion
    char **mock_argv = NULL; // should fail non-null assertion

    expect_assert_failure(flog_config_new(mock_argc, mock_argv, &error));
}

static void test_new_config_with_no_error_ptr_calls_assert(void **state) {
    int mock_argc = 1;      // should pass >0 assertion
    char *mock_argv[] = {}; // should pass non-null assertion

    expect_assert_failure(flog_config_new(mock_argc, mock_argv, NULL));
}

static void
flog_config_new_with_short_invalid_opt_fails(void **state) {
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

static void flog_config_with_short_subsystem_option_and_no_subsystem_fails(void **state) {
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

static void flog_config_with_long_subsystem_option_and_no_subsystem_fails(void **state) {
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
flog_config_new_with_message_succeeds(void **state) {
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
flog_config_new_with_short_version_opt_succeeds(void **state) {
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
flog_config_new_with_long_version_opt_succeeds(void **state) {
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
    expect_assert_failure(flog_config_get_subsystem(NULL));
}

static void
flog_config_set_subsystem_with_null_config_arg_fails(void **state) {
    expect_assert_failure(flog_config_set_subsystem(NULL, TEST_SUBSYSTEM));
}

static void
flog_config_set_subsystem_with_null_subsystem_arg_fails(void **state) {
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
flog_config_get_category_with_null_config_arg_fails(void **state) {
    expect_assert_failure(flog_config_get_category(NULL));
}

static void
flog_config_set_category_with_null_config_arg_fails(void **state) {
    expect_assert_failure(flog_config_set_category(NULL, TEST_CATEGORY));
}

static void
flog_config_set_category_with_null_category_arg_fails(void **state) {
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
flog_config_get_output_file_with_null_config_arg_fails(void **state) {
    expect_assert_failure(flog_config_get_output_file(NULL));
}

static void
flog_config_set_output_file_with_null_config_arg_fails(void **state) {
    expect_assert_failure(flog_config_set_output_file(NULL, TEST_OUTPUT_FILE));
}

static void
flog_config_set_output_file_with_null_output_file_arg_fails(void **state) {
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
        cmocka_unit_test(flog_config_with_short_subsystem_option_and_no_subsystem_fails),
        cmocka_unit_test(flog_config_with_long_subsystem_option_and_no_subsystem_fails),

        // flog_config_new() success tests
        cmocka_unit_test(flog_config_new_with_message_succeeds),
        cmocka_unit_test(flog_config_new_with_short_subsystem_opt_and_message_succeeds),
        cmocka_unit_test(flog_config_new_with_long_subsystem_opt_and_message_succeeds),
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

        // flog_config_set_category() and flog_config_get_category() precondition tests
        cmocka_unit_test(flog_config_get_category_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_category_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_category_with_null_category_arg_fails),

        // flog_config_set_category() and flog_config_get_category() success tests
        cmocka_unit_test(flog_config_get_category_succeeds),
        cmocka_unit_test(flog_config_set_and_get_category_succeeds),

        // flog_config_set_output_file() and flog_config_get_output_file() precondition tests
        cmocka_unit_test(flog_config_get_output_file_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_output_file_with_null_config_arg_fails),
        cmocka_unit_test(flog_config_set_output_file_with_null_output_file_arg_fails),

        // flog_config_set_output_file() and flog_config_get_output_file() success tests
        cmocka_unit_test(flog_config_get_output_file_succeeds),
        cmocka_unit_test(flog_config_set_and_get_output_file_succeeds),
    };

    return cmocka_run_group_tests_name("FlogConfig tests", tests, NULL, NULL);
}
