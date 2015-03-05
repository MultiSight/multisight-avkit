
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_YUV420PToARGB24_h
#define __AVKit_YUV420PToARGB24_h

#include "AVKit/Packet.h"
#include "AVKit/PacketFactory.h"
#include "XSDK/Types.h"

namespace AVKit
{

class YUV420PToARGB24
{
public:
    X_API YUV420PToARGB24();

    X_API virtual ~YUV420PToARGB24() throw();

    X_API void SetPacketFactory( XIRef<PacketFactory> pf ) { _pf = pf; }

    X_API void Transform( XIRef<Packet> intput, size_t width, size_t height );

    X_API XIRef<Packet> Get();

private:
    YUV420PToARGB24( const YUV420PToARGB24& obj );
    YUV420PToARGB24& operator = ( const YUV420PToARGB24& );

    XIRef<Packet> _rgb24;
    XIRef<PacketFactory> _pf;
};

}

#endif
