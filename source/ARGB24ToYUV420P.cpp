
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "AVKit/ARGB24ToYUV420P.h"
#include "AVKit/Locky.h"

#include "XSDK/XException.h"
#include "XSDK/XSocket.h"

using namespace AVKit;
using namespace XSDK;

static const int DEFAULT_PADDING = 16;

ARGB24ToYUV420P::ARGB24ToYUV420P() :
    _yuv420(),
    _pf( new PacketFactoryDefault() ),
    _scaler( NULL ),
    _currentWidth( 0 ),
    _currentHeight( 0 )
{
    if( !Locky::IsRegistered() )
        X_THROW(("Please register AVKit::Locky before using this class."));
}

ARGB24ToYUV420P::~ARGB24ToYUV420P() throw()
{
    _DestroyScaler();
}

void ARGB24ToYUV420P::Transform( XIRef<Packet> pkt, size_t width, size_t height )
{
    uint8_t* src = pkt->Map();

    AVFrame frame;
    frame.data[0] = src;
    frame.linesize[0] = (width*4);

    size_t pictureSize = width * height * 1.5;
    _yuv420 = _pf->Get( pictureSize + DEFAULT_PADDING );
    _yuv420->SetDataSize( pictureSize );

    uint8_t* dst = _yuv420->Map();

    AVPicture pict;
    pict.data[0] = dst;
    dst += (width*height);
    pict.data[1] = dst;
    dst += ((width/2)*(height/2));
    pict.data[2] = dst;

    pict.linesize[0] = width;
    pict.linesize[1] = (width/2);
    pict.linesize[2] = (width/2);

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

XIRef<Packet> ARGB24ToYUV420P::Get()
{
    return _yuv420;
}

void ARGB24ToYUV420P::_InitScaler( size_t width, size_t height )
{
    int scaleFlags = SWS_FAST_BILINEAR;

    _scaler = sws_getContext( width,
                              height,
                              PIX_FMT_BGRA,
                              width,
                              height,
                              PIX_FMT_YUV420P,
                              scaleFlags,
                              NULL,
                              NULL,
                              NULL );

    if( !_scaler )
        X_THROW(("Unable to allocate scaler context!"));
}

void ARGB24ToYUV420P::_DestroyScaler()
{
    if( _scaler )
    {
        sws_freeContext( _scaler );
        _scaler = NULL;
    }
}
