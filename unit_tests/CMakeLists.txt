cmake_minimum_required(VERSION 2.8)
project(ut)

set(SOURCES source/framework.cpp
            source/AVDeMuxerTest.cpp
            source/AVMuxerTest.cpp
            source/PacketTest.cpp
            source/H264DecoderTest.cpp
            source/H264EncoderTest.cpp
            source/H264TranscoderTest.cpp
            source/JPEGEncoderTest.cpp)

set(COMMON_LIBS XSDK AVKit)
set(WINDOWS_LIBS XSDK AVKit DbgHelp ws2_32 iphlpapi)

include(common.cmake NO_POLICY_SCOPE)
include("${devel_artifacts_path}/build/base_ut.cmake" NO_POLICY_SCOPE)
