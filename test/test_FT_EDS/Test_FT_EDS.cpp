#include <QtCore>
#include <QtTest>

#include <EEPROM.h>
#include "FT_EDS.h"
#include "HexDump.h"

class Test_FT_EDS : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void test_MAC();
};

void Test_FT_EDS::test_MAC()
{
    //qDebug() << "test";
    FT_EDS eds;

    eds.init();

    QCOMPARE((unsigned int)eds.getFree(), (unsigned int)(EEPROM_MAX_SIZE-7));

    //Test the internal reads on the magic numbers...
    QCOMPARE(eds.read16(0), (uint16_t)0x2345);
    QCOMPARE(eds.read16(2), (uint16_t)0x5432);
    QCOMPARE(eds.read32(0), (uint32_t)0x23455432);

    //Test to write and read at a free pos
    eds.write16(8, 0xBABE);
    QCOMPARE(eds.read16(8), (uint16_t)0xBABE);

    eds.write32(8, 0xCAFEBABE);
    QCOMPARE(eds.read32(8), (uint32_t)0xCAFEBABE);
    //HEXDUMP(&EEPROM.prom);

    uint8_t mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x02 };

    if(eds.readDE(EDS_ETH_MAC, EDS_BYTE_ARRAY, mac, 6))
    {
        QFAIL("Not created so we should get a false here...!");
    }

    //qDebug() << eds.getDEC();
    QCOMPARE(eds.getDEC(), (uint16_t)0);

    QVERIFY(eds.updateDE(EDS_ETH_MAC, EDS_BYTE_ARRAY, mac, 6));

    QCOMPARE((unsigned int)eds.getFree(), (unsigned int)(EEPROM_MAX_SIZE-7-10-6));

    //HEXDUMP(&EEPROM.prom);

    {
        uint8_t mac2[6];
        QVERIFY(eds.readDE(EDS_ETH_MAC, EDS_BYTE_ARRAY, mac2, 6));

        for( int i=0 ; i<6 ; i++ )
        {
            QCOMPARE(mac[i], mac2[i]);
        }
    }

    QCOMPARE(eds.getDEC(), (uint16_t)1);

    mac[0] = 0x4D;
    mac[1] = 0x41;
    mac[2] = 0x43;
    mac[3] = 0xFE;
    mac[4] = 0xFE;
    mac[5] = 0x21;
    QVERIFY(eds.updateDE(EDS_ETH_MAC, EDS_BYTE_ARRAY, mac, 6));

    QCOMPARE(eds.getDEC(), (uint16_t)1);
    QCOMPARE((unsigned int)eds.getFree(), (unsigned int)(EEPROM_MAX_SIZE-7-10-6));

    //HEXDUMP(&EEPROM.prom);

    /// @todo Add something with len 0..3 so it fits inside the data field...

    uint8_t keylist_s2[2][8] =
    {
        { 0x01, 0x5D, 0x79, 0xA7, 0x09, 0x00, 0x00, 0x66},
        { 0x01, 0x12, 0xE8, 0xA5, 0x09, 0x00, 0x00, 0x4A}
    };
    QVERIFY(eds.updateDE(EDS_ONEWIRE_LIST, EDS_BYTE_ARRAY, &keylist_s2[0][0], 2*8));

    //HEXDUMP(&EEPROM.prom);

    QCOMPARE(eds.getDEC(), (uint16_t)2);
    QCOMPARE((unsigned int)eds.getFree(), (unsigned int)(EEPROM_MAX_SIZE-7-10-6-10-16));

    //Do a keylist with 3*8 to the same id
    //and use more space...
    //Valid to do if the pointer in DE
    //is the same as the posFreeData pointer!
    uint8_t keylist_s4[4][8] =
    {
        { 0x01, 0x5D, 0x79, 0xA7, 0x09, 0x00, 0x00, 0x66},
        { 0x01, 0x12, 0xE8, 0xA5, 0x09, 0x00, 0x00, 0x4A},
        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08},
        { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18}
    };
    if(!eds.updateDE(EDS_ONEWIRE_LIST, EDS_BYTE_ARRAY, &keylist_s4[0][0], 4*8))
    {
        HEXDUMP(&EEPROM.prom);
        QFAIL("Error update failed!");
    }


    //Then read the mac data so it is still the same!
    {
        uint8_t mac2[6];
        QVERIFY(eds.readDE(EDS_ETH_MAC, EDS_BYTE_ARRAY, mac2, 6));

        for( int i=0 ; i<6 ; i++ )
        {
            QCOMPARE(mac[i], mac2[i]);
        }
    }

    QCOMPARE(eds.getDEC(), (uint16_t)2);
    QCOMPARE((unsigned int)eds.getFree(), (unsigned int)(EEPROM_MAX_SIZE-7-10-6-10-(4*8)));

    HEXDUMP(&EEPROM.prom);

    //Check that init can restore the pointers to free data....
    unsigned int nextDE   = eds.posNextDE;
    unsigned int freeData = eds.posFreeData;
    eds.init();
    QCOMPARE(nextDE, eds.posNextDE);
    QCOMPARE(freeData,eds.posFreeData);
}

QTEST_MAIN(Test_FT_EDS)
#include "Test_FT_EDS.moc"
