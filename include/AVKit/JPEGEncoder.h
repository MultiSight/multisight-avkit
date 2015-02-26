
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_JPEGEncoder_h
#define __AVKit_JPEGEncoder_h

#include "AVKit/Options.h"
#include "AVKit/Packet.h"
#include "AVKit/PacketFactory.h"
#include "XSDK/XMemory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace AVKit
{

const int JPEG_ENCODE_ATTEMPTS = 16;

class JPEGEncoder
{
public:
    X_API JPEGEncoder( const struct CodecOptions& options, int encodeAttempts = JPEG_ENCODE_ATTEMPTS );

    X_API virtual ~JPEGEncoder() throw();

    X_API void SetPacketFactory( XIRef<PacketFactory> pf ) { _pf = pf; }

    X_API void EncodeYUV420P( XIRef<Packet> input );

    X_API XIRef<Packet> Get();

    X_API static void WriteJPEGFile( const XSDK::XString& fileName, XIRef<Packet> jpeg );

private:
    JPEGEncoder( const JPEGEncoder& obj );
    JPEGEncoder& operator = ( const JPEGEncoder& );

    AVCodec* _codec;
    AVCodecContext* _context;
    struct CodecOptions _options;
    int _encodeAttempts;
    XIRef<Packet> _output;
    XIRef<PacketFactory> _pf;
};

}

#endif
