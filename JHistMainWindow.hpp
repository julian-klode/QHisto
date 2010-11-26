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
    void clear();
    void remove();
    void save();
    void saveAs();
public Q_SLOTS:
    /**
     * \brief Open a file for reading.
     *
     * If the argument defName is an empty string, open a file chooser
     * and open the file chosen by the user. If defName is not empty,
     * open the file with that name. If the file cannot be opened, ask
     * the user what to do.
     *
     * \param defName The name of the file to open, empty string causes
     *                a file chooser to ask the user for a file.
     */
    void open(QString defName = "");
private:
    JItemHistModel model;
    JHistView histview;
    QTableView tableview;
    QPushButton button_add;
    QPushButton button_remove;
    JColorItemDelegate color_delegate;
    QString fileName;
};
