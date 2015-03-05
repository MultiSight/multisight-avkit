
#include "framework.h"

class H264TranscoderTest : public test_fixture
{
public:
    TEST_SUITE(H264TranscoderTest);
        TEST(H264TranscoderTest::TestConstructor);
        TEST(H264TranscoderTest::TestBasic);
    TEST_SUITE_END();

    virtual ~H264TranscoderTest() throw() {}

    void setup();
    void teardown();

    void TestConstructor();
    void TestBasic();
};
