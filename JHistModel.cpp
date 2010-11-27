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

double JHistModel::maximumValue() const
{
    double scale = 0;
    for (int i=0; i<= size(); i++)
        scale = qMax(scale, getValue(i));
    return scale;
}

double JHistModel::minimumValue() const
{
    double scale = 0;
    for (int i=0; i<= size(); i++)
        scale = qMin(scale, getValue(i));
    return scale;
}

/* Now comes the simple read-write support */

/**
 * \brief Escape or unescape the given string.
 *
 * Escape or unescape a string according to the specification
 * in QHisto(5).
 *
 * \param reading If true, escape sequences will be replaced by
 *                characters. Otherwise, characters will be escaped.
 */
static QString escape(QString string, bool reading) {
    static const struct {
        char character[2];
        char escaped[3];
    } sequences[] = { { "\\", "\\\\" }, { "\n", "\\n"}, {"\r", "\\r" },
                      { "\t", "\\t" }, { "#", "\\#"} };
    for (unsigned i = 0; i < sizeof(sequences) / sizeof(sequences[0]); i++) {
        if (reading)
            string.replace(sequences[i].escaped, sequences[i].character);
        else
            string.replace(sequences[i].character, sequences[i].escaped);
    }
    return string;
}

void JHistModel::readFromFile(const QString &filename) throw (QString)
{
    QFile device(filename);
    QByteArray line;
    if (!device.open(QIODevice::ReadOnly | QIODevice::Text))
        throw device.errorString();

    clear();
    while ((line = device.readLine()).size()) {
        if (line.at(0) == '#')
            continue;
        if (line.at(line.size() -1) == '\n')
            line.truncate(line.size() - 1);
        QList<QByteArray> list = line.split('\t');
        add(escape(QString::fromUtf8(list[0]), true),
            list[1].toDouble(), QColor(QString(list[2])));
    }
}

void JHistModel::writeToFile(const QString &filename) const throw (QString)
{
    QFile device(filename);
    if (!device.open(QIODevice::WriteOnly))
        throw device.errorString();
    for (int i=0; i < size(); i++) {
        device.write(escape(getLabel(i), false).toUtf8());
        device.write("\t");
        device.write(QString::number(getValue(i)).toUtf8());
        device.write("\t");
        device.write(getColor(i).name().toUtf8());
        device.write("\n");
    }
}
