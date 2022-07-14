#ifndef FEN_H
#define FEN_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QPoint>

using namespace std;

class Board64
{
public:
    void getPos(char line, char row);
    string showPos();//a1

    void getPieceName(string pieceName);
    string showPieceName();//rnbqkp RNBQKP 0
private:
    string pieceName = "0";//rnbqkp RNBQKP 0表示空白 lastmove表示最近一步
    char pos[2];//line abcdefgh  row 12345678
};
class Fen
{
public:
    Fen();
    ~Fen();
    void setBoardStr(string boardStr);//设置初始局面str
    void setBoard64();//设置初始局面

    string coordinateToStr(int y,int x);//not use
    QPoint strToCoordinate(string coordinate);

    //移动棋子 更新boardstr 更新chessfen
    void moveToboardStr(string move);//a1b1 -> bdstr
    void moveToboardStr(int x0,int y0,int x1,int y1);//(1,1,2,2) -> bdstr
    void boardStrToChessFen();//update this->chessfen
    //已知stringwalk
    //更新boardstr->输入chessfen->stringwalk->更新boardstr&&移动棋子name
    //移动棋子返回str->更新boardstr
    void movePiece(string move);//eg movePiece(c2b3);自动走棋
    void afterMyMove(int x0,int y0,int x1,int y1);//人走棋完执行的指令 updatebd64

    string showBoardStr();
    string showChessFen();

    Board64 board64[8][8];//就放在public里长生吧
    string player = "b";
    string KQkq = "KQkq";
    string time = "2828";
private:
    string boardStr;
    string chessFen;//可用于上传 .../8/8/8...
    string chessFenMoves = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves";// wait for shape 不知道升变怎么办
};

#endif // FEN_H
