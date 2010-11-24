#ifndef J_HISTOGRAM_VIEW_H
#define J_HISTOGRAM_VIEW_H

#include <QWidget>

class QPainter;
class QPaintEvent;
class JHistoModel;

class JHistoView : public QWidget {
    Q_OBJECT
  private:
    JHistoModel *model;
    int getY(double value);
    double getYValueDistance();
    double paintAxisY(QPainter &painter);
  public:
    JHistoView(QWidget *parent=NULL);
    void paintEvent(QPaintEvent *e);
    void setModel(JHistoModel *model);
};

#endif  /* J_HISTOGRAM_VIEW_H */
