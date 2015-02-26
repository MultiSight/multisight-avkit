
#include "AVDeMuxerTest.h"

#include "AVKit/AVMuxer.h"
#include "AVKit/AVDeMuxer.h"
#include "AVKit/H264Decoder.h"
#include "AVKit/Locky.h"
#include "AVKit/Options.h"
#include "XSDK/XMemory.h"
#include "XSDK/LargeFiles.h"

#include "gop.c"

using namespace std;
using namespace XSDK;
using namespace AVKit;

REGISTER_TEST_FIXTURE(AVDeMuxerTest);

void AVDeMuxerTest::setup()
{
    Locky::RegisterFFMPEG();

    struct CodecOptions options;

    options.gop_size = 15;
    options.bit_rate = 1424400; // size of gop * 8 == bit_rate
    options.width = 1280;
    options.height = 720;
    options.time_base_num = 1;
    options.time_base_den = 30;

    {
        XRef<AVMuxer> m = new AVMuxer( options,
                                       "AVDeMuxerTest.mp4",
                                       AVMuxer::OUTPUT_LOCATION_FILE );

        for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
        {
            int index = i % NUM_FRAMES_IN_GOP;
            XIRef<Packet> pkt = new Packet( gop[index].frame, gop[index].frameSize, false );
            m->WriteVideoPacket( pkt, ((i % 15) == 0) ? true : false );
        }

        m->FinalizeFile();
    }

    {
        XRef<AVMuxer> m = new AVMuxer( options,
                                       "big.mp4",
                                       AVMuxer::OUTPUT_LOCATION_FILE );

        for( int i = 0; i < (NUM_FRAMES_IN_GOP * 10); i++ )
        {
            int index = i % NUM_FRAMES_IN_GOP;
            XIRef<Packet> pkt = new Packet( gop[index].frame, gop[index].frameSize, false );
            m->WriteVideoPacket( pkt, ((i % 15) == 0) ? true : false );
        }

        m->FinalizeFile();
    }
}

void AVDeMuxerTest::teardown()
{
    Locky::UnregisterFFMPEG();

    remove( "big.mp4" );
    remove( "AVDeMuxerTest.mp4" );
}

void AVDeMuxerTest::TestConstructor()
{
    XRef<AVDeMuxer> deMuxer = new AVDeMuxer( "AVDeMuxerTest.mp4" );
}

void AVDeMuxerTest::TestExamineFile()
{
    XRef<AVDeMuxer> deMuxer = new AVDeMuxer( "AVDeMuxerTest.mp4" );

    int videoStreamIndex = deMuxer->GetVideoStreamIndex();

    int index = 0;
    int i = 0;
    int streamIndex = 0;
    while( deMuxer->ReadFrame( streamIndex ) )
    {
        UT_ASSERT( streamIndex == videoStreamIndex );

        int index = i % NUM_FRAMES_IN_GOP;

        bool keyness = ((i % NUM_FRAMES_IN_GOP) == 0) ? true : false;

        UT_ASSERT( deMuxer->IsKey() == keyness );

        XIRef<Packet> frame = deMuxer->Get();

        UT_ASSERT( frame->GetDataSize() == gop[index].frameSize );

        // XXX Note: The contents of the frame cannot be compared for equality because FFMPEG stores the frames in
        // annexb format (with start codes).

        i++;
    }

    UT_ASSERT( deMuxer->EndOfFile() );
}

void AVDeMuxerTest::TestFileFromMemory()
{
    XRef<AVDeMuxer> deMuxer = new AVDeMuxer( AVDeMuxer::LoadFile( "AVDeMuxerTest.mp4" ) );

    int videoStreamIndex = deMuxer->GetVideoStreamIndex();

    int index = 0;
    int i = 0;
    int streamIndex = 0;
    while( deMuxer->ReadFrame( streamIndex ) )
    {
        UT_ASSERT( streamIndex == videoStreamIndex );

        int index = i % NUM_FRAMES_IN_GOP;

        bool keyness = ((i % NUM_FRAMES_IN_GOP) == 0) ? true : false;

        UT_ASSERT( deMuxer->IsKey() == keyness );

        XIRef<Packet> frame = deMuxer->Get();

        UT_ASSERT( frame->GetDataSize() == gop[index].frameSize );

        // XXX Note: The contents of the frame cannot be compared for equality because FFMPEG stores the frames in
        // annexb format (with start codes).

        i++;
    }

    UT_ASSERT( deMuxer->EndOfFile() );
}

void AVDeMuxerTest::TestGetContainerStatistics()
{
    struct StreamStatistics stats = AVDeMuxer::GetVideoStreamStatistics( "big.mp4" );

    UT_ASSERT( !stats.averageBitRate.IsNull() );
    // Is the averageBitRate in the right ballpark?
    UT_ASSERT( (stats.averageBitRate.Value() > 2500000) && (stats.averageBitRate.Value() < 3000000) );

    UT_ASSERT( !stats.frameRate.IsNull() );
    UT_ASSERT( stats.frameRate.Value() == 30 );

    UT_ASSERT( !stats.gopSize.IsNull() );
    UT_ASSERT( stats.gopSize.Value() == 15 );
}
