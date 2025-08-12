// file sysdep.c

#include "sysdep.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "copyrt.h"

/* system dependent call to get IEEE node ID.
   This sample implementation generates a random node ID. */
void get_ieee_node_identifier(uuid_node_t *node) {
    static int inited = 0;
    static uuid_node_t saved_node;
    char seed[16];
    const char *filename = "nodeid";
    FILE *fp;
    int success = 0;

    if (!inited) {
        fp = fopen(filename, "rb");
        if (fp) {
            if (fread(&saved_node, sizeof(saved_node), 1, fp) == 1) {
                success = 1;  // Successfully read node from file
            }
            (void)fclose(fp);
        }

        if (!success) {
            get_random_info(seed);
            seed[0] |= 0x01;  // Set multicast bit per RFC 4122
            memcpy(&saved_node, seed, sizeof(saved_node));

            fp = fopen(filename, "wb");
            if (fp) {
                if (fwrite(&saved_node, sizeof(saved_node), 1, fp) == 1) {
                    success = 1;  // Successfully wrote node to file
                }
                (void)fclose(fp);
            }
        }

        if (success) {
            inited = 1;
        } else {
            // Handle failure: no valid saved_node available
            memset(&saved_node, 0, sizeof(saved_node));
            // TODO(CK): You might want to log this error or handle differently
        }
    }

    *node = saved_node;
}

/* system dependent call to get the current system time. Returned as
   100ns ticks since UUID epoch, but resolution may be less than
   100ns. */
#ifdef _WINDOWS_

void get_system_time(uuid_time_t *uuid_time) {
    ULARGE_INTEGER time;

    /* NT keeps time in FILETIME format which is 100ns ticks since
       Jan 1, 1601. UUIDs use time in 100ns ticks since Oct 15, 1582.
       The difference is 17 Days in Oct + 30 (Nov) + 31 (Dec)
       + 18 years and 5 leap days. */
    GetSystemTimeAsFileTime((FILETIME *)&time);
    time.QuadPart += (uint64_t)(1000 * 1000 * 10)                // seconds
                     * (uint64_t)(60 * 60 * 24)                  // days
                     * (uint64_t)(17 + 30 + 31 + 365 * 18 + 5);  // # of days
    *uuid_time = time.QuadPart;
}

/* Sample code, not for use in production; see RFC 1750 */
void get_random_info(char seed[16]) {
    MD5_CTX c;
    struct {
        MEMORYSTATUS m;
        SYSTEM_INFO s;
        FILETIME t;
        LARGE_INTEGER pc;
        DWORD tc;
        DWORD l;
        char hostname[MAX_COMPUTERNAME_LENGTH + 1];
    } r;

    MD5Init(&c);
    GlobalMemoryStatus(&r.m);
    GetSystemInfo(&r.s);
    GetSystemTimeAsFileTime(&r.t);
    QueryPerformanceCounter(&r.pc);
    r.tc = GetTickCount();
    r.l = MAX_COMPUTERNAME_LENGTH + 1;
    GetComputerName(r.hostname, &r.l);
    MD5Update(&c, (uint8_t *)&r, sizeof r);
    MD5Final((uint8_t *)seed, &c);
}

#else

void get_system_time(uuid_time_t *uuid_time) {
    struct timeval tp;

    gettimeofday(&tp, (struct timezone *)0);

    /* Offset between UUID formatted times and Unix formatted times.
       UUID UTC base time is October 15, 1582.
       Unix base time is January 1, 1970.*/
    *uuid_time = ((uint64_t)tp.tv_sec * 10000000) +
                 ((uint64_t)tp.tv_usec * 10) + I64(0x01B21DD213814000);
}

/* Sample code, not for use in production; see RFC 1750 */
void get_random_info(char seed[16]) {
    MD5_CTX c;
    struct {
#ifdef __linux__
        struct sysinfo s;
#endif
        struct timeval t;
        char hostname[257];
    } r;

    MD5Init(&c);
#ifdef __linux__
    sysinfo(&r.s);
#endif
    gettimeofday(&r.t, (struct timezone *)0);
    gethostname(r.hostname, 256);
    MD5Update(&c, (uint8_t *)&r, sizeof r);
    MD5Final((uint8_t *)seed, &c);
}

#endif

#ifdef _WIN32
#include <wincrypt.h>
#include <windows.h>

uint32_t get_secure_random(void) {
    uint32_t num = 0;
    HCRYPTPROV hProv = 0;
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        // TODO(CK): You might want to log this error or handle differently
        return 0;  // error
    }

    if (!CryptGenRandom(hProv, sizeof(num), (BYTE *)&num)) {
        // TODO(CK): You might want to log this error or handle differently
        num = 0;  // error
    }
    CryptReleaseContext(hProv, 0);
    return num;
}

#elif defined(__linux__)
#include <fcntl.h>
#include <unistd.h>

uint32_t get_secure_random(void) {
    uint32_t num;
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        // TODO(CK): You might want to log this error or handle differently
        return 0;  // error
    }

    ssize_t result = read(fd, &num, sizeof(num));
    close(fd);
    if (result != sizeof(num)) {
        // TODO(CK): You might want to log this error or handle differently
        return 0;  // error
    }

    return num;
}

#else                // BSD/macOS and others
#include <stdlib.h>  // arc4random

uint32_t get_secure_random(void) { return arc4random(); }
#endif
