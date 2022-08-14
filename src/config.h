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

#include <stdbool.h>

/*! \file config.h
 *
 *  Configuration object and associated functions for command-line logging system.
 */

/*! \brief An enumerated type representing the log level. */
typedef enum FlogConfigLevelData {
    Default,
    Info,
    Debug,
    Error,
    Fault
} FlogConfigLevel;

/*! \brief An enumerated type representing the log message type. */
typedef enum FlogConfigMessageTypeData {
    Public,
    Private,
} FlogConfigMessageType;

/*! \brief An opaque type representing a FlogConfig logger configuration object. */
typedef struct FlogConfigData FlogConfig;

/*! \brief Create a FlogConfig object representing configuration to be used
 *         with a FlogCli logger object.
 *
 *  \param argc An integer representing the number of command-line arguments
 *  \param argv A pointer to an array of null-terminated strings representing
 *              command-line arguments
 *
 *  \return A pointer to a FlogConfig object
 */
FlogConfig * flog_config_new(int argc, char *argv[]);

/*! \brief Free a FlogConfig object.
 *
 *  \param config A pointer to the FlogConfig object that should be freed
 *
 *  \pre \c config is \e not \c NULL
 *
 *  \return void
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
 *
 *  \return void
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
 *
 *  \return void
 */
void flog_config_set_category(FlogConfig *config, const char *category);

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
 *
 *  \return void
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
 *
 *  \return void
 */
void flog_config_set_message(FlogConfig *config, const char *message);

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
 *
 *  \return void
 */
void flog_config_set_message_type(FlogConfig *config, FlogConfigMessageType type);

/*! \brief Check whether the FlogConfig object has log message set or not.
 *
 *  \param config A pointer to the FlogConfig object
 *
 *  \pre \c config is \e not \c NULL
 *
 *  \return \c true if the FlogConfig object has a log message, \c false if not
 */
bool flog_config_has_message(const FlogConfig *config);

#endif //FLOG_CONFIG_H
