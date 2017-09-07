# Overview of various libc's regarding the secure C11 extensions

C11 defined the optional secure extensions to be demanded by 
`__STDC_WANT_LIB_EXT1__`.

From the following tested libc implementations:

* glibc
* musl
* FreeBSD and DragonFly libc
* OpenBSD libc
* newlib
* Microsoft Windows/MINGW\_HAS\_SECURE_API
* Android Bionic
* Embarcadero C++ libc

only the following implement the secure C11 extensions:

* Microsoft Windows
* Android Bionic
* Embarcadero C++ libc

# General quirks

Generally most 64-bit versions use a 4 byte wchar_t, but windows/cygwin
uses 2-byte.

The wide sprintf variants do not allow a NULL buffer argument to
return the size of the resulting buffer. If the initial buffer is too
small, you need to realloc and redo.

## Caveat's for the various implementations regarding C11 Annex K/safec

# snprintf, vsnprintf, snwprintf, vsnwprintf, tmpnam_s

They are all considered unsafe. The 4 'n' truncating printf versions
don't guarantee null-delimited destination buffers.

`tmpnam_s` and `tmpnam` are racy.

# Microsoft Windows/MINGW_HAS_SECURE_API

* `fopen_s`, `freopen_s` deviate in the API: restrict is missing.

* `strtok_s`, `wcstok_s`,`vsnprintf_s` miss the dmax argument.

* `vsnprintf_s` adds a maxarg argument.

* `vswprintf` adds a maxarg argument on w32. (with `__STRICT_ANSI__`
  undefined)

* no `strnlen` on mingw32

# safeclib

* safeclib does not check optional NULL parameters to the `scanf_s`,
  `printf_s` functions. This would need tighter integration into the
  upstream libc.

* safeclib `fgets_s` permits temporary writes of dmax+1 characters
  into dest.

## Other caveat's for the various implementations

# glibc

* SEGV with `freopen(NULL, "rb", stdin)` with asan on some systems,
  calling an invalid strlen() on NULL.

# newlib

* `vswscanf` is broken with a format string containing L"%%n"

* The following multibyte API's are missing, and can be defined like
  this:

```
    mbstate_t st;
    #define wctomb(dest, wc) wcrtomb((dest), (wc), &st)
    #define wcstombs(dest, src, len) wcsrtombs((dest), &(src), (len), &st)
    #define mbstowcs(dest, src, len) mbsrtowcs((dest), &(src), (len), &st)
```

# FreeBSD libc

* `vswscanf` is broken with a format string containing L"%%n"

* `mbstowcs` is broken with (NULL, '\0')

# musl

* `wmemcmp` returns not -1,0,1 but the full ptr diff.

* `mbtowc` and `wctomb` accept and convert illegal 4 byte characters
  in the ASCII locale to surrogate pairs, as it would be unicode.
  e.g. it converts \xa0 to \xdfa0.

----

It's now 10 years after the secure libc extensions were designed, C11
adopted them, and still almost nobody implements them.  This library
was written 2008 as MIT.

Reini Urban 2017