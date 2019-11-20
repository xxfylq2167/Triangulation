#ifndef INTERSECTIONSEGMENTS_H
#define INTERSECTIONSEGMENTS_H


#define PI 3.1415926

#include <QPointF>
#include <QVector>
#include <QDebug>
#include <math.h>
#include <qmath.h>
#include <QRectF>

#include "sortvertexbycoordinate.h"
#include "computetriarea.h"
#include "linesegments.h"

class IntersectionSegments
{
public:
    IntersectionSegments();
    bool IntersectSegmentsArea(QPointF Line1_P_A,QPointF Line1_P_B,QPointF Line2_P_C,QPointF Line2_P_D);//judge the intersection of two segments
    bool PointInPolygon (QPointF pDetermined, QVector<QPointF> pointsPolygon,QPointF pOutPolygon); //if point is in the polygon
    double Determinant2(double A11, double A12, double B12, double B22);
    bool IntersectSegmentsCramer(QPointF Line1_A, QPointF Line1_B, QPointF Line2_C, QPointF Line2_D);

    QVector<LineSegments> Intersections_of_EachHatchLine(
            QPointF pHatchA_onY, QPointF pHatchB_onX, QVector<QPointF> pointsPolygon);


    QVector<LineSegments> CalculateAllHatchLine( QVector<QPointF> pointsPolygon,
            double alpha, double distanceHatchLine,QRectF minRectofPolygon);

    int intersectionNumber;//record all the section of one segment and polygon

    //record if one vertex is on the anther segment, that is, they have one common vertex,
    //or they inetrsect and the intersection is on the one of segment
    int commonIntersection;
    double Line1_Lambda; // if intersect, record Lambda of Line1 for calculate the intersection
    double Line2_Mu;    // if intersect, record Mu of Line2 for calculate the intersection

};

#endif // INTERSECTIONSEGMENTS_H
