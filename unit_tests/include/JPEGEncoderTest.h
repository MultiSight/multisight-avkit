
#include "framework.h"
#include "AVKit/Packet.h"

class JPEGEncoderTest : public test_fixture
{
public:
    TEST_SUITE(JPEGEncoderTest);
        TEST(JPEGEncoderTest::TestConstructor);
        TEST(JPEGEncoderTest::TestEncode);
    TEST_SUITE_END();

    virtual ~JPEGEncoderTest() throw() {}

    void setup();
    void teardown();

    void TestConstructor();
    void TestEncode();
private:
    XIRef<AVKit::Packet> _pic;
};
