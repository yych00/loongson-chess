#include "init.h"
int size = 60;
QPoint stayPoint[8][8] ={};
void init()
{
    for (int i=0;i<8;i++)
        for(int j=0;j<8;j++)
        {
            stayPoint[i][j].setX(i*size);
            stayPoint[i][j].setY(j*size);
        }
}

