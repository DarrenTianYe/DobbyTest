//
// Created by darren on 2021/12/8.
//

#include <string.h>
#include <inttypes.h>
#include <pthread.h>
#include <jni.h>
#include <android/log.h>
#include <assert.h>
#include <stdio.h>


// Android log function wrappers
static const char* kTAG = "Jni_test";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))


// 读取文件内容
int h_get_content(const char *path, size_t size, long base, char *content)
{
    int ret = 0;
    do {
        if (path == NULL || content == NULL) {
            LOGE("Attack -> _h_get_file_content ");
            ret = 1;
            break;
        }

        FILE *fp = fopen(path, "r");
        if (fp == NULL) {
            LOGE("Attack -> _h_get_file_content open %s failed", path);
            ret = 2;
            break;
        }

        if (fseek(fp, base-1, SEEK_SET) != 0) {
            LOGE("Attack -> _h_get_file_content fseek %s failed", path);
            ret = 3;
            break;
        }

        if (fread(content, size, 1, fp) != 1) {
            LOGE("Attack -> _h_get_file_content fread %s failed", path);
            ret = 4;
            break;
        }

        fclose(fp);
        fp = NULL;
    } while (0);
    return ret;
}

void dump_memory(long base, int size ){

    //dump_maps();
    const char *mem_file_name = "/proc/self/mem";
    int buffer_len = 16;
    char content[buffer_len] ;
    char content1[buffer_len] ;



    h_get_content(mem_file_name, buffer_len ,base,  content);
    for(int i=0; i< size; i++){
        LOGE("content==val: %x", content[i]);
    }


    int count = sprintf(content1, "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
                        content[0],content[1],content[2],content[3],
                        content[4],content[5],content[6],content[7],
                        content[8],content[9],content[10],content[11],
                        content[12],content[13],content[14],content[15]);

    LOGE("content1: %d, ====%s", count, content1);

//    void *handle = bp_dlopen("libart.so", RTLD_NOW);
//    void *pretty_method = bp_dlsym(handle, "_ZN3art9ArtMethod12PrettyMethodEb");
//    LOGE(" bypass dlopen, dlopen result: %p,  dlsym result: %p", handle, pretty_method);
//    dump_memory((long)pretty_method, 100);

}

#define RET_DATA_1   0x001  /* Read-only open */
#define RET_DATA_2   0x002  /* Write-only open */
#define RET_DATA_4     0x004  /* Read-write open. */


static int dump_maps() {
    int ret_code = 0;
    FILE *fp;
    char buff[256]={0};
    if ((fp = fopen("/proc/self/maps", "r")) == NULL) {
        LOGE("######### /proc/self/maps open  failed! #########");
        return 0;
    }
    while (fgets(buff, sizeof(buff), fp)) {
        if (strstr(buff, "libart.so")){
            LOGE("dump_maps: %s", buff);
            ret_code |= RET_DATA_1;
            if (strstr(buff, "rwx")){
                ret_code |= RET_DATA_2;
            }
            if (strstr(buff, "rwx")){
                ret_code |= RET_DATA_4;
            }
        }
    }
    fclose(fp);
    return ret_code;
}


int test(){

    dump_maps();

    return 0;
}