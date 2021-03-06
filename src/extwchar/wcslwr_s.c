/*------------------------------------------------------------------
 * wcslwr_s.c
 *
 * September 2017, Reini Urban
 *
 * Copyright (c) 2017 by Reini Urban
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#ifdef FOR_DOXYGEN
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

#ifndef HAVE_TOWLOWER
EXTERN wint_t towlower(wint_t wc);
#else
#include <wctype.h>
#endif

/**
 * @def wcslwr_s(src,slen)
 * @brief
 *    Scans the string converting uppercase characters to simple lowercase,
 *    leaving all other characters unchanged.  The scanning stops at the first
 *    null or after slen characters.  The conversion is determined by the
 *    LC_CTYPE category setting of the locale. Other characters are not
 *    affected. It only performs simple case folding via \c towlower(), it
 *    does not do full multi-char folding and does not obey the special casing
 *    rules for context. See \c wcsfc_s() instead. Thus the length of buffer
 *    stays the same. It returns a pointer to the altered string. Because the
 *    modification is done in place, the pointer returned is the same as the
 *    pointer passed as the input argument.
 *
 * @remark IMPLEMENTED IN
 *    * Microsoft Windows Secure API
 *    * Novell NDK
 *    * Nokia openc s60.com
 *
 * @param[out]  src   wide string
 * @param[in]   slen  maximum length of string
 *
 * @pre  src shall not be a null pointer.
 * @pre  slen shall not be greater than RSIZE_MAX_WSTR and size of dest.
 *
 * @retval  EOK         on successful operation or slen = 0
 * @retval  ESNULLP     when src is NULL pointer
 * @retval  ESLEMAX     when slen > RSIZE_MAX_WSTR
 * @retval  EOVERFLOW   when slen > size of src (optionally, when the compiler
 *                      knows the object_size statically)
 *
 * @see
 *    wcsfc_s(), strtolowercase_s(), strlwr_s(), wcsupr_s()
 */

EXPORT errno_t _wcslwr_s_chk(wchar_t *restrict src, rsize_t slen,
                             const size_t srcbos) {
    const size_t srcsz = slen * sizeof(wchar_t);

    if (unlikely(slen == 0)) { /* Since C11 slen=0 is allowed */
        return EOK;
    }
    CHK_SRC_NULL("wcsupr_s", src)
    if (unlikely(slen > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("wcslwr_s: "
                                           "slen exceeds max",
                                           (void *)src, ESLEMAX);
        return (ESLEMAX);
    }
    if (srcbos == BOS_UNKNOWN) {
        BND_CHK_PTR_BOUNDS(src, srcsz);
    } else {
        if (unlikely(srcsz > srcbos)) {
            invoke_safe_str_constraint_handler("wcsupr_s"
                                               ": smax exceeds src",
                                               (void *)src, EOVERFLOW);
            return RCNEGATE(EOVERFLOW);
        }
    }

    while (*src && slen) {
        *src = towlower(*src);
        src++;
        slen--;
    }

    return (EOK);
}
