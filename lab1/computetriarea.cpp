#include "computetriarea.h"

ComputeTriArea::ComputeTriArea()
{
}

/******************************
//Name: ComputeTri_Area
//Author: Xufeng
//Description: Calculate the area of triangle
//Input:  array of the coordiantes of three points
//Output: the result of triangle area
//Date: 2014-01-23
//Version: 1.00
*****************************/
double ComputeTriArea::ComputeTri_Area(QVector<QPointF> p)
{
    double area = 0.5 * ComputeDet(p);
    return area;
}

/******************************
//Name: ComputeDet
//Author: Xufeng
//Description: Calculate the determinant
//Input: array of the coordiantes of three points
//Output: the result of determianant
//Date: 2014-01-23
//Version: 1.00
*****************************/
double ComputeTriArea::ComputeDet(QVector<QPointF> p)
{
    int n = 3;

    double **det = new double*[n];
    for(int i=0; i<n; i++)
    {
      det[i]=new DataType[n];
    }
    for(int i=0; i< n;i++)
    {
        det[i][0] = p[i].x();
        det[i][1] = p[i].y();
        det[i][n-1] = 1;
    }
    return Det.ComputeDet(det,3);
}
