

#ifndef IOT_SECURITY_SDK_LOGGER_H
#define IOT_SECURITY_SDK_LOGGER_H
#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif


#define SEC_NET_LOG 1
#define __android__ 1

#define MAX_LOG_LEN     2048
#define SEC_TAG         "SEC-SDKN"
#define SEC_TAG_L       "SEC-SDKL"

#include <android/log.h>
#define  LOGX(T, TAG, ll_format, ...)                                                                                       \
    {                                                                                                                          \
        char ll_cmsg[MAX_LOG_LEN]  = {0};                                                                                      \
        snprintf(ll_cmsg, MAX_LOG_LEN, " %s:%d ", __FILE__, __LINE__);                                                         \
        if (strcmp("V", T) == 0)__android_log_print(ANDROID_LOG_VERBOSE, TAG, "%-100s " ll_format "", ll_cmsg, ##__VA_ARGS__);  \
        if (strcmp("D", T) == 0)__android_log_print(ANDROID_LOG_DEBUG,   TAG, "%-100s " ll_format "", ll_cmsg, ##__VA_ARGS__);  \
        if (strcmp("I", T) == 0)__android_log_print(ANDROID_LOG_INFO,    TAG, "%-100s " ll_format "", ll_cmsg, ##__VA_ARGS__);  \
        if (strcmp("W", T) == 0)__android_log_print(ANDROID_LOG_WARN,    TAG, "%-100s " ll_format "", ll_cmsg, ##__VA_ARGS__);  \
        if (strcmp("E", T) == 0)__android_log_print(ANDROID_LOG_ERROR,   TAG, "%-100s " ll_format "", ll_cmsg, ##__VA_ARGS__);  \
        if (strcmp("F", T) == 0)__android_log_print(ANDROID_LOG_FATAL,   TAG, "%-100s " ll_format "", ll_cmsg, ##__VA_ARGS__);  \
        if (strcmp("T", T) == 0)__android_log_print(ANDROID_LOG_INFO,    TAG, "%-100s " ll_format "", ll_cmsg, ##__VA_ARGS__);  \
    }

#define SDK_INIT_CODE_SUCCEED   "00"
#define SDK_INIT_CODE_FAILED    "01"

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif
#endif //IOT_SECURITY_SDK_LOGGER_H
