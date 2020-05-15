#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new QSettings("settings.ini", QSettings::IniFormat, this);
    this->move(settings->value("pos").toPoint());
    this->resize(settings->value("size").toSize());
    this->jsonPath = settings->value("path").toString();
    ui->lbl_path_to_json->setText(jsonPath);
    loadJson();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadJson()
{
    QFile fileJson(jsonPath);
    if(!fileJson.open(QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file!");
    }
    QJsonDocument jsonDoc(QJsonDocument::fromJson(fileJson.readAll()));
    QJsonObject jsonObj = jsonDoc.object();
    jsonArr = jsonObj["data"].toArray();

    curIdx= QRandomGenerator::global()->bounded(jsonArr.count());
    auto curObj = jsonArr.at(curIdx);
    ui->lbl_title->setText(curObj["title"].toString());
    ui->te_text->setPlainText(curObj["text"].toString().replace("\n","\n\n"));

    setWindowTitle(tr("Speaking - %1").arg(curObj["cat"].toString().toUpper()));
}


void MainWindow::on_pushButton_clicked()
{
    jsonPath = QFileDialog::getOpenFileName(
                this, "Open Json File", this->jsonPath, "Json File (*.json)");
    if(jsonPath.isEmpty()) return;

    ui->lbl_path_to_json->setText(jsonPath);
    loadJson();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    settings->setValue("pos", QVariant(this->pos()).toPoint());
    settings->setValue("size", QVariant(this->size()).toSize());
    settings->setValue("path", QVariant(this->jsonPath).toString());
}

void MainWindow::on_pushButton_2_clicked()
{
    loadJson();
}
