/**
 * @file common.h
 * common internal api header.
 */



#ifndef COMMON_H
#define COMMON_H


#    define CONFIG_WIN32

#    define EMULATE_INTTYPES
//MFC ADD
#    define HAVE_AV_CONFIG_H
//ADD END

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

#ifdef HAVE_AV_CONFIG_H
/* only include the following when compiling package */
#    include "config.h"
#    include <stdlib.h>
#    include <stdio.h>
#    include <string.h>
#    include <ctype.h>
#    include <limits.h>
#    ifndef __BEOS__
#        include <errno.h>
#    else
#        include "berrno.h"
#    endif
#    include <math.h>

#    ifndef ENODATA
#        define ENODATA  61
#    endif

#include <stddef.h>
#ifndef offsetof
# define offsetof(T,F) ((unsigned int)((char *)&((T *)0)->F))
#endif

#define AVOPTION_CODEC_BOOL(name, help, field) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_BOOL }
#define AVOPTION_CODEC_DOUBLE(name, help, field, minv, maxv, defval) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_DOUBLE, minv, maxv, defval }
#define AVOPTION_CODEC_FLAG(name, help, field, flag, defval) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_FLAG, flag, 0, defval }
#define AVOPTION_CODEC_INT(name, help, field, minv, maxv, defval) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_INT, minv, maxv, defval }
#define AVOPTION_CODEC_STRING(name, help, field, str, val) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_STRING, .defval = val, .defstr = str }
#define AVOPTION_CODEC_RCOVERRIDE(name, help, field) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_RCOVERRIDE, .defval = 0, .defstr = NULL }
#define AVOPTION_SUB(ptr) { .name = NULL, .help = (const char*)ptr }
#define AVOPTION_END() AVOPTION_SUB(NULL)

#endif /* HAVE_AV_CONFIG_H */

/* Suppress restrict if it was not defined in config.h.  */
#ifndef restrict
#define restrict
#endif

#ifndef always_inline
#define always_inline inline
#endif

#ifndef attribute_used
#    define attribute_used
#endif

#ifndef attribute_unused
#    define attribute_unused
#endif
	//MFC ADD
    typedef unsigned int size_t;
	//ADD END
    typedef signed char  int8_t;
    typedef signed short int16_t;
    typedef signed int   int32_t;
    typedef unsigned char  uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned int   uint32_t;

    typedef signed __int64   int64_t;
    typedef unsigned __int64 uint64_t;

#ifndef PRId64
#define PRId64 "lld"
#endif

#ifndef PRIu64
#define PRIu64 "llu"
#endif

#ifndef PRIx64
#define PRIx64 "llx"
#endif

#ifndef PRId32
#define PRId32 "d"
#endif

#ifndef PRIdFAST16
#define PRIdFAST16 PRId32
#endif

#ifndef PRIdFAST32
#define PRIdFAST32 PRId32
#endif

#ifndef INT16_MIN
#define INT16_MIN       (-0x7fff-1)
#endif

#ifndef INT16_MAX
#define INT16_MAX       0x7fff
#endif

#ifndef INT32_MIN
#define INT32_MIN       (-0x7fffffff-1)
#endif

#ifndef INT32_MAX
#define INT32_MAX       0x7fffffff
#endif

#ifndef UINT32_MAX
#define UINT32_MAX      0xffffffff
#endif

#ifndef INT64_MIN

#define INT64_MIN       (-0x7fffffffffffffffL-1)


#endif

#ifndef INT64_MAX
#define INT64_MAX int64_t_C(9223372036854775807)
#endif

#ifndef UINT64_MAX
#define UINT64_MAX uint64_t_C(0xFFFFFFFFFFFFFFFF)
#endif
// typedef int size_t;
// #define NULL 0
#ifdef EMULATE_FAST_INT
typedef signed char int_fast8_t;
typedef signed int  int_fast16_t;
typedef signed int  int_fast32_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int  uint_fast16_t;
typedef unsigned int  uint_fast32_t;
typedef uint64_t      uint_fast64_t;
#endif

#ifndef INT_BIT
#    if INT_MAX != 2147483647
#        define INT_BIT 64
#    else
#        define INT_BIT 32
#    endif
#endif

#ifdef CONFIG_WIN32

/* windows */

#define int64_t_C(c)     (c ## i64)
#define uint64_t_C(c)    (c ## i64)

#define inline __inline

#    ifdef HAVE_AV_CONFIG_H
#        ifdef _DEBUG
#            define DEBUG
#        endif

#        define snprintf _snprintf
#        define vsnprintf _vsnprintf

#        ifdef CONFIG_WINCE
#            define perror(a)
#        endif

#    endif
/* CONFIG_WIN32 end */
#endif /* !CONFIG_WIN32 && !CONFIG_OS2 */

#ifdef HAVE_AV_CONFIG_H

#  define FF_IMPORT_ATTR

#    include "bswap.h"

// Use rip-relative addressing if compiling PIC code on x86-64.

/* debug stuff */

#    include <assert.h>

inline void dprintf(const char* fmt,...) {}

#    ifdef CONFIG_WINCE
#            define abort()
#    endif

#    define av_abort()      do { av_log(NULL, AV_LOG_ERROR, "Abort at %s:%d\n", __FILE__, __LINE__); abort(); } while (0)

//rounded divison & shift
#define RSHIFT(a,b) ((a) > 0 ? ((a) + ((1<<(b))>>1))>>(b) : ((a) + ((1<<(b))>>1)-1)>>(b))
/* assume b>0 */
#define ROUNDED_DIV(a,b) (((a)>0 ? (a) + ((b)>>1) : (a) - ((b)>>1))/(b))
#define ABS(a) ((a) >= 0 ? (a) : (-(a)))

#define FFMAX(a,b) ((a) > (b) ? (a) : (b))
#define FFMIN(a,b) ((a) > (b) ? (b) : (a))

extern const uint32_t inverse[256];

#    define FASTDIV(a,b)   ((a)/(b))

/* define it to include statistics code (useful only for optimizing
   codec efficiency */
//#define STATS

/* misc math functions */
extern FF_IMPORT_ATTR const uint8_t ff_log2_tab[256];

static inline int av_log2(unsigned int v)
{
    int n;

    n = 0;
    if (v & 0xffff0000) {
        v >>= 16;
        n += 16;
    }
    if (v & 0xff00) {
        v >>= 8;
        n += 8;
    }
    n += ff_log2_tab[v];

    return n;
}

static inline int av_log2_16bit(unsigned int v)
{
    int n;

    n = 0;
    if (v & 0xff00) {
        v >>= 8;
        n += 8;
    }
    n += ff_log2_tab[v];

    return n;
}

/* median of 3 */
static inline int mid_pred(int a, int b, int c)
{
    if(a>b){
        if(c>b){
            if(c>a) b=a;
            else    b=c;
        }
    }else{
        if(b>c){
            if(c>a) b=c;
            else    b=a;
        }
    }
    return b;

}

/**
 * clip a signed integer value into the amin-amax range
 * @param a value to clip
 * @param amin minimum value of the clip range
 * @param amax maximum value of the clip range
 * @return cliped value
 */
static inline int clip(int a, int amin, int amax)
{
    if (a < amin)
        return amin;
    else if (a > amax)
        return amax;
    else
        return a;
}

/**
 * clip a signed integer value into the 0-255 range
 * @param a value to clip
 * @return cliped value
 */
static inline uint8_t clip_uint8(int a)
{
    if (a&(~255)) return (-a)>>31;
    else          return a;
}

/* math */
extern FF_IMPORT_ATTR const uint8_t ff_sqrt_tab[128];

int64_t ff_gcd(int64_t a, int64_t b);

static inline int ff_sqrt(int a)
{
    int ret=0;
    int s;
    int ret_sq=0;

    if(a<128) return ff_sqrt_tab[a];

    for(s=15; s>=0; s--){
        int b= ret_sq + (1<<(s*2)) + (ret<<s)*2;
        if(b<=a){
            ret_sq=b;
            ret+= 1<<s;
        }
    }
    return ret;
}

/**
 * converts fourcc string to int
 */
static inline int ff_get_fourcc(const char *s){
    assert( strlen(s)==4 );

    return (s[0]) + (s[1]<<8) + (s[2]<<16) + (s[3]<<24);
}

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))
#define MKBETAG(a,b,c,d) (d | (c << 8) | (b << 16) | (a << 24))

#define MASK_ABS(mask, level)\
            mask= level>>31;\
            level= (level^mask)-mask;

#define COPY3_IF_LT(x,y,a,b,c,d)\
if((y)<(x)){\
     (x)=(y);\
     (a)=(b);\
     (c)=(d);\
}

#define START_TIMER
#define STOP_TIMER(id) {}

/* avoid usage of various functions */
// 
// #define malloc please_use_av_malloc
// #define free please_use_av_free
// #define realloc please_use_av_realloc
// #define time time_is_forbidden_due_to_security_issues
// #define rand rand_is_forbidden_due_to_state_trashing
// #define srand srand_is_forbidden_due_to_state_trashing
// #define sprintf sprintf_is_forbidden_due_to_security_issues_use_snprintf
// #define strcat strcat_is_forbidden_due_to_security_issues_use_pstrcat

#define CHECKED_ALLOCZ(p, size)\
{\
    p= av_mallocz(size);\
    if(p==NULL && (size)!=0){\
        perror("malloc");\
        goto fail;\
    }\
}
#endif /* HAVE_AV_CONFIG_H */

#endif /* COMMON_H */
