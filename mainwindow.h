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

    /* Gui helper containers */
    std::vector<std::variant<int,float,double,std::string,QString,bool>> draft;
    std::map<QString,void * > dictation;//int instead use vector
    std::map<QString,QString> kindDictation;
    std::map<QString,int> lengthDictataion;


    /* shared memory */
    std::map<QString,void *> configDictionary;
    char * tempConfig;
    int ConfigOblockSize;
    char * temp;

public slots:
    void checkBoxSlot(bool b);
    void lineEditSlot(const QString &);
    void comboBoxSlot(int index);
    void openArrayFromFileSlot();
private slots:
    void on_apply_pb_clicked();

private:
    Ui::MainWindow *ui;
    void show_message_error(QString message);
    int file_string_matches_json(QString str_file,int len,QString type);
    int check_type_of_input_file(QString values,QString type);
};
#endif // MAINWINDOW_H
