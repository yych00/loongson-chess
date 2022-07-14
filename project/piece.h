#ifndef PIECE_H
#define PIECE_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>

#include <paintboard.h>

#include <iostream>

using namespace std;

class Paintboard;

class Piece : public QPushButton
{
    Q_OBJECT
public:
    explicit Piece(QWidget *parent = nullptr);
    void init(Paintboard *paintboard,string Pos,int y,int x,string pieceCharName);//初始化棋子 0 P p Q k
    void showLastMove();//显示最近下的一步棋(坐标)
    void hidePiece();//检查要不要吃子 放在this->to之后
    QString getPieceName();//pieceName eg e1 b4
    string pieceCharName = "0";
    QPoint from,to;//从from移动到to
signals:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    int size = 60;
    QPoint pressPoint;//点击时鼠标坐标
    void checkStay(int x,int y);//拖动时落子位置
    QString StyleSheet;
    void transformStyleSheet(string pieceName);
    Paintboard *paintboard;
};

#endif // PIECE_H
