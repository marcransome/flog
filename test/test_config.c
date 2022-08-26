#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "config.h"

#define TEST_PROGRAM_NAME "flog"
#define TEST_MESSAGE      "test message"

void flog_usage() {}
void flog_version() {}

static void test_new_config_with_zero_arg_count_calls_assert(void **state) {
    int mock_argc = 0;      // should fail >0 assertion
    char *mock_argv[] = {}; // should pass non-null assertion

    expect_assert_failure(flog_config_new(mock_argc, mock_argv));
}

static void test_new_config_with_null_arg_values_calls_assert(void **state) {
    int mock_argc = 1;      // should pass >0 assertion
    char *mock_argv = NULL; // should fail non-null assertion

    expect_assert_failure(flog_config_new(mock_argc, NULL));
}

static void test_new_config_returns_non_null(void **state) {
    int mock_argc = 2;
    char *mock_argv[] = {
        TEST_PROGRAM_NAME,
        TEST_MESSAGE
    };

    FlogConfig *config = flog_config_new(mock_argc, mock_argv);
    assert_non_null(config);
}

int main(void) {
    cmocka_set_message_output(CM_OUTPUT_TAP);

    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_new_config_with_zero_arg_count_calls_assert),
            cmocka_unit_test(test_new_config_with_null_arg_values_calls_assert),
            cmocka_unit_test(test_new_config_returns_non_null)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
