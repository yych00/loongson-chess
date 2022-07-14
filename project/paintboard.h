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
#include <fstream>
#include <string>
#include <QTimer>
#include <QEventLoop>

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
    void uiMovePiece(string move, bool arm = 0);
    QString uiGetPieceName(int y,int x);//(0,2) e4
    //ui文字栏this->ui->fishText->setPlainText(" ");
    void showStr(string str);

    void initEngine();   //初始化引擎
    void initCmdLine();  //初始化命令行行棋
    string fishMove();

    void printFen();

    Fen fen;//"rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR"

    string lastFishMove = "wula";// + piece 检查引擎是否停止思考

    QProcess *engine;
    QProcess *copyCmdLine; //io
    QProcess *cmdLine;     //io
    QProcess *photo;       //io
    QTimer *timer;         //io
    QProcess *arm;         //io
signals:
    void backToMenu();
private slots:
    void cmdMove();
private slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    int size = 60; //grid size 小格子大小
    Ui::Paintboard *ui;
    Piece *tempPiece;
};
#endif // PAINTBOARD_H

