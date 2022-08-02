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

//json specific
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

using json = nlohmann::json;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    std::ifstream f("/home/manofaction/Documents/omid-project/JsonParser/OJsonParsee/jsonExample.json");





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

    for(int i = 0 ; i < data.value("configs").toArray().size() ; i++){



        if(data.value("configs").toArray().at(i).toObject().value("type") == "bool")
        {
            QString label = data.value("configs").toArray().at(i).toObject().value("label").toString();
            ConfigGridLayout->addWidget(new QLabel(label.replace('"',' ')),i+1,1);
            auto check_box = new QCheckBox();
            ConfigGridLayout->addWidget(check_box,i+1,2);
        }else if(data.value("configs").toArray().at(i).toObject().value("type") == "text"){
            QString label = data.value("configs").toArray().at(i).toObject().value("label").toString();
            ConfigGridLayout->addWidget(new QLabel(label.replace('"',' ')),i+1,1);
            ConfigGridLayout->addWidget(new QLineEdit(),i+1,2);
        }else if(data.value("configs").toArray().at(i).toObject().value("type") == "enum"){
            QString label = data.value("configs").toArray().at(i).toObject().value("label").toString();
            ConfigGridLayout->addWidget(new QLabel(label.replace('"',' ')),i+1,1);

            auto combo = new QComboBox();
            for(int j = 0; j < data.value("configs").toArray().at(i).toObject().value("enums").toArray().size() ; j++){
                combo->addItem(data.value("configs").toArray().at(i).toObject().value("enums").toArray().at(j).toString());
            }
            ConfigGridLayout->addWidget(combo,i+1,2);
        }else if(data.value("configs").toArray().at(i).toObject().value("type") == "struct"){
            QGridLayout * g = new QGridLayout();
            for(int j = 0; j < data.value("configs").toArray().at(i).toObject().value("struct").toArray().size() ; j++){
                g->addWidget(new QLabel(data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("label").toString()),j,0);
                if(data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("type") == "text")
                {
                    g->addWidget(new QLineEdit(),j,1);

                }else if(data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("type") == "bool"){
                    g->addWidget(new QCheckBox(),j,1);
                }else if (data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("type") == "enum"){
                    auto combo = new QComboBox();
                    for(int m = 0; m < data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("enums").toArray().size() ; m++){
                        combo->addItem(data.value("configs").toArray().at(i).toObject().value("struct").toArray().at(j).toObject().value("enums").toArray().at(m).toString());
                    }
                    g->addWidget(combo,j,1);

                }

                auto structure_group_box = new QGroupBox(data.value("configs").toArray().at(i).toObject().value("label").toString());
                structure_group_box->setLayout(g);
                ConfigGridLayout->addWidget(structure_group_box,i+1,1,1,2);
            }
        }

    }





//        QHBoxLayout *l;
//        QWidget *w;

//        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//        ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

//        ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
//        ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1,
//                                0,
//                             new QTableWidgetItem("binary"));

//        l = new QHBoxLayout();
//        l->addWidget(new QCheckBox());


//        w = new QWidget();
//        l->setAlignment(Qt::AlignCenter);

//        w->setLayout(l);

//        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,1, w);

//        ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
//        ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1,
//                                0,
//                             new QTableWidgetItem("enumeration"));

//        l = new QHBoxLayout();
//        auto combo = new QComboBox();

//        combo->addItem("IRAN");
//        combo->addItem("USA");
//        combo->addItem("UNITED KINGDOM");
//        combo->addItem("WALES");

//        l->addWidget(combo);



//        w = new QWidget();
//        l->setAlignment(Qt::AlignCenter);
//        w->setLayout(l);

//        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,1, w);

//        ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
//        ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1,
//                                0,
//                             new QTableWidgetItem("structure"));


//        QGridLayout * g = new QGridLayout();
//        g->addWidget(new QLabel("struct_int"),0,0);
//        g->addWidget(new QLineEdit(),0,1);
//        g->addWidget(new QLabel("struct_bool"),1,0);
//        g->addWidget(new QCheckBox(),1,1);
//        g->addWidget(new QLabel("struct_enum"),2,0);
//        combo = new QComboBox();

//        combo->addItem("MOHAMMAD");
//        combo->addItem("ALI");
//        combo->addItem("HASSAN");
//        combo->addItem("HOSSIEN");
//        combo->addItem("FATEMEH");
//        g->addWidget(combo,2,1);


//        w = new QWidget();
//        g->setAlignment(Qt::AlignCenter);

//        w->setLayout(g);

//        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,1, w);


//        ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
//        ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1,
//                                0,
//                             new QTableWidgetItem("text"));

//        l = new QHBoxLayout();
//        l->addWidget(new QLineEdit());


//        w = new QWidget();
//        l->setAlignment(Qt::AlignCenter);

//        w->setLayout(l);

//        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,1, w);

}

MainWindow::~MainWindow()
{
    delete ui;
}

