cmake_minimum_required(VERSION 3.10)

project(PainterEngine LANGUAGES C CXX)

set(TARGET_NAME "PainterEngine")
set(PROJECT_PATH "../../project")
set(PAINTERENGINE_PATH "../..")

file(GLOB PROJECT_SRC
    "${PROJECT_PATH}/*.c"
    "${PROJECT_PATH}/*.cpp"
)
file(GLOB PAINTERENGINE_CORE_SRC "${PAINTERENGINE_PATH}/core/*.c")
file(GLOB PAINTERENGINE_KERNEL_SRC "${PAINTERENGINE_PATH}/kernel/*.c")
file(GLOB PAINTERENGINE_ARCHITECTURE_SRC "${PAINTERENGINE_PATH}/architecture/*.c")
file(GLOB PAINTERENGINE_PLATFORM_SRC
    "${PAINTERENGINE_PATH}/platform/linux/*.c"
    "${PAINTERENGINE_PATH}/platform/linux/*.cpp"
)

include_directories(
    "${PAINTERENGINE_PATH}"
    "${PROJECT_PATH}"
    "${PAINTERENGINE_PATH}/platform/linux"
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

# Use Clang compiler
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

# Set compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -std=c++17 -fno-rtti -stdlib=libc++")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(${TARGET_NAME}
    OpenGL::GL
    GLUT::GLUT
    Threads::Threads
)
