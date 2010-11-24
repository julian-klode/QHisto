/* JHistoView.cpp - Implementation of the JHistoView class.
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
#include "JHistoModel.hpp"
#include "JHistoView.hpp"
#include <QPainter>
#include <QRect>
#include <QtGlobal>

JHistoView::JHistoView(QWidget *parent)
    : QWidget(parent)
{
};

int JHistoView::getY(double value)
{
    return height() - 0.8 * height() * (value / model->scale());
}

void JHistoView::setModel(JHistoModel * model)
{
    this->model = model;
}

double JHistoView::paintAxisY(QPainter &painter)
{
    const double scale = model->scale();
    const double distance = scale / 4;
    double offset = 0;
    for (double i = 0; i <= scale; i = qMin(i + distance, scale)) {
        QString text = QString::number(i, 'g');

        offset = qMax(offset, 1.2 * fontMetrics().width(text));

        painter.drawText(0, getY(i), text);
        if (i == scale)
            break;
    }
    return offset;
}

void JHistoView::paintEvent(QPaintEvent*)
{
    if (model->size() == 0)
        return;

    QPainter painter(this);
    QRect rect;
    const double offset = paintAxisY(painter);

    rect.setWidth(model->size() ? 0.75 * width() / model->size() : 0);

    for (int i = 0; i < model->size(); i++) {
        // Set height and position of the rectangle
        rect.setHeight(height() - getY(model->getValue(i)));
        rect.setX(offset + i * rect.width() * 1.25);
        rect.setY(getY(model->getValue(i)));

        // Draw the rectangle, write value into it
        painter.setBrush(model->getColor(i));
        painter.drawRect(rect);
        painter.drawText(rect, Qt::AlignCenter,
                         QString::number(model->getValue(i)));

        // Draw the label above the rectangle
        rect.setHeight(fontMetrics().height());
        rect.moveTop(rect.top() - 1.1 * rect.height());
        painter.drawText(rect, Qt::AlignCenter, model->getLabel(i));
    }
}
