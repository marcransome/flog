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
#include <errno.h>
#include "config.h"
#include "defs.h"

#define TEST_PROGRAM_NAME "flog"
#define TEST_MESSAGE "test message"

#define TEST_OPTION_VERSION_SHORT "-v"
#define TEST_OPTION_VERSION_LONG "--version"
#define TEST_OPTION_HELP_SHORT "-h"
#define TEST_OPTION_HELP_LONG "--help"
#define TEST_OPTION_SUBSYSTEM_SHORT "-s"
#define TEST_OPTION_SUBSYSTEM_LONG "--subsystem"

void flog_usage() {}
void flog_version() {}

static void test_new_config_with_zero_arg_count_calls_assert(void **state) {
    int mock_argc = 0;      // should fail >0 assertion
    char *mock_argv[] = {}; // should pass non-null assertion

    expect_assert_failure(flog_config_new(mock_argc, mock_argv));
}

static void test_new_config_with_null_arg_values_calls_assert(void **state) {
    int mock_argc = 1;      // should pass >0 assertion
    char **mock_argv = NULL; // should fail non-null assertion

    expect_assert_failure(flog_config_new(mock_argc, mock_argv));
}

static void test_new_config_returns_non_null(void **state) {
    int mock_argc = 2;
    char *mock_argv[] = {
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv);
    assert_non_null(config);

    flog_config_free(config);
}

static void test_new_config_with_short_version_option(void **state) {
    int mock_argc = 2;
    char *mock_argv[] = {
        TEST_PROGRAM_NAME,
        TEST_OPTION_VERSION_SHORT
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv);
    assert_true(flog_config_get_version_flag(config));

    flog_config_free(config);
}

static void test_new_config_with_long_version_option(void **state) {
    int mock_argc = 2;
    char *mock_argv[] = {
        TEST_PROGRAM_NAME,
        TEST_OPTION_VERSION_LONG
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv);
    assert_true(flog_config_get_version_flag(config));

    flog_config_free(config);
}

static void test_new_config_with_short_help_option(void **state) {
    int mock_argc = 2;
    char *mock_argv[] = {
        TEST_PROGRAM_NAME,
        TEST_OPTION_HELP_SHORT
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv);
    assert_true(flog_config_get_help_flag(config));

    flog_config_free(config);
}

static void test_new_config_with_long_help_option(void **state) {
    int mock_argc = 2;
    char *mock_argv[] = {
        TEST_PROGRAM_NAME,
        TEST_OPTION_HELP_LONG
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv);
    assert_true(flog_config_get_help_flag(config));

    flog_config_free(config);
}

static void test_new_config_with_short_subsystem_option_returns_null_and_sets_errno(void **state) {
    int mock_argc = 2;
    char *mock_argv[] = {
        TEST_PROGRAM_NAME,
        TEST_OPTION_SUBSYSTEM_SHORT
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv);
    assert_null(config);
    assert_int_equal(errno, ERR_PROGRAM_OPTIONS);
}

static void test_new_config_with_long_subsystem_option_returns_null_and_sets_errno(void **state) {
    int mock_argc = 2;
    char *mock_argv[] = {
        TEST_PROGRAM_NAME,
        TEST_OPTION_SUBSYSTEM_LONG
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv);
    assert_null(config);
    assert_int_equal(errno, ERR_PROGRAM_OPTIONS);
}

int main(void) {
    cmocka_set_message_output(CM_OUTPUT_TAP);

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_new_config_with_zero_arg_count_calls_assert),
        cmocka_unit_test(test_new_config_with_null_arg_values_calls_assert),
        cmocka_unit_test(test_new_config_returns_non_null),
        cmocka_unit_test(test_new_config_with_short_version_option),
        cmocka_unit_test(test_new_config_with_long_version_option),
        cmocka_unit_test(test_new_config_with_short_help_option),
        cmocka_unit_test(test_new_config_with_long_help_option),
        cmocka_unit_test(test_new_config_with_short_subsystem_option_returns_null_and_sets_errno),
        cmocka_unit_test(test_new_config_with_long_subsystem_option_returns_null_and_sets_errno),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
