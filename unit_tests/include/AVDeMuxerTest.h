
//=============================================================================
// Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: AVDeMuxerTest.h $
// $Revision:$
// $Date:$ 11/15/2013
// $Author: Tony Di Croce $
//=============================================================================

#ifndef _AVDeMuxerTest_H_
#define _AVDeMuxerTest_H_

#include <cppunit/extensions/HelperMacros.h>
#include "XSDK/XMemory.h"

class AVDeMuxerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(AVDeMuxerTest);
        CPPUNIT_TEST(TestConstructor);
        CPPUNIT_TEST(TestExamineFile);
        CPPUNIT_TEST(TestFileFromMemory);
        CPPUNIT_TEST(TestGetContainerStatistics);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual ~AVDeMuxerTest() throw() {}

    void setUp();
    void tearDown();

protected:
    void TestConstructor();
    void TestExamineFile();
    void TestFileFromMemory();
    void TestGetContainerStatistics();
};

#endif

