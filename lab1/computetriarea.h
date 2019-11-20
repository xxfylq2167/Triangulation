#ifndef COMPUTETRIAREA_H
#define COMPUTETRIAREA_H

#include <QPointF>
#include "determinant.h"
#include <QVector>
#include <QDebug>



class ComputeTriArea
{
public:
    ComputeTriArea();
    double ComputeTri_Area(QVector<QPointF> p);
    double ComputeDet(QVector<QPointF> p);

    determinant Det;
};

#endif // COMPUTETRIAREA_H
