#include <cstddef>
#include <map>
#include <QString>
#include <QColor>

#ifndef J_HISTO_MODEL_H
#define J_HISTO_MODEL_H

class JHistoModel {
public:
    virtual QString getLabel(size_t index);
    virtual double getValue(size_t index);
    virtual QColor getColor(size_t index);
    virtual size_t size();
};

#endif /* J_HISTO_MODEL_H */
