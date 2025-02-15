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

#include "config.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/syslimits.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <popt.h>
#include <unistd.h>
#include <sys/stat.h>

#ifdef UNIT_TESTING
#include "../test/testing.h"
#endif

bool is_regular_file_or_pipe(int fd, FlogError *error);

const size_t subsystem_len = 128;
const size_t category_len  = 128;
const size_t message_len   = 1024;

FlogConfigLevel flog_config_parse_level(const char *str);

static struct poptOption options[] = {
    { "version",    'v',  POPT_ARG_NONE,    NULL,  'v',  NULL,  NULL },
    { "level",      'l',  POPT_ARG_STRING,  NULL,  'l',  NULL,  NULL },
    { "subsystem",  's',  POPT_ARG_STRING,  NULL,  's',  NULL,  NULL },
    { "category",   'c',  POPT_ARG_STRING,  NULL,  'c',  NULL,  NULL },
    { "help",       'h',  POPT_ARG_NONE,    NULL,  'h',  NULL,  NULL },
    { "private",    'p',  POPT_ARG_NONE,    NULL,  'p',  NULL,  NULL },
    { "append",     'a',  POPT_ARG_STRING,  NULL,  'a',  NULL,  NULL },
    POPT_TABLEEND
};

struct FlogConfigData {
    FlogConfigLevel level;
    FlogConfigMessageType message_type;
    char subsystem[subsystem_len];
    char category[category_len];
    char output_file[PATH_MAX];
    char message[message_len];
    bool version;
    bool help;
};

FlogConfig *
flog_config_new(int argc, char *argv[], FlogError *error) {
    assert(argv != NULL);
    assert(error != NULL);

    *error = FLOG_ERROR_NONE;

    FlogConfig *config = calloc(1, sizeof(struct FlogConfigData));
    if (config == NULL) {
        *error = FLOG_ERROR_ALLOC;
        return NULL;
    }

    flog_config_set_level(config, LVL_DEFAULT);
    flog_config_set_message_type(config, MSG_PUBLIC);
    flog_config_set_version_flag(config, false);
    flog_config_set_help_flag(config, false);

    poptContext context = poptGetContext("uk.co.fidgetbox.flog", argc, (const char**) argv, options, 0);
    poptReadDefaultConfig(context, 0);

    int option;
    while ((option = poptGetNextOpt(context)) > 0) {
        char *option_argument = poptGetOptArg(context);

        switch (option) {
            case 'h':
                flog_config_set_help_flag(config, true);
                poptFreeContext(context);
                return config;
            case 'v':
                flog_config_set_version_flag(config, true);
                poptFreeContext(context);
                return config;
            case 'a':
                *error = flog_config_set_output_file(config, option_argument);
                if (*error != FLOG_ERROR_NONE) {
                    flog_config_free(config);
                    poptFreeContext(context);
                    return NULL;
                }
                break;
            case 'l':
                flog_config_set_level(config, flog_config_parse_level(option_argument));
                if (flog_config_get_level(config) == LVL_UNKNOWN) {
                    flog_config_free(config);
                    poptFreeContext(context);
                    *error = FLOG_ERROR_LVL;
                    return NULL;
                }
                break;
            case 's':
                flog_config_set_subsystem(config, option_argument);
                break;
            case 'c':
                flog_config_set_category(config, option_argument);
                break;
            case 'p':
                flog_config_set_message_type(config, MSG_PRIVATE);
                break;
        }
    }

    if (option < -1) {
        fprintf(stderr, "%s: %s %s\n",
                PROGRAM_NAME,
                poptStrerror(option),
                poptBadOption(context, POPT_BADOPTION_NOALIAS));

        flog_config_free(config);
        poptFreeContext(context);
        *error = FLOG_ERROR_OPTS;
        return NULL;
    }

    if (strlen(flog_config_get_category(config)) > 0 && strlen(flog_config_get_subsystem(config)) == 0) {
        flog_config_free(config);
        poptFreeContext(context);
        *error = FLOG_ERROR_SUBSYS;
        return NULL;
    }

    const char **message_args;
    FlogError stream_error = FLOG_ERROR_NONE;

    if ((message_args = poptGetArgs(context)) != NULL) {
        flog_config_set_message_from_args(config, message_args);
    } else if (is_regular_file_or_pipe(fileno(stdin), &stream_error)) {
        flog_config_set_message_from_stream(config, stdin);
    } else {
        flog_config_free(config);
        poptFreeContext(context);

        if (stream_error != FLOG_ERROR_NONE) {
            *error = stream_error;
        } else {
            *error = FLOG_ERROR_MSG;
        }

        return NULL;
    }

    poptFreeContext(context);

    return config;
}

bool
is_regular_file_or_pipe(int fd, FlogError *error) {
    struct stat statbuf;

    if (fstat(fd, &statbuf) == -1) {
        *error = FLOG_ERROR_STAT;
        return false;
    }

    return S_ISFIFO(statbuf.st_mode) || S_ISREG(statbuf.st_mode);
}

void
flog_config_free(FlogConfig *config) {
    assert(config != NULL);

    free(config);
}

const char *
flog_config_get_subsystem(const FlogConfig *config) {
    assert(config != NULL);

    return config->subsystem;
}

void
flog_config_set_subsystem(FlogConfig *config, const char *subsystem) {
    assert(config != NULL);
    assert(subsystem != NULL);

    if (strlcpy(config->subsystem, subsystem, subsystem_len) >= subsystem_len) {
        fprintf(stderr, "%s: subsystem name truncated to %lu bytes\n", PROGRAM_NAME, subsystem_len - 1);
    }
}

const char *
flog_config_get_category(const FlogConfig *config) {
    assert(config != NULL);

    return config->category;
}

void
flog_config_set_category(FlogConfig *config, const char *category) {
    assert(config != NULL);
    assert(category != NULL);

    if (strlcpy(config->category, category, category_len) >= category_len) {
        fprintf(stderr, "%s: category name truncated to %lu bytes\n", PROGRAM_NAME, category_len - 1);
    }
}

const char *
flog_config_get_output_file(const FlogConfig *config) {
    assert(config != NULL);

    return config->output_file;
}

FlogError
flog_config_set_output_file(FlogConfig *config, const char *output_file) {
    assert(config != NULL);
    assert(output_file != NULL);

    if (strlcpy(config->output_file, output_file, PATH_MAX) >= PATH_MAX) {
        return FLOG_ERROR_FILE;
    }

    return FLOG_ERROR_NONE;
}

FlogConfigLevel
flog_config_get_level(const FlogConfig *config) {
    assert(config != NULL);

    return config->level;
}

void
flog_config_set_level(FlogConfig *config, FlogConfigLevel level) {
    assert(config != NULL);

    config->level = level;
}

FlogConfigLevel
flog_config_parse_level(const char *str) {
    FlogConfigLevel level;

    if (strcmp(str, "default") == 0) {
        level = LVL_DEFAULT;
    } else if (strcmp(str, "info") == 0) {
        level = LVL_INFO;
    } else if (strcmp(str, "debug") == 0) {
        level = LVL_DEBUG;
    } else if (strcmp(str, "error") == 0) {
        level = LVL_ERROR;
    } else if (strcmp(str, "fault") == 0) {
        level = LVL_FAULT;
    } else {
        level = LVL_UNKNOWN;
    }

    return level;
}

const char *
flog_config_get_message(const FlogConfig *config) {
    assert(config != NULL);

    return config->message;
}

void
flog_config_set_message(FlogConfig *config, const char *message) {
    assert(config != NULL);
    assert(message != NULL);

    if (strlcpy(config->message, message, message_len) >= message_len) {
        fprintf(stderr, "%s: message truncated to %lu bytes\n", PROGRAM_NAME, message_len - 1);
    }
}

void
flog_config_set_message_from_args(FlogConfig *config, const char **args) {
    assert(config != NULL);
    assert(args != NULL);

    config->message[0] = '\0';

    bool message_truncated = false;
    while (*args != NULL) {
        if (strlcat(config->message, *args, message_len) >= message_len) {
            message_truncated = true;
            break;
        }
        if (*(args + 1) != NULL) {
            if (strlcat(config->message, " ", message_len) >= message_len) {
                message_truncated = true;
                break;
            }
        }
        ++args;
    }

    if (message_truncated) {
        fprintf(stderr, "%s: message truncated to %lu bytes\n", PROGRAM_NAME, message_len - 1);
    }
}

void
flog_config_set_message_from_stream(FlogConfig *config, FILE *restrict stream) {
    assert(config != NULL);
    assert(stream != NULL);

    config->message[0] = '\0';

    // Read message from stream
    size_t bytes_read = fread(config->message, sizeof(char), message_len - 1, stream);
    if (ferror(stream)) {
        fprintf(stderr, "%s: error reading message from stream\n", PROGRAM_NAME);
        clearerr(stream);
        return;
    }

    // Ensure safe bounds and null termination
    bytes_read = (bytes_read >= message_len - 1) ? message_len - 1 : bytes_read;
    config->message[bytes_read] = '\0';

    // Check for message truncation
    char c;
    if (fread(&c, sizeof(char), 1, stream) > 0) {
        fprintf(stderr, "%s: message truncated to %lu bytes\n", PROGRAM_NAME, message_len - 1);
    }
    clearerr(stream);
}

FlogConfigMessageType
flog_config_get_message_type(const FlogConfig *config) {
    assert(config != NULL);

    return config->message_type;
}

void
flog_config_set_message_type(FlogConfig *config, FlogConfigMessageType message_type) {
    assert(config != NULL);

    config->message_type = message_type;
}

bool
flog_config_get_version_flag(const FlogConfig *config) {
    assert(config != NULL);

    return config->version;
}

void
flog_config_set_version_flag(FlogConfig *config, bool version) {
    assert(config != NULL);

    config->version = version;
}

bool
flog_config_get_help_flag(const FlogConfig *config) {
    assert(config != NULL);

    return config->help;
}

void
flog_config_set_help_flag(FlogConfig *config, bool help) {
    assert(config != NULL);

    config->help = help;
}
