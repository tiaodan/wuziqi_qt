// ---- 五子棋游戏模型类 ---- //
#ifndef GAMEMODEL_H  // 创建完文件自带，头文件保护
#define GAMEMODEL_H  // 创建完文件自带，头文件保护

// 导入向量，类似于数组。本程序2作用，存每个棋子状态；2）AI下棋时，存最优下棋点
#include <vector>


// 初始化
enum GameType
{
    PERSON,
    BOT
};

// 游戏状态
// DRAW -》平局结束
enum GameStatus
{
    PLAYING,
    WIN,
    LOSE,
    DRAW
};


class GameModel  // 创建完文件自带，类声明
{
public:  // 创建完文件自带，写 共有的属性 + 方法
    // int类型 二维向量。保存棋盘每个点位 状态
    std::vector<std::vector<int>> boardVector;

    GameModel();  // 创建完文件自带，构造函数


    GameType gameType;  // 游戏模式，人人，还是人机？
    GameStatus gameStatus;  // 游戏状态
    // 表示下棋方，1表示白-true, 0 表示黑-false
    bool playerFlag=true;  // new 游戏时，默认黑方先手
    bool isWinFlag = false;  // 是否有输赢标志,是否棋局结束了
    bool isDrawFlag = false;  // 是否和棋标志
    int whoWin = 0;  // 胜利者颜色：1=黑，0=无，-1=白
     // 提示的 落子位置 所在二维向量的 索引
    int clickPosRow, clickPosCol;

    void startGame(GameType type);  // 开始游戏
    void initGame();  // 初始化游戏
    void initPVPGame();  // 初始化游戏 - 人 VS 人 模式
    void initPVEGame();  // 初始化游戏 - 人 VS 机器 模式
    void chessOneByPerson();
    void updateIndexByPos(int x, int y);  // 根据x,y位置，更新落子提示位置的索引
    bool isWin(int row, int col);  // 判断输赢,参数行，列
    bool isDraw();  // 是否和棋
    void resetGame();  // 重置游戏状态
};

#endif // GAMEMODEL_H // 创建完文件自带，头文件保护
