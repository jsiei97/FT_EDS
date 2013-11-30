/**
 * @file HexDump.cpp
 * @author Johan Simonsson
 * @brief HexDump a QByteArray
 */

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

#include <QByteArray>
#include <QDebug>
#include <math.h>

#include "HexDump.h"

void HexDump::dump(QByteArray* raw, const char* func, int line)
{
    qDebug() << func << line << "RawDump: BEGIN";
	//qDebug() << raw->size() << raw->toHex();

	unsigned int size = raw->size();

	int ze = 0;
	while(size!=0 && ze < 8)
	{
		size /= 10;
		ze++;
	}

	int lines = ceil(raw->size()/8.0);
	//qDebug() << lines;
	int pos=0;

	QByteArray part;
	QString out;

	for( int i=0 ; i<lines ; i++ )
	{
		out.clear();
		part.clear();
		part = raw->mid(pos, 8);

		out.append("0x");
		out.append(QString("%1").arg(pos,ze,16,QChar('0')));
		out.append(" / ");
		out.append(QString("%1").arg(pos,ze,10,QChar('0')));
		out.append(" - ");
		out.append(part.toHex());

		if(part.size() != 8)
		{
			int padd = 8-part.size();
			while(padd!=0)
			{
				out.append("  ");
				padd--;
			}
		}

		out.append(" - ");

		for( int p=0 ; p<part.size() ; p++ )
		{
			char c = part[p];
			if(c >= 0x20 && c <= 0x7E)
			{
				out.append(c);
			}
			else
			{
				out.append(' ');

			}
		}

		//qDebug() << out << part.toHex();
		qDebug() << out;
		pos +=8;
	}

	qDebug() << func << line << "RawDump: END";
}

/*
int main(int argc, char *argv[])
{
	QByteArray arr;

	char tmp=0x0;
	for( int i=0 ; i<150 ; i++ )
	{
		arr.append(tmp++);
	}

	//arr.fill(0x6, 50);

	//qDebug() << arr.toHex();

	//dump(&arr, __func__, __LINE__);
	DUMP(&arr);
}
*/
