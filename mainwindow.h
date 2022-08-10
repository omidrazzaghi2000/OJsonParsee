#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void readJson();
    ~MainWindow();
    std::vector<std::variant<int,float,std::string,bool>> draft;
    std::map<QString,void * > dictation;//int instead use vector
    std::map<QString,void *> configDictionary;
    char * tempConfig;
    int ConfigOblockSize;
    char * temp;

public slots:
    void checkBoxSlot(bool b);
private slots:
    void on_apply_pb_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
