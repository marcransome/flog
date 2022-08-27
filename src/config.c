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
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/errno.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define SUBSYSTEM_LEN 257
#define CATEGORY_LEN 257
#define MESSAGE_LEN 8193

FlogConfigLevel flog_config_parse_level(const char *str);

static struct option long_options[] = {
    { "version",    no_argument,        NULL,  'v' },
    { "level",      required_argument,  NULL,  'l' },
    { "subsystem",  required_argument,  NULL,  's' },
    { "category",   required_argument,  NULL,  'c' },
    { "help",       no_argument,        NULL,  'h' },
    { "private",    no_argument,        NULL,  'p' },
    { NULL,         0,                  NULL,  0   }
};

struct FlogConfigData {
    FlogConfigLevel level;
    FlogConfigMessageType message_type;
    char subsystem[SUBSYSTEM_LEN];
    char category[CATEGORY_LEN];
    char message[MESSAGE_LEN];
};

FlogConfig *
flog_config_new(int argc, char *argv[]) {
    assert(argc > 0);
    assert(argv != NULL);

    if (argc == 1) {
        flog_usage();
        exit(EXIT_FAILURE);
    }

    FlogConfig *config = calloc(1, sizeof(struct FlogConfigData));
    if (config == NULL) {
        perror(PROGRAM_NAME);
        exit(errno);
    }

    // Default config values
    flog_config_set_level(config, Default);
    flog_config_set_message_type(config, Public);

    int ch;
    while ((ch = getopt_long(argc, argv, "vhl:s:c:p", long_options, NULL)) != -1) {
        switch (ch) {
            case 'h':
                flog_usage();
                exit(EXIT_SUCCESS);
            case 'v':
                flog_version();
                exit(EXIT_SUCCESS);
            case 'l':
                flog_config_set_level(config, flog_config_parse_level(optarg));
                break;
            case 's':
                flog_config_set_subsystem(config, optarg);
                break;
            case 'c':
                flog_config_set_category(config, optarg);
                break;
            case 'p':
                flog_config_set_message_type(config, Private);
                break;
            case '?':
                // getopt_long() generates errors for missing arguments
                exit(EXIT_FAILURE);
        }
    }
    argc -= optind;
    argv += optind;

    if (strlen(flog_config_get_category(config)) > 0 && strlen(flog_config_get_subsystem(config)) == 0) {
        fprintf(stderr, "%s: category option requires a subsystem name (use -s|--subsystem)\n", PROGRAM_NAME);
        exit(EXIT_FAILURE);
    }

    if (argc == 0) {
        flog_usage();
        exit(EXIT_FAILURE);
    }

    flog_config_set_message_from_args(config, argc, argv);

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
        fprintf(stderr, "%s: specify a subsystem value up to a maximum of %d characters\n", PROGRAM_NAME, SUBSYSTEM_LEN - 1);
        exit(EXIT_FAILURE);
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
        fprintf(stderr, "%s: specify a category value up to a maximum of %d characters\n", PROGRAM_NAME, CATEGORY_LEN - 1);
        exit(EXIT_FAILURE);
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
        fprintf(stderr, "%s: unknown log level '%s'\n", PROGRAM_NAME, str);
        exit(EXIT_FAILURE);
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
flog_config_set_message_from_args(FlogConfig *config, size_t count, char *args[]) {
    assert(count > 0);
    assert(args != NULL);

    bool message_truncated = false;
    char message_buff[MESSAGE_LEN] = {0};
    for (int i = 0; i < count; i++) {
        if (strlcat(message_buff, args[i], MESSAGE_LEN) >= MESSAGE_LEN) {
            message_truncated = true;
        }
        if (i != count - 1) {
            if (strlcat(message_buff, " ", MESSAGE_LEN) >= MESSAGE_LEN) {
                message_truncated = true;
            }
        }
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
