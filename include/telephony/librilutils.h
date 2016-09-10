/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIBRILUTILS_H
#define LIBRILUTILS_H

#include <stdint.h>
#include <utils/Log.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Return system time in nanos.
 *
 * This is a monotonicly increasing clock and
 * return the same value as System.nanoTime in java.
 */
uint64_t ril_nano_time();

void __ril_hexDump(const char *logTag, char *desc, void *addr, int len);

#define RIL_HEX_DUMP 1

#if RIL_HEX_DUMP
#define ril_hexDump(desc,addr,len) __ril_hexDump(LOG_TAG,desc,addr,len)
#else
#define ril_hexDump(desc,addr,len)
#endif

#ifdef __cplusplus
}
#endif

#endif // LIBRILUTILS_H
