#ifndef PAINTBOARD_H
#define PAINTBOARD_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QProcess>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <iostream>
using namespace std;

#include <piece.h>
#include <fen.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Paintboard; }
QT_END_NAMESPACE

class Piece;

class Paintboard : public QWidget
{
    Q_OBJECT
public:
    Paintboard(QWidget *parent = nullptr);
    ~Paintboard();

    //ui棋子移动处理
    void uiLastMove(QPoint from,QPoint to);
    void uiHidePiece(QString PieceName);//c3
    void uiHidePiece(int y,int x);//(0,2)
    void uiMovePiece(string move);
    QString uiGetPieceName(int y,int x);//(0,2) e4
    //ui文字栏this->ui->fishText->setPlainText(" ");
    void showStr(string str);
    //引擎处理
    void initEngine();
    string fishMove();

    void printFen();

    Fen fen;//"rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR"
    QProcess *engine;
    string lastFishMove = "wula";// + piece 检查引擎是否停止思考
signals:
    void backToMenu();
private slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
private:
    int size = 60; //grid size 小格子大小
    Ui::Paintboard *ui;
    Piece *tempPiece;
};
#endif // PAINTBOARD_H

