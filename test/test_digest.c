/* test_digest.c
 *
 * Copyright (C) 2019-2023 wolfSSL Inc.
 *
 * This file is part of wolfengine.
 *
 * wolfengine is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * wolfengine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */

#include "unit.h"

#ifdef WE_HAVE_DIGEST

int test_digest_op(const EVP_MD *md, ENGINE *e, unsigned char *msg,
                          size_t len, unsigned char *prev,
                          unsigned int *prevLen)
{
    int err;
    EVP_MD_CTX *ctx;
    unsigned char digest[64] = {0,};
    unsigned int dLen = sizeof(digest);

    err = (ctx = EVP_MD_CTX_new()) == NULL;
    if (err == 0) {
        err = EVP_DigestInit_ex(ctx, md, e) != 1;
    }
    if (err == 0) {
        err = EVP_DigestUpdate(ctx, msg, len/2) != 1;
    }
    if (err == 0) {
        err = EVP_DigestUpdate(ctx, msg + len/2, len - len/2) != 1;
    }
    if (err == 0) {
        err = EVP_DigestFinal_ex(ctx, digest, &dLen) != 1;
    }
    if (err == 0) {
        PRINT_BUFFER("Digest", digest, dLen);

        if (*prevLen == 0) {
            memcpy(prev, digest, dLen);
            *prevLen = dLen;
        }
        else {
            if (memcmp(digest, prev, *prevLen) != 0) {
                PRINT_ERR_MSG("Digests don't match");
                err = 1;
            }
            else {
                PRINT_MSG("Digests match");
            }
        }
    }

    EVP_MD_CTX_free(ctx);

    return err;
}

/******************************************************************************/
static int test_create_digest(const EVP_MD *md, ENGINE *e, void *data)
{
    int err = 0;
    unsigned char *msg = (unsigned char *)"Test pattern";
    unsigned char longMsg[1300];
    unsigned char digest[64];
    unsigned int dLen;

    (void)data;

    /* Must memset longMsg to make valgrind happy */
    XMEMSET(digest, 0, sizeof(digest));
    XMEMSET(longMsg, 0, sizeof(longMsg));

    RAND_bytes(longMsg, sizeof(longMsg));

    dLen = 0;
    PRINT_MSG("Digest with OpenSSL");
    err = test_digest_op(md, NULL, msg, strlen((char*)msg), digest, &dLen);
    if (err == 0) {
        PRINT_MSG("Digest With wolfengine");
        err = test_digest_op(md, e, msg, strlen((char*)msg), digest, &dLen);
    }
    if (err == 0) {
        dLen = 0;
        PRINT_MSG("Digest with OpenSSL");
        err = test_digest_op(md, NULL, longMsg, sizeof(longMsg), digest, &dLen);
    }
    if (err == 0) {
        PRINT_MSG("Digest With wolfengine");
        err = test_digest_op(md, e, longMsg, sizeof(longMsg), digest, &dLen);
    }

    return err;
}

#ifdef WE_HAVE_SHA1

int test_sha(ENGINE *e, void *data)
{
    return test_create_digest(EVP_sha1(), e, data);
}
#endif /* WE_HAVE_SHA1 */

#ifdef WE_HAVE_SHA224
int test_sha224(ENGINE *e, void *data)
{
    return test_create_digest(EVP_sha224(), e, data);
}
#endif

#ifdef WE_HAVE_SHA256
int test_sha256(ENGINE *e, void *data)
{
    return test_create_digest(EVP_sha256(), e, data);
}
#endif

/******************************************************************************/

#ifdef WE_HAVE_SHA384
int test_sha384(ENGINE *e, void *data)
{
    return test_create_digest(EVP_sha384(), e, data);
}
#endif

/******************************************************************************/

#ifdef WE_HAVE_SHA512
int test_sha512(ENGINE *e, void *data)
{
    return test_create_digest(EVP_sha512(), e, data);
}
#endif

/******************************************************************************/

#ifdef WE_HAVE_SHA3_224
int test_sha3_224(ENGINE *e, void *data)
{
    return test_create_digest(EVP_sha3_224(), e, data);
}
#endif

/******************************************************************************/

#ifdef WE_HAVE_SHA3_256
int test_sha3_256(ENGINE *e, void *data)
{
    return test_create_digest(EVP_sha3_256(), e, data);
}
#endif

/******************************************************************************/

#ifdef WE_HAVE_SHA3_384
int test_sha3_384(ENGINE *e, void *data)
{
    return test_create_digest(EVP_sha3_384(), e, data);
}
#endif

/******************************************************************************/

#ifdef WE_HAVE_SHA3_512
int test_sha3_512(ENGINE *e, void *data)
{
    return test_create_digest(EVP_sha3_512(), e, data);
}
#endif

/******************************************************************************/

#endif /* WE_HAVE_DIGEST */
