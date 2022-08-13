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

#ifndef FLOG_CONFIG_H
#define FLOG_CONFIG_H

typedef enum FlogConfigLevelData {
    Info,
    Debug,
    Error,
    Fault
} FlogConfigLevel;

typedef struct FlogConfigData FlogConfig;

FlogConfig * flog_config_new(int argc, char *argv[]);
void flog_config_free(FlogConfig *config);

const char * flog_config_get_subsystem(const FlogConfig *config);
void flog_config_set_subsystem(FlogConfig *config, const char *subsystem);

const char * flog_config_get_category(const FlogConfig *config);
void flog_config_set_category(FlogConfig *config, const char *category);

FlogConfigLevel flog_config_get_level(const FlogConfig *config);
void flog_config_set_level(FlogConfig *config, FlogConfigLevel level);

const char * flog_config_get_message(const FlogConfig *config);
void flog_config_set_message(FlogConfig *config, const char *message);

#endif //FLOG_CONFIG_H
