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
#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <popt.h>

#ifdef UNIT_TESTING
extern void mock_assert(const int result, const char* const expression,
    const char * const file, const int line);

#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);
#endif

#define SUBSYSTEM_LEN 257
#define CATEGORY_LEN  257
#define MESSAGE_LEN   8193

FlogConfigLevel flog_config_parse_level(const char *str);

static struct poptOption options[] = {
    { "version",    'v',  POPT_ARG_NONE,    NULL,  'v',  NULL,  NULL },
    { "level",      'l',  POPT_ARG_STRING,  NULL,  'l',  NULL,  NULL },
    { "subsystem",  's',  POPT_ARG_STRING,  NULL,  's',  NULL,  NULL },
    { "category",   'c',  POPT_ARG_STRING,  NULL,  'c',  NULL,  NULL },
    { "help",       'h',  POPT_ARG_NONE,    NULL,  'h',  NULL,  NULL },
    { "private",    'p',  POPT_ARG_NONE,    NULL,  'p',  NULL,  NULL },
    POPT_TABLEEND
};

struct FlogConfigData {
    FlogConfigLevel level;
    FlogConfigMessageType message_type;
    char subsystem[SUBSYSTEM_LEN];
    char category[CATEGORY_LEN];
    char message[MESSAGE_LEN];
    bool version;
    bool help;
};

FlogConfig *
flog_config_new(int argc, char *argv[]) {
    assert(argc > 0);
    assert(argv != NULL);

    if (argc == 1) {
        errno = ERR_NO_ARGUMENTS_PROVIDED;
        return NULL;
    }

    FlogConfig *config = calloc(1, sizeof(struct FlogConfigData));
    if (config == NULL) {
        fprintf(stderr, "%s: config allocation failure\n", PROGRAM_NAME);
        errno = ERR_CONFIG_ALLOCATION;
        return NULL;
    }

    flog_config_set_level(config, Default);
    flog_config_set_message_type(config, Public);
    flog_config_set_version_flag(config, false);
    flog_config_set_help_flag(config, false);

    poptContext context = poptGetContext(NULL, argc, (const char**) argv, options, 0);

    int option;
    while ((option = poptGetNextOpt(context)) >= 0) {
        char *option_argument = poptGetOptArg(context);

        switch (option) {
            case 'h':
                flog_config_set_help_flag(config, true);
                return config;
            case 'v':
                flog_config_set_version_flag(config, true);
                return config;
            case 'l':
                flog_config_set_level(config, flog_config_parse_level(option_argument));
                break;
            case 's':
                flog_config_set_subsystem(config, option_argument);
                break;
            case 'c':
                flog_config_set_category(config, option_argument);
                break;
            case 'p':
                flog_config_set_message_type(config, Private);
                break;
        }

        free(option_argument);
    }

    if (option < -1) {
        fprintf(stderr, "%s: %s %s\n",
                PROGRAM_NAME,
                poptStrerror(option),
                poptBadOption(context, POPT_BADOPTION_NOALIAS));

        errno = ERR_PROGRAM_OPTIONS;
        flog_config_free(config);
        poptFreeContext(context);
        return NULL;
    }

    if (strlen(flog_config_get_category(config)) > 0 && strlen(flog_config_get_subsystem(config)) == 0) {
        fprintf(stderr, "%s: category option requires subsystem option\n", PROGRAM_NAME);
        errno = ERR_CATEGORY_OPTION_REQUIRES_SUBSYSTEM;
        flog_config_free(config);
        poptFreeContext(context);
        return NULL;
    }

    const char **message_args = poptGetArgs(context);
    if (message_args == NULL) {
        fprintf(stderr, "%s: message string required\n", PROGRAM_NAME);
        errno = ERR_NO_MESSAGE_STRING_PROVIDED;
        flog_config_free(config);
        poptFreeContext(context);
        return NULL;
    }

    flog_config_set_message_from_args(config, poptGetArgs(context));
    poptFreeContext(context);

    return config;
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

    if (strlcpy(config->subsystem, subsystem, SUBSYSTEM_LEN) >= SUBSYSTEM_LEN) {
        fprintf(stderr, "%s: subsystem name truncated to %d characters\n", PROGRAM_NAME, SUBSYSTEM_LEN - 1);
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

    if (strlcpy(config->category, category, CATEGORY_LEN) >= CATEGORY_LEN) {
        fprintf(stderr, "%s: category name truncated to %d characters\n", PROGRAM_NAME, CATEGORY_LEN - 1);
    }
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
        level = Default;
    } else if (strcmp(str, "info") == 0) {
        level = Info;
    } else if (strcmp(str, "debug") == 0) {
        level = Debug;
    } else if (strcmp(str, "error") == 0) {
        level = Error;
    } else if (strcmp(str, "fault") == 0) {
        level = Fault;
    } else {
        level = Unknown;
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

    if (strlcpy(config->message, message, MESSAGE_LEN) >= MESSAGE_LEN) {
        fprintf(stderr, "%s: message was truncated to %d characters\n", PROGRAM_NAME, MESSAGE_LEN - 1);
    }
}

void
flog_config_set_message_from_args(FlogConfig *config, const char **args) {
    assert(config != NULL);
    assert(args != NULL);

    bool message_truncated = false;
    char message_buff[MESSAGE_LEN] = {0};
    while (*args != NULL) {
        if (strlcat(message_buff, *args, MESSAGE_LEN) >= MESSAGE_LEN) {
            message_truncated = true;
        }
        if ((args + 1) != NULL) {
            if (strlcat(message_buff, " ", MESSAGE_LEN) >= MESSAGE_LEN) {
                message_truncated = true;
            }
        }
        ++args;
    }

    if (strlcpy(config->message, message_buff, MESSAGE_LEN) >= MESSAGE_LEN) {
        message_truncated = true;
    }

    if (message_truncated) {
        fprintf(stderr, "%s: message was truncated to %d characters\n", PROGRAM_NAME, MESSAGE_LEN - 1);
    }
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
