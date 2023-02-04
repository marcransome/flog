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
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include "defs.h"
#include "config.h"

#ifdef UNIT_TESTING
extern void mock_assert(const int result, const char* const expression,
                        const char * const file, const int line);

#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);
#endif

#define OS_LOG_FORMAT_PUBLIC "%{public}s"
#define OS_LOG_FORMAT_PRIVATE "%s"

void flog_commit_public_message(FlogCli *flog);
void flog_commit_private_message(FlogCli *flog);

struct FlogCliData {
    FlogConfig *config;
    os_log_t log;
};

FlogCli *
flog_cli_new(FlogConfig *config) {
    assert(config != NULL);

    FlogCli *flog = calloc(1, sizeof(struct FlogCliData));
    if (flog == NULL) {
        errno = ERR_FLOG_ALLOCATION;
        return NULL;
    }

    flog_cli_set_config(flog, config);

    const char *subsystem = flog_config_get_subsystem(config);
    if (strlen(subsystem) != 0) {
        flog->log = os_log_create(flog_config_get_subsystem(config), flog_config_get_category(config));
    } else {
        flog->log = OS_LOG_DEFAULT;
    }

    return flog;
}

void
flog_cli_free(FlogCli *flog) {
    assert(flog != NULL);

    if (flog->log != NULL) {
        os_release(flog->log);
    }

    free(flog);
}

FlogConfig *
flog_cli_get_config(const FlogCli *flog) {
    assert(flog != NULL);

    return flog->config;
}

void
flog_cli_set_config(FlogCli *flog, FlogConfig *config) {
    assert(flog != NULL);
    assert(config != NULL);

    flog->config = config;
}

void
flog_commit_message(FlogCli *flog) {
    assert(flog != NULL);

    FlogConfig *config = flog_cli_get_config(flog);
    if (flog_config_get_message_type(config) == Public) {
        flog_commit_public_message(flog);
    } else if (flog_config_get_message_type(config) == Private) {
        flog_commit_private_message(flog);
    }
}

void
flog_append_message_output(FlogCli *flog) {
    assert(flog != NULL);

    FlogConfig *config = flog_cli_get_config(flog);
    const char *output_file = flog_config_get_output_file(config);

    if (strlen(output_file) > 0) {

        mode_t original_umask = umask(S_IWGRP | S_IWOTH);

        FILE *fd = fopen(output_file, "a");
        if (fd == NULL) {
            fprintf(stderr, "%s: unable to append log message to file (%s)\n", PROGRAM_NAME, strerror(errno));
            exit(errno);
        }

        umask(original_umask);

        fprintf(fd, "%s", flog_config_get_message(config));
        fclose(fd);
    }
}

void
flog_commit_public_message(FlogCli *flog) {
    assert(flog != NULL);

    FlogConfig *config = flog_cli_get_config(flog);
    FlogConfigLevel level = flog_config_get_level(config);
    const char *message = flog_config_get_message(config);

    switch (level) {
        case Default:
            os_log(flog->log, OS_LOG_FORMAT_PUBLIC, message);
            break;
        case Info:
            os_log_info(flog->log, OS_LOG_FORMAT_PUBLIC, message);
            break;
        case Debug:
            os_log_debug(flog->log, OS_LOG_FORMAT_PUBLIC, message);
            break;
        case Error:
            os_log_error(flog->log, OS_LOG_FORMAT_PUBLIC, message);
            break;
        case Fault:
            os_log_fault(flog->log, OS_LOG_FORMAT_PUBLIC, message);
            break;
        default:
            fprintf(stderr, "%s: unknown log level; using 'default'\n", PROGRAM_NAME);
            os_log(flog->log, OS_LOG_FORMAT_PUBLIC, message);
    }
}

void
flog_commit_private_message(FlogCli *flog) {
    assert(flog != NULL);

    FlogConfig *config = flog_cli_get_config(flog);
    FlogConfigLevel level = flog_config_get_level(config);
    const char *message = flog_config_get_message(config);

    switch (level) {
        case Default:
            os_log(flog->log, OS_LOG_FORMAT_PRIVATE, message);
            break;
        case Info:
            os_log_info(flog->log, OS_LOG_FORMAT_PRIVATE, message);
            break;
        case Debug:
            os_log_debug(flog->log, OS_LOG_FORMAT_PRIVATE, message);
            break;
        case Error:
            os_log_error(flog->log, OS_LOG_FORMAT_PRIVATE, message);
            break;
        case Fault:
            os_log_fault(flog->log, OS_LOG_FORMAT_PRIVATE, message);
            break;
        default:
            fprintf(stderr, "%s: unknown log level; using 'default'\n", PROGRAM_NAME);
            os_log(flog->log, OS_LOG_FORMAT_PRIVATE, message);
    }
}
