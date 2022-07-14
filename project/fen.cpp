#include "fen.h"
void Board64::getPos(char row, char line)
{
    this->pos[0] = row;
    this->pos[1] = line;
}
string Board64::showPos()
{
    string temp(pos,2);
    return temp;
}
void Board64::getPieceName(string pieceName)
{
    this->pieceName = pieceName;
}
string Board64::showPieceName()
{
    string temp = this->pieceName;
    return temp;
}


Fen::Fen()
{

}
Fen::~Fen()
{

}

void Fen::setBoardStr(string boardStr)
{
    this->boardStr = boardStr;
    this->setBoard64();
}
void Fen::setBoard64()
{
    char line[8], row[8];
    char firstline = '8', firstrow = 'a';
    for (int i = 0; i < 8; i++)
    {
        line[i] = firstline--;
        row[i] = firstrow++;
    }
    if (this->boardStr.length() != 64)
        cout << "error boardStr.length() " << this->boardStr.length() << endl;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            this->board64[i][j].getPos(row[j], line[i]);//a8
            int p = i * 8 + j;
            string tempStr = this->boardStr.substr(p, 1);
            this->board64[i][j].getPieceName(tempStr);
        }
}
string Fen::coordinateToStr(int y,int x)
{
    return this->board64[y][x].showPieceName();
}
void Fen::moveToboardStr(string move)
{
    string from = move.substr(0,2);
    string to = move.substr(2,2);
    string fromName;
    //接下来是坐标64里棋子种类替换
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
        {
            if(this->board64[i][j].showPos() == from)
            {
                fromName = this->board64[i][j].showPieceName();
                this->board64[i][j].getPieceName("0");
                break;
            }
        }
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
        {
            if(this->board64[i][j].showPos() == to)
            {
                this->board64[i][j].getPieceName(fromName);
                break;
            }
        }
    //接下来改变this->boardStr 其实char*换地址就可以这里重新从64中读取
    this->boardStr.clear();
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
        {
            this->boardStr.append(this->board64[i][j].showPieceName());
        }
}
void Fen::moveToboardStr(int x0,int y0,int x1,int y1)
{
    string tempName = this->board64[y0][x0].showPieceName();
    this->board64[y0][x0].getPieceName("0");
    this->board64[y1][x1].getPieceName(tempName);
    //接下来改变this->boardStr 其实char*换地址就可以这里重新从64中读取
    this->boardStr.clear();
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
        {
            this->boardStr.append(this->board64[i][j].showPieceName());
        }
}
void Fen::boardStrToChessFen()
{
    string boardStr =this->boardStr;
    string tempChessFen;
    for (int i = 8; i > 0; i--)
        boardStr.insert(i * 8, "/");
    int numof0 = 0;
    for (int j = 0; j < int(boardStr.length()); j++)
    {
        string tmpchar = boardStr.substr(j, 1);
        if (!tmpchar.compare("0"))
            numof0++;
        else
        {
            if (numof0 == 0)
                tempChessFen.append(tmpchar);
            else
            {
                tempChessFen.append(to_string(numof0));
                numof0 = 0;
                tempChessFen.append(tmpchar);
            }
        }
    }
    tempChessFen.pop_back();
    this->chessFen = tempChessFen;
}

QPoint Fen::strToCoordinate(string coordinate)//a8 ->(0,0)
{
    QPoint p;
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
        {
            if(coordinate == this->board64[j][i].showPos())
               {
                    p.setX(i);
                    p.setY(j);
                    return p;
               }
        }
    return p;
}

void Fen::movePiece(string move)
{
    cout << move << endl;
    //lastmove 四位有效
}

void Fen::afterMyMove(int x0, int y0, int x1, int y1)//(1,1,2,2)
{
    //电孬下的话就这样更新bd64
    //movePiece(string move);
    //string move;
    //this->moveToboardStr(move);


    this->moveToboardStr(x0, y0, x1, y1);
    this->boardStrToChessFen();
}

string Fen::showBoardStr()
{
    return this->boardStr;
}
string Fen::showChessFen()
{
    return this->chessFen;
}
