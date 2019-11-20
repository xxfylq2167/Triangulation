#include "computetricenter.h"

ComputeTriCenter::ComputeTriCenter()
{
}


/******************************
/*Name: ComputeTri_Center
/*Author: Xufeng
/*Description: Calculate the center of triangle
/*Input: the coordiantes of three points
/*Output: the result center of gravity
/*Date: 2014-01-27
/*Version: 1.00
*****************************/
QPointF ComputeTriCenter::ComputeTri_Center(QVector<QPointF> p)
{

    double X=0,Y=0;
    for(int i=0; i<p.count();i++)
    {
        X=X+p[i].x();
        Y=Y+p[i].y();
    }

    X=X/3;
    Y=Y/3;

    return QPointF(X,Y);
}
