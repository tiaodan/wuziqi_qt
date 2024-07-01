#ifndef MAINWINDOW_H  // 创建项目自带
#define MAINWINDOW_H  // 创建项目自带

#include <QMainWindow>  // 创建项目自带
#include "gamemodel.h"  // 引入游戏逻辑类


// 变量初始化 - 因为这些常量，可能被其他文件引用，所以放入头文件中
const int chessboardCellNum = 15;  // 五子棋棋盘，常见就是15个单元格 *15
const int chessboardMargin = 40;  // 棋盘边缘空隙，用于上下左右空隙，原来是30，但是挨顶部菜单栏太近了，先改成40
const int chessboardCellWidth = 40;  // 棋盘每个单元的大小（正方形）
const int chessboardCellHeight = 40;  // 棋盘每个单元的大小（正方形）,写这个变量是为了，易于理解
const int chessRadius = 15;  // 棋子半径
const int markSize = 6; // 落子标记边长
const int kPosDelta = 20; // 鼠标点击的模糊距离上限


class MainWindow : public QMainWindow  // 创建项目自带
{
    Q_OBJECT  // 创建项目自带

public:  // 创建项目自带
    MainWindow(QWidget *parent = 0);  // 创建项目自带
    ~MainWindow();  // 创建项目自带

/*
    // 变量初始化 - 因为这些常量，可能被其他文件引用，所以放入头文件中
    // 如果这些常量写在类里，只能通过类的对象调用。报错：'chessboardMargin' was not declared in this scope
    // 如果这些常量写在类里，其他类 #include "mainwindow.h"头文件后，访问不到这些常量。
    // 因此这些常量要放在类外面
    const int chessboardCellNum = 15;  // 五子棋棋盘，常见就是15个单元格 *15
    const int chessboardMargin = 40;  // 棋盘边缘空隙，用于上下左右空隙，原来是30，但是挨顶部菜单栏太近了，先改成40
    const int chessboardCellWidth = 40;  // 棋盘每个单元的大小（正方形）
    const int chessboardCellHeight = 40;  // 棋盘每个单元的大小（正方形）,写这个变量是为了，易于理解
    const int chessRadius = 15;  // 棋子半径
*/
    GameModel *game; // 游戏模型类，-》游戏逻辑类

    // 五子棋规则中，一般是黑棋先手。因此黑棋用1表示

    // @override 重写父类绘制方法 paintEvent()
    void paintEvent(QPaintEvent *event);
    // 监听鼠标移动事件,重写父类方法？目的：画一个落子的提示方框
    void mouseMoveEvent(QMouseEvent *event);
    // 监听鼠标释放事件，重写父类方法？目的：获取鼠标点击所在 位置 二维向量索引
    void mouseReleaseEvent(QMouseEvent *event);

    // 画游戏内容
    void drawGame();
    void drawPVPGame();
    void drawPVEGame();


};

#endif // MAINWINDOW_H  // 创建项目自带
