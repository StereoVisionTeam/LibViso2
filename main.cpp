#include "mainwindow.h"
#include <QApplication>
#include "libviso2/demo.h"
int main(int argc, char *argv[])
{
    argv[1]="/home/haskis/Pictures/Projects/LibViso2DataSet/2010_03_09_drive_0019";
    demo(2,argv);




    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
