#include <stddef.h>
#include <stdarg.h>
#include <wchar.h>
#include <ctype.h>

extern "C" {

struct _reent;
struct _reent* _impure_ptr = nullptr;

char* getenv(const char*) { return nullptr; }

static void _putc_buf(char** p, char* end, char c) {
    if (*p < end) { **p = c; ++(*p); }
}

static void _puts_buf(char** p, char* end, const char* s) {
    while (s && *s) _putc_buf(p, end, *s++);
}

static void _putu_buf(char** p, char* end, unsigned long long v, int base) {
    if (v >= (unsigned long long)base) _putu_buf(p, end, v / base, base);
    char c = (char)(v % base);
    _putc_buf(p, end, c < 10 ? '0' + c : 'a' + c - 10);
}

int vsnprintf(char* buf, size_t n, const char* fmt, va_list ap) {
    char* p = buf, * end = buf + (n > 0 ? n - 1 : 0);
    for (; *fmt; ++fmt) {
        if (*fmt != '%') { _putc_buf(&p, end, *fmt); continue; }
        ++fmt;
        if (*fmt == 'd' || *fmt == 'i') { long long v = va_arg(ap, int); if (v < 0) { _putc_buf(&p, end, '-'); v = -v; } _putu_buf(&p, end, (unsigned long long)v, 10); }
        else if (*fmt == 'u') { _putu_buf(&p, end, va_arg(ap, unsigned), 10); }
        else if (*fmt == 'x') { _putu_buf(&p, end, va_arg(ap, unsigned), 16); }
        else if (*fmt == 's') { _puts_buf(&p, end, va_arg(ap, const char*)); }
        else if (*fmt == 'c') { _putc_buf(&p, end, (char)va_arg(ap, int)); }
        else if (*fmt == '%') { _putc_buf(&p, end, '%'); }
        else { va_arg(ap, int); }
    }
    if (n > 0) *p = 0;
    return (int)(p - buf);
}

int snprintf(char* buf, size_t n, const char* fmt, ...) {
    va_list ap; va_start(ap, fmt); int r = vsnprintf(buf, n, fmt, ap); va_end(ap); return r;
}
int sprintf(char* buf, const char* fmt, ...) {
    va_list ap; va_start(ap, fmt); int r = vsnprintf(buf, 4096, fmt, ap); va_end(ap); return r;
}

int fputc(int c, void*) { return c; }
int fputs(const char*, void*) { return 0; }
size_t fwrite(const void*, size_t, size_t, void*) { return 0; }

void* memchr(const void* s, int c, size_t n) {
    const unsigned char* p = (const unsigned char*)s;
    for (size_t i = 0; i < n; ++i) if (p[i] == (unsigned char)c) return (void*)(p + i);
    return nullptr;
}
double atof(const char* s) {
    double r = 0.0, frac = 1.0; int neg = 0;
    while (*s == ' ') ++s;
    if (*s == '-') { neg = 1; ++s; } else if (*s == '+') ++s;
    while (*s >= '0' && *s <= '9') r = r * 10.0 + (*s++ - '0');
    if (*s == '.') { ++s; while (*s >= '0' && *s <= '9') { frac /= 10.0; r += (*s++ - '0') * frac; } }
    return neg ? -r : r;
}
int tolower(int c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }

wchar_t* wmemchr(const wchar_t* s, wchar_t c, size_t n) {
    for (size_t i=0;i<n;++i) if (s[i]==c) return (wchar_t*)(s+i); return nullptr;
}
int wmemcmp(const wchar_t* a, const wchar_t* b, size_t n) {
    for (size_t i=0;i<n;++i) { if (a[i]<b[i]) return -1; if (a[i]>b[i]) return 1; } return 0;
}
size_t wcslen(const wchar_t* s) { size_t n=0; while(s[n]) ++n; return n; }
wchar_t* wmemcpy(wchar_t* d, const wchar_t* s, size_t n) { for(size_t i=0;i<n;++i) d[i]=s[i]; return d; }
wchar_t* wmemmove(wchar_t* d, const wchar_t* s, size_t n) {
    if (d<s) for(size_t i=0;i<n;++i) d[i]=s[i]; else for(size_t i=n;i>0;--i) d[i-1]=s[i-1]; return d;
}
wchar_t* wmemset(wchar_t* s, wchar_t c, size_t n) { for(size_t i=0;i<n;++i) s[i]=c; return s; }

float sinf(float x) { return __builtin_sinf(x); }
float cosf(float x) { return __builtin_cosf(x); }
float tanf(float x) { return __builtin_tanf(x); }
float sqrtf(float x) { return __builtin_sqrtf(x); }
float logf(float x) { return __builtin_logf(x); }
float powf(float x, float y) { return __builtin_powf(x, y); }
float atan2f(float y, float x) { return __builtin_atan2f(y, x); }

void abort() { while(1) {} }

}
