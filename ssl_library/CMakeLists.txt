# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

include_directories(src/main/cpp/include)

add_library(util SHARED
    src/main/cpp/utils.cpp
    src/main/cpp/device_id.cpp)

find_library(log-lib log)
target_link_libraries(util ${log-lib})

add_library(crypto STATIC IMPORTED)
set_target_properties(crypto
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libcrypto.a)

add_library(ssl STATIC IMPORTED)
set_target_properties(ssl
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libssl.a)

target_link_libraries(util crypto ssl)
