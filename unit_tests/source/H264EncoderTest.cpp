
#include "H264EncoderTest.h"
#include "AVKit/H264Encoder.h"
#include "AVKit/H264Decoder.h"
#include "AVKit/JPEGEncoder.h"
#include "AVKit/Locky.h"
#include "AVKit/Options.h"
#include "AVKit/FrameTypes.h"

extern "C"
{
#include "libavformat/avformat.h"
};

#include "gop.c"
#include "pic.c"

using namespace std;
using namespace XSDK;
using namespace AVKit;

REGISTER_TEST_FIXTURE(H264EncoderTest);

void H264EncoderTest::setup()
{
    Locky::RegisterFFMPEG();

    // pic_0 comes from the above included file pic.c
    _pic = new Packet( pic_0, pic_0_len );
}

void H264EncoderTest::teardown()
{
    Locky::UnregisterFFMPEG();
}

void H264EncoderTest::TestConstructor()
{
    XRef<H264Encoder> e;
    UT_ASSERT_NO_THROW( e = new H264Encoder( GetFastH264EncoderOptions( 500000, 1280, 720, 15, 1, 15 ) ) );
}

void H264EncoderTest::TestEncodeKey()
{
    XRef<H264Encoder> e;
    UT_ASSERT_NO_THROW( e = new H264Encoder( GetFastH264EncoderOptions( 500000, 1280, 720, 15, 1, 15 ) ) );

    UT_ASSERT_NO_THROW( e->EncodeYUV420P( _pic, FRAME_TYPE_KEY ) );

    XIRef<Packet> output;
    UT_ASSERT_NO_THROW( output = e->Get() );

    UT_ASSERT( output->GetDataSize() > 0 );
}

void H264EncoderTest::TestEncodeGOP()
{
    XRef<H264Decoder> d;
    UT_ASSERT_NO_THROW( d = new H264Decoder ( GetFastH264DecoderOptions() ) );

    d->SetOutputWidth( 640 );
    d->SetOutputHeight( 360 );

    XRef<H264Encoder> e;
    UT_ASSERT_NO_THROW( e = new H264Encoder ( GetFastH264EncoderOptions( 500000, 640, 360, 15, 1, 5 ) ) );

    list<XIRef<Packet> > outputFrames;

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        int index = i % NUM_FRAMES_IN_GOP;
        XIRef<Packet> pkt = new Packet( gop[index].frame, gop[index].frameSize, false );
        d->Decode( pkt );
        e->EncodeYUV420P( d->Get() );
        outputFrames.push_back( e->Get() );
    }

    UT_ASSERT_NO_THROW( d = new H264Decoder( GetFastH264DecoderOptions() ) );

    d->SetOutputWidth( 640 );
    d->SetOutputHeight( 360 );

    list<XIRef<Packet> >::iterator i;
    for( i = outputFrames.begin(); i != outputFrames.end(); i++ )
    {
        XIRef<Packet> frame = *i;
        d->Decode( frame );

        XIRef<Packet> pic;
        UT_ASSERT_NO_THROW( pic = d->Get() );
    }
}
