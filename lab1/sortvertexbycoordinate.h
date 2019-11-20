#ifndef SORTVERTEXBYCOORDINATE_H
#define SORTVERTEXBYCOORDINATE_H

#include <QPointF>
#include <QVector>
#include <qmath.h>

class SortVertexbyCoordinate
{
public:
    SortVertexbyCoordinate();

    QVector<QPointF> SortPoints(QVector<QPointF> Points);
    int sortSign;
};

#endif // SORTVERTEXBYCOORDINATE_H
