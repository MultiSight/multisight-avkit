
#include "framework.h"
#include "AVKit/Packet.h"

class AVMuxerTest : public test_fixture
{
public:
    TEST_SUITE(AVMuxerTest);
        TEST(AVMuxerTest::TestConstructor);
        TEST(AVMuxerTest::TestMP4);
        TEST(AVMuxerTest::TestRecontainerize);
        TEST(AVMuxerTest::TestBuffer);
        TEST(AVMuxerTest::TSLeak);
    TEST_SUITE_END();

    virtual ~AVMuxerTest() throw() {}

    void setup();
    void teardown();

    void TestConstructor();
    void TestMP4();
    void TestRecontainerize();
    void TestBuffer();
    void TSLeak();
private:
    XIRef<AVKit::Packet> _pic;
};
