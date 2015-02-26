
#include "framework.h"
#include "AVKit/Packet.h"

class H264EncoderTest : public test_fixture
{
public:
    TEST_SUITE(H264EncoderTest);
        TEST(H264EncoderTest::TestConstructor);
        TEST(H264EncoderTest::TestEncodeKey);
        TEST(H264EncoderTest::TestEncodeGOP);
    TEST_SUITE_END();

    virtual ~H264EncoderTest() throw() {}

    void setup();
    void teardown();

    void TestConstructor();
    void TestEncodeKey();
    void TestEncodeGOP();

private:
    XIRef<AVKit::Packet> _pic;
};
