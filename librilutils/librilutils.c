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

#include <telephony/librilutils.h>
#include <time.h>
#include <stdarg.h>
#include <utils/Log.h>

uint64_t ril_nano_time() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000000LL + now.tv_nsec;
}

#define ssprintf(pBuf, ...) pBuf += sprintf(pBuf, __VA_ARGS__)

void __ril_hexDump(const char *logTag, char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;
    char sbuff[16384];
    char *sbp = sbuff;

    // Output description if given.
    if (desc != NULL)
        ssprintf(sbp, "%s:\n", desc);

    if (len == 0) {
        ssprintf(sbp, "  ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        ssprintf(sbp, "  NEGATIVE LENGTH: %i\n",len);
        return;
    }

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                ssprintf(sbp, "  %s\n", buff);

            // Output the offset.
            ssprintf(sbp, "  %04x ", i);
        }

        // Now the hex code for the specific character.
        ssprintf(sbp, " %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        ssprintf(sbp, "   ");
        i++;
    }

    // And print the final ASCII bit.
    ssprintf(sbp, "  %s\n", buff);

    __android_log_buf_print(LOG_ID_RADIO, ANDROID_LOG_DEBUG, logTag, "%s", sbuff);
}

