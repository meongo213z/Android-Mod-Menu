#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <thread>
#include <string>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.hpp"
#include "Menu/Menu.hpp"
#include "Menu/Jni.hpp"
#include "Includes/Macros.h"
#include "dobby.h"

// TẠO MENU HIỂN THỊ
jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    const char *features[] = {
            // Nút công tắc số 0
            OBFUSCATE("0_Toggle_Vo han Go va Da (Free Build)")
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray) env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")), env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

// KHAI BÁO TÊN THƯ VIỆN GAME
#define targetLibName OBFUSCATE("libil2cpp.so")

// XỬ LÝ KHI NGƯỜI DÙNG BẬT/TẮT CÔNG TẮC
void Changes(JNIEnv *env, jclass clazz, jobject obj, jint featNum, jstring featName, jint value, jlong Lvalue, jboolean boolean, jstring text) {

    switch (featNum) {
        case 0:
            // 1. Vô hiệu hóa hàm trừ tài nguyên (SubtractResources)
            // Lệnh C0 03 5F D6 (RET)
            PATCH_SWITCH(targetLibName, "0x2C4D2AC", "C0 03 5F D6", boolean);

            // 2. Luôn báo đủ điều kiện xây dựng kể cả khi có 0 tài nguyên (EnoughResources)
            // Lệnh 20 00 80 52 C0 03 5F D6 (mov w0, #1; ret -> Return True)
            PATCH_SWITCH(targetLibName, "0x2C4D4A4", "20 00 80 52 C0 03 5F D6", boolean);
            break;
            
        default:
            break;
    }
}

// LUỒNG CHẠY NGẦM ĐỂ CHỜ GAME TẢI XONG CODE
void hack_thread() {
    // Vòng lặp chờ libil2cpp.so giải nén vào RAM
    while (!isLibraryLoaded(targetLibName)) {
        sleep(1); 
    }
    LOGI(OBFUSCATE("Menu Da San Sang!"));
}

// HÀM KHỞI CHẠY ĐẦU TIÊN KHI MỞ GAME
__attribute__((constructor))
void lib_main() {
    std::thread(hack_thread).detach();
}
