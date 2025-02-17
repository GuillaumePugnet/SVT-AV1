#
# Copyright(c) 2019 Intel Corporation
#
# This source code is subject to the terms of the BSD 3 Clause Clear License and
# the Alliance for Open Media Patent License 1.0. If the BSD 3 Clause Clear License
# was not distributed with this source code in the LICENSE file, you can
# obtain it at https://www.aomedia.org/license/license. If the Alliance for Open
# Media Patent License 1.0 was not distributed with this source code in the
# PATENTS file, you can obtain it at https://www.aomedia.org/license/patent-license.
#

# Encoder Library directory CMakeLists.txt

# Shared Encoder Version
set(ENC_VERSION_MAJOR 0)
set(ENC_VERSION_MINOR 9)
set(ENC_VERSION_PATCH 1)
set(ENC_VERSION ${ENC_VERSION_MAJOR}.${ENC_VERSION_MINOR}.${ENC_VERSION_PATCH})

if(UNIX)
    if(NOT APPLE)
        find_library(M_LIB NAMES m)
        if(M_LIB)
            list(APPEND PLATFORM_LIBS m)
        endif()
        list(APPEND PLATFORM_LIBS Threads::Threads)
    endif()
    set(LIBS_PRIVATE "-lpthread -lm")
endif()

if(COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU")
        set(LIBS_PRIVATE "-lgcov ${LIBS_PRIVATE}")
    else(CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(LIBS_PRIVATE "--coverage ${LIBS_PRIVATE}")
    endif()
endif()

include_directories(${PROJECT_SOURCE_DIR}/Source/API/
    ${PROJECT_SOURCE_DIR}/Source/Lib/Common/Codec/
    ${PROJECT_SOURCE_DIR}/Source/Lib/Common/C_DEFAULT/
    ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/C_DEFAULT/
    ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/Codec/
    ${PROJECT_SOURCE_DIR}/third_party/fastfeat/)
link_directories(${PROJECT_SOURCE_DIR}/Source/Lib/Common/Codec/
    ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/C_DEFAULT/
    ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/Codec/
    ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/Globals/)

add_subdirectory(C_DEFAULT)
add_subdirectory(Codec)
add_subdirectory(Globals)
if(NOT COMPILE_C_ONLY AND HAVE_X86_PLATFORM)
    # Include Encoder Subdirectories
    include_directories(${PROJECT_SOURCE_DIR}/Source/Lib/Common/ASM_SSE2/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Common/ASM_SSSE3/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Common/ASM_SSE4_1/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Common/ASM_AVX2/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Common/ASM_AVX512/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/ASM_SSE2/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/ASM_SSSE3/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/ASM_SSE4_1/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/ASM_AVX2/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/ASM_AVX512/)

    link_directories(
        ${PROJECT_SOURCE_DIR}/Source/Lib/Common/ASM_SSE2/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Common/C_DEFAULT/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Common/ASM_SSSE3/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Common/ASM_SSE4_1/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Common/ASM_AVX2/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Common/ASM_AVX512/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/ASM_SSE2/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/ASM_SSSE3/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/ASM_SSE4_1/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/ASM_AVX2/
        ${PROJECT_SOURCE_DIR}/Source/Lib/Encoder/ASM_AVX512/)
    add_subdirectory(ASM_SSE2)
    add_subdirectory(ASM_SSSE3)
    add_subdirectory(ASM_SSE4_1)
    add_subdirectory(ASM_AVX2)
    add_subdirectory(ASM_AVX512)
endif()

# Required for cmake to be able to tell Xcode how to link all of the object files
if(CMAKE_GENERATOR STREQUAL "Xcode")
    file(WRITE ${PROJECT_BINARY_DIR}/dummy.c "")
endif()

file(GLOB all_files
    "*.h"
    "*.c")

get_target_property(common_lib_source common_lib INTERFACE_SOURCES)
if(common_lib_source)
    list(APPEND all_files ${common_lib_source} ${PROJECT_BINARY_DIR}/dummy.c)
endif()

# Encoder Lib Source Files
if(NOT COMPILE_C_ONLY AND HAVE_X86_PLATFORM)
    add_library(SvtAv1Enc
        ${all_files}
        $<TARGET_OBJECTS:COMMON_CODEC>
        $<TARGET_OBJECTS:FASTFEAT>
        $<TARGET_OBJECTS:COMMON_C_DEFAULT>
        $<TARGET_OBJECTS:COMMON_ASM_SSE2>
        $<TARGET_OBJECTS:COMMON_ASM_SSSE3>
        $<TARGET_OBJECTS:COMMON_ASM_SSE4_1>
        $<TARGET_OBJECTS:COMMON_ASM_AVX2>
        $<TARGET_OBJECTS:COMMON_ASM_AVX512>
        $<TARGET_OBJECTS:ENCODER_GLOBALS>
        $<TARGET_OBJECTS:ENCODER_CODEC>
        $<TARGET_OBJECTS:ENCODER_C_DEFAULT>
        $<TARGET_OBJECTS:ENCODER_ASM_SSE2>
        $<TARGET_OBJECTS:ENCODER_ASM_SSSE3>
        $<TARGET_OBJECTS:ENCODER_ASM_SSE4_1>
        $<TARGET_OBJECTS:ENCODER_ASM_AVX2>
        $<TARGET_OBJECTS:ENCODER_ASM_AVX512>)
else()
    add_library(SvtAv1Enc
        ${all_files}
        $<TARGET_OBJECTS:COMMON_CODEC>
        $<TARGET_OBJECTS:FASTFEAT>
        $<TARGET_OBJECTS:COMMON_C_DEFAULT>
        $<TARGET_OBJECTS:ENCODER_GLOBALS>
        $<TARGET_OBJECTS:ENCODER_CODEC>
        $<TARGET_OBJECTS:ENCODER_C_DEFAULT>)
endif()

set_target_properties(SvtAv1Enc PROPERTIES VERSION ${ENC_VERSION})
set_target_properties(SvtAv1Enc PROPERTIES SOVERSION ${ENC_VERSION_MAJOR})
set_target_properties(SvtAv1Enc PROPERTIES C_VISIBILITY_PRESET hidden)
target_link_libraries(SvtAv1Enc PUBLIC ${PLATFORM_LIBS})
if(NOT COMPILE_C_ONLY AND HAVE_X86_PLATFORM)
    target_link_libraries(SvtAv1Enc PRIVATE cpuinfo_public)
endif()

set(ENC_PKG_CONFIG_EXTRA_CFLAGS "")
if (BUILD_SHARED_LIBS)
    target_compile_definitions(SvtAv1Enc
        PUBLIC EB_DLL
        PRIVATE EB_BUILDING_SHARED_LIBS)
    set(ENC_PKG_CONFIG_EXTRA_CFLAGS " -DEB_DLL")
endif()
install(TARGETS SvtAv1Enc
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

configure_file(pkg-config.pc.in ${CMAKE_BINARY_DIR}/SvtAv1Enc.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/SvtAv1Enc.pc DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
