cmake_minimum_required(VERSION 3.19)

project(PainterEngine)
#################################################################
set(PAINTERENGINE_DIR ../../)
set(PAINTERENGINE_PROJECT_DIR ../../project)
#################################################################
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ANDROID_STL c++_static)

include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(${PAINTERENGINE_DIR})
include_directories(${PAINTERENGINE_DIR}/platform/android)

find_library(log-lib log)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

set(ANDROID_NATIVE_APP_GLUE ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
aux_source_directory(${PAINTERENGINE_DIR}/platform/android PAINTERENGINE_PLATFORM_ANDROID_SOURCES)
aux_source_directory(${PAINTERENGINE_DIR}/core PAINTERENGINE_CORE_SOURCES)
aux_source_directory(${PAINTERENGINE_DIR}/kernel PAINTERENGINE_KERNEL_SOURCES)
aux_source_directory(${PAINTERENGINE_DIR}/architecture PAINTERENGINE_ARCH_SOURCES)

add_library(PAINTERENGINELIB
        STATIC
        ${ANDROID_NATIVE_APP_GLUE}
        ${PAINTERENGINE_PLATFORM_ANDROID_SOURCES}
        ${PAINTERENGINE_ARCH_SOURCES}
        ${PAINTERENGINE_KERNEL_SOURCES}
        ${PAINTERENGINE_CORE_SOURCES})

include_directories(${PAINTERENGINE_PROJECT_DIR})
aux_source_directory(${PAINTERENGINE_PROJECT_DIR} PROJECT_DIR_SRC)
add_library(PainterEngineForAndroid SHARED ${PROJECT_DIR_SRC})

target_link_libraries(PainterEngineForAndroid
        PAINTERENGINELIB
        android
        ${log-lib}
        EGL
        GLESv3
        OpenSLES
        log)