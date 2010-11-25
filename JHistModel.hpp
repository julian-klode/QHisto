/* JHistModel.hpp - Interface of the JHistModel class.
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

#ifndef J_HISTO_MODEL_H
#define J_HISTO_MODEL_H

#include <QObject>
#include <QString>
#include <QColor>

/**
 * \brief Abstract base class for models representing histograms.
 *
 * This class provides an interface describing a typical histogram. Most
 * of its functions are virtual, but some are not. JHistView objects can
 * represent the data from this model visually as histograms.
 *
 * \see JHistView
 */
class JHistModel : public QObject {
    Q_OBJECT
public:
    /**
     * \brief A label describing the value at 'index'
     *
     * Return a label describing the value. For example,
     * if you are creating a histogram showing the speed
     * various internet providers can provide, the return
     * value should be the name of the internet provider.
     *
     * \param index An index in the model
     * \see getLabel(), getColor(), size()
     */
    virtual QString getLabel(int index) = 0;

    /**
     * \brief The value the drawn bar shall represent.
     *
     * Return the value that shall be represented by
     * a bar.
     *
     * \param index An index in the model
     * \see getLabel(), getColor(), size()
     */
    virtual double getValue(int index) = 0;

    /**
     * \brief The color the drawn bar shall receive (if possible).
     *
     * Return the bar for the graph representing the values at the
     * position 'index' in the model.
     *
     * \param index An index in the model
     * \see getLabel(), getValue(), size()
     */
    virtual QColor getColor(int index) = 0;

    /**
     * \brief The number of items stored in this model.
     *
     * Return the number of items stored in this model, where
     * 'item' refers to one bar in a histogram.
     */
    virtual int size() = 0;

    /**
     * \brief The maximum value stored in this model.
     *
     * Subclasses should reimplement this function if possible, this
     * basic implementation queries the whole list for the maximum
     * value and thus has O(n) complexity. Implementations should
     * store the value in a member variable if the maximum value
     * changes and return the value of that member variable here.
     */
    virtual double maximumValue();

public Q_SLOTS:
    /**
     * \brief Add a new item to the model.
     *
     * May be connected to a button in the user interface to
     * insert a new model; or something similar.
     */
    virtual void add() = 0;
};

#endif /* J_HISTO_MODEL_H */
