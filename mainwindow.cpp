#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <QDebug>
#include <string>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}
MainWindow::~MainWindow()
{
    delete ui;
}
