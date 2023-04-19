cmake_minimum_required(VERSION 3.10)

project(PainterEngine LANGUAGES C CXX)

set(TARGET_NAME "PainterEngine")
set(PROJECT_PATH "project")
set(PAINTERENGINE_PATH ".")

if(WIN32)
    set(PLATFORM "windows")
    message("Windows platform detected")
else()
    set(PLATFORM "linux")
    message("Linux platform detected")
endif()

file(GLOB PROJECT_SRC
    "${PROJECT_PATH}/*.c"
    "${PROJECT_PATH}/*.cpp"
)
file(GLOB PAINTERENGINE_CORE_SRC "${PAINTERENGINE_PATH}/core/*.c")
file(GLOB PAINTERENGINE_KERNEL_SRC "${PAINTERENGINE_PATH}/kernel/*.c")
file(GLOB PAINTERENGINE_ARCHITECTURE_SRC "${PAINTERENGINE_PATH}/architecture/*.c")
file(GLOB PAINTERENGINE_PLATFORM_SRC
    "${PAINTERENGINE_PATH}/platform/${PLATFORM}/*.c"
    "${PAINTERENGINE_PATH}/platform/${PLATFORM}/*.cpp"
)

include_directories(
    "${PAINTERENGINE_PATH}"
    "${PROJECT_PATH}"
    "${PAINTERENGINE_PATH}/platform/${PLATFORM}"
)

add_executable(${TARGET_NAME}
    ${PROJECT_SRC}
    ${PAINTERENGINE_CORE_SRC}
    ${PAINTERENGINE_KERNEL_SRC}
    ${PAINTERENGINE_ARCHITECTURE_SRC}
    ${PAINTERENGINE_PLATFORM_SRC}
)

# Set C++17 standard
set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

if(PLATFORM STREQUAL "windows")
    set(CMAKE_CXX_COMPILER "cl")
    set(CMAKE_C_COMPILER "cl")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O1 /std:c++17 /GR- /EHsc /utf-8")

    target_link_libraries(${TARGET_NAME}
        OpenGL::GL
        Threads::Threads
    )
else()
    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -std=c++17 -fno-rtti -stdlib=libc++")

    find_package(GLUT REQUIRED)
    target_link_libraries(${TARGET_NAME}
        OpenGL::GL
        GLUT::GLUT
        Threads::Threads
    )
endif()