#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cstring"
#include <./nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QGroupBox>
#include <QFileDialog>
// memory
#include <vector>
#include <variant>
#include <map>
//json specific
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <ConfigOBlock.h>

#include <QProcess>
#include <QMessageBox>
#include <QRegularExpression>
#include <QValidator>
#include <QRegularExpressionValidator>
using json = nlohmann::json;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    std::ifstream f("/home/manofaction/Documents/omid-project/JsonParser/OJsonParsee/jsonExample.json");


//    auto process = new QProcess(this);
//    connect(process,&QProcess::finished,[this](int e ,QProcess::ExitStatus){
//        if(e == 0){
//            this->readJson();
//        }
//        else
//        {
//            qDebug() << "e";
//        }

//    });
//    connect(process,&QProcess::readyReadStandardError,[process](){
//        qDebug() << "OPMID";
//        qDebug() << process->readAllStandardError().data();
//    });
//    QString script = "echo";
//    process->start(script,QStringList() << "navid/jhkl");

//    self.p.finished.connect(self.process_finished)
//    self.p.readyReadStandardError.connect(self.error_occured)
//    self.p.start("./script.sh")


    ConfigOblockSize = sizeof(ConfigOBlock);
    tempConfig = new char[ConfigOblockSize];
    memset((void *)tempConfig,0,ConfigOblockSize);
    temp = tempConfig;
    qDebug();
    readJson();
    }


void MainWindow::readJson(){
    QFile file;
    file.setFileName("/home/manofaction/Documents/omid-project/JsonParser/OJsonParsee/jsonExample.json");
    if(!file.open(QIODevice::ReadOnly)){
       qDebug() << "Json filef couldn't be opened/found";
       return;
    }
    QByteArray byteArray;
    byteArray = file.readAll();
    file.close();


    QJsonParseError parseError;
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(byteArray, &parseError);
    if(parseError.error != QJsonParseError::NoError){
        qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
        return;
    }

    QJsonObject data;
    data = jsonDoc.object();


    qDebug() << data.value("configs").toArray().at(0).toObject().value("label").toString();

    QGridLayout * ConfigGridLayout = ui->gridLayout_config;
    int ii = 0;
    for(int i = 0 ; i < data.value("configs").toArray().size()+ii ; i++){



        if(data.value("configs").toArray().at(i).toObject().value("type") == "bool")
        {
            QString label = data.value("configs").toArray().at(i).toObject().value("label").toString();
            ConfigGridLayout->addWidget(new QLabel(label.replace('"',' ')),i+1,1);

            kindDictation[QString::number(i+ii)] = QString("bool");

            auto check_box = new QCheckBox();

            // connection between draft and vector
            qDebug() << i <<" " <<  ii;
            dictation[QString::number(i+ii)] = check_box;
            draft.push_back(check_box->isChecked());
            connect(check_box,SIGNAL(toggled(bool)),this,SLOT(checkBoxSlot(bool)));

            configDictionary[QString::number(i+ii)] = temp;
            temp += sizeof(bool);


            ConfigGridLayout->addWidget(check_box,i+1,2);
        }
        else if(data.value("configs").toArray().at(i).toObject().value("type") == "text")
        {
            /* Label */
            QString label = data.value("configs").toArray().at(i).toObject().value("label").toString();
            ConfigGridLayout->addWidget(new QLabel(label.replace('"',' ')),i+1,1);

            /* Kind : for example float, uint32 , string and etc ...*/
            QString kind = data.value("configs").toArray().at(i).toObject().value("kind").toString();
            kindDictation[QString::number(i+ii)] = kind;

            /* gui dictionary */
            auto line_edit = new QLineEdit();
            dictation[QString::number(i+ii)] = line_edit;

            /* vector of values */
            draft.push_back(line_edit->text());

            /* signal */
            connect(line_edit,SIGNAL(textChanged(QString)),this,SLOT(lineEditSlot(QString)));

            /* config shared memory */
            configDictionary[QString::number(i+ii)] = temp;
            QValidator *validator;
            if(kind == "int")
            {
                temp += sizeof(int);
                QRegularExpression exp ("[+-]?([0-9]*)");
                validator = new QRegularExpressionValidator(exp,this);
                line_edit->setValidator(validator);

            }else if(kind == "float")
            {
                QRegularExpression exp ("[+-]?([0-9]*[.])?[0-9]+");
                validator = new QRegularExpressionValidator(exp,this);
                line_edit->setValidator(validator);
                temp += sizeof(float);
            }else if(kind == "double")
            {
                QRegularExpression exp ("[+-]?([0-9]*[.])?[0-9]+");
                validator = new QRegularExpressionValidator(exp,this);
                line_edit->setValidator(validator);
                temp += sizeof(double);
            }


            ConfigGridLayout->addWidget(line_edit,i+1,2);
        }
        else if(data.value("configs").toArray().at(i).toObject().value("type") == "enum")
        {
            QString label = data.value("configs").toArray().at(i).toObject().value("label").toString();
            ConfigGridLayout->addWidget(new QLabel(label.replace('"',' ')),i+1,1);

            auto combo = new QComboBox();
            for(int j = 0; j < data.value("configs").toArray().at(i).toObject().value("enums").toArray().size() ; j++){
                combo->addItem(data.value("configs").toArray().at(i).toObject().value("enums").toArray().at(j).toString());
            }

            /* gui dictionary */
            dictation[QString::number(i+ii)] = combo;

            /* vector of values */
            draft.push_back(combo->currentIndex());

            /* signal */
            connect(combo,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxSlot(int)));

            /* change config of shared memory*/
            configDictionary[QString::number(i+ii)] = temp;

            /* go forward */
            temp += sizeof(int);


            ConfigGridLayout->addWidget(combo,i+1,2);
        }
        else if(data.value("configs").toArray().at(i).toObject().value("type") == "struct")
        {
            ii = data.value("configs").toArray().at(i).toObject().value("struct").toArray().size()-1;

            QGridLayout * g = new QGridLayout();
            for(int j = 0; j < data.value("configs").toArray().at(i).toObject().value("struct").toArray().size() ; j++){
                g->addWidget(new QLabel(data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("label").toString()),j,0);
                if(data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("type") == "text")
                {


                    /* Kind : for example float, uint32 , string and etc ...*/
                    QString kind = data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("kind").toString();
                    kindDictation[QString::number(i+j)] = kind;

                    /* gui dictionary */
                    auto line_edit = new QLineEdit();

                    dictation[QString::number(i+j)] = line_edit;

                    /* vector of values */
                    draft.push_back(line_edit->text());

                    /* signal */
                    connect(line_edit,SIGNAL(textChanged(QString)),this,SLOT(lineEditSlot(QString)));

                    /* config shared memory */
                    configDictionary[QString::number(i+j)] = temp;
                    QValidator *validator;
                    if(kind == "int")
                    {
                        QRegularExpression exp ("[+-]?([0-9]*)");
                        validator = new QRegularExpressionValidator(exp,this);
                        line_edit->setValidator(validator);
                        temp += sizeof(int);
                    }else if(kind == "float")
                    {
                        QRegularExpression exp ("[+-]?([0-9]*[.])?[0-9]+");
                        validator = new QRegularExpressionValidator(exp,this);
                        line_edit->setValidator(validator);
                        temp += sizeof(float);
                    }else if(kind == "double")
                    {
                        QRegularExpression exp ("[+-]?([0-9]*[.])?[0-9]+");
                        validator = new QRegularExpressionValidator(exp,this);
                        line_edit->setValidator(validator);
                        temp += sizeof(double);
                    }
                    g->addWidget(line_edit,j,1);

                }else if(data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("type") == "bool"){
                    kindDictation[QString::number(i+j)] = QString("bool");

                    auto check_box = new QCheckBox();

                    // connection between draft and vector
                    dictation[QString::number(i+j)] = check_box;
                    draft.push_back(check_box->isChecked());
                    connect(check_box,SIGNAL(toggled(bool)),this,SLOT(checkBoxSlot(bool)));

                    configDictionary[QString::number(i+j)] = temp;
                    temp += sizeof(bool);

                    g->addWidget(check_box,j,1);

                }else if (data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("type") == "enum"){
                    auto combo = new QComboBox();
                    for(int m = 0; m < data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("enums").toArray().size() ; m++){
                        combo->addItem(data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("enums").toArray().at(m).toString());
                    }



                    /* gui dictionary */
                    dictation[QString::number(i+j)] = combo;

                    /* vector of values */
                    draft.push_back(combo->currentIndex());

                    /* signal */
                    connect(combo,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxSlot(int)));

                    /* change config of shared memory*/
                    configDictionary[QString::number(i+j)] = temp;

                    /* go forward */
                    temp += sizeof(int);


                    ConfigGridLayout->addWidget(combo,i+1,2);

                    g->addWidget(combo,j,1);

                }

                auto structure_group_box = new QGroupBox(data.value("configs").toArray().at(i).toObject().value("label").toString());
                structure_group_box->setLayout(g);
                ConfigGridLayout->addWidget(structure_group_box,i+1,1,1,2);


            }

        }/*************************     ARRAY     ****************************************/
        else if(data.value("configs").toArray().at(i).toObject().value("type") == "array")
        {
            QString label = data.value("configs").toArray().at(i).toObject().value("label").toString();
            ConfigGridLayout->addWidget(new QLabel(label.replace('"',' ')),i+1,1);

            QPushButton * openfileButton = new QPushButton("import from csv");
            dictation[QString::number(i+ii)] = openfileButton;
            connect(openfileButton,SIGNAL(clicked()),this,SLOT(openArrayFromFileSlot()));
            ConfigGridLayout->addWidget(openfileButton,i+1,2);

            /* Kind : for example float, uint32 , string and etc ...*/
            QString kind = data.value("configs").toArray().at(i).toObject().value("kind").toString();
            kindDictation[QString::number(i+ii)] = kind;

            /* Length */
            auto mmmmm = data.value("configs").toArray().at(i).toObject();
            int length = data.value("configs").toArray().at(i).toObject().value("arrlength").toInt();
            lengthDictataion[QString::number(i+ii)] = length;

            int len = length;


            /* config shared memory */

            configDictionary[QString::number(i+ii)] = temp;

            /* vector of values */
            if(kind == "int")
            {
                int a[len];
                memset(a,0,len*sizeof(int));
                for(int i = 0 ; i < len ; i++)
                {
                    draft.push_back(a[i]);
                }
                temp += sizeof(int)*len;
            }else if(kind == "float")
            {
                float a[len];
                memset(a,0,len*sizeof(float));
                for(int i = 0 ; i < len ; i++)
                {
                    draft.push_back(a[i]);
                }
                temp += sizeof(float)*len;

            }else if(kind == "double")
            {
                double a[len];
                memset(a,0,len*sizeof(double));
                for(int i = 0 ; i < len ; i++)
                {
                    draft.push_back(a[i]);
                }
                temp += sizeof(double)*len;
            }



        }
}


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkBoxSlot(bool b)
{
    auto ref = dynamic_cast<QCheckBox*>(sender());
    int i;
    for(i = 0 ; i < dictation.size() ; i++)
    {
        if(ref == dictation[QString::number(i)]){
            std::get<bool>(draft.at(i))=b;
            break;
        }
    }

    memcpy(
    configDictionary[QString::number(i)],
    &b,
    sizeof(bool));

}

void MainWindow::lineEditSlot(const QString& text)
{

    auto ref = dynamic_cast<QLineEdit*>(sender());
    int i;
    for(i = 0 ; i < dictation.size() ; i++)
    {

        if(ref == dictation[QString::number(i)]){
            std::get<QString>(draft.at(i))=text;
            break;
        }
    }

    /* get cast type from kindDictation */
    if(kindDictation[QString::number(i)]=="int")
    {
        int a = text.toInt();
        qDebug() << *(int * )(configDictionary[QString::number(i)]);
        memcpy(configDictionary[QString::number(i)],&a,sizeof(int));
        qDebug() << *(int * )(configDictionary[QString::number(i)]);
    }else
    if(kindDictation[QString::number(i)]=="float")
    {
        float a = text.toFloat();
        memcpy(configDictionary[QString::number(i)],&a,sizeof(float));
    }else
    if(kindDictation[QString::number(i)]=="double"){
        double a = text.toDouble();
        memcpy(configDictionary[QString::number(i)],&a,sizeof(double));
    }


}

void MainWindow::comboBoxSlot(int index)
{
    /* find sender */
    auto ref = dynamic_cast<QComboBox*>(sender());
    int i;
    for(i = 0 ; i < dictation.size() ; i++)
    {
        if(ref == dictation[QString::number(i)]){
            std::get<int>(draft.at(i))=index;
            break;
        }
    }

    memcpy(configDictionary[QString::number(i)],&index,sizeof(int));

}

void MainWindow::openArrayFromFileSlot(){
    QString FileName = QFileDialog::getOpenFileName(this,"Choose csv file of data","","csv (*.csv)");
    QStringList wordList;
    QFile file(FileName);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << file.errorString();

            return;
        }
        QString line = file.readAll();

        wordList = QStringList(line.split(','));



        /* find sender */
        auto ref = dynamic_cast<QPushButton*>(sender());
        int i;
        for(i = 0 ; i < dictation.size() ; i++)
        {
            if(ref == dictation[QString::number(i)]){
                int len = 0;
                len = lengthDictataion[QString::number(i)];
                int check = file_string_matches_json(line,len,kindDictation[QString::number(i)]);

                /** Fill Vector (draft) **/
                if(!check)
                {
                    char * tempFill = (char*)configDictionary[QString::number(i)];
                    for(int j = 0 ; j < len ; j++)
                    {
                        if(kindDictation[QString::number(i)]=="int")
                        {
                            std::get<int>(draft.at(i+j))=wordList.at(j).toInt();
                            int a = std::get<int>(draft.at(i+j));
                            memcpy(tempFill,&a,len*sizeof(int));
                            tempFill+= sizeof(int);
                        }else if(kindDictation[QString::number(i)]=="float")
                        {
                            std::get<float>(draft.at(i+j))=wordList.at(j).toFloat();
                            float a = std::get<float>(draft.at(i+j));
                            memcpy(tempFill,&a,len*sizeof(float));
                            tempFill+= sizeof(float);
                        }else if(kindDictation[QString::number(i)]=="double")
                        {
                            std::get<double>(draft.at(i+j))=wordList.at(j).toDouble();
                            double a = std::get<double>(draft.at(i+j));
                            memcpy(tempFill,&a,len*sizeof(double));
                            tempFill+= sizeof(double);
                        }

                    }
                }



                break;
            }
        }



}

void MainWindow::on_apply_pb_clicked()
{
    ConfigOBlock Alireza;
    memset(&Alireza,0,ConfigOblockSize);
    memcpy(&Alireza,tempConfig,ConfigOblockSize);

    qDebug()<< sizeof(int) << sizeof(bool) << sizeof(float) << sizeof(double);
}

void MainWindow::show_message_error(QString message)
{
    QMessageBox::critical(this,"Error",message);
}

int MainWindow::file_string_matches_json(QString str_file, int len,QString type)
{
    QStringList list = str_file.split(",");
    if (list.size() != len)
    {
        show_message_error("Your file is not compatible as length with array length.");
        return 0;
    }
    int b = check_type_of_input_file(str_file,type);
    return b;
}

int MainWindow::check_type_of_input_file(QString values, QString type)
{
    QValidator::State st = QValidator::Invalid;
    if(type == "int")
    {
        QRegularExpression regEx ("([+-]?[0-9]+[,]{0,1})+");
        QRegularExpressionValidator v(regEx,0);
        int pos = 0;
        st = v.validate(values,pos);

    }else if(type == "float")
    {
        QRegularExpression regEx ("([+-]?([0-9]*[.])?[0-9]+[,]{0,1})+");
        QRegularExpressionValidator v(regEx,0);
        int pos = 0;
        st = v.validate(values,pos);

    }else if(type == "double")
    {
        QRegularExpression regEx ("([+-]?([0-9]*[.])?[0-9]+[,]{0,1})+");
        QRegularExpressionValidator v(regEx,0);
        int pos = 0;
        st = v.validate(values,pos);

    }

    if(st != QValidator::Acceptable)
    {
        show_message_error("Your file is incorrect!!");
        return 0;
    }
    return 1;
}





