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

#ifndef FLOG_H
#define FLOG_H

#include "config.h"

/*! \file flog.h
 *
 *  Logger object and associated functions for command-line logging system.
 */

/*! \struct FlogCli
 *
 *  \brief An opaque type representing a FlogCli logger object.
 */
typedef struct FlogCliData FlogCli;

/*! \brief Create a FlogCli object to be used for logging messages to the unified logging system.
 *
 *  \param config A pointer to a FlogConfig object
 *
 *  \pre \c config is \e not \c NULL
 *
 *  \return A pointer to a FlogCli object
 */
FlogCli * flog_cli_new(FlogConfig *config);

/*! \brief Free a FlogCli object.
 *
 *  \param flog A pointer to the FlogCli object that should be freed
 *
 *  \pre \c flog is \e not \c NULL
 */
void flog_cli_free(FlogCli *flog);

/*! \brief Get the FlogConfig object associated with a FlogCli object.
 *
 *  \param flog A pointer to the FlogCli object
 *
 *  \pre \c flog is \e not \c NULL
 *
 *  \return A pointer to the FlogConfig object associated with the FlogCli object
 */
FlogConfig * flog_cli_get_config(const FlogCli *flog);

/*! \brief Set the FlogConfig object associated with a FlogCli object.
 *
 *  \param flog   A pointer to the FlogCli object
 *  \param config A pointer to the FlogConfig object
 *
 *  \pre \c flog is \e not \c NULL
 *  \pre \c config is \e not \c NULL
 */
void flog_cli_set_config(FlogCli *flog, FlogConfig *config);

/*! \brief Commit the current log message to the unified logging system.
 *
 *  \param flog A pointer to the FlogCli object
 *
 *  \pre \c flog is \e not \c NULL
 */
void flog_commit_message(FlogCli *flog);

#endif //FLOG_H
