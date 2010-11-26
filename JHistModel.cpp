/* JHistModel.cpp - Implementation of parts of the JHistModel class.
 *
 * Copyright (C) 2010 Julian Andres Klode <jak@jak-linux.org>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "JHistModel.hpp"

#include <QtGlobal>
#include <QFile>

double JHistModel::maximumValue()
{
    double scale = 0;
    for (int i=0; i<= size(); i++)
        scale = qMax(scale, getValue(i));
    return scale;
}

double JHistModel::minimumValue()
{
    double scale = 0;
    for (int i=0; i<= size(); i++)
        scale = qMin(scale, getValue(i));
    return scale;
}

void JHistModel::readFromFile(const QString &filename) throw (QString)
{
    QFile device(filename);
    QByteArray line;
    if (!device.open(QIODevice::ReadOnly))
        throw device.errorString();
    while ((line = device.readLine()).size()) {
        QList<QByteArray> list = line.left(line.size() - 1).split('\t');
        add(list[0].replace("\\t", "\t"), list[1].toDouble(),
            QColor(QString(list[2])));
    }
}

void JHistModel::writeToFile(const QString &filename) throw (QString)
{
    QFile device(filename);
    if (!device.open(QIODevice::WriteOnly))
        throw device.errorString();
    for (int i=0; i < size(); i++) {
        device.write(getLabel(i).replace("\t", "\\t").toAscii());
        device.write("\t");
        device.write(QString::number(getValue(i)).toAscii());
        device.write("\t");
        device.write(getColor(i).name().toAscii());
        device.write("\n");
    }
};
