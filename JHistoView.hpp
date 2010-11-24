#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include "JHistoModel.hpp"

class JHistoView : public QWidget {
    Q_OBJECT

    JHistoModel *model;

    public:
        JHistoView(QWidget *parent=NULL) : QWidget(parent) {};
        void setModel(JHistoModel *model)
        {
            this->model = model;
        }
        
        void paintEvent(QPaintEvent *e) {
        QPainter p(this);
        int rwidth = width() / (1.5 * model->size());
        for (size_t i=0; i < model->size(); i++) {
            int rheight = height() * 0.01 * model->getValue(i) * (i+1);
            

            p.setBrush(model->getColor(i));
            p.drawRect (i * rwidth * 1.25, height() - rheight - 1, rwidth, rheight);
            p.drawText (i * rwidth * 1.25, height() - rheight - 20, rwidth, 50, 0, model->getLabel(i), 0);
        }
    

        


    }
};
