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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "flog.h"
#include "common.h"

int
main(int argc, char *argv[]) {

    if (argc == 1 && isatty(fileno(stdin))) {
        flog_usage();
        return 1;
    }

    FlogError error = FLOG_ERROR_NONE;
    FlogConfig *config = flog_config_new(argc, argv, &error);
    if (config == NULL) {
        if (error != FLOG_ERROR_OPTS) {
            flog_print_error(error);
        }
        return error;
    }

    if (flog_config_get_version_flag(config)) {
        flog_config_free(config);
        flog_version();
        return EXIT_SUCCESS;
    } else if (flog_config_get_help_flag(config)) {
        flog_config_free(config);
        flog_usage();
        return EXIT_SUCCESS;
    }

    error = FLOG_ERROR_NONE;
    FlogCli *flog = flog_cli_new(config, &error);
    if (flog == NULL) {
        flog_config_free(config);
        flog_print_error(error);
        return error;
    }

    error = FLOG_ERROR_NONE;
    error = flog_append_message_output(flog);
    if (error != FLOG_ERROR_NONE) {
        flog_cli_free(flog);
        flog_config_free(config);
        flog_print_error(error);
        return error;
    }

    flog_commit_message(flog);

    flog_cli_free(flog);
    flog_config_free(config);

    return EXIT_SUCCESS;
}
