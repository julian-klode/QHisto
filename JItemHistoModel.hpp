#include <cstddef>
#include <QString>
#include <QColor>
#include <QAbstractItemModel>
#include "JHistoModel.hpp"

#ifndef J_ITEM_HISTO_MODEL_H
#define J_ITEM_HISTO_MODEL_H

/**
 * \brief Represent a QAbstractItemModel as a histogram.
 *
 * 
 */
class JItemHistoModel : public JHistoModel {
    Q_OBJECT

    QAbstractItemModel *items;

  public:
    /** \brief Set the model containing the items. */
    void setItemModel(QAbstractItemModel *items);
    /** \brief Returns the color stored at row 'index', first column. */
    QString getLabel(int index);
    /** \brief Returns the color stored at row 'index', second column. */
    double getValue(int index);
    /** \brief Returns the color stored at row 'index', third column. */
    QColor getColor(int index);
    /** \brief Returns the number of rows in the model */
    int size();

  public Q_SLOTS:
    /** \brief Slot that inserts an empty row.
     *
     * Add a new empty row. This function only works if the model is
     * used to expose the data of a QStandardItemModel.
     */
    void add();
};

#endif /* J_ITEM_HISTO_MODEL_H */
