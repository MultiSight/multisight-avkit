
#include "framework.h"

class H264DecoderTest : public test_fixture
{
public:
    TEST_SUITE(H264DecoderTest);
        TEST(H264DecoderTest::TestConstructor);
        TEST(H264DecoderTest::TestInputDimensions);
        TEST(H264DecoderTest::TestOutputDimensions);
        TEST(H264DecoderTest::TestDecodeGOP);
    TEST_SUITE_END();

    virtual ~H264DecoderTest() throw() {}

    void setup();
    void teardown();

    void TestConstructor();
    void TestInputDimensions();
    void TestOutputDimensions();
    void TestDecodeGOP();
};
