
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "AVKit/JPEGEncoder.h"
#include "AVKit/Locky.h"

#include "XSDK/XException.h"

using namespace AVKit;
using namespace XSDK;

static const size_t DEFAULT_JPEG_ENCODE_BUFFER_SIZE = (1024*1024);
static const size_t DEFAULT_PADDING = 16;

JPEGEncoder::JPEGEncoder( const struct CodecOptions& options, int encodeAttempts ) :
    _codec( avcodec_find_encoder( CODEC_ID_MJPEG ) ),
    _context( avcodec_alloc_context3( _codec ) ),
    _options( options ),
    _encodeAttempts( encodeAttempts ),
    _output(),
    _pf( new PacketFactoryDefault() )
{
    if( !Locky::IsRegistered() )
        X_THROW(("Please register AVKit::Locky before using this class."));

    if( !_codec )
        X_THROW(("Unable to locate MJPEG codec."));

    if( !_context )
        X_THROW(("Unable to allocate MJPEG codec context."));

    _context->codec_id = CODEC_ID_MJPEG;
    _context->codec_type = AVMEDIA_TYPE_VIDEO;

    _context->time_base.num = 1;
    _context->time_base.den = 15;

    _context->pix_fmt = PIX_FMT_YUVJ420P;
    _context->color_range = AVCOL_RANGE_JPEG;

    _context->gop_size = 1;
    _context->bit_rate_tolerance = (int)((double)10000000 *
                                         ((double)_context->time_base.num / (double)_context->time_base.den));
    _context->bit_rate_tolerance += 1; // Make sure we pass the comparison check

    if( !_options.width.IsNull() )
        _context->width = _options.width.Value();
    else X_THROW(("Required option missing: width"));

    if( !_options.height.IsNull() )
        _context->height = _options.height.Value();
    else X_THROW(("Required option missing: height"));

    if( !_options.bit_rate.IsNull() )
        _context->bit_rate = _options.bit_rate.Value();
    else X_THROW(("Required option missing: bit_rate"));

    if( !_options.qmin.IsNull() )
        _context->qmin = _options.qmin.Value();

    if( !_options.qmax.IsNull() )
        _context->qmax = _options.qmax.Value();

    if( !_options.max_qdiff.IsNull() )
        _context->max_qdiff = _options.max_qdiff.Value();

    if( avcodec_open2( _context, _codec, NULL ) < 0 )
        X_THROW(("Failed to open encoding context."));
}

JPEGEncoder::~JPEGEncoder() throw()
{
    avcodec_close( _context );

    av_free( _context );
}

void JPEGEncoder::EncodeYUV420P( XIRef<Packet> input )
{
    AVFrame frame;
    avcodec_get_frame_defaults( &frame );

    _output = _pf->Get( DEFAULT_JPEG_ENCODE_BUFFER_SIZE + DEFAULT_PADDING );

    uint8_t* pic = input->Map();

    frame.data[0] = pic;
    pic += (_context->width * _context->height);
    frame.data[1] = pic;
    pic += ((_context->width/4) * _context->height);
    frame.data[2] = pic;

    frame.linesize[0] = _context->width;
    frame.linesize[1] = (_context->width/2);
    frame.linesize[2] = (_context->width/2);

    int attempt = 0;
    int gotPacket = 0;
    AVPacket pkt;

    do
    {
        av_init_packet( &pkt );
        pkt.data = _output->Map();
        pkt.size = _output->GetBufferSize();

        if( avcodec_encode_video2( _context,
                                   &pkt,
                                   &frame,
                                   &gotPacket ) < 0 )
            X_THROW(("Error while encoding."));

        attempt++;
    } while( gotPacket == 0 && (attempt < _encodeAttempts) );

    _output->SetDataSize( pkt.size );
}

XIRef<Packet> JPEGEncoder::Get()
{
    return _output;
}

void JPEGEncoder::WriteJPEGFile( const XSDK::XString& fileName, XIRef<Packet> jpeg )
{
    FILE* outFile = fopen( fileName.c_str(), "wb" );
    if( !outFile )
        X_THROW(("Unable to open output file."));

    fwrite( jpeg->Map(), 1, jpeg->GetDataSize(), outFile );

    fclose( outFile );
}
