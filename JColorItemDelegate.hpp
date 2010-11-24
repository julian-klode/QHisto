/* JColorItemDelegate.hpp - Interface of JColorItemDelegate.
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

#ifndef J_COLOR_ITEM_DELEGATE_H
#define J_COLOR_ITEM_DELEGATE_H

#include <QStyledItemDelegate>

/**
 * \brief Represents a color item in an QAbstractItemModel.
 *
 * The QAbstractItemModel contains the name of the color, and
 * this class causes the item to display just the color and on
 * clicks, open a dialog to choose a color.
 */
class JColorItemDelegate : public QStyledItemDelegate {
protected:
    /**
     * \brief Called when the item shall be painted
     *
     * Use painter to paint the rectangle in option using the
     * color stored at index.
     *
     * \param painter A QPainter that is used for painting.
     * \param option Description of the item (includes a 'rect' member).
     * \param index The index in the model.
     * \see QStyleOptionViewItem::rect
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    /**
     * \brief Called when an editor event is emitted.
     *
     * Take care of mouse clicks. Leave everything else
     * to the default handlers. This allows the item to
     * be editable via keyboard by entering a name.
     */
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index);

};
#endif /* J_COLOR_ITEM_DELEGATE_H */
