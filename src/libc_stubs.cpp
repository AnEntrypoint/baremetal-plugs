#include <stddef.h>
#include <stdarg.h>
#include <wchar.h>
#include <stdio.h>

extern "C" {

char* getenv(const char*) { return nullptr; }

int sprintf(char* buf, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int r = vsnprintf(buf, 4096, fmt, ap);
    va_end(ap);
    return r;
}

wchar_t* wmemchr(const wchar_t* s, wchar_t c, size_t n) {
    for (size_t i = 0; i < n; ++i) if (s[i] == c) return (wchar_t*)(s + i);
    return nullptr;
}

int wmemcmp(const wchar_t* a, const wchar_t* b, size_t n) {
    for (size_t i = 0; i < n; ++i) { if (a[i] < b[i]) return -1; if (a[i] > b[i]) return 1; }
    return 0;
}

size_t wcslen(const wchar_t* s) {
    size_t n = 0; while (s[n]) ++n; return n;
}

wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, size_t n) {
    for (size_t i = 0; i < n; ++i) dst[i] = src[i]; return dst;
}

void abort() { while(1) {} }

}
