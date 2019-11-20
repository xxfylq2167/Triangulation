#ifndef COMPUTETRICENTER_H
#define COMPUTETRICENTER_H

#include <QPointF>
#include <QVector>
#include <QDebug>


class ComputeTriCenter
{
public:
    ComputeTriCenter();
    QPointF ComputeTri_Center(QVector<QPointF> p);
};

#endif // COMPUTETRICENTER_H
