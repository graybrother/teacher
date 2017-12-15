#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <iostream>
#include <string>

#include "opencv.hpp"
#include "face.hpp"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_lkvb2_pushButton_clicked();

    void on_openvideo_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString filename;
    QString s;
    cv::Mat image;
    cv::Mat result;
};

#endif // MAINWINDOW_H
