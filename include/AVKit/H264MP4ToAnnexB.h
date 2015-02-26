
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_H264MP4ToAnnexB_h
#define __AVKit_H264MP4ToAnnexB_h

#include "AVKit/Options.h"
#include "AVKit/AVDeMuxer.h"
#include "AVKit/Packet.h"
#include "AVKit/PacketFactory.h"

#include "XSDK/Types.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace AVKit
{

class H264MP4ToAnnexB
{
public:
    X_API H264MP4ToAnnexB( AVDeMuxer& deMuxer );

    X_API virtual ~H264MP4ToAnnexB() throw();

    X_API void SetPacketFactory( XIRef<PacketFactory> pf ) { _pf = pf; }

    X_API void Transform( XIRef<Packet> input, bool keyFrame );

    X_API XIRef<Packet> Get();

private:
    H264MP4ToAnnexB( const H264MP4ToAnnexB& obj );
    H264MP4ToAnnexB& operator = ( const H264MP4ToAnnexB& );

    void _FreeFilteredPacket();

    AVBitStreamFilterContext* _bsfc;
    AVCodecContext* _codec;
    AVPacket _filteredPacket;
    XIRef<PacketFactory> _pf;
};

}

#endif
