#include <jni.h>
#include <string>

#include <android/log.h>
#include <fcntl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "Dobby/include/dobby.h"
#include "tools/Logcat.h"

extern "C"{

#include "JniTest.h"

}




JNIEXPORT jint JNICALL meaning_of_the_universe(JNIEnv *env, jclass clazz);

static int (*orig_log_print)(int prio, const char* tag, const char* fmt, ...);
static int my_libtest_log_print(int prio, const char* tag, const char* fmt, ...)
{
    va_list ap;
    char buf[1024];
    int r;

    snprintf(buf, sizeof(buf), "[%s] %s", (NULL == tag ? "" : tag), (NULL == fmt ? "" : fmt));
    va_start(ap, fmt);
    r = __android_log_vprint(prio, "Dobby_libtest", buf, ap);
    va_end(ap);
    return r;
}

static int (*orig_open)(const char *, int, ...);
static int my_open(const char *path, int oflag, ...) {
    va_list ap = {0};
    mode_t mode = 0;

    LOGX("V", "darren_Dobby", "my_open");

    printf("Calling my_open open('%s')\n", path);
    if ((oflag & O_CREAT) != 0) {
        // mode only applies to O_CREAT
        va_start(ap, oflag);
        mode = va_arg(ap, int);
        va_end(ap);
        printf("Calling real open('%s', %d, %d)\n", path, oflag, mode);
        return open(path, oflag, mode);
    } else {
        printf("Calling real open('%s', %d)\n", path, oflag);
        return open(path, oflag, mode);
    }
}


void* oldfunc_fstat;
static int my_fstat(int id_conn,struct stat * info){


    LOGX("V", "darren_Dobby", "my_fstat %p", info);
   // int ret = fstat(id_conn, info);
    LOGX("V", "darren_Dobby", "my_fstat end:%p ", info->st_ctim);

    return 0;
}


void* oldfuncPuts;
int myputs(char* str){

    printf("hook :%s\n",str);
    int i=1;
    for(;*(str+i)!=0;i++){}

    LOGX("V", "darren_Dobby", "%s", str);

    return i;
}

uintptr_t getCallFirstArg(RegisterContext *ctx) {
    uintptr_t result;
#if defined(_M_X64) || defined(__x86_64__)
    #if defined(_WIN32)
  result = ctx->general.regs.rcx;
#else
  result = ctx->general.regs.rdi;
#endif
#elif defined(__arm64__) || defined(__aarch64__)
    result = ctx->general.regs.x0;
#elif defined(__arm__)
    result = ctx->general.regs.r0;
#else
#error "Not Support Architecture."
#endif
    return result;
}

void format_integer_manually(char *buf, uint64_t integer) {
    int tmp = 0;
    for (tmp = integer; tmp > 0; tmp = (tmp >> 4)) {
        buf += (tmp % 16);
        buf--;
    }
}



void malloc_handler(RegisterContext *ctx, const HookEntryInfo *info) {
    size_t size_ = 0;
    size_        = getCallFirstArg(ctx);
    char *buffer = "[-] function malloc first arg: 0x00000000.\n";
    format_integer_manually(strchr(buffer, '.') - 1, size_);
    puts(buffer);

}


void* oldfuncmeaning_of_the_universe_my;
JNIEXPORT jint JNICALL meaning_of_the_universe_my(JNIEnv *env, jclass clazz) {
    return 10000;
}

__attribute__((constructor)) static void ctor() {


      log_switch_to_file("/sdcard/log.aa");
      log_switch_to_syslog();


      DobbyHook((void*)&meaning_of_the_universe,(void*)meaning_of_the_universe_my,(void **)&oldfuncmeaning_of_the_universe_my);

      DobbyHook((void *) DobbySymbolResolver(NULL, "__android_log_print"), (void *) my_libtest_log_print,(void **) &orig_log_print);

    //  DobbyHook((void *) DobbySymbolResolver(NULL, "fstat"), (void *) my_fstat,(void **) &oldfunc_fstat);

      puts("puts1\n");

      DobbyHook((void*)&puts,(void*)myputs,(void **)&oldfuncPuts);
      //DobbyHook((void*)&memcpy,(void*)myputs,(void **)&oldfuncPuts);

//    DobbyInstrument((void *)malloc, malloc_handler);

   // DobbyHook((void*)&openat,(void*)my_open,(void **)&orig_open);
   // DobbyHook((void*)&fstat,(void*)my_fstat,(void **)&oldfunc_fstat);

  //  DobbyHook((void *) DobbySymbolResolver(NULL, "open"), (void *) my_open,(void **) &orig_open);
  //  DobbyHook((void *) DobbySymbolResolver(NULL, "fstat"), (void *) my_fstat,(void **) &orig_fstat);

}

//extern "C"
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_dobbytest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from my dobby ~~~~";

    LOGX("V", "darren_Dobby", "xxxx");
    puts("puts2 Java_com_example_dobbytest_MainActivity_stringFromJNI\n");

    int ret_test = test();
    //LOGX("V", "ret==%d", ret_test);

    return env->NewStringUTF(hello.c_str());
}

#define JNI_REG_CLASS "com/example/dobbytest/MainActivity" // path of Java file

JNIEXPORT jstring JNICALL get_hello(JNIEnv *env, jclass clazz) {
    return env->NewStringUTF("hello from jni get_hello");
}

JNIEXPORT jint JNICALL meaning_of_the_universe(JNIEnv *env, jclass clazz) {
    return 42;
}
//
//int count = sprintf(content1, "%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %d",
//                    content[0],content[1],content[2],content[3],
//                    content[4],content[5],content[6],content[7],
//                    content[8],content[9],content[10],content[11],
//                    content[12],content[13],content[14],content[15], ret_libart);
//


static JNINativeMethod g_methods[] = {
        { "getHello", "()Ljava/lang/String;", (void*)get_hello},
        { "meaningOfTheUniverse", "()I", (void*)meaning_of_the_universe},
};

// must define this function
JNIEXPORT int JNICALL JNI_OnLoad(JavaVM *vm,void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env,JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    LOGX("V", "darren_Dobby", "reserved:%x", reserved);

    jclass javaClass = env->FindClass(JNI_REG_CLASS);
    if (javaClass == NULL){
        return JNI_ERR;
    }

    int method_count = sizeof(g_methods) / sizeof(g_methods[0]);
    if (env->RegisterNatives(javaClass, g_methods, method_count) < 0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

