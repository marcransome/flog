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

/*! \file config.h
 *
 *  Configuration object and associated functions for command-line logging system.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "common.h"

extern const int subsystem_len;
extern const int category_len;
extern const int message_len;

/*! \brief An enumerated type representing the log level. */
typedef enum FlogConfigLevelData {
    LVL_DEFAULT,
    LVL_INFO,
    LVL_DEBUG,
    LVL_ERROR,
    LVL_FAULT,
    LVL_UNKNOWN
} FlogConfigLevel;

/*! \brief An enumerated type representing the log message type. */
typedef enum FlogConfigMessageTypeData {
    MSG_PUBLIC,
    MSG_PRIVATE
} FlogConfigMessageType;

/*! \struct FlogConfig
 *
 *  \brief An opaque type representing a FlogConfig logger configuration object.
 */
typedef struct FlogConfigData FlogConfig;

/*! \brief Create a FlogConfig object representing configuration to be used
 *         with a FlogCli logger object.
 *
 *  \param[in]  argc  An integer representing the number of command-line arguments
 *  \param[in]  argv  A pointer to an array of null-terminated strings representing
 *                    command-line arguments
 *  \param[out] error A pointer to a FlogError object that will be used to represent
 *                    an error condition on failure
 *
 *  \pre \c argv is \e not \c NULL
 *  \pre \c error is \e not \c NULL
 *
 *  \return If successful, a pointer to a FlogConfig object; if there is an error
 *          a \c NULL pointer is returned and \c error will be set to a FlogError
 *          variant representing an error condition
 */
FlogConfig * flog_config_new(int argc, char *argv[], FlogError *error);

/*! \brief Free a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object that should be freed
 *
 *  \pre \c config is \e not \c NULL
 */
void flog_config_free(FlogConfig *config);

/*! \brief Get the subsystem name from a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object
 *
 *  \pre \c config is \e not \c NULL
 *
 *  \return A pointer to the null-terminated subsystem name string
 */
const char * flog_config_get_subsystem(const FlogConfig *config);

/*! \brief Set the subsystem name for a FlogConfig object.
 *
 *  \param config    A pointer to the FlogConfig object
 *  \param subsystem A pointer to the null-terminated subsystem name string
 *
 *  \pre \c config is \e not \c NULL
 *  \pre \c subsystem is \e not \c NULL
 */
void flog_config_set_subsystem(FlogConfig *config, const char *subsystem);

/*! \brief Get the category name from a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object
 *
 *  \pre \c config is \e not \c NULL
 *
 *  \return A pointer to the null-terminated category name string
 */
const char * flog_config_get_category(const FlogConfig *config);

/*! \brief Set the category name for a FlogConfig object.
 *
 *  \param config   A pointer to the FlogConfig object
 *  \param category A pointer to the null-terminated category name string
 *
 *  \pre \c config is \e not \c NULL
 *  \pre \c category is \e not \c NULL
 */
void flog_config_set_category(FlogConfig *config, const char *category);

/*! \brief Get the output file path from a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object
 *
 *  \pre \c config is \e not \c NULL
 *
 *  \return A pointer to the null-terminated output file path
 */
const char * flog_config_get_output_file(const FlogConfig *config);

/*! \brief Set the output file path for a FlogConfig object.
 *
 *  \param config      A pointer to the FlogConfig object
 *  \param output_file A pointer to the null-terminated output file path
 *
 *  \pre \c config is \e not \c NULL
 *  \pre \c output_file is \e not \c NULL
 *
 *  \return If successful, the FlogError variant FLOG_ERROR_NONE, or FLOG_ERROR_FILE
 *          if the output_file path exceeds the maximum path limit
 */
FlogError flog_config_set_output_file(FlogConfig *config, const char *output_file);

/*! \brief Get the log level value from a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object
 *
 *  \pre \c config is \e not \c NULL
 *
 *  \return A FlogConfigLevel value representing the log level
 */
FlogConfigLevel flog_config_get_level(const FlogConfig *config);

/*! \brief Set the log level value for a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object
 *  \param level  A FlogConfigLevel value representing the log level
 *
 *  \pre \c config is \e not \c NULL
 */
void flog_config_set_level(FlogConfig *config, FlogConfigLevel level);

/*! \brief Get the log message from a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object
 *
 *  \pre \c config is \e not \c NULL
 *
 *  \return A pointer to the null-terminated log message string
 */
const char * flog_config_get_message(const FlogConfig *config);

/*! \brief Set the log message for a FlogConfig object.
 *
 *  \param config  A pointer to the FlogConfig object
 *  \param message A pointer to the null-terminated log message string
 *
 *  \pre \c config is \e not \c NULL
 *  \pre \c category is \e not \c NULL
 */
void flog_config_set_message(FlogConfig *config, const char *message);

/*! \brief Set the log message for a FlogConfig object by combining multiple
 *         command-line arguments.
 *
 *  \param config A pointer to the FlogConfig object
 *  \param args   A pointer to an array of null-terminated strings representing
 *                command-line arguments; the final element of the array \e must
 *                point to NULL indicating the end of the arguments array
 *
 *  \pre \c config is \e not \c NULL
 *  \pre \c args is \e not \c NULL
 */
void flog_config_set_message_from_args(FlogConfig *config, const char **args);

/*! \brief Set the log message for a FlogConfig object by reading from a stream.
 *
 *  \param config A pointer to the FlogConfig object
 *  \param stream A pointer to a stream
 *
 *  \pre \c config is \e not \c NULL
 *  \pre \c stream is \e not \c NULL
 */
void flog_config_set_message_from_stream(FlogConfig *config, FILE *restrict stream);

/*! \brief Get the log message type from a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object
 *
 *  \pre \c config is \e not \c NULL
 *
 *  \return A FlogConfigMessageType value representing the log message type
 */
FlogConfigMessageType flog_config_get_message_type(const FlogConfig *config);

/*! \brief Set the log message type for a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object
 *  \param type   A FlogConfigMessageType value representing the log message
 *                type
 *
 *  \pre \c config is \e not \c NULL
 */
void flog_config_set_message_type(FlogConfig *config, FlogConfigMessageType type);

/*! \brief Get the version flag from a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object
 *
 *  \pre \c config is \e not \c NULL
 *
 *  \return \c true if the version flag is set otherwise \c false
 */
bool flog_config_get_version_flag(const FlogConfig *config);

/*! \brief Set the version flag for a FlogConfig object.
 *
 *  \param config  A pointer to the FlogConfig object
 *  \param version A boolean value representing whether the version flag is set
 *
 *  \pre \c config is \e not \c NULL
 */
void flog_config_set_version_flag(FlogConfig *config, bool version);

/*! \brief Get the help flag from a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object
 *
 *  \pre \c config is \e not \c NULL
 *
 *  \return \c true if the help flag is set otherwise \c false
 */
bool flog_config_get_help_flag(const FlogConfig *config);

/*! \brief Set the help flag for a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object
 *  \param help   A boolean value representing whether the help flag is set
 *
 *  \pre \c config is \e not \c NULL
 */
void flog_config_set_help_flag(FlogConfig *config, bool help);

#endif //FLOG_CONFIG_H
