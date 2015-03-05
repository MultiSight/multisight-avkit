
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_H264Encoder_h
#define __AVKit_H264Encoder_h

#include "AVKit/Options.h"
#include "AVKit/FrameTypes.h"
#include "AVKit/Encoder.h"
#include "AVKit/Packet.h"
#include "AVKit/PacketFactory.h"
#include "XSDK/XMemory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace AVKit
{

const int H264_ENCODE_ATTEMPTS = 16;

class H264Encoder : public Encoder
{
public:

    X_API H264Encoder( const struct CodecOptions& options,
                       bool annexB = true,
                       int encodeAttempts = H264_ENCODE_ATTEMPTS );

    X_API virtual ~H264Encoder() throw();

    X_API void SetPacketFactory( XIRef<PacketFactory> pf ) { _pf = pf; }

    X_API virtual void EncodeYUV420P( XIRef<Packet> input, FrameType type = FRAME_TYPE_AUTO_GOP );

    X_API virtual XIRef<Packet> Get();

    X_API virtual bool LastWasKey() const { return _lastWasKey; }

    X_API virtual struct CodecOptions GetOptions() const;

    X_API virtual XIRef<XSDK::XMemory> GetExtraData() const;

private:
    H264Encoder( const H264Encoder& obj );
    H264Encoder& operator = ( const H264Encoder& );

    AVCodec* _codec;
    AVCodecContext* _context;
    struct CodecOptions _options;
    uint8_t _gopSize;
    uint8_t _numTillKey;
    int _encodeAttempts;
    XSDK::XMemory _extraData;
    bool _lastWasKey;
    bool _annexB;
    XIRef<PacketFactory> _pf;
    XIRef<Packet> _output;
};

}

#endif
