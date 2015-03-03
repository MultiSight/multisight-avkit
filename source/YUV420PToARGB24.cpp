
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

#define GETR(y,u,v) ((1.164 * (y - 16)) + (1.596 * ((v) - 128)))
#define GETG(y,u,v) ((1.164 * (y - 16)) - (0.813 * ((v) - 128)) - (0.391 * ((u) - 128)))
#define GETB(y,u,v) ((1.164 * (y - 16)) + (2.018 * ((u) - 128)))

#define CLIP(val,min,max) (val<min)?min:(val>max)?max:val

static const size_t DEFAULT_PADDING = 16;

YUV420PToARGB24::YUV420PToARGB24() :
    _rgb24(),
    _pf( new PacketFactoryDefault() )
{
}

YUV420PToARGB24::~YUV420PToARGB24() throw()
{
}

void YUV420PToARGB24::Transform( XIRef<Packet> input, size_t width, size_t height )
{
    size_t dataSize = height * (width*4);
    _rgb24 = _pf->Get( dataSize + DEFAULT_PADDING );
    _rgb24->SetDataSize( dataSize );

    uint8_t* dst = _rgb24->Map();

    const uint8_t* srcY = input->Map();
    const uint8_t* srcU = srcY + (width*height);
    const uint8_t* srcV = srcU + ((width/2)*(height/2));

    for( size_t dstY = 0; dstY < height; dstY++ )
    {
        for( size_t dstX = 0; dstX < width; dstX++ )
        {
            double dstFracY = ((double)dstY / height);
            double dstFracX = ((double)dstX / width);

            size_t uvY = (size_t)(dstFracY*(height/2));
            size_t uvX = (size_t)(dstFracX*(width/2));

            uint8_t* writer = dst + (( dstY * (width*4) ) + (dstX * 4));
            const uint8_t* readerY = srcY + (( dstY * width ) + dstX );
            const uint8_t* readerU = srcU + (( uvY * (width/2) ) + uvX );
            const uint8_t* readerV = srcV + (( uvY * (width/2) ) + uvX );

            uint32_t pixel = 0;
            uint8_t* pixelWriter = (uint8_t*)&pixel;

            *pixelWriter = 0x00;
            pixelWriter++;
            *pixelWriter = (uint8_t)(CLIP(GETR( *readerY, *readerU, *readerV ),0,255));
            pixelWriter++;
            *pixelWriter = (uint8_t)(CLIP(GETG( *readerY, *readerU, *readerV ),0,255));
            pixelWriter++;
            *pixelWriter = (uint8_t)(CLIP(GETB( *readerY, *readerU, *readerV ),0,255));

            uint32_t word = ntohl( pixel );
            *((uint32_t*)writer) = word;
        }
    }
}

XIRef<Packet> YUV420PToARGB24::Get()
{
    return _rgb24;
}
