
#include "framework.h"

class AVDeMuxerTest : public test_fixture
{
public:
    TEST_SUITE(AVDeMuxerTest);
        TEST(AVDeMuxerTest::TestConstructor);
        TEST(AVDeMuxerTest::TestExamineFile);
        TEST(AVDeMuxerTest::TestFileFromMemory);
        TEST(AVDeMuxerTest::TestGetContainerStatistics);
    TEST_SUITE_END();

    virtual ~AVDeMuxerTest() throw() {}

    void setup();
    void teardown();

    void TestConstructor();
    void TestExamineFile();
    void TestFileFromMemory();
    void TestGetContainerStatistics();
};
