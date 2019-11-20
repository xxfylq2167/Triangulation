#ifndef INCIRCLE_H
#define INCIRCLE_H

#include <QPointF>
#include "determinant.h"
#include <qmath.h>
#include <qdebug.h>


class InCircle
{
public:
    InCircle();
     double ComputeDetInCircle(QPointF fourthPoint,QPointF V1,QPointF V2,QPointF V3);
};

#endif // INCIRCLE_H
