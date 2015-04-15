
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "AVKit/YUV420PToARGB24.h"

#include "XSDK/XException.h"
#include "XSDK/XSocket.h"

using namespace AVKit;
using namespace XSDK;

static const size_t DEFAULT_PADDING = 16;

YUV420PToARGB24::YUV420PToARGB24() :
    _rgb24(),
    _pf( new PacketFactoryDefault() ),
    _scaler( NULL ),
    _currentWidth( 0 ),
    _currentHeight( 0 )
{
}

YUV420PToARGB24::~YUV420PToARGB24() throw()
{
    _DestroyScaler();
}

void YUV420PToARGB24::Transform( XIRef<Packet> input, size_t width, size_t height )
{
    uint8_t* src = input->Map();

    AVFrame frame;
    frame.data[0] = src;
    src += width * height;
    frame.data[1] = src;
    src += ((width/2) * (height/2));
    frame.data[2] = src;

    frame.linesize[0] = width;
    frame.linesize[1] = (width/2);
    frame.linesize[2] = (width/2);

    size_t dataSize = height * (width*4);
    _rgb24 = _pf->Get( dataSize + DEFAULT_PADDING );
    _rgb24->SetDataSize( dataSize );

    AVPicture pict;
    pict.data[0] = _rgb24->Map();
    pict.linesize[0] = width * 4;

    if( (width != _currentWidth) || (height != _currentHeight) )
        _DestroyScaler();

    if( !_scaler )
        _InitScaler( width, height );

    int ret = sws_scale( _scaler,
                         frame.data,
                         frame.linesize,
                         0,
                         height,
                         pict.data,
                         pict.linesize );
}

XIRef<Packet> YUV420PToARGB24::Get()
{
    return _rgb24;
}

void YUV420PToARGB24::_InitScaler( size_t width, size_t height )
{
    int scaleFlags = SWS_FAST_BILINEAR;

    _scaler = sws_getContext( width,
                              height,
                              PIX_FMT_YUV420P,
                              width,
                              height,
                              PIX_FMT_BGRA,
                              scaleFlags,
                              NULL,
                              NULL,
                              NULL );

    if( !_scaler )
        X_THROW(("Unable to allocate scaler context!"));
}

void YUV420PToARGB24::_DestroyScaler()
{
    if( _scaler )
    {
        sws_freeContext( _scaler );
        _scaler = NULL;
    }
}
