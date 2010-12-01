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
#include <QPrinter>

JHistView::JHistView(QWidget *parent)
    : QWidget(parent), model(NULL)
{
};

enum columns {
    COLUMN_VALUE = 1
};

static double maximumValue(QAbstractItemModel *model, int column)
{
    double max = 0;
    for (int i = 0; i < model->rowCount(); i++)
        max = qMax(max, model->index(i, column).data().toDouble());
    return max;
}

static double minimumValue(QAbstractItemModel *model, int column)
{
    double min = 0;
    for (int i = 0; i < model->rowCount(); i++)
        min = qMin(min, model->index(i, column).data().toDouble());
    return min;
}

int JHistView::getY(double value)
{
    const double dist = (maximumValue(model, COLUMN_VALUE) -
                         minimumValue(model, COLUMN_VALUE));
    return (0.8 * height() * (maximumValue(model, COLUMN_VALUE) - value) / dist
            + fontMetrics().height() );
}

void JHistView::setModel(QAbstractItemModel *model)
{
    if (this->model != NULL)
        this->model->disconnect(this);

    this->model = model;

    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(repaint()));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(repaint()));
    connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(repaint()));
    connect(model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(repaint()));
    connect(model, SIGNAL(modelReset()), this, SLOT(repaint()));
}

double JHistView::paintAxisY(QPainter &painter)
{
    const double scale = (maximumValue(model, COLUMN_VALUE) -
                          minimumValue(model, COLUMN_VALUE));
    const double maximum = maximumValue(model, COLUMN_VALUE);
    const double distance = scale / 4;
    double offset = 0;

    for (double i = minimumValue(model, COLUMN_VALUE); i <= maximum; i =
         qMin(i + distance, maximum)) {
        QString text = QString::number(i, 'g');

        offset = qMax(offset, 1.5 * fontMetrics().width(text));

        painter.drawText(0, getY(i), text);
        if (i == maximum)
            break;
    }
    painter.drawLine(offset, getY(maximumValue(model, COLUMN_VALUE)),
                     offset, getY(minimumValue(model, COLUMN_VALUE)));
    return offset + 1;
}

void JHistView::paintOnTo(QPrinter *device)
{
    return paintOnTo(static_cast<QPaintDevice*>(device));
}

void JHistView::paintOnTo(QPaintDevice *device)
{
    if (model->rowCount() == 0)
        return;

    painter.begin(device);

    const double offset = paintAxisY(painter);
    // Relative distance between two rectangles
    const double rectDist = 0.05;
    // width of one rectangle
    const double rectWidth = ((width() - offset) /
                              ((1 + rectDist) * model->rowCount()));

    for (int i = 0; i < model->rowCount(); i++) {
        QString label = model->index(i, 0).data().toString();
        double value = model->index(i, 1).data().toDouble();
        QColor color = model->index(i, 2).data().value<QColor>();
        // Create a new rectangle using QRect(x, y, width, height)
        QRect rect(offset + (i+1) * rectDist * rectWidth  + i * rectWidth,
                   getY(value),
                   rectWidth,
                   getY(0) - getY(value));

        // The height is negative, normalize the rectangle
        if (rect.height() < 0) {
            rect = rect.normalized();
            // TODO: Why: move to x-axis, it otherwise differs by one pixel.
            rect.moveTop(getY(0));
        }

        // Draw the rectangle, write value into it
        painter.setBrush(color);
        painter.drawRect(rect);
        painter.drawText(rect, Qt::AlignCenter,
                         QString::number(value));

        // Draw the label above the rectangle
        rect.setHeight(fontMetrics().height());
        rect.moveTop(rect.top() - 1.1 * rect.height());
        painter.drawText(rect, Qt::AlignCenter, label);

        // Draw an x-axis filling the distance to the left.
        painter.drawLine(rect.left() - rectWidth * rectDist, getY(0),
                         rect.left(), getY(0));
    }
    painter.end();
}

void JHistView::paintEvent(QPaintEvent*)
{
    paintOnTo(this);
}
