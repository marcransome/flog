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

#ifndef FLOG_COMMON_H
#define FLOG_COMMON_H

/*! \file common.h
 *
 *  Constants, helper functions, and error handling types for common operations.
 */

#define PROGRAM_NAME "flog"
#define PROGRAM_VERSION "v1.6.4"

/*! \brief An enumerated type representing error conditions. */
typedef enum FlogErrorData {
    FLOG_ERROR_NONE,
    FLOG_ERROR_ALLOC,
    FLOG_ERROR_APPEND,
    FLOG_ERROR_LVL,
    FLOG_ERROR_MSG,
    FLOG_ERROR_OPTS,
    FLOG_ERROR_SUBSYS,
    FLOG_ERROR_FILE
} FlogError;

/*! \brief Print usage information to stdout stream. */
void flog_usage(void);

/*! \brief Print version string to stdout stream. */
void flog_version(void);

/*! \brief Return a string representation of an error condition.
 *
 * \param[in] error A FlogError enumeration variant
 *
 * \return A pointer to a null-terminated string describing the error condition
 */
const char * flog_error_string(FlogError error);

/*! \brief Print a formatted error message representation of the error condition.
 *
 * \param[in] error A FlogError enumeration variant representing the error condition
 */
void flog_print_error(FlogError error);

#endif //FLOG_COMMON_H
