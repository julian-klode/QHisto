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
