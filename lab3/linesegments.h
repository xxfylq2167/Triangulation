#ifndef LINESEGMENTS_H
#define LINESEGMENTS_H

#include <QPointF>
class LineSegments
{
public:
    LineSegments();
    LineSegments(QPointF pStart,QPointF pEnd);
    QPointF  LineSegments_StartVertex;
    QPointF  LineSegments_EndVertex;
};

#endif // LINESEGMENTS_H
