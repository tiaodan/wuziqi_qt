// 游戏模型，里面是游戏逻辑
#include "gamemodel.h" // 创建完文件自带，导入头文件
#include "mainwindow.h"  // 游戏窗口类，为了使用 棋盘、棋子的变量
#include <QDebug>  // qt 打印日志，每个文件只要打日志，都要导入
#include <math.h>  // c++ STL 标准库，数学类

GameModel::GameModel()  // 创建完文件自带，构造函数
{
    try
    {
        // 别的地方只要1个地方 引入了 QDebug,所有地方都可以使用qDebug()对象
        qDebug() << "进入GameModel() 构造方法()" << endl;
        // 初始化 棋子 所在二维向量的状态
        // 步骤1：初始化
        // 初始化棋盘 每个下棋的点位状态，都改为0
        boardVector.clear();  // 清除所有点位状态，clear是删除所有，变成一个空向量
        // 初始化每个点位状态 = 0。 五子棋规则中黑棋先手。1-》黑子； 0-》空； -1 -》白子
        // 这里 chessboardCellNum 在mainwindow.h 定义的，不导入mainwindow.h,访问不到
        for(int i=0; i<chessboardCellNum + 1;i++)
        {
            std::vector<int> lineBoard;  // 棋盘单行 向量，类似于数组
            // 最开始的写法错误，导致宝座gamemodel.cpp 构造函数出现异常，exception =  std::bad_alloc
            // for(int j=0;i<chessboardCellNum + 1;j++)
            for(int j=0;j<chessboardCellNum + 1;j++)
            {
                lineBoard.push_back(0); // 默认全是空子，1-》黑子； -1-》白子
            }
            boardVector.push_back(lineBoard);
        }
        // 打印每个棋子状态
//        for(int i=0; i<chessboardCellNum + 1;i++)
//        {
//            for(int j=0;j<chessboardCellNum + 1;j++)
//            {
//               qDebug() << "每个棋子状态" << boardVector[i][j];
//            }
//        }
    } catch (const std::exception &e){
        qDebug() << "gamemodel.cpp 构造函数出现异常，exception = " << e.what();
    }
}


// 开始游戏
// 如果头文件声明了void, cpp文件也要用void。否则报错：C:\A_software\C_wokspace\c++\qt\wuziqi_qt\gamemodel.h:16: error: candidate is: void GameModel::startGame()
// void startGame();
//      ^
void GameModel::startGame(GameType type)
{

    // 目前还不知道这个方法有啥用，方法内没写内容
}


// 人下棋
void GameModel::chessOneByPerson()
{
    // 默认人 和 人下棋
    // 只有有效点击，才落子。并且该处状态为空
    if(clickPosRow >= 0 && clickPosCol >=0 && boardVector[clickPosRow][clickPosCol] == 0)
    {
        // 步骤1：获取离鼠标释放，最近的点（提示落子点）。作为绘制棋子的点。
        // 传递行、列索引。更新棋子向量状态
        qDebug() << "gamemodel.cpp, chessOneByPerson(),playerFlag= " << playerFlag;
        if(playerFlag)  // true = 黑方
        {
            boardVector[clickPosRow][clickPosCol] = 1;
        } else
        {
            boardVector[clickPosRow][clickPosCol] = -1;  // 白
        }
        // 步骤2: 换手
        playerFlag = !playerFlag;
        qDebug() << "gamemodel.cpp, 落子行列= " << clickPosRow << " ," << clickPosCol
                 << "向量值boardVector[row][col]=" << boardVector[clickPosRow][clickPosCol]
                 << "转换playerFlag= " << playerFlag;
    }
    // 打印每个位置状态
    // 打印每个棋子状态
    for(int i=0; i<chessboardCellNum + 1;i++)
    {
        for(int j=0;j<chessboardCellNum + 1;j++)
        {
            if(boardVector[i][j] != 0)
            {
//                qDebug() << "gamemodel.cpp, row,col = "<<i << " "<< j << " 每个棋子状态" << boardVector[i][j];
            }
        }
    }
}


// 根据鼠标移动的x,y坐标，更新 提示用户落子位置 的 row,col
void GameModel::updateIndexByPos(int x, int y)
{
    // 获取最近的左上角的点
    int row = (y - chessboardMargin) / chessboardCellHeight;
    int col = (x - chessboardMargin) / chessboardCellWidth;

    int leftTopPosX = chessboardMargin + chessboardCellWidth * col;  // 左上角x坐标
    int leftTopPosY = chessboardMargin + chessboardCellHeight * row;  // 左上角y坐标

    // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
    clickPosRow = -1; // 初始化最终的值
    clickPosCol = -1;
    int len = 0; // 计算完后取整就可以了

    // 确定一个误差在范围内的点，且只可能确定一个出来
    // 鼠标放到一个位置，它有左上、右上、左下、右下 4个点位可以提示用户
    // 左上
    len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
    if (len < kPosDelta)
    {
        clickPosRow = row;
        clickPosCol = col;
    }
    //
    len = sqrt((x - leftTopPosX - chessboardCellHeight) * (x - leftTopPosX - chessboardCellHeight) + (y - leftTopPosY) * (y - leftTopPosY));
    if (len < kPosDelta)
    {
        clickPosRow = row;
        clickPosCol = col + 1;
    }
    len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - chessboardCellHeight) * (y - leftTopPosY - chessboardCellHeight));
    if (len < kPosDelta)
    {
        clickPosRow = row + 1;
        clickPosCol = col;
    }
    len = sqrt((x - leftTopPosX - chessboardCellHeight) * (x - leftTopPosX - chessboardCellHeight) + (y - leftTopPosY - chessboardCellHeight) * (y - leftTopPosY - chessboardCellHeight));
    if (len < kPosDelta)
    {
        clickPosRow = row + 1;
        clickPosCol = col + 1;
    }
//        qDebug() << "监听到鼠标移动,返回落子所在的行，列= " << row <<"," << col;
}


// 判断输赢,参数一个点，所在的行、列索引.并更新输赢标志
bool GameModel::isWin(int row, int col)
{
    // 判断4个方向，横竖，左斜、右斜
    // 思路1：落子点，所在4个方向所有棋子，查看是否有五子连珠情况
    // 思路2：落子点，左右各延伸4个，这9个点范围内，查看是否有五子连珠情况
    // 用思路1

    // 步骤0：初始化
    // 判断落子点棋子颜色
    int chessColor = boardVector[row][col];
    int count = 1;  // 颜色相同个数,因为落的子算成1个
    if (chessColor == 0)
    {
        return false;  // 落子点是空，不处理
    }
//    qDebug() << "gamemodel.cpp,isWin(), 进入判断 上下左右是否有五子连珠？";
    // 步骤1：判断横 row
    for(int i=0;i<=15;i++)
    {
        if(boardVector[row][i] != chessColor)
        {
            continue;
        }
        if (i+4 <= chessboardCellNum +1 &&
            boardVector[row][i] == boardVector[row][i+1] &&
            boardVector[row][i] == boardVector[row][i+2] &&
            boardVector[row][i] == boardVector[row][i+3] &&
            boardVector[row][i] == boardVector[row][i+4])
        {
            isWinFlag = true;  // 更新输赢标志
            whoWin = chessColor;  // 更新胜者颜色
            return true;
        }
    }

    // 步骤2：判断竖 col
    for(int i=0;i<=15;i++)
    {
        if(boardVector[i][col] != chessColor)
        {
            continue;
        }
        if (i+4 <= chessboardCellNum +1 &&
            boardVector[i][col] == boardVector[i+1][col] &&
            boardVector[i][col] == boardVector[i+2][col] &&
            boardVector[i][col] == boardVector[i+3][col] &&
            boardVector[i][col] == boardVector[i+4][col])
        {
            isWinFlag = true;  // 更新输赢标志
            whoWin = chessColor;  // 更新胜者颜色
            return true;
        }
    }

    // 步骤3：判断右斜\这种方向。右上方找4个棋子，右下方找4个棋子
    // 右上循环判断4次,不判断自己
    for(int i=1;i<5;i++)
    {
        // 遇到边界或者不是相同颜色的棋子就break
        if(row-i < 0 || col-i < 0 || chessColor != boardVector[row-i][col-i])
        {
            break;
        }
        if(chessColor == boardVector[row-i][col-i])
        {
            count++;
        }
    }
    // 右下循环判断4次，不判断自己，将这几次累加
    // 遇到边界或者不是相同颜色的棋子就break
    for(int i=1;i<5;i++)
    {
        // 遇到边界或者不是相同颜色的棋子就break
        if(row+i < 0 || col+i < 0 || chessColor != boardVector[row+i][col+i])
        {
            break;
        }
        if(chessColor == boardVector[row+i][col+i])
        {
            count++;
        }
        if(count >=5)
        {
            isWinFlag = true;  // 更新输赢标志
            whoWin = chessColor;  // 更新胜者颜色
            return true;
        }
    }

    // 步骤4：判断左斜/ 这种方向。左下方找4个棋子，右上方找4个棋子，这个麻烦，可以自己画个图
    // 左下循环判断4次,不判断自己
    count = 1; // 重置计数
    for(int i=1;i<5;i++)
    {
        // 遇到边界或者不是相同颜色的棋子就break
        if(row+i < 0 || col-i < 0 || chessColor != boardVector[row+i][col-i])
        {
            break;
        }
        if(chessColor == boardVector[row+i][col-i])
        {
            count++;
        }
    }
    // 右上循环判断4次，不判断自己，将这几次累加
    // 遇到边界或者不是相同颜色的棋子就break
    for(int i=1;i<5;i++)
    {
        // 遇到边界或者不是相同颜色的棋子就break
        if(row-i < 0 || col+i < 0 || chessColor != boardVector[row-i][col+i])
        {
            break;
        }
        if(chessColor == boardVector[row-i][col+i])
        {
            count++;
        }
        if(count >=5)
        {
            isWinFlag = true;  // 更新输赢标志
            whoWin = chessColor;  // 更新胜者颜色
            return true;
        }
    }

    return false;  // 没有输赢
}


// 判断是否和棋
bool GameModel::isDraw()
{
    // 情况1：没有可落子的点位，即每个点位都有棋子
    for(int i=0;i<chessboardCellNum+1;i++)
    {
        for(int j=0;j<chessboardCellNum+1;j++)
        {
            if(boardVector[i][j] == 0)
            {
                return false;
            }
        }
    }
    isDrawFlag = true;  // 修改是否和棋标志位
    return true; // 无落子点，返回true
}


// 重置游戏状态
void GameModel::resetGame()
{
    qDebug() << "重置游戏";
    boardVector.clear();  // 清空棋子数据
    // 这块size()确实变成0了，肯定是哪里有错误
    qDebug() << "gamemodel.cpp,resetGame()，重置后，向量大小=" << boardVector.size();

    // 打印每个棋子状态
    for(int i=0; i<chessboardCellNum + 1;i++)
    {
        for(int j=0;j<chessboardCellNum + 1;j++)
        {
           if(boardVector[i][j] != 0)
           {
               qDebug() << "gamemodel.cpp,resetGame(),游戏重置后，非0的棋子状态,row,col="<<i <<" "<<j << "" << boardVector[i][j];
               break;
           }
        }
    }
    // 初始棋盘
    for (int i = 0; i < chessboardCellNum; i++)
    {
        std::vector<int> lineBoard;
        for (int j = 0; j < chessboardCellNum; j++)
            lineBoard.push_back(0);
        boardVector.push_back(lineBoard);
    }

    playerFlag = true;  // 下棋方
    isWinFlag = false;  // 输赢标志
    isDrawFlag = false; // 和棋标志
    whoWin = 0;  // 胜方 int
    clickPosRow = 0;  // 落子行 索引
    clickPosCol = 0;  // 落子列 索引
    gameType = PERSON; // 游戏类型，还是人人
    gameStatus = WIN;  // 游戏状态
}
