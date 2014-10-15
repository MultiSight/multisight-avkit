cmake_minimum_required(VERSION 2.8)
project(AVKit)

set(SOURCES source/ARGB24ToYUV420P.cpp
            source/AVDeMuxer.cpp
            source/AVMuxer.cpp
            source/H264Decoder.cpp
            source/H264Encoder.cpp
            source/H264MP4ToAnnexB.cpp
            source/H264Transcoder.cpp
            source/JPEGEncoder.cpp
            source/Locky.cpp
            source/Options.cpp
            source/Utils.cpp
            source/YUV420PToARGB24.cpp)

set(WINDOWS_LIBS XSDK ws2_32 avutil-54 avcodec-56 avformat-56 avfilter-5 swscale-3 avutil-54)
set(LINUX_LIBS XSDK avutil avcodec avformat swscale)

#if(CMAKE_SYSTEM MATCHES "Linux-")
#    include_directories(/usr/include/ffmpeg)
#endif(CMAKE_SYSTEM MATCHES "Linux-")

add_definitions(-D__STDC_CONSTANT_MACROS)

include(common.cmake NO_POLICY_SCOPE)
include("${devel_artifacts_path}/build/base_lib.cmake" NO_POLICY_SCOPE)
