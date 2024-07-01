#include "mainwindow.h"  // 创建项目自带
#include <QPainter>  // qt 画图工具
#include <QDebug>  // qt 打印日志
#include <QMenuBar>  // qt菜单栏
#include <QMenu>  // qt 每一个菜单项，跟QMenuBar关联
#include <QAction>  // qt 这里用于菜单栏下拉选项，相当于按钮
#include <QBrush>  // qt 刷子，画棋子用
#include <QPoint>  // qt 点
#include <QMouseEvent>  // qt鼠标事件
#include <QSound>  // qt 播放声音, 需要在.pro文件中，引入 QT += multimedia
#include <QMessageBox>  // qt 消息盒子

// 定义全局常量
#define CHESS_DOWN_SOUND ":/res/sound/chessdown.wav"
#define WIN_SOUND ":/res/sound/win.wav"
#define LOSE_SOUND ":/res/sound/lose.wav"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)  // 创建项目自带
{
    // 步骤1：设置棋盘大小
    // kBoardSizeNum 在GameModel里，表示每行每列有多少格
    // 长=边缘空隙*2 + 每个小格边长+格子数
    // 宽=边缘空隙*2 + 每个小格边长*格子数

    // 窗口宽度 = 边缘空隙*2 + 棋盘单元格宽度*单元格数量（单元格正方形，因此用宽度就可以了）
    int windowWidth = chessboardMargin * 2 + chessboardCellWidth * chessboardCellNum;
    int windowHeight = windowWidth;  // 因为棋盘长宽一样，上下左右空隙也一样
    // 参数单位是像素 px
    setFixedSize(windowWidth, windowHeight);

    // 开启鼠标追踪，跟 mouseMoveEvent()方法相关。不开的话，只有鼠标点击后，再拖拽，才会触发 mouseMoveEvent()事件
    setMouseTracking(true);

    // 导入Qdebug,自动就可以用qDebug()对象
    // 不用endl,用了endl会多打印一行？
    qDebug() << "窗口长，宽 = " << windowWidth << ", " << windowHeight << endl;

    // 步骤2：画菜单栏
    // 菜单栏会占用窗口的空间。比如画棋盘，棋盘边界40px，纯文字的菜单栏会占用一部分，估计30px
    // 菜单栏，名字叫“开始游戏”。“->”主要用于通过指针访问对象的成员，而“.”用于直接通过对象访问其成员。
    // menuBar() 默认是存在的，只要#include <QMenuBar> 就可以直接使用这个对象
    QMenu *startGameMenu = menuBar() -> addMenu(tr("开始游戏"));
    // QAction 在这里租用：菜单栏下拉框，this -> mainWindow。 new QAction里的this,是要把Action和一个东西绑定
    // 人和人对战模式
    QAction *actionPVP = new QAction("Person VS Person", this);
//    connect(actionPVP, SIGNAL(triggered()), this, SLOT(initPVPGame()));
    // 菜单 添加 下拉选项
    startGameMenu->addAction(actionPVP);

    // 菜单 添加 下拉选项：人机模式 ： person vs enviroment
    QAction *actionPVE = new QAction("Person VS Enviroment", this);
    startGameMenu->addAction(actionPVE);

    // 步骤3：画游戏内容
    drawGame();
}


// 析构函数
MainWindow::~MainWindow()  // 创建项目自带
{
    // 清除内存,为什么只删除 游戏逻辑的指针？
    // 析构函数什么时候调用？下面代码怎么用？是否会影响棋子的绘制？
    qDebug() << "进入mainwindow 析构函数";
    if(game)
    {
        qDebug() << "mainwindow 析构函数，检测到 game 内存未删除,进行删除内存操作";
        delete game;  // 删除指针内存？
        game = nullptr; // 设置为空指针
    }
}


// 初始化游戏，-》默认人和人 打游戏
void MainWindow::drawGame()
{
    // 创建游戏逻辑对象
    // 这里自动初始化了
    game = new GameModel;  // 这里GameModel为什么不加（）？
    drawPVPGame();
}


// 初始化游戏 -》人 VS 人
void MainWindow::drawPVPGame()
{
    game->gameType = PERSON;
    game->gameStatus = PLAYING;
//    update();  // 重绘操作，调用paintEvent()?
}


// @override 重写父类QWidget -> paintEvent() 方法，实现绘制棋盘
// 因为paintEvent()是绘图方法，只要某些操作调用update()方法，就会调用此方法
// 因此这个方法里，不能写 菜单栏 QMenuBar相关操作，会产生一堆菜单。我刚开始写qt，就在里面写了菜单栏操作，==！
// 目前主要知道，经常会被调用很多次，尤其是窗口内容有变化的时候
// 思路：可以把棋子坐标，棋子黑、白、空状态放到一个二维矩阵中，每次更新就遍历有变化的内容即可，纯数字操作，就重绘有变化的棋子即可
// paintEvent()是在MainWindow 的构造函数结束后，调用？ - 好像不是，具体再问问网络 + 自己测试
void MainWindow::paintEvent(QPaintEvent *event)
{
//    qDebug() << "执行了一次 paintEvent()"<< endl;  // 加了endl会打印多一空行？
//    qDebug() << "执行了一次 paintEvent()";  // 加了endl会打印多一空行？

    // 打印每个棋子状态
//    for(int i=0; i<chessboardCellNum + 1;i++)
//    {
//        for(int j=0;j<chessboardCellNum + 1;j++)
//        {
//            if(game->boardVector[i][j] != 0)
//            {
//                qDebug() << "mainwindow.cpp, row,col = "<<i << " "<< j << " 每个棋子状态" << game->boardVector[i][j];
//            }
//        }
//    }
    try {
        // 步骤0：初始化
        // 必须加this,要将绘的图绑定到当前对象上。如果不加this,如果要绑定对象，还要调用 QPainter::begin()方法
        // QPainter 画棋盘用,表示画家对象
        QPainter painter(this);

        // qt 刷子，用于画棋子
        QBrush brush;
        // Solid ->实心； Pattern ->图案、样式的意思，这里表示：实心样式
        brush.setStyle(Qt::SolidPattern);

        // 步骤1：画棋盘
        // 先画横线，再画竖线，线的数量 = 单元格数量+1
        for(int i=0; i<chessboardCellNum + 1; i++)
        {
            // 循环画横线
            // drawLine(pointA_x, pointA_y, pointB_x, pointB_y) 需要给出线段2个点坐标，此处使用：4个int类型参数，代表2个点坐标
            // 横线2点 的y坐标是相等的
            // pointA_x = 棋盘边缘空隙(固定不变)
            // pointA_y = 棋盘边缘空隙 + 棋盘单元格高度* 单元格序号
            // pointB_x = 棋盘边缘空隙 + 棋盘单元格宽度* 单元格总数 (固定不变) = 窗口宽度 - 棋盘边缘空隙
            // pointB_y = point_y
            int pointA_x = chessboardMargin;
            int pointA_y = chessboardMargin + chessboardCellHeight * i;
            int pointB_x = chessboardMargin + chessboardCellHeight * chessboardCellNum;
            int pointB_y = pointA_y;
            painter.drawLine(pointA_x, pointA_y, pointB_x, pointB_y);

            // 循环画竖线
            // drawLine(pointA_x, pointA_y, pointB_x, pointB_y) 需要给出线段2个点坐标，此处使用：4个int类型参数，代表2个点坐标
            // 竖线2点 的x坐标是相等的
            // pointA_x = 棋盘边缘空隙 + 棋盘单元格宽度* 单元格序号
            // pointA_y = 棋盘边缘空隙  (固定不变)
            // pointB_x = pointA_x;
            // pointB_y = 棋盘边缘空隙 + 棋盘单元格高度* 单元格总数 (固定不变) = 窗口宽度 - 棋盘边缘空隙
            pointA_x = chessboardMargin + chessboardCellWidth * i;
            pointA_y = chessboardMargin;
            pointB_x = pointA_x;
            pointB_y = chessboardMargin + chessboardCellHeight * chessboardCellNum;
            painter.drawLine(pointA_x, pointA_y, pointB_x, pointB_y);
        }

        // 步骤2：画落子标记
        // 绘制落子标记(防止鼠标出框越界)
        if (game->clickPosRow >= 0 && game->clickPosRow <= chessboardCellNum + 1 &&
            game->clickPosCol >= 0 && game->clickPosCol <= chessboardCellNum + 1 &&
            game->boardVector[game->clickPosRow][game->clickPosCol] == 0)
        {
            // 五子棋规则：黑方先手。程序中默认黑方=true
            if (game->playerFlag)
                brush.setColor(Qt::black);
            else
                brush.setColor(Qt::white);
            painter.setBrush(brush);
//            qDebug() << "落子标记，刷子颜色=" << brush.color() << " 黑白标志= " << game->playerFlag;
            // 在网格交汇点，画一个4px左右的矩形
            // 参数(左上角坐标x y，矩形宽度，矩形高度)
            // rectX = centerX - 矩形宽度/2 = margin + cellWidth * clickPosCol - 矩形宽度/2
            // rectY = centerY - 矩形高度/2 = margin + cellHeight * clickPosRow - 矩形宽度/2
            int rectX = chessboardMargin + chessboardCellWidth * game->clickPosCol - markSize / 2;
            int rectY = chessboardMargin + chessboardCellHeight * game->clickPosRow - markSize / 2;
            painter.drawRect(rectX, rectY, markSize, markSize);
        }

        // 步骤3：算法1）遍历棋盘225个点的状态（黑、白、空），绘制棋盘上所有棋子
        // 目前算法1）每下一个棋子，就遍历225个点状态（黑、白、空），然后绘制棋盘上所有棋子
        // 画一个棋子
        // Ellipse -> 椭圆，该方法可以画圆，也可以画椭圆，都是一个方法
        // drawEllipse（圆心x坐标，圆心y坐标，宽度-圆的直径，高度-圆的直径）
        // centerX = margin + cellWidth * j
        // centerY = margin + cellHeight * i
        int centerX = chessboardMargin + chessboardCellWidth * 0; // 中心点x坐标
        int centerY = chessboardMargin + chessboardCellHeight * 0; // 中心点y坐标
        int radiusWidth = chessRadius ; // 棋子半径宽度
        int radiusHeight = chessRadius ; // 棋子半径高度

        // 这个方法是按照椭圆所在矩形 -》 左上角坐标来绘制的，width -》 椭圆半径宽，height -> 椭圆半径高
        for(int i=0; i < chessboardCellNum + 1; i++)
        {
            // 二维向量不支持qDebug()打印，会引起程序异常退出
            for(int j=0; j < chessboardCellNum + 1; j++)
            {
                // 状态1 -> 黑子； -1 -> 白子； 0 -> 没子
                // 如果棋子状态是 空
                if(game->boardVector[i][j] == 0)
                {
    //                qDebug() << "棋子状态=0=空";
                    continue;
                }
//                qDebug() << "[i][j]=" << i <<"," << j << " ,boardVector[i][j] = " << game->boardVector[i][j];
                // 如果状态是1 = 黑子
                if(game->boardVector[i][j] == 1)
                {
                    brush.setColor(Qt::black);
//                    qDebug() << "该位置棋子状态="<<game->boardVector[i][j] << ",应该放黑子";
                }else if(game->boardVector[i][j] == -1)
                {
                    brush.setColor(Qt::white);
//                    qDebug() << "该位置棋子状态="<<game->boardVector[i][j] << ",应该放白子";
                }

                // 这个方法是按照椭圆所在矩形 -》 左上角坐标来绘制的，width -》 椭圆半径宽，height -> 椭圆半径高
                // centerX = margin + cellWidth * j
                // centerY = margin + cellHeight * i
                centerX = chessboardMargin + chessboardCellWidth * j; // 中心点x坐标
                centerY = chessboardMargin + chessboardCellHeight * i; // 中心点y坐标
    //            qDebug() << "centerX=" << centerX << " centerY= " << centerY;
                // 将刷子 与 画家 关联
                painter.setBrush(brush);
                painter.drawEllipse(QPoint(centerX, centerY), radiusWidth, radiusHeight);
                // 这个方法是按照椭圆所在矩形 -》 左上角坐标来绘制的,width -》矩形宽,height -> 矩形高
                // painter.drawEllipse(centerX, centerY, width, height);
            }
        } // 画棋子end

        // 步骤4：判断输赢
        if(game->isWinFlag)
        {
            QString winnerStr = "黑方";
            if(game->whoWin == -1)
            {
                winnerStr = "白方";
            }
            painter.end();  // 关闭画笔，否则报错 （这个错，是点击QMessageBox触发的）QBackingStore::endPaint() called with active painter; did you forget to destroy it or call QPainter::end() on it?
            QMessageBox::StandardButton btnWinValue = QMessageBox::information(this, "title恭喜", winnerStr + "win!");
            if(btnWinValue == QMessageBox::Ok)
            {
                // 重置
//                btnWinValue.close();
                qDebug() << "点击了OK";
                // 重置一些游戏内容，可以封装成一个方法。暂时没实现
                game->resetGame();  // 重置游戏,重置逻辑有问题
            }
        }else
        {
            // 步骤5：判断平局
            if(game->isDrawFlag)
            {
                painter.end();
                QMessageBox::StandardButton btnDrawValue = QMessageBox::information(this, "title和棋", "和棋");
                if(btnDrawValue == QMessageBox::Ok)
                {
                    // 重置
//                    btnDrawValue.close();
                    qDebug() << "点击了OK";
                    game->resetGame();  // 重置游戏,重置逻辑有问题
                }
            }
        }
    } catch (const std::exception &e){
        qDebug() << "mainwindow.cpp paintEvent() 出现异常，exception = " << e.what();
    }
}


// 重写父类方法，监听鼠标移动事件，目的：落子给个提示，画个小方框
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 通过鼠标的hover确定落子的标记
    int x = event->x();
    int y = event->y();
    
    // 给出鼠标位置，提示 最近的落子点
    // size()是窗口大小，不包括标题
    // 如果鼠标在棋盘 网格范围内
    if (x >= chessboardMargin &&
            x <= size().width() - chessboardMargin &&
            y >= chessboardMargin &&
            y <= size().height() - chessboardMargin
            )
    {
        // 根据鼠标移动的x,y坐标，更新 提示用户落子位置 的 row,col
        game->updateIndexByPos(x, y);
    }
    // 存了坐标后，也要重绘
    update();
}


// 重写父类方法，监听鼠标释放事件，目的：绘制落子
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "进入mouseReleseEvent.........";
    try {
        // 更新二维向量值，更新下棋方状态
        game->chessOneByPerson();  //调用封装方法。
        // 步骤3：播放下棋声音
        QSound::play(CHESS_DOWN_SOUND);
        // 步骤4：重绘
        update();
        // 步骤5：判断是否胜利，不放在paintEvent()，是因为经常会被调用
        bool isWinReturn = game->isWin(game->clickPosRow, game->clickPosCol);
//        qDebug() << "mainwindow.cpp,mouseReleaseEvent(), isWin返回 = "<< isWinReturn;

        // 步骤6：判断是否和棋，不放在paintEvent()，是因为经常会被调用
        if(!game->isWinFlag)
        {
            game->isDraw();
        }
    } catch (const std::exception &e){
        qDebug() << "mainwindow.cpp,mouseReleaseEvent()出现异常，exception = " << e.what();
    }
}

