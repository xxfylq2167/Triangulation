#include "incircle.h"


InCircle::InCircle()
{
}

double InCircle::ComputeDetInCircle(QPointF fourthPoint,QPointF V1,QPointF V2,QPointF V3)
{
    determinant Det;
    int n = 4;
    double **det = new double*[n];
    for(int i=0; i<n; i++)
    {
      det[i]=new double[n];
    }

    det[0][0] = fourthPoint.x();
    det[0][1] = fourthPoint.y();
    det[0][2] = qPow(fourthPoint.x(),2)+ qPow(fourthPoint.y(),2);
    det[0][n-1] = 1;

    det[1][0] = V1.x();
    det[1][1] = V1.y();
    det[1][2] = qPow(V1.x(),2)+ qPow(V1.y(),2);
    det[1][n-1] = 1;

    det[2][0] = V2.x();
    det[2][1] = V2.y();
    det[2][2] = qPow(V2.x(),2)+ qPow(V2.y(),2);
    det[2][n-1] = 1;

    det[3][0] = V3.x();
    det[3][1] = V3.y();
    det[3][2] = qPow(V3.x(),2)+ qPow(V3.y(),2);
    det[3][n-1] = 1;

    return Det.ComputeDet(det,4);
}
