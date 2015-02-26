
#include "H264DecoderTest.h"
#include "AVKit/H264Decoder.h"
#include "AVKit/JPEGEncoder.h"
#include "AVKit/Options.h"
#include "AVKit/Locky.h"

extern "C"
{
#include "libavformat/avformat.h"
};

#include "gop.c"

using namespace std;
using namespace XSDK;
using namespace AVKit;

REGISTER_TEST_FIXTURE(H264DecoderTest);

void H264DecoderTest::setup()
{
    Locky::RegisterFFMPEG();
}

void H264DecoderTest::teardown()
{
    Locky::UnregisterFFMPEG();
}

void H264DecoderTest::TestConstructor()
{
    XRef<H264Decoder> d;
    UT_ASSERT_NO_THROW( d = new H264Decoder( GetFastH264DecoderOptions() ) );
}

void H264DecoderTest::TestInputDimensions()
{
    XRef<H264Decoder> decoder = new H264Decoder( GetFastH264DecoderOptions() );
    XIRef<Packet> pkt = new Packet( gop[0].frame, gop[0].frameSize, false );
    decoder->Decode( pkt );

    UT_ASSERT( decoder->GetInputWidth() == 1280 );
    UT_ASSERT( decoder->GetInputHeight() == 720 );
}

void H264DecoderTest::TestOutputDimensions()
{
    XRef<H264Decoder> decoder = new H264Decoder( GetFastH264DecoderOptions() );
    XIRef<Packet> pkt = new Packet( gop[0].frame, gop[0].frameSize, false );
    decoder->Decode( pkt );

    decoder->SetOutputWidth( 640 );
    decoder->SetOutputHeight( 360 );

    XIRef<Packet> pic;
    UT_ASSERT_NO_THROW( pic = decoder->Get() );
    UT_ASSERT( pic->GetDataSize() == 345600 );
}

void H264DecoderTest::TestDecodeGOP()
{
    XRef<H264Decoder> decoder = new H264Decoder( GetFastH264DecoderOptions() );

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        XIRef<Packet> pkt = new Packet( gop[i].frame, gop[i].frameSize, false );
        UT_ASSERT_NO_THROW( decoder->Decode( pkt ) );

        XIRef<Packet> pic;
        UT_ASSERT_NO_THROW( pic = decoder->Get() );

        UT_ASSERT( pic->GetDataSize() == 1382400 );
    }
}
