
#include "framework.h"

class PacketTest : public test_fixture
{
public:
    TEST_SUITE(PacketTest);
        TEST(PacketTest::TestSizeCTOROwning);
        TEST(PacketTest::TestForeignBufferCTOROwning);
        TEST(PacketTest::TestForeignBufferCTORNonOwning);
        TEST(PacketTest::TestCopyCTOROwning);
        TEST(PacketTest::TestCopyCTORNonOwning);
        TEST(PacketTest::TestCopyAssignment);
        TEST(PacketTest::TestConfigOwning);
        TEST(PacketTest::TestConfigNonOwning);
    TEST_SUITE_END();

    virtual ~PacketTest() throw() {}

    void setup();
    void teardown();

    void TestSizeCTOROwning();
    void TestForeignBufferCTOROwning();
    void TestForeignBufferCTORNonOwning();
    void TestCopyCTOROwning();
    void TestCopyCTORNonOwning();
    void TestCopyAssignment();
    void TestConfigOwning();
    void TestConfigNonOwning();
};
