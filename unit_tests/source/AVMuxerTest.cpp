
#include "AVMuxerTest.h"
#include "AVKit/Options.h"
#include "AVKit/AVMuxer.h"
#include "AVKit/H264Decoder.h"
#include "AVKit/H264Encoder.h"
#include "AVKit/JPEGEncoder.h"
#include "AVKit/Locky.h"
#include "AVKit/Options.h"
#include "XSDK/XMemory.h"

#include "gop.c"
#include "pic.c"

using namespace std;
using namespace XSDK;
using namespace AVKit;

REGISTER_TEST_FIXTURE(AVMuxerTest);

void AVMuxerTest::setup()
{
    Locky::RegisterFFMPEG();
}

void AVMuxerTest::teardown()
{
    Locky::UnregisterFFMPEG();
}

void AVMuxerTest::TestConstructor()
{
    XRef<AVMuxer> c;
    UT_ASSERT_NO_THROW( c = new AVMuxer( GetFastH264EncoderOptions( 500000, 1280, 720, 15, 1, 15 ), "foo.mp4", AVMuxer::OUTPUT_LOCATION_BUFFER ) );
}

void AVMuxerTest::TestMP4()
{
    XRef<H264Decoder> d = new H264Decoder( GetFastH264DecoderOptions() );
    d->SetOutputWidth( 640 );
    d->SetOutputHeight( 360 );
    XRef<H264Encoder> e = new H264Encoder( GetFastH264EncoderOptions( 250000, 640, 360, 15, 1, 15 ) );
    XRef<AVMuxer> c = new AVMuxer( e->GetOptions(),
                                   "foo.mp4",
                                   AVMuxer::OUTPUT_LOCATION_FILE );

    c->SetExtraData( e->GetExtraData() );

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        int index = i % NUM_FRAMES_IN_GOP;

        XIRef<Packet> pkt = new Packet( gop[index].frame, gop[index].frameSize, false );
        d->Decode( pkt );
        e->EncodeYUV420P( d->Get() );
        c->WriteVideoPacket( e->Get(), ((i % 15) == 0) ? true : false );
    }

    c->FinalizeFile();
}

void AVMuxerTest::TestRecontainerize()
{
    struct CodecOptions options;

    options.gop_size = 15;
    options.bit_rate = 1424400; // size of gop * 8 == bit_rate
    options.width = 1280;
    options.height = 720;
    options.time_base_num = 1;
    options.time_base_den = 30;

    XRef<AVMuxer> c = new AVMuxer( options,
                                   "bar.mp4",
                                   AVMuxer::OUTPUT_LOCATION_FILE );

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        int index = i % NUM_FRAMES_IN_GOP;
        XIRef<Packet> pkt = new Packet( gop[index].frame, gop[index].frameSize, false );
        c->WriteVideoPacket( pkt, ((i % 15) == 0) ? true : false );
    }

    c->FinalizeFile();
}
