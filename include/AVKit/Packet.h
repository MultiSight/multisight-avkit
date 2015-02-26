
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit__Packet_h
#define __AVKit__Packet_h

#include "XSDK/XIRef.h"
#include "XSDK/XBaseObject.h"

class PacketTest;

namespace AVKit
{

class Packet : public XSDK::XBaseObject
{
    friend class ::PacketTest;

public:
    Packet( size_t sz );
    Packet( uint8_t* src, size_t sz, bool owning = true );
    Packet( const Packet& obj );

    virtual ~Packet() throw();

    Packet& operator = ( const Packet& obj );

    uint8_t* Map() const;

    size_t GetBufferSize() const;

    void SetDataSize( size_t sz );
    size_t GetDataSize() const;

    void SetTS( uint32_t ts );
    uint32_t GetTS() const;

    void SetTSFreq( uint32_t freq );
    uint32_t GetTSFreq() const;

    void SetKey( bool key );
    bool IsKey() const;

private:
    void _Clear() throw();

    // The actual size of the buffer we may or may not have allocated (includes padding).
    size_t _bufferSize;
    // The requested buffer size.
    size_t _requestedSize;
    // true if we own the memory of our buffer (and we therefore need to copy the data in), false otherwise.
    bool _owning;
    // a pointer to our buffer (that we may more may not have allocated).
    uint8_t* _buffer;
    // the amount of valid (set) memory in the buffer.
    size_t _dataSize;
    // the current timestamp
    uint32_t _ts;
    // how many _ts ticks are in one second.
    uint32_t _ticksInSecond;
    // whether this packet is a key frame
    bool _key;
};

}

#endif
