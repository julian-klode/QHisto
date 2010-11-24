#include "JHistoModel.hpp"

#include <QtGlobal>

double JHistoModel::scale()
{
    double scale = 0;
    for (int i=0; i<= size(); i++)
        scale = qMax(scale, getValue(i));
    return scale;
}
