/*
 * Copyright (c) 2022 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef Utils_h
#define Utils_h

#include <stdio.h>

#include "Defines.h"

namespace dyld4 {

namespace Utils {

VIS_HIDDEN
const char* strrstr(const char* str, const char* sub);

VIS_HIDDEN
size_t concatenatePaths(char *path, const char *suffix, size_t pathsize);

}; /* namespace Utils */

}; /* namespace dyld4 */


// escape a cstring literal, output buffer is always null terminated and parameter `end` will point to the null terminator if given
VIS_HIDDEN
void escapeCStringLiteral(const char* s, char* b, size_t bufferLength, char**end=nullptr);

#endif /* Utils_h */
