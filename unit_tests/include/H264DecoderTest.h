
//=============================================================================
// Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: H264DecoderTest.h $
// $Revision:$
// $Date:$ 11/15/2013
// $Author: Tony Di Croce $
//=============================================================================

#ifndef _H264DecoderTest_H_
#define _H264DecoderTest_H_

#include <cppunit/extensions/HelperMacros.h>

class H264DecoderTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(H264DecoderTest);
        CPPUNIT_TEST(TestConstructor);
        CPPUNIT_TEST(TestInputDimensions);
        CPPUNIT_TEST(TestOutputDimensions);
        CPPUNIT_TEST(TestDecodeGOP);
    CPPUNIT_TEST_SUITE_END();

public:

    virtual ~H264DecoderTest() throw()
    {}

    void setUp();
    void tearDown();

protected:

    void TestConstructor();
    void TestInputDimensions();
    void TestOutputDimensions();
    void TestDecodeGOP();
};

#endif

