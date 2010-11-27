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
    virtual QString getLabel(int index) const = 0;

    /**
     * \brief The value the drawn bar shall represent.
     *
     * Return the value that shall be represented by
     * a bar.
     *
     * \param index An index in the model
     * \see getLabel(), getColor(), size()
     */
    virtual double getValue(int index) const = 0;

    /**
     * \brief The color the drawn bar shall receive (if possible).
     *
     * Return the bar for the graph representing the values at the
     * position 'index' in the model.
     *
     * \param index An index in the model
     * \see getLabel(), getValue(), size()
     */
    virtual QColor getColor(int index) const = 0;

    /**
     * \brief The number of items stored in this model.
     *
     * Return the number of items stored in this model, where
     * 'item' refers to one bar in a histogram.
     */
    virtual int size() const = 0;

    /**
     * \brief The maximum value stored in this model.
     *
     * Subclasses should reimplement this function if possible, this
     * basic implementation queries the whole list for the maximum
     * value and thus has O(n) complexity. Implementations should
     * store the value in a member variable if the maximum value
     * changes and return the value of that member variable here.
     */
    virtual double maximumValue() const;

    /**
     * \brief The minimum value stored in this model.
     *
     * Subclasses should reimplement this function if possible, this
     * basic implementation queries the whole list for the minimum
     * value and thus has O(n) complexity. Implementations should
     * store the value in a member variable if the minimum value
     * changes and return the value of that member variable here.
     */
    virtual double minimumValue() const;

    /**
     * \brief Read the model data from a file.
     *
     * Read files in the format used by writeToFile(). In addition
     * to the format written by writeToFile(), this function also
     * supports the use of names to specify colors, as specified
     * in the SVG Specification. In general any string recognized
     * by QColor is supported.
     *
     * This function deletes all previous entries from the model,
     * use it with care.
     *
     * If an error occurred, a QString describing the error is
     * thrown and it is undefined whether anything has been
     * read from the file.
     *
     * \param filename The name of the file to be read from.
     * \throw A QString describing the error.
     * \see QColor, writeToFile,
     *      http://www.w3.org/TR/SVG/types.html#ColorKeywords
     */
    void readFromFile(const QString &filename) throw(QString);

    /**
     * \brief Write the model data to a file.
     *
     * Store the model data into a file. Each line represents
     * one row and contains at least 3 tab-separated fields. The
     * first field is the label, the second one is the value, and
     * the third one the color. More fields are not specified.
     *
     * If the label contains a tabulator, it is escaped using
     * standard C conventions. The color is represented as
     * by its hexadecimal specifier prefixed by "#", for example,
     * red is represented by "#ff0000".
     *
     * If an error occurred, a QString describing the error is
     * thrown and it is undefined whether anything has been
     * written to the file.
     *
     * \param filename The name of the file to write to.
     * \throw A QString describing the error.
     */
    void writeToFile(const QString &filename) const throw(QString);

Q_SIGNALS:
    /** \brief Emitted when the data in the model changes */
    virtual void changed();

public Q_SLOTS:
    /**
     * \brief Add a new item to the model.
     *
     * May be connected to a button in the user interface to
     * insert a new model; or something similar.
     */
    virtual void add() = 0;

    /** \brief Remove all items from the model. */
    virtual void clear() = 0;
protected:
    /**
     * \brief Insert a new row into the model.
     *
     * Insert a new row into the model, using the data given
     * via the arguments.
     *
     * \param label The label of the new row.
     * \param value The value. May change the result of maximumValue().
     * \param color The color the bar should have.
     */
    virtual void add(QString label, double value, QColor color) = 0;
};

#endif /* J_HISTO_MODEL_H */
