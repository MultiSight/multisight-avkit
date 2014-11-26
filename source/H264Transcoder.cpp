
#include "AVKit/H264Transcoder.h"

using namespace AVKit;
using namespace XSDK;

H264Transcoder::H264Transcoder( int inputTimeBaseNum, int inputTimeBaseDen,
                                int outputTimeBaseNum, int outputTimeBaseDen,
                                double speed,
                                bool decodeSkipping ) :
    _step( 0.0 ),
    _speed( speed ),
    _decodeSkipping( decodeSkipping )
{
    // When the input and output framerates were specified with single integers, this was how we did it...
    // _outputFramesPerInputFrame( (((double)outputFrameRate) / (((double)inputFrameRate) * speed)) ),

    AVRational inputRational = { inputTimeBaseNum, inputTimeBaseDen };
    AVRational outputRational = { outputTimeBaseNum, outputTimeBaseDen };

    _outputFramesPerInputFrame = av_q2d( inputRational ) / (av_q2d( outputRational ) * _speed);
}

void H264Transcoder::EncodeYUV420PAndMux( H264Encoder& encoder,
                                          AVMuxer& muxer,
                                          XIRef<XSDK::XMemory> pic,
                                          AVKit::FrameType type )
{
    XIRef<XSDK::XMemory> encodeBuffer = encoder.EncodeYUV420P( pic, type );

    muxer.WriteVideoFrame( encodeBuffer, encoder.LastWasKey() );
}

int64_t H264Transcoder::ComputeNumOutputFrames( int64_t numInputFrames,
                                                int inputTimeBaseNum, int inputTimeBaseDen,
                                                int outputTimeBaseNum, int outputTimeBaseDen,
                                                double speed )
{
    AVRational inputRational = { inputTimeBaseNum, inputTimeBaseDen };
    AVRational outputRational = { outputTimeBaseNum, outputTimeBaseDen };

    double outputFramesPerInputFrame = av_q2d( inputRational ) / (av_q2d( outputRational ) * speed);

    return (int64_t)(outputFramesPerInputFrame * (double)numInputFrames);
}
