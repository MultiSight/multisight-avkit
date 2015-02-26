
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_PacketFactory_h
#define __AVKit_PacketFactory_h

#include "XSDK/XIRef.h"
#include "XSDK/XBaseObject.h"
#include "AVKit/Packet.h"

namespace AVKit
{

class PacketFactory : public XSDK::XBaseObject
{
public:
    PacketFactory() :
        XBaseObject()
    {
    }

    virtual ~PacketFactory() throw() {}

    virtual XIRef<Packet> Get( size_t sz ) const = 0;
};

class PacketFactoryDefault : public PacketFactory
{
public:
    PacketFactoryDefault() :
        PacketFactory()
    {
    }

    virtual ~PacketFactoryDefault() throw() {}

    virtual XIRef<Packet> Get( size_t sz ) const { return new Packet( sz ); }

private:
    PacketFactoryDefault( const PacketFactoryDefault& );
    PacketFactoryDefault& operator = ( const PacketFactoryDefault& );
};

}

#endif
