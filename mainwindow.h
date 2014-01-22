#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <core.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
