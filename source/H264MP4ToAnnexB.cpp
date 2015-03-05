
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "AVKit/H264MP4ToAnnexB.h"

#include "XSDK/XException.h"

using namespace AVKit;
using namespace XSDK;

static const size_t DEFAULT_PADDING = 16;

H264MP4ToAnnexB::H264MP4ToAnnexB( AVDeMuxer& deMuxer ) :
    _bsfc( av_bitstream_filter_init( "h264_mp4toannexb" ) ),
    _codec( deMuxer._context->streams[deMuxer._videoStreamIndex]->codec ),
    _filteredPacket(),
    _pf( new PacketFactoryDefault )
{
    if( !_bsfc )
        X_THROW(("Unable to initialize h264_mp4toannexb bitstream filter."));

    if( !_codec )
        X_THROW(("H264MP4ToAnnexB requires valid codec context (either from an encoder or a demuxer)."));
}

H264MP4ToAnnexB::~H264MP4ToAnnexB() throw()
{
    _FreeFilteredPacket();

    av_bitstream_filter_close( _bsfc );
}

void H264MP4ToAnnexB::Transform( XIRef<Packet> input, bool keyFrame )
{
    AVPacket inputPacket;
    av_init_packet( &inputPacket );
    inputPacket.data = input->Map();
    inputPacket.size = input->GetDataSize();

    _FreeFilteredPacket();

    // If the return value is 0, the output buffer is not allocated and should be considered identical to the
    // input buffer, or in case *poutbuf was set it points to the input buffer (not necessarily to its starting
    // address).

    av_bitstream_filter_filter( _bsfc, _codec, NULL,
                                &_filteredPacket.data, &_filteredPacket.size,
                                inputPacket.data, inputPacket.size,
                                (keyFrame) ? 1 : 0 );

    // Given the super tricky comment above (lifted right from the ffmpeg documentation!) our strategy is this:
    // If our output buffer overlaps our input, then allocate storage for it, copy the data into it and reset
    // our _filteredPacket.data and _filteredPacket.size to point to it. This way, we can always guarantee our
    // _filteredPacket is distinct from out inputPacket.

    if( _filteredPacket.data >= inputPacket.data && _filteredPacket.data < (inputPacket.data + inputPacket.size) )
    {
        size_t filteredSize = inputPacket.size - (_filteredPacket.data - inputPacket.data);
        uint8_t* data = (uint8_t*)av_malloc( filteredSize );
        if( !data )
            X_THROW(("Unable to allocate filter packet."));
        memcpy( data, _filteredPacket.data, filteredSize );
        _filteredPacket.data = data;
        _filteredPacket.size = filteredSize;
    }
}

XIRef<Packet> H264MP4ToAnnexB::Get()
{
    XIRef<Packet> output = _pf->Get( _filteredPacket.size + DEFAULT_PADDING );
    memcpy( output->Map(), _filteredPacket.data, _filteredPacket.size );
    output->SetDataSize( _filteredPacket.size );

    return output;
}

void H264MP4ToAnnexB::_FreeFilteredPacket()
{
    if( _filteredPacket.size > 0 )
    {
        av_free( _filteredPacket.data );
        _filteredPacket.data = NULL;
        _filteredPacket.size = 0;
    }
}
