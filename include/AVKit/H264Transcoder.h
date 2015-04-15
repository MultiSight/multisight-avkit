
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_H264Transcoder_h
#define __AVKit_H264Transcoder_h

#include "AVKit/AVDeMuxer.h"
#include "AVKit/Encoder.h"
#include "AVKit/Decoder.h"
#include "AVKit/AVMuxer.h"
#include "AVKit/Options.h"
#include "AVKit/FrameTypes.h"
#include "AVKit/Packet.h"

namespace AVKit
{

class H264Transcoder
{
public:
    X_API H264Transcoder( int inputTimeBaseNum, int inputTimeBaseDen, int outputTimeBaseNum, int outputTimeBaseDen, double speed = 1.0, bool decodeSkipping = false );

    // The job of this method is to decode frames until it is time to make an output frame. If the output
    // framerate is greater than the input framerate this means that sometimes calls to this method will not
    // actually decode a frame (because we need to encode multiple outputs from the same input in that case).
    // Likewise, when the output framerate is less than the input framerate, we will sometimes decode multiple
    // times before we return. In either case, after this method returns, we are ready to make an output frame.
    template<class T>
    X_API bool Decode( T& avDeMuxer, Decoder& decoder )
    {
        int videoStreamIndex = avDeMuxer.GetVideoStreamIndex();

        if( _step < 1.0 )
        {
            while( !avDeMuxer.EndOfFile() && (_step < 1.0) )
            {
                int streamIndex = 0;
                if( !avDeMuxer.ReadFrame( streamIndex ) )
                    return false;

                if( streamIndex == videoStreamIndex )
                {
                    _step += _outputFramesPerInputFrame;

                    if( _step > 1.0 || !_decodeSkipping )
                        decoder.Decode( avDeMuxer.Get() );
                }
            }
        }

        _step -= 1.0;

        return true;
    }

    X_API void EncodeYUV420PAndMux( Encoder& encoder,
                                    AVMuxer& muxer,
                                    XIRef<Packet> pic,
                                    AVKit::FrameType type = AVKit::FRAME_TYPE_AUTO_GOP );

    X_API static int64_t ComputeNumOutputFrames( int64_t numInputFrames,
                                                 int inputTimeBaseNum, int inputTimeBaseDen,
                                                 int outputTimeBaseNum, int outputTimeBaseDen,
                                                 double speed = 1.0 );

private:
    double _outputFramesPerInputFrame;
    double _step;
    double _speed;
    bool _decodeSkipping;
};

};

#endif
