#ifndef UTIL_H
#define UTIL_H

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define __MIN_IMPL(_x, _y, _xn, _yn) __extension__({\
        __typeof__(_x) _xn = (_x);\
        __typeof__(_y) _yn = (_y);\
        (_xn < _yn ? _xn : _yn);\
        })
#define MIN(_x, _y) __MIN_IMPL(_x, _y, CONCAT(__x, __COUNTER__), CONCAT(__y, __COUNTER__))

#define __MAX_IMPL(_x, _y, _xn, _yn) __extension__({\
        __typeof__(_x) _xn = (_x);\
        __typeof__(_y) _yn = (_y);\
        (_xn > _yn ? _xn : _yn);\
        })
#define MAX(_x, _y) __MAX_IMPL(_x, _y, CONCAT(__x, __COUNTER__), CONCAT(__y, __COUNTER__))

#define CLAMP(_x, _mi, _ma) (MAX(_mi, MIN(_x, _ma)))

// returns the highest set bit of x
// i.e. if x == 0xF, HIBIT(x) == 3 (4th index)
// WARNING: currently only works for up to 32-bit types
#define HIBIT(_x) (31 - __builtin_clz((_x)))

// returns the lowest set bit of x
#define LOBIT(_x)\
    __extension__({ __typeof__(_x) __x = (_x); HIBIT(__x & -__x); })

// returns _v with _n-th bit = _x
#define BIT_SET(_v, _n, _x) __extension__({\
        __typeof__(_v) __v = (_v);\
        (__v ^ ((-(_x) ^ __v) & (1 << (_n))));\
        })

#define PACKED __attribute__((packed))

#ifndef asm
#define asm __asm__ volatile
#endif

#define CLI() asm ("cli")
#define STI() asm ("sti")

static inline unsigned short inports(unsigned short port) {
    unsigned short r;
    asm("inw %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

static inline void outports(unsigned short port, unsigned short data) {
    asm("outw %1, %0" : : "dN" (port), "a" (data));
}

static inline unsigned char inportb(unsigned short port) {
    unsigned char r;
    asm("inb %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

static inline void outportb(unsigned short port, unsigned char data) {
    asm("outb %1, %0" : : "dN" (port), "a" (data));
}

static inline unsigned int strlen(const char *str) {
    unsigned int l = 0;
    while (*str++ != 0) {
        l++;
    }
    return l;
}

static inline char *itoa(int x, char *s, unsigned int sz) {
    if (sz < 20) {
        // idfk
    }

    unsigned int tmp;
    int i, j;

    tmp = x;
    i = 0;

    do {
        tmp = x % 10;
        s[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (x /= 10);
    s[i--] = 0;

    for (j = 0; j < i; j++, i--) {
        tmp = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }

    return s;
}

static inline void memset(void *dst, unsigned char value, unsigned int n) {
    unsigned char *d = dst;

    while (n-- > 0) {
        *d++ = value;
    }
}

static inline void *memcpy(void *dst, const void *src, unsigned int n) {
    unsigned char *d = dst;
    const unsigned char *s = src;

    while (n-- > 0) {
        *d++ = *s++;
    }

    return d;
}

static inline void *memmove(void *dst, const void *src, unsigned int n) {
    // OK since we know that memcpy copies forwards
    if (dst < src) {
        return memcpy(dst, src, n);
    }

    unsigned char *d = dst;
    const unsigned char *s = src;

    for (unsigned int i = n; i > 0; i--) {
        d[i - 1] = s[i - 1];
    }

    return dst;
}

// SEE: https://opensource.apple.com/source/Libc/Libc-1158.30.7/string/strlcat.c.auto.html
static inline unsigned int strlcat(char *dst, const char *src, unsigned int size) {
    const unsigned int sl = strlen(src),
          dl = strlen(dst);

    if (dl == size) {
        return size + sl;
    }

    if (sl < (size - dl)) {
        memcpy(dst + dl, src, sl + 1);
    } else {
        memcpy(dst + dl, src, size - dl - 1);
        dst[size - 1] = '\0';
    }

    return sl + dl;
}

static inline unsigned int strlcpy(char *dst, const char *src, unsigned int n) {
    // copy as many bytes as can fit
    char *d = dst;
    const char *s = src;
    unsigned int size = n;

    while (--n > 0) {
        if ((*d++ = *s++) == 0) {
            break;
        }
    }

    // if we ran out of space, null terminate
    if (n == 0) {
        if (size != 0) {
            *d = 0;
        }

        // traverse the rest of s
        while (*s++);
    }

    return s - src - 1;
}

char shift_char(char ch);

#endif
