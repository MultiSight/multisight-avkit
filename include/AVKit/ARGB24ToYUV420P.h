
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_ARGB24ToYUV420P_h
#define __AVKit_ARGB24ToYUV420P_h

#include "AVKit/Packet.h"
#include "AVKit/PacketFactory.h"

#include "XSDK/Types.h"
#include "XSDK/XMemory.h"

namespace AVKit
{

class ARGB24ToYUV420P
{
public:
    X_API ARGB24ToYUV420P();
    X_API virtual ~ARGB24ToYUV420P() throw();

    X_API void SetPacketFactory( XIRef<PacketFactory> pf ) { _pf = pf; }

    X_API void Transform( XIRef<Packet> pkt, size_t width, size_t height );

    X_API XIRef<Packet> Get();

private:
    ARGB24ToYUV420P( const ARGB24ToYUV420P& obj );
    ARGB24ToYUV420P& operator = ( const ARGB24ToYUV420P& );

    XIRef<Packet> _yuv420;
    XIRef<PacketFactory> _pf;
};

}

#endif
