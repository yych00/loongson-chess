#include "paintboard.h"
#include "ui_paintboard.h"
#include "QDebug"
Paintboard::Paintboard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Paintboard)
{
    ui->setupUi(this);
    QPixmap pixmap;
    this->size = 60;
    pixmap = QPixmap(this->size*8, this->size*8);//画布大小
    QPainter painter(&pixmap);//画笔
    QColor color[2];
    color[0].setRgb(240, 217, 181);
    color[1].setRgb(181, 136, 99);
    for(int i = 0;i < 8; i++)
        for(int j = 0;j < 8; j++)
            painter.fillRect(this->size*i, this->size*j, this->size, this->size, color[(i+j)%2]);
    ui->chessbd->setPixmap(pixmap);
    ui->lastmove_0->hide();
    ui->lastmove_1->hide();
    // @_@
    string BoardStr ="rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR";
    this->fen.setBoardStr(BoardStr);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << i << " " << j << " ";
            cout << fen.board64[i][j].showPos() << " ";
            cout << fen.board64[i][j].showPieceName() << "     ";

            string Pos = fen.board64[i][j].showPos();
            string PieceName = fen.board64[i][j].showPieceName();
            if(PieceName.compare("0"))
            {
                tempPiece = new Piece(ui->chessboard);//新建一堆棋子
                tempPiece->init(this,Pos,i,j,PieceName);//初始化棋子
            }
        }
        cout << endl;
    }
    this->setAttribute(Qt::WA_DeleteOnClose);//delete

    //创建引擎
    this->initEngine();

    //init io photo ssh
    this->photo = new QProcess(this);
    this->photo->setWorkingDirectory(QDir::currentPath());
    this->photo->setStandardOutputFile("0photo.txt", QIODevice::WriteOnly);
    this->photo->start("python3 ssh.py\n");
    QEventLoop eventloop;
    QTimer::singleShot(5000, &eventloop, SLOT(quit()));
    eventloop.exec();
    this->photo->write("\n");
    this->photo->write("\n");
    this->photo->write("cd Desktop/loongson\n");
    this->photo->write("\n");
    this->photo->write("python3 chess_photo.py\n");
    this->photo->write("\n");

    //init io arm ssh
    this->arm = new QProcess(this);
    this->arm->setWorkingDirectory(QDir::currentPath());
    this->arm->setStandardOutputFile("0arm.txt", QIODevice::WriteOnly);
    this->arm->start("python3 ssh.py\n");
    QTimer::singleShot(5000, &eventloop, SLOT(quit()));
    eventloop.exec();
    this->arm->write("\n");
    this->arm->write("\n");
    this->arm->write("cd Desktop/loongson\n");
    this->arm->write("\n");

    //io 命令行行棋
    this->initCmdLine();
}

Paintboard::~Paintboard()
{
    delete ui;
}
void Paintboard::printFen()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << i << " " << j << " ";
            cout << fen.board64[i][j].showPos() << " ";
            cout << fen.board64[i][j].showPieceName() << "     ";
        }
        cout << endl;
    }
    cout << this->fen.showBoardStr() << endl;
    cout << this->fen.showChessFen() << endl;
    cout << endl;
}
void Paintboard::cmdMove()
{
    //input file
    string filename = "cmdLine.txt";
    ifstream fin;
    fin.open(filename);
    if (fin.is_open())
    {
        string lastLine;
        while (true)
            {
                getline(fin, lastLine);
                if (fin.peek() == EOF)
                    break;
            }
       if(lastLine.length() == 4)
       {
           string move = lastLine.substr(lastLine.length()-4,4);
           this->uiMovePiece(move);
           this->repaint();//更新一下棋盘显示
           string fishWalk = this->fishMove();
           if(!fishWalk.compare("end"))
               return;
           if(!fishWalk.compare("stockfishFile.txt not found"))
               return;
           if(!fishWalk.compare(this->lastFishMove))//检查引擎是否停止思考
           {
               this->showStr("end");
               return;
           }
           else this->lastFishMove = fishWalk;

           //接下来是电脑走棋 读取 四个字符a1a2
           this->uiMovePiece(fishWalk, 1);
       }
       fin.close();
       fstream file("cmdLine.txt", ios::out);
       file.close();
    }
}
void Paintboard::uiLastMove(QPoint from,QPoint to)
{
    this->ui->lastmove_0->move(from);
    this->ui->lastmove_1->move(to);
    this->ui->lastmove_0->show();
    this->ui->lastmove_1->show();
}
void Paintboard::uiHidePiece(QString PiecePos)
{
    Piece *tempMove = ui->chessboard->findChild<Piece *>(PiecePos);
    if(tempMove)
    {
         qDebug() << tempMove->objectName() << endl;
         tempMove->setObjectName("0");
         tempMove->hide();
    }
}
void Paintboard::uiHidePiece(int y,int x)
{
    string str = this->fen.board64[y][x].showPos();;//坐标名称
    QString PieceName = QString::fromStdString(str);
    Piece *tempMove = ui->chessboard->findChild<Piece *>(PieceName);
    if(tempMove)
    {
         tempMove->setObjectName("0");
         tempMove->hide();
    }
}
QString Paintboard::uiGetPieceName(int y,int x)
{
    string str = this->fen.board64[y][x].showPos();//坐标名称
    return QString::fromStdString(str);
}
void Paintboard::uiMovePiece(string move, bool arm)
{
    QString fromName = QString::fromStdString(move.substr(0,2));
    QString toName = QString::fromStdString(move.substr(2,2));
    //找到to棋子 hide io arm
    bool hide = 0;
    Piece *tempMoveTo = ui->chessboard->findChild<Piece *>(toName);
    if(tempMoveTo)
    {
         tempMoveTo->setObjectName("0");
         tempMoveTo->hide();
         hide = 1;
    }
    //移动to的坐标 pointTo
    QPoint pointTo = this->fen.strToCoordinate(move.substr(2,2));
    pointTo.setX(pointTo.x()*size);
    pointTo.setY(pointTo.y()*size);
    //找到from棋子 移动 改名
    Piece *tempMoveFrom = ui->chessboard->findChild<Piece *>(fromName);
    if(tempMoveFrom)
    {
         tempMoveFrom->from = tempMoveFrom->to;//记录前移动point（60，60）
         tempMoveFrom->to = pointTo;//记录后移动point
         tempMoveFrom->setObjectName(toName);
         tempMoveFrom->move(pointTo);
         tempMoveFrom->showLastMove();
         //此时this->board64没变 变一下 @
         this->fen.afterMyMove(tempMoveFrom->from.x()/size,tempMoveFrom->from.y()/size,tempMoveFrom->to.x()/size,tempMoveFrom->to.y()/size);
    }
    if(arm == 1) //io arm
    {
        if(hide == 1)
        {
            string armWalk = "python3 move.py ";
            armWalk.append(move.substr(2, 2));
            armWalk.append("\n");
            this->arm->write(armWalk.data());

            QEventLoop eventloop;
            QTimer::singleShot(6000, &eventloop, SLOT(quit()));
            eventloop.exec();
        }

        string armWalk = "python3 move.py ";
        armWalk.append(move);
        armWalk.append("\n");
        this->arm->write(armWalk.data());

        //QEventLoop eventloop;
        //QTimer::singleShot(6000, &eventloop, SLOT(quit()));
        //eventloop.exec();

        string armWalkDefault = "python3 move.py default\n";
        this->arm->write(armWalkDefault.data());
        QEventLoop eventloop;
        QTimer::singleShot(15000, &eventloop, SLOT(quit()));
        eventloop.exec();
        this->on_pushButton_clicked();
    }
}
void Paintboard::showStr(string str)
{
    this->ui->fishText->setPlainText(QString::fromStdString(str));
}
void Paintboard::initEngine()
{
    this->engine = new QProcess(this);
    engine->setWorkingDirectory(QDir::currentPath());
    engine->setStandardOutputFile("stockfishFile.txt", QIODevice::WriteOnly);
    engine->start("stockfish\n");
    this->engine->waitForStarted();
}
void Paintboard::initCmdLine()  //初始化命令行行棋
{
    fstream file("cmdLine.txt", ios::out);
    file.close();
    this->timer = new QTimer(this);
    this->connect(timer, SIGNAL(timeout()), this, SLOT(cmdMove()));
    timer->start(2000);
}
string Paintboard::fishMove()
{
    //添加局势 position fen
    string fenStr = "position fen ";
    fenStr.append(this->fen.showChessFen());
    fenStr.append(" ");
    fenStr.append(this->fen.player);
    //fenStr.append(" ");
    //fenStr.append(this->fen.KQkq);
    fenStr.append("\n");
    this->engine->write(fenStr.data());
    //计算 go movetime 114514
    string goMove = "go movetime ";
    goMove.append(this->fen.time);
    goMove.append("\n");
    this->engine->write(goMove.data());
    this->engine->waitForFinished(3000);             //  wait for shape```

    //output
    string filename = "stockfishFile.txt";
    ifstream fin;
    fin.open(filename);
    if (fin.is_open())
    {
       fin.seekg(-4, ios::end);                     // go to one spot before the EOF

       int keepLooping = 1917;
       while (keepLooping--) {
           char ch;
           fin.get(ch);                             // Get current byte's data
           if ((int)fin.tellg() <= 1) {             // If the data was at or before the 0th byte
               fin.seekg(0);                        // The first line is the last line
               keepLooping = false;                 // So stop there
           }
           else if (ch == '\n') {                   // If the data was a newline
               keepLooping = false;                 // Stop at the current position.
           }
           else {                                   // If the data was neither a newline nor at the 0 byte
               fin.seekg(-2, ios::cur);             // Move to the front of that data, then to the front of the data before it
           }
       }
       string lastLine;
       getline(fin, lastLine);                      // Read the current line

       this->ui->fishText->setPlainText(QString::fromStdString(lastLine));

       fin.close();


       //
       if(lastLine.substr(0,8).compare("bestmove")) //不是bestmove
       {
           this->ui->fishText->setPlainText("end");
           return "end";
       }
       string moves = lastLine.substr(9,4);         //  wait for shape```

       return moves;
    }
    this->ui->fishText->setPlainText("stockfishFile.txt not found");
    return "stockfishFile.txt not found";           //  wait for shape```
}

void Paintboard::on_pushButton_1_clicked()
{
    this->close();
}

void Paintboard::on_pushButton_2_clicked()
{
    this->backToMenu();
    this->close();
    delete this;
}

void Paintboard::on_pushButton_3_clicked() //行棋检测
{
    this->ui->fishText->setPlainText("行棋检测ing");
    this->photo->write("2\n");
    //6 seconds
    QEventLoop eventloop;
    QTimer::singleShot(6000, &eventloop, SLOT(quit()));
    eventloop.exec();
    this->photo->write("\n");
    this->ui->fishText->setPlainText("行棋检测已完成");
    this->copyCmdLine = new QProcess(this);
    copyCmdLine->setWorkingDirectory(QDir::currentPath());
    copyCmdLine->start("python3 cmdLine.py");
    QTimer::singleShot(1000, &eventloop, SLOT(quit()));
    eventloop.exec();
}

void Paintboard::on_pushButton_clicked() //棋盘标定
{
    this->ui->fishText->setPlainText("棋盘标定ing");
    this->photo->write("1\n");
    QEventLoop eventloop;
    QTimer::singleShot(6000, &eventloop, SLOT(quit()));
    eventloop.exec();
    this->ui->fishText->setPlainText("棋盘标定已完成");
    //6 seconds
}
