#include "piece.h"

Piece::Piece(QWidget *parent) : QPushButton(parent)
{
    this->setEnabled(true);
    this->setCursor(QCursor(Qt::ArrowCursor));
    this->setAutoFillBackground(false);
    this->setFlat(true);
}

void Piece::init(Paintboard *paintboard,string Pos,int y,int x,string pieceName)
{
    this->paintboard = paintboard;
    if(pieceName.compare("0"))
    {
        QString ObjectName = QString::fromStdString(Pos);
        this->setObjectName(ObjectName);

        QRect Geometry(x*this->size,y*this->size,this->size,this->size);
        this->setGeometry(Geometry);

        this->transformStyleSheet(pieceName);
        this->setStyleSheet(this->StyleSheet);

        this->to.setX(x*this->size);
        this->to.setY(y*this->size);
    }
}

void Piece::showLastMove()
{
    if(this->from.x()!=this->to.x()||this->from.y()!=this->to.y())
        this->paintboard->uiLastMove(this->from,this->to);
}

void Piece::hidePiece()
{
    this->paintboard->uiHidePiece(this->to.y()/this->size,this->to.x()/this->size);
}
QString Piece::getPieceName()
{
    return this->paintboard->uiGetPieceName(this->to.y()/this->size,this->to.x()/this->size);
}
void Piece::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        this->raise(); //将此按钮移动到顶层显示
        this->pressPoint = event->pos();//mouse坐标
        QWidget::setCursor(QCursor(Qt::BlankCursor));
    }
}
void Piece::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        this->move(this->mapToParent(event->pos() - this->pressPoint));
        //防止按钮移出父窗口
        if(this->mapToParent(this->rect().topLeft()).x() <= 0)
        {
            this->move(0, this->pos().y());
        }
        if(this->mapToParent(this->rect().bottomRight()).x() >= this->parentWidget()->rect().width())
        {
            this->move(this->parentWidget()->rect().width() - this->width(), this->pos().y());
        }
        if(this->mapToParent(this->rect().topLeft()).y() <= 0)
        {
            this->move(this->pos().x(), 0);
        }
        if(this->mapToParent(this->rect().bottomRight()).y() >= this->parentWidget()->rect().height())
        {
            this->move(this->pos().x(), this->parentWidget()->rect().height() - this->height());
        }
    }
}
void Piece::mouseReleaseEvent(QMouseEvent *event)
{
    if(event)
        QWidget::setCursor(QCursor(Qt::ArrowCursor));
    int x = this->pos().x() + size/2;
    int y = this->pos().y() + size/2;
    this->checkStay(x,y);
}
void Piece::checkStay(int x,int y)
{
    int stay_x = 0, stay_y = 0;
    for(int i = 0 ; i < 8; i++)
    {
        if((i*this->size<x)&&((i+1)*this->size>=x))
        {
            stay_x=i*this->size;
            break;
        }
    }
    for(int i = 0; i < 8 ;i++)
    {
        if((i*this->size<y)&&((i+1)*this->size>=y))
        {
            stay_y=i*this->size;
            break;
        }
    }
    QPoint stayPos(stay_x,stay_y);//固定落子

    //以下为提子等规则 标出最后一步
    if(this->to.x() == stayPos.x()&&this->to.y() == stayPos.y())
    {
        this->move(stayPos);
        return;
    }
    this->from=this->to;//记录前移动point（60，60）
    this->to=stayPos;//记录后移动point
    this->hidePiece();//根据坐标名称提子

    this->setObjectName(this->getPieceName());//改变棋子坐标为名称
    this->move(stayPos);
    this->showLastMove();//拜登：我标记了一处地点（指框出最后一步）
    this->paintboard->repaint();//更新一下棋盘显示

    //此时this->board64没变 变一下
    this->paintboard->fen.afterMyMove(this->from.x()/size,this->from.y()/size,this->to.x()/size,this->to.y()/size);
    //显示一下board64 (del)
    //this->paintboard->printFen();

    string fishWalk = this->paintboard->fishMove();

    if(!fishWalk.compare("end"))
        return;
    if(!fishWalk.compare("stockfishFile.txt not found"))
        return;
    if(!fishWalk.compare(this->paintboard->lastFishMove))//检查引擎是否停止思考
    {
        this->paintboard->showStr("end");
        return;
    }
    else this->paintboard->lastFishMove = fishWalk;

    //接下来是电脑走棋 读取 四个字符a1a2
    this->paintboard->uiMovePiece(fishWalk);


    //显示一下board64 (del)
    this->paintboard->printFen();
}

void Piece::transformStyleSheet(string pieceName)
{
    if(!pieceName.compare("0")){this->StyleSheet = "0";return;}

    if(!pieceName.compare("p")){this->StyleSheet = "border-image: url(:/chess_pieces/bP.png);background:transparent;";return;}
    if(!pieceName.compare("P")){this->StyleSheet = "border-image: url(:/chess_pieces/wP.png);background:transparent;";return;}

    if(!pieceName.compare("r")){this->StyleSheet = "border-image: url(:/chess_pieces/bR.png);background:transparent;";return;}
    if(!pieceName.compare("R")){this->StyleSheet = "border-image: url(:/chess_pieces/wR.png);background:transparent;";return;}

    if(!pieceName.compare("n")){this->StyleSheet = "border-image: url(:/chess_pieces/bN.png);background:transparent;";return;}
    if(!pieceName.compare("N")){this->StyleSheet = "border-image: url(:/chess_pieces/wN.png);background:transparent;";return;}

    if(!pieceName.compare("b")){this->StyleSheet = "border-image: url(:/chess_pieces/bB.png);background:transparent;";return;}
    if(!pieceName.compare("B")){this->StyleSheet = "border-image: url(:/chess_pieces/wB.png);background:transparent;";return;}

    if(!pieceName.compare("q")){this->StyleSheet = "border-image: url(:/chess_pieces/bQ.png);background:transparent;";return;}
    if(!pieceName.compare("Q")){this->StyleSheet = "border-image: url(:/chess_pieces/wQ.png);background:transparent;";return;}

    if(!pieceName.compare("k")){this->StyleSheet = "border-image: url(:/chess_pieces/bK.png);background:transparent;";return;}
    if(!pieceName.compare("K")){this->StyleSheet = "border-image: url(:/chess_pieces/wK.png);background:transparent;";return;}
}
