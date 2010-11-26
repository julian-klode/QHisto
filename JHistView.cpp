/* JHistView.cpp - Implementation of the JHistView class.
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
#include "JHistView.hpp"
#include <QPainter>
#include <QRect>
#include <QtGlobal>

JHistView::JHistView(QWidget *parent)
    : QWidget(parent)
{
};

int JHistView::getY(double value)
{
    const double dist = model->maximumValue() - model->minimumValue();
    return (0.8 * height() * (model->maximumValue() - value) / dist
            + fontMetrics().height() );
}


void JHistView::setModel(JHistModel * model)
{
    this->model = model;
}

double JHistView::paintAxisY(QPainter &painter)
{
    const double scale = model->maximumValue() - model->minimumValue();
    const double distance = scale / 4;
    double offset = 0;
    for (double i = model->minimumValue(); i <= scale; i =
         qMin(i + distance, scale)) {
        QString text = QString::number(i, 'g');

        offset = qMax(offset, 1.5 * fontMetrics().width(text));

        painter.drawText(0, getY(i), text);
        if (i == scale)
            break;
    }
    return offset;
}

void JHistView::paintEvent(QPaintEvent*)
{
    if (model->size() == 0)
        return;

    QPainter painter(this);
    const double offset = paintAxisY(painter);
    const double rectWidth = width() / (1.5 * model->size());

    for (int i = 0; i < model->size(); i++) {
        // Create a new rectangle using QRect(x, y, width, height)
        QRect rect(offset + i * rectWidth * 1.25,
                   getY(model->getValue(i)),
                   rectWidth,
                   getY(0) - getY(model->getValue(i)));

        // Draw the rectangle, write value into it
        painter.setBrush(model->getColor(i));
        painter.drawRect(rect);
        painter.drawText(rect, Qt::AlignCenter,
                         QString::number(model->getValue(i)));

        // Draw the label above the rectangle
        rect.setHeight(fontMetrics().height());
        rect.moveTop(rect.top() - 1.1 * rect.height());
        painter.drawText(rect, Qt::AlignCenter, model->getLabel(i));

        // Draw an x-axis
        if (i > 0)
            painter.drawLine(rect.left() - rectWidth * 0.25, getY(0),
                             rect.left(), getY(0));
    }
}
