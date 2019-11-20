#include "sortvertexbycoordinate.h"

SortVertexbyCoordinate::SortVertexbyCoordinate()
{
}

bool isLessThan(const QPointF &P1, const QPointF &P2)
{


        if(P1.x() < P2.x())
        {
            return true;
        }
        else
        {
            return false;
        }

}

QVector<QPointF> SortVertexbyCoordinate::SortPoints(QVector<QPointF> Points)
{
    qSort(Points.begin(), Points.end(),isLessThan);
    return Points;
}
