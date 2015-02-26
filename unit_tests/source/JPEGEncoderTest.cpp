
#include "JPEGEncoderTest.h"
#include "AVKit/JPEGEncoder.h"
#include "AVKit/Locky.h"
#include "AVKit/Options.h"
#include "XSDK/XMemory.h"

extern "C"
{
#include "libavformat/avformat.h"
};

#include "gop.c"
#include "pic.c"

using namespace std;
using namespace XSDK;
using namespace AVKit;

REGISTER_TEST_FIXTURE(JPEGEncoderTest);

void JPEGEncoderTest::setup()
{
    Locky::RegisterFFMPEG();

    // pic_0 comes from the above included file pic.c
    _pic = new Packet( pic_0, pic_0_len );
}

void JPEGEncoderTest::teardown()
{
    Locky::UnregisterFFMPEG();
}

void JPEGEncoderTest::TestConstructor()
{
    XRef<JPEGEncoder> e;
    UT_ASSERT_NO_THROW( e = new JPEGEncoder( GetJPEGOptions( 1280, 720 ) ) );
}

void JPEGEncoderTest::TestEncode()
{
    XRef<JPEGEncoder> e;
    UT_ASSERT_NO_THROW( e = new JPEGEncoder( GetJPEGOptions( 1280, 720 ) ) );

    e->EncodeYUV420P( _pic );
    XIRef<Packet> jpeg = e->Get();

    UT_ASSERT( jpeg->GetDataSize() > 0 );

#if 1 // To verify that jpeg contains a valid jpeg, enable this block of code.
    JPEGEncoder::WriteJPEGFile( "outy.jpg", jpeg );
#endif
}
