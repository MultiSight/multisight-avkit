
//=============================================================================
// Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: JPEGEncoderTest.h $
// $Revision:$
// $Date:$ 11/15/2013
// $Author: Tony Di Croce $
//=============================================================================

#ifndef _JPEGEncoderTest_H_
#define _JPEGEncoderTest_H_

#include <cppunit/extensions/HelperMacros.h>
#include "XSDK/XMemory.h"

class JPEGEncoderTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(JPEGEncoderTest);
        CPPUNIT_TEST(TestConstructor);
        CPPUNIT_TEST(TestEncode);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual ~JPEGEncoderTest() throw() {}

    void setUp();
    void tearDown();

protected:
    void TestConstructor();
    void TestEncode();
private:
    XIRef<XSDK::XMemory> _pic;
};

#endif

