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

#include "flog.h"
#include <os/log.h>
#include <sys/errno.h>
#include "defs.h"
#include "config.h"

#define OS_LOG_FORMAT_STRING "%{public}s"

struct FlogCliData {
    FlogConfig *config;
    os_log_t log;
};

FlogCli *
flog_cli_new(FlogConfig *config) {
    if (config == NULL) {
        fprintf(stderr, "%s: no configuration provided\n", PROGRAM_NAME);
        exit(1);
    }

    FlogCli *flog = calloc(1, sizeof(struct FlogCliData));
    if (flog == NULL) {
        perror(PROGRAM_NAME);
        exit(errno);
    }

    flog_cli_set_config(flog, config);
    flog->log = os_log_create(flog_config_get_subsystem(config), flog_config_get_category(config));
    return flog;
}

void
flog_cli_free(FlogCli *flog) {
    os_release(flog->log);
    free(flog);
}

FlogConfig *
flog_cli_get_config(const FlogCli *flog) {
    return flog->config;
}

void
flog_cli_set_config(FlogCli *flog, FlogConfig *config) {
    flog->config = config;
}

void
flog_commit_message(FlogCli *flog) {
    FlogConfig *config = flog_cli_get_config(flog);
    FlogConfigLevel level = flog_config_get_level(config);
    const char *message = flog_config_get_message(config);

    switch (level) {
        case Info:
            os_log_info(flog->log, OS_LOG_FORMAT_STRING, message);
            break;
        case Debug:
            os_log_debug(flog->log, OS_LOG_FORMAT_STRING, message);
            break;
        case Error:
            os_log_error(flog->log, OS_LOG_FORMAT_STRING, message);
            break;
        case Fault:
            os_log_fault(flog->log, OS_LOG_FORMAT_STRING, message);
            break;
        default:
            fprintf(stderr, "%s: unknown log level variant\n", PROGRAM_NAME);
            exit(1);
    }
}