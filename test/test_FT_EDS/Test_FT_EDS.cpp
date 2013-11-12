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

    //qDebug() << eds.getDEC();
    QCOMPARE(eds.getDEC(), (uint16_t)0);

    QVERIFY(eds.updateDE(EDS_ETH_MAC, EDS_BYTE_ARRAY, mac, 6));

    HEXDUMP(&EEPROM.prom);

    unsigned int* len;
    uint8_t mac2[6];
    QVERIFY(eds.readDE(EDS_ETH_MAC, EDS_BYTE_ARRAY, mac2, 6));

    for( int i=0 ; i<6 ; i++ )
    {
        QCOMPARE(mac[i], mac2[i]);
    }


    QCOMPARE(eds.getDEC(), (uint16_t)1);

    mac[0] = 0x4D;
    mac[1] = 0x41;
    mac[2] = 0x43;
    mac[3] = 0xFE;
    mac[4] = 0xFE;
    mac[5] = 0x21;
    QVERIFY(eds.updateDE(EDS_ETH_MAC, EDS_BYTE_ARRAY, mac, 6));

    HEXDUMP(&EEPROM.prom);
    QCOMPARE(eds.getDEC(), (uint16_t)1);
}

QTEST_MAIN(Test_FT_EDS)
#include "Test_FT_EDS.moc"
