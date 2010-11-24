#include <cstddef>
#include <QString>
#include <QColor>
#include <QAbstractItemModel>
#include "JHistoModel.hpp"

#ifndef J_ITEM_HISTO_MODEL_H
#define J_ITEM_HISTO_MODEL_H

class JItemHistoModel : public JHistoModel {

    QAbstractItemModel *items;
public:
    void setItemModel(QAbstractItemModel *items);
    QString getLabel(size_t index);
    double getValue(size_t index);
    QColor getColor(size_t index);
    size_t size();
};

#endif /* J_ITEM_HISTO_MODEL_H */
