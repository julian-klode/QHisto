#include "JItemHistModel.hpp"
#include "JHistView.hpp"
#include "JColorItemDelegate.hpp"


#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>

class JHistMainWindow : public QWidget {
    Q_OBJECT

public:
    JHistMainWindow();
private Q_SLOTS:
    void about();
    void remove();
private:
    JItemHistModel model;
    JHistView histview;
    QTableView tableview;
    QPushButton button_add;
    QPushButton button_remove;
    JColorItemDelegate color_delegate;
};
