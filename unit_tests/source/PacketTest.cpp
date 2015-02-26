
#include "PacketTest.h"
#include "AVKit/Packet.h"

using namespace std;
using namespace XSDK;
using namespace AVKit;

REGISTER_TEST_FIXTURE(PacketTest);

void PacketTest::setup()
{
}

void PacketTest::teardown()
{
}

void PacketTest::TestSizeCTOROwning()
{
    {
        XIRef<Packet> pkt = new Packet(32768);
        UT_ASSERT( pkt->_bufferSize == 32768 );
        UT_ASSERT( pkt->_requestedSize == 32768 );
        UT_ASSERT( pkt->_owning == true );
        UT_ASSERT( pkt->_buffer != NULL );
        UT_ASSERT( pkt->_dataSize == 0 );
        UT_ASSERT( pkt->_ts == 0 );
        UT_ASSERT( pkt->_ticksInSecond == 90000 );

        pkt->SetDataSize( 16384 );
        UT_ASSERT( pkt->GetDataSize() == 16384 );

        uint8_t* writer = pkt->Map();
        for( size_t i = 0; i < 32768; i++ )
            *writer++ = 1;

        size_t sum = 0;
        uint8_t* reader = pkt->Map();
        for( size_t i = 0; i < 32768; i++ )
            sum += *reader++;

        UT_ASSERT( sum == 32768 );
    }

    {
        // test whether the buffer pads itself correctly
        XIRef<Packet> pkt = new Packet(32766);
        UT_ASSERT( pkt->_bufferSize == 32768 );
        UT_ASSERT( pkt->_requestedSize == 32766 );
        UT_ASSERT( pkt->_owning == true );
        UT_ASSERT( pkt->_buffer != NULL );
        UT_ASSERT( pkt->_dataSize == 0 );
        UT_ASSERT( pkt->_ts == 0 );
        UT_ASSERT( pkt->_ticksInSecond == 90000 );
    }
}

void PacketTest::TestForeignBufferCTOROwning()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    XIRef<Packet> pkt = new Packet( foreignBuffer, 16384 );

    uint8_t* src = pkt->Map();
    uint8_t* cmp = &foreignBuffer[0];
    for( size_t i = 0; i < 16384; i++ )
        UT_ASSERT( *src++ == *cmp++ );

    UT_ASSERT( pkt->_owning == true );
    UT_ASSERT( pkt->_buffer != &foreignBuffer[0] );
}

void PacketTest::TestForeignBufferCTORNonOwning()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    XIRef<Packet> pkt = new Packet( foreignBuffer, 16384, false );

    uint8_t* src = pkt->Map();
    UT_ASSERT( pkt->_owning == false );
    UT_ASSERT( pkt->_buffer == &foreignBuffer[0] );
}

void PacketTest::TestCopyCTOROwning()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    Packet pktA( foreignBuffer, 16384 );
    Packet pktB = pktA;

    UT_ASSERT( pktB._bufferSize == pktA._bufferSize );
    UT_ASSERT( pktB._requestedSize == pktA._requestedSize );
    UT_ASSERT( pktB._owning == pktA._owning );
    UT_ASSERT( pktB._buffer != pktA._buffer );
    UT_ASSERT( pktB._dataSize == pktA._dataSize );
    UT_ASSERT( pktB._ts == pktA._ts );
    UT_ASSERT( pktB._ticksInSecond == pktA._ticksInSecond );

    for( size_t i = 0; i < 16384; i++ )
        UT_ASSERT( pktB._buffer[i] == 42 );
}

void PacketTest::TestCopyCTORNonOwning()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    Packet pktA( foreignBuffer, 16384, false );
    Packet pktB = pktA;

    UT_ASSERT( pktB._bufferSize == pktA._bufferSize );
    UT_ASSERT( pktB._requestedSize == pktA._requestedSize );
    UT_ASSERT( pktB._owning == pktA._owning );
    UT_ASSERT( pktB._buffer == pktA._buffer );
    UT_ASSERT( pktB._dataSize == pktA._dataSize );
    UT_ASSERT( pktB._ts == pktA._ts );
    UT_ASSERT( pktB._ticksInSecond == pktA._ticksInSecond );

    UT_ASSERT( pktA._buffer == &foreignBuffer[0] );
    UT_ASSERT( pktB._buffer == &foreignBuffer[0] );
}

void PacketTest::TestCopyAssignment()
{
    uint8_t foreignBuffer[16384];
    for( size_t i = 0; i < 16384; i++ )
        foreignBuffer[i] = 42;

    Packet pktA( foreignBuffer, 16384 );
    Packet pktB( 1024 );

    pktB = pktA;

    UT_ASSERT( pktB._bufferSize == pktA._bufferSize );
    UT_ASSERT( pktB._requestedSize == pktA._requestedSize );
    UT_ASSERT( pktB._owning == pktA._owning );
    UT_ASSERT( pktB._buffer != pktA._buffer );
    UT_ASSERT( pktB._dataSize == pktA._dataSize );
    UT_ASSERT( pktB._ts == pktA._ts );
    UT_ASSERT( pktB._ticksInSecond == pktA._ticksInSecond );

    for( size_t i = 0; i < 16384; i++ )
        UT_ASSERT( pktB._buffer[i] == 42 );
}
