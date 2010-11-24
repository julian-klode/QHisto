#ifndef J_HISTO_MODEL_H
#define J_HISTO_MODEL_H

#include <QObject>
#include <QString>
#include <QColor>

class JHistoModel : public QObject {
    Q_OBJECT
public:
    virtual QString getLabel(int index) = 0;
    virtual double getValue(int index) = 0;
    virtual QColor getColor(int index) = 0;
    virtual int size() = 0;
    virtual double scale();
};

#endif /* J_HISTO_MODEL_H */
