#include "computecircumcirclecenter.h"

ComputeCircumcircleCenter::ComputeCircumcircleCenter()
{
}
/*
 *      |y1  z1  1 |           |x1  z1  1 |       |x1  y1  1 |
 *  P = |y2  z2  1 |        Q= |x2  z2  1 |   R=  |x2  y2  1 |
 *      |y3  z3  1 |           |x3  z3  1 |       |x3  y3  1 |
 *
 *  z1 = pow(x1,2) + pow(y1,2)
 *
 *  Xc = - 1/2*(P/R)
 *  Yc = - 1/2*(Q/R)
 */
QPointF ComputeCircumcircleCenter::ComputeCenterCircumcircle(QPointF P1,QPointF P2,QPointF P3)
{

    double P = ComputeDet_P(P1,P2,P3);
    double Q = ComputeDet_Q(P1,P2,P3);
    double R = ComputeDet_R(P1,P2,P3);

    double Xc = - (0.5) *(P/R);
    double Yc = - (0.5) *(Q/R);

    QPointF Pc(Xc,Yc);
    return Pc;
}

double ComputeCircumcircleCenter::ComputeDet_P(QPointF P1,QPointF P2,QPointF P3)
{

    determinant Det;

    int n = 3;
    double **det1 = new double*[n];
    for(int i=0; i<n; i++)
    {
      det1[i]=new double[n];
    }

    det1[0][0] = P1.y();
    det1[0][1] = qPow(P1.x(),2)+ qPow(P1.y(),2);
    det1[0][n-1] = 1;

    det1[1][0] = P2.y();
    det1[1][1] = qPow(P2.x(),2)+ qPow(P2.y(),2);
    det1[1][n-1] = 1;

    det1[2][0] = P3.y();
    det1[2][1] = qPow(P3.x(),2)+ qPow(P3.y(),2);
    det1[2][n-1] = 1;


    double result  = Det.ComputeDet(det1,n);

     for(int i=0;i<n-1;i++)
     {
         delete[] det1[i];
     }
     delete[] det1;

     return result;
}

double ComputeCircumcircleCenter::ComputeDet_Q(QPointF P1,QPointF P2,QPointF P3)
{

    determinant Det;

    int n = 3;
    double **det1 = new double*[n];
    for(int i=0; i<n; i++)
    {
      det1[i]=new double[n];
    }

    det1[0][0] = P1.x();
    det1[0][1] = qPow(P1.x(),2)+ qPow(P1.y(),2);
    det1[0][n-1] = 1;

    det1[1][0] = P2.x();
    det1[1][1] = qPow(P2.x(),2)+ qPow(P2.y(),2);
    det1[1][n-1] = 1;

    det1[2][0] = P3.x();
    det1[2][1] = qPow(P3.x(),2)+ qPow(P3.y(),2);
    det1[2][n-1] = 1;


    double result  = - Det.ComputeDet(det1,n);

     for(int i=0;i<n-1;i++)
     {
         delete[] det1[i];
     }
     delete[] det1;

     return result;
}


double ComputeCircumcircleCenter::ComputeDet_R(QPointF P1,QPointF P2,QPointF P3)
{

    determinant Det;

    int n = 3;
    double **det1 = new double*[n];
    for(int i=0; i<n; i++)
    {
      det1[i]=new double[n];
    }

    det1[0][0] = P1.x();
    det1[0][1] = P1.y();
    det1[0][n-1] = 1;

    det1[1][0] = P2.x();
    det1[1][1] = P2.y();
    det1[1][n-1] = 1;

    det1[2][0] = P3.x();
    det1[2][1] = P3.y();
    det1[2][n-1] = 1;


    double result  = Det.ComputeDet(det1,n);

     for(int i=0;i<n-1;i++)
     {
         delete[] det1[i];
     }
     delete[] det1;

     return result;
}
