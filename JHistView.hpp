/* JHistView.hpp - Interface of the JHistView class.
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

#ifndef J_HISTOGRAM_VIEW_H
#define J_HISTOGRAM_VIEW_H

#include <QWidget>
#include <QPainter>
#include <QFontMetrics>

class QPainter;
class QPrinter;
class QPaintEvent;
class QAbstractItemModel;

/**
 * \brief Class to display the data from a JHistModel.
 *
 * This widget is responsible for painting the bars corresponding
 * to the values in the JHistModel. The widget can be resized,
 * bars and axises will be resized as well.
 *
 * \see JHistModel
 */
class JHistView : public QWidget {
    Q_OBJECT

public:
    /**
     * \brief Constructor for the histogram view.
     *
     * \param parent The parent widget.
     * \see QWidget::QWidget()
     */
    JHistView(QWidget *parent=NULL);

    /**
     * \brief Set the model displayed by this view.
     *
     * \param model The model, ownership is not transferred.
     */
    void setModel(QAbstractItemModel *model);

protected:
    /**
     * \brief Called when the widget shall be painted.
     *
     * \param event The event the causes the painting.
     * \see QWidget::paintEvent()
     */
    void paintEvent(QPaintEvent *event);

public Q_SLOTS:
    /** \brief Paint onto the given device. */
    void paintOnTo(QPaintDevice *device);
    /** \brief Specialization of paintOnTo() for printers. */
    void paintOnTo(QPrinter *device);

private:
    /** \brief The model of which data is displayed. */
    QAbstractItemModel *model;
    /** \brief The painter does the painting on the device. */
    QPainter painter;

    /**
     * \brief Get the y-coordinate corresponding to the value.
     *
     * Convert between model values and the coordinate system
     * in the widget.
     *
     * \return The y-coordinate at which the rectangle shall be drawn.
     */
    int getY(double value);

    /**
     * \brief Paint a linear scale on the y-axis.
     *
     * Paint a scale on the y-axis starting at 0 until the
     * maximum value stored in the model.
     *
     * \return The width of the painted area, including padding
     * \see JHistModel::scale()
     */
    double paintAxisY(QPainter &painter);

    /** \brief Wrapper function for height. */
    int height() { return painter.device()->height(); }
    /** \brief Wrapper function for width. */
    int width() { return painter.device()->width(); }
    /** \brief Wrapper function for fontMetrics */
    QFontMetrics fontMetrics() { return painter.fontMetrics(); }
};

#endif  /* J_HISTOGRAM_VIEW_H */
