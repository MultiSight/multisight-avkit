
#include "AVKit/Packet.h"
#include "XSDK/XException.h"

extern "C"
{
#include "libavutil/avutil.h"
}

using namespace AVKit;

static const size_t PADDING = 16;

Packet::Packet( size_t sz ) :
    XBaseObject(),
    _bufferSize( ((sz % PADDING)==0) ? sz : sz + (PADDING - (sz % PADDING)) ),
    _requestedSize( sz ),
    _owning( true ),
    _buffer( NULL ),
    _dataSize( 0 ),
    _ts( 0 ),
    _ticksInSecond( 90000 ),
    _key( false )
{
    if( _bufferSize > 0 )
    {
        _buffer = (uint8_t*)av_malloc( _bufferSize );
        if( !_buffer )
            X_THROW(("Unable to allocate packet buffer."));
    }
}

Packet::Packet( uint8_t* src, size_t sz, bool owning ) :
    XBaseObject(),
    _bufferSize( ((sz % PADDING)==0) ? sz : sz + (PADDING - (sz % PADDING)) ),
    _requestedSize( sz ),
    _owning( owning ),
    _buffer( src ),
    _dataSize( sz ),
    _ts( 0 ),
    _ticksInSecond( 90000 ),
    _key( false )
{
    if( _owning )
    {
        _buffer = (uint8_t*)av_malloc( _bufferSize );
        if( !_buffer )
            X_THROW(("Unable to allocate packet buffer."));

        memcpy( _buffer, src, sz );
    }
    else
    {
        _bufferSize = sz;
    }
}

Packet::Packet( const Packet& obj ) :
    XBaseObject(),
    _bufferSize( 0 ),
    _owning( false ),
    _buffer( NULL ),
    _dataSize( 0 ),
    _ts( 0 ),
    _ticksInSecond( 90000 ),
    _key( obj._key )
{
    _Clear();

    _bufferSize = obj._bufferSize;
    _requestedSize = obj._requestedSize;
    _dataSize = obj._dataSize;
    _owning = obj._owning;

    if( obj._owning )
    {
        _buffer = (uint8_t*)av_malloc( _bufferSize );
        if( !_buffer )
            X_THROW(("Unable to allocate packet buffer."));

        memcpy( _buffer, obj._buffer, _bufferSize );
    }
    else
    {
        _buffer = obj._buffer;
    }

    _ts = obj._ts;
    _ticksInSecond = obj._ticksInSecond;
}

Packet::~Packet() noexcept
{
    _Clear();
}

Packet& Packet::operator = ( const Packet& obj )
{
    _Clear();
    _bufferSize = obj._bufferSize;
    _requestedSize = obj._requestedSize;
    _dataSize = obj._dataSize;
    _owning = obj._owning;

    if( obj._owning )
    {
        _buffer = (uint8_t*)av_malloc( _bufferSize );
        if( !_buffer )
            X_THROW(("Unable to allocate packet buffer."));

        memcpy( _buffer, obj._buffer, _bufferSize );
    }
    else
    {
        _buffer = obj._buffer;
    }

    _ts = obj._ts;
    _ticksInSecond = obj._ticksInSecond;
    _key = obj._key;

    return *this;
}

void Packet::Config( uint8_t* src, size_t sz, bool owning )
{
    _Clear();

    _bufferSize = sz;
    _requestedSize = sz;
    _owning = owning;
    _dataSize = sz;

    if( _owning )
    {
        _buffer = (uint8_t*)av_malloc( _bufferSize );
        if( !_buffer )
            X_THROW(("Unable to allocate packet buffer."));

        memcpy( _buffer, src, _bufferSize );
    }
    else
    {
        _buffer = src;
    }

    _ts = 0;
    _ticksInSecond = 90000;
    _key = false;
}

uint8_t* Packet::Map() const
{
    return _buffer;
}

size_t Packet::GetBufferSize() const
{
    return _requestedSize;
}

void Packet::SetDataSize( size_t sz )
{
    if( sz > _requestedSize )
        X_THROW(("Unable to set data size to amount greater than buffer size."));
    _dataSize = sz;
}

size_t Packet::GetDataSize() const
{
    return _dataSize;
}

void Packet::SetTS( uint32_t ts )
{
    _ts = ts;
}

uint32_t Packet::GetTS() const
{
    return _ts;
}

void Packet::SetTSFreq( uint32_t freq )
{
    _ticksInSecond = freq;
}

uint32_t Packet::GetTSFreq() const
{
    return _ticksInSecond;
}

void Packet::SetKey( bool key )
{
    _key = key;
}

bool Packet::IsKey() const
{
    return _key;
}

void Packet::_Clear() noexcept
{
    if( _owning && _buffer )
        av_free( _buffer );

    _buffer = NULL;
    _bufferSize = 0;
    _requestedSize = 0;
    _dataSize = 0;
    _ts = 0;
    _ticksInSecond = 90000;
}
