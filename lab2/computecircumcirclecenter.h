#ifndef COMPUTECIRCUMCIRCLECENTER_H
#define COMPUTECIRCUMCIRCLECENTER_H

#include <QPointF>
#include "determinant.h"
#include "qmath.h"
class ComputeCircumcircleCenter
{
public:
    ComputeCircumcircleCenter();
    double ComputeDet_P(QPointF P1,QPointF P2,QPointF P3);
    double ComputeDet_Q(QPointF P1,QPointF P2,QPointF P3);
    double ComputeDet_R(QPointF P1,QPointF P2,QPointF P3);

    QPointF ComputeCenterCircumcircle(QPointF P1,QPointF P2,QPointF P3);
};

#endif // COMPUTECIRCUMCIRCLECENTER_H
