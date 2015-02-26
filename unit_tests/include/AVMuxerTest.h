
#include "framework.h"
#include "AVKit/Packet.h"

class AVMuxerTest : public test_fixture
{
public:
    TEST_SUITE(AVMuxerTest);
        TEST(AVMuxerTest::TestConstructor);
        TEST(AVMuxerTest::TestMP4);
        TEST(AVMuxerTest::TestRecontainerize);
    TEST_SUITE_END();

    virtual ~AVMuxerTest() throw() {}

    void setup();
    void teardown();

    void TestConstructor();
    void TestMP4();
    void TestRecontainerize();
private:
    XIRef<AVKit::Packet> _pic;
};
