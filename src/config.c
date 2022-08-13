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

#define SUBSYSTEM_LEN 257
#define CATEGORY_LEN 257
#define MESSAGE_LEN 1024

FlogConfigLevel flog_config_parse_level(const char *str);

static struct option longopts[] = {
    { "version",    no_argument,        NULL,  'v' },
    { "level",      required_argument,  NULL,  'l' },
    { "message",    required_argument,  NULL,  'm' },
    { "subsystem",  required_argument,  NULL,  's' },
    { "category",   required_argument,  NULL,  'c' },
    { "help",       no_argument,        NULL,  'h' },
    { "private",    no_argument,        NULL,  'p' }
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
    if (argc == 1) {
        flog_usage();
        exit(1);
    }

    FlogConfig *config = calloc(1, sizeof(struct FlogConfigData));
    if (config == NULL) {
        perror(PROGRAM_NAME);
        exit(errno);
    }

    // Default config values
    flog_config_set_level(config, Info);
    flog_config_set_message_type(config, Public);

    int ch;
    while ((ch = getopt_long(argc, argv, "vhl:m:s:c:", longopts, NULL)) != -1) {
        switch (ch) {
            case 'h':
                flog_usage();
                exit(0);
            case 'v':
                flog_version();
                exit(0);
            case 'm':
                flog_config_set_message(config, optarg);
                break;
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
                exit(1);
        }
    }
    argc -= optind;
    argv += optind;

    if (argc > 0 || !flog_config_has_message(config)) {
        flog_usage();
        exit(1);
    }

    return config;
}

void
flog_config_free(FlogConfig *config) {
    free(config);
}

const char *
flog_config_get_subsystem(const FlogConfig *config) {
    return config->subsystem;
}

void
flog_config_set_subsystem(FlogConfig *config, const char *subsystem) {
    if (strlcpy(config->subsystem, subsystem, SUBSYSTEM_LEN) >= SUBSYSTEM_LEN) {
        fprintf(stderr, "%s: specify a subsystem value up to a maximum of %d characters\n", PROGRAM_NAME, SUBSYSTEM_LEN - 1);
        exit(1);
    }
}

const char *
flog_config_get_category(const FlogConfig *config) {
    return config->category;
}

void
flog_config_set_category(FlogConfig *config, const char *category) {
    if (strlcpy(config->category, category, CATEGORY_LEN) >= CATEGORY_LEN) {
        fprintf(stderr, "%s: specify a category value up to a maximum of %d characters\n", PROGRAM_NAME, CATEGORY_LEN - 1);
        exit(1);
    }
}

FlogConfigLevel
flog_config_get_level(const FlogConfig *config) {
    return config->level;
}

void
flog_config_set_level(FlogConfig *config, FlogConfigLevel level) {
    config->level = level;
}

FlogConfigLevel
flog_config_parse_level(const char *str) {
    FlogConfigLevel level;

    if (strcmp(str, "info") == 0) {
        level = Info;
    } else if (strcmp(str, "debug") == 0) {
        level = Debug;
    } else if (strcmp(str, "error") == 0) {
        level = Error;
    } else if (strcmp(str, "fault") == 0) {
        level = Fault;
    } else {
        fprintf(stderr, "%s: unknown log level '%s'\n", PROGRAM_NAME, str);
        exit(1);
    }

    return level;
}

const char * flog_config_get_message(const FlogConfig *config) {
    return config->message;
}

void flog_config_set_message(FlogConfig *config, const char *message) {
    if (strlcpy(config->message, message, MESSAGE_LEN) >= MESSAGE_LEN) {
        // TODO review os/log.h and print per-level warnings based on maximum supported length for each level
        fprintf(stderr, "%s: long messages may be truncated by the unified logging system\n", PROGRAM_NAME);
    }
}

FlogConfigMessageType
flog_config_get_message_type(const FlogConfig *config) {
    return config->message_type;
}

void
flog_config_set_message_type(FlogConfig *config, FlogConfigMessageType message_type) {
    config->message_type = message_type;
}

bool
flog_config_has_message(const FlogConfig *config) {
    return strlen(flog_config_get_message(config)) > 0 ? true : false;
}