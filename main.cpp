#include "mainwindow.h"  // 创建项目自带，引入头文件
#include <QApplication>  // 创建项目自带

int main(int argc, char *argv[])  // 创建项目自带
{
    QApplication a(argc, argv);  // 创建项目自带
    MainWindow w; // 创建项目自带
    w.show();  // 创建项目自带

    return a.exec();  // 创建项目自带
}
