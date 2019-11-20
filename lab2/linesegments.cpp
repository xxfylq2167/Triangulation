#include "linesegments.h"

LineSegments::LineSegments()
{
}
LineSegments::LineSegments(QPointF pStart,QPointF pEnd)
{
    LineSegments_StartVertex = pStart;
    LineSegments_EndVertex = pEnd;
}
