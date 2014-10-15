
//=============================================================================
// Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: AVMuxerTest.h $
// $Revision:$
// $Date:$ 11/15/2013
// $Author: Tony Di Croce $
//=============================================================================

#ifndef _AVMuxerTest_H_
#define _AVMuxerTest_H_

#include <cppunit/extensions/HelperMacros.h>
#include "XSDK/XMemory.h"

class AVMuxerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(AVMuxerTest);
        CPPUNIT_TEST(TestConstructor);
        CPPUNIT_TEST(TestMP4);
        CPPUNIT_TEST(TestReContainerize);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual ~AVMuxerTest() throw() {}

    void setUp();
    void tearDown();

protected:
    void TestConstructor();
    void TestMP4();
    void TestReContainerize();
private:
    XIRef<XSDK::XMemory> _pic;
};

#endif

