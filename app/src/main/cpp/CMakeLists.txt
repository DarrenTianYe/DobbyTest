## For more information about using CMake with Android Studio, read the
## documentation: https://d.android.com/studio/projects/add-native-code.html
#
## Sets the minimum version of CMake required to build the native library.
#
cmake_minimum_required(VERSION 3.10.2)
#
## Declares and names the project.
#
project("dobbytest")

# 这里指定静态链接，生成一个so;默认为 ON，生成两个so
set(GENERATE_SHARED ON)
# 指定 dobby 库目录
set(DOBBY_SOURCE_DIR Dobby)
add_subdirectory(${DOBBY_SOURCE_DIR} dobby.out)

add_library( # Sets the name of the library.
        native-lib
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        native-lib.cpp
        JniTest.c
        tools/Logcat.h)

find_library( # Sets the name of the path variable.
        log-lib
        log)

target_link_libraries( # Specifies the target library.
        native-lib
        # 添加 dobby 库
        dobby
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})


