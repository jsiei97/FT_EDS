/*
 * Copyright (C) 2013 Johan Simonsson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore>
#include <QtTest>

#include "IntegerExtra.h"

class Test_IntegerExtra : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void test_hex2uint();
        void test_hex2uint_data();
};

void Test_IntegerExtra::test_hex2uint_data()
{
    QTest::addColumn<QString>("string");

    QTest::newRow("test") << "0x1";
    QTest::newRow("test") << "0xa";
    QTest::newRow("test") << "0xB";

    QTest::newRow("test") << "0x23";
    QTest::newRow("test") << "0xa4";
    QTest::newRow("test") << "0xAb";

    QTest::newRow("test") << "0x300";
    QTest::newRow("test") << "0xc03";
    QTest::newRow("test") << "0xD0b";

    QTest::newRow("test") << "0x2001";
    QTest::newRow("test") << "0xc001";
    QTest::newRow("test") << "0xF001";

    QTest::newRow("test") << "2";
    QTest::newRow("test") << "23";
    QTest::newRow("test") << "543";
    QTest::newRow("test") << "3543";
}

void Test_IntegerExtra::test_hex2uint()
{
    QFETCH(QString, string);

    bool ok;
    unsigned int hex = string.toUInt(&ok, 10);

    if(!ok)
        hex = string.toUInt(&ok,16);
    
    QVERIFY(ok);

    char* str = string.toAscii().data();

    unsigned int result = IntegerExtra::hex2uint(str);

    qDebug() << result << hex;
    QCOMPARE(result, hex);
}

QTEST_MAIN(Test_IntegerExtra)
#include "Test_IntegerExtra.moc"
