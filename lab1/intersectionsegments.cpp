#include "intersectionsegments.h"

IntersectionSegments::IntersectionSegments()
{
     commonIntersection=0;
}

/******************************
//Name: IntersectSegmentsArea
//Author: Xufeng
//Description: judge whether two segments are intersected(five cases)use area
//Input: four point of two line segements
//Output: the result of determianant
//Date: 2014-01-23
//Version: 1.00
*****************************/
bool IntersectionSegments::IntersectSegmentsArea(
        QPointF Line1_P_A,QPointF Line1_P_B,QPointF Line2_P_C,QPointF Line2_P_D)
{

    ComputeTriArea TriArea;
    QVector<QPointF> Tri_ABC;
    QVector<QPointF> Tri_ABD;
    QVector<QPointF> Tri_CDA;
    QVector<QPointF> Tri_CDB;

    // organize point to form triangles
    // triangle ABC
    Tri_ABC.append(Line1_P_A);
    Tri_ABC.append(Line1_P_B);
    Tri_ABC.append(Line2_P_C);
    // triangle ABD
    Tri_ABD.append(Line1_P_A);
    Tri_ABD.append(Line1_P_B);
    Tri_ABD.append(Line2_P_D);
    // triangle CDA
    Tri_CDA.append(Line2_P_C);
    Tri_CDA.append(Line2_P_D);
    Tri_CDA.append(Line1_P_A);
    // triangle CDB
    Tri_CDB.append(Line2_P_C);
    Tri_CDB.append(Line2_P_D);
    Tri_CDB.append(Line1_P_B);

    //computer the determinants formed by three vertexes of triangle,
    //it equals to 2 times of area of triangle,
    //if the order of three point are anticlockwise, its value is positive
    //and the vertex C is in the left of the line AB(the direction is A to B, and det(ABC)>0)
    //if the value is negative, C is in the right of AB
    double Det_ABC = TriArea.ComputeDet(Tri_ABC);
    double Det_ABD = TriArea.ComputeDet(Tri_ABD);
    double Det_CDA = TriArea.ComputeDet(Tri_CDA);
    double Det_CDB = TriArea.ComputeDet(Tri_CDB);

    // vertex C and D are in the same side of AB
    //(Det_ABC <0 && Det_ABD <0) || (Det_ABC >0 && Det_ABD >0)
    if( Det_ABC * Det_ABD > 0 )
    {
        return false;
    }
    //vertex C and D are in different sides of AB, that is, they are possible intersected
    else
    {
        //vertex A and B are in the same side of CD,
        if(Det_CDA * Det_CDB > 0)
        {
            return false;
        }
        //vertex A,B are in different side of CD, they are intersected
        else
        {
//            qDebug()<<"qAbs(Det_ABC)"<< qAbs(Det_ABC);
//            qDebug()<<"qAbs(Det_ABD)"<< qAbs(Det_ABD);
//            qDebug()<<"qAbs(Det_CDB)"<< qAbs(Det_CDB);
//            qDebug()<<"qAbs(Det_CDA)"<< qAbs(Det_CDA);


            double Lambda = (Det_CDA/2)/(Det_CDA/2 - Det_CDB/2);
            double Mu = (Det_ABC/2)/(Det_ABC/2 - Det_ABD/2);

            Line1_Lambda = Lambda;
            Line2_Mu = Mu;

            // AB and CD intersect and the intersection is on certain vertex of two segments
            if(qAbs(Lambda-0) <= 0.00001 || qAbs(Lambda-1) <= 0.00001
                 ||qAbs(Mu-0) <= 0.00001 || qAbs(Mu-1) <= 0.00001)
            {

                commonIntersection++;//
                return false;
            }
            // AB and CD are intersected, that is, A,B in two sides of CD  and C,D in two sides of AB
            else
            {
                return true;
            }
        }
    }



}

//     |A11  B12|
// D = |A12  B22|
double IntersectionSegments::Determinant2(double A11, double A12, double B12, double B22)
{
    return (A11*B22 - A12*B12);
}

/******************************
//Name: SegmentsIntersection
//Author: Xufeng
//Description: judge whether two segments are intersected, use the Cramer rules
//Input: four points of two line segements
//Output: whether intersect and output intersection
//Date: 2014-01-23
//Version: 1.00
*****************************/

/******************************
//according to:  vector(r) = (1-Lambda)*vector()p + Lambda*vector(q)
//point A,B,C D  judge if A,B(segment 1) and C,D(segment 2) are intersected.
//so the coordinate of intersection is:  xr = (1-Lambda)*A.x + Lambda*B.x  (1)
//similiarly,   yr = (1-Lambda)*A.y + Lambda*B.y (2)
//for C,D :     xr = (1-Mu)*C.x + Mu*D.x   (3)
//similiarly,   yr = (1-Mu)*C.y + Mu*D.y   (4)
//from (1) and (3): (B.x -A.x)*Lambda + A.x = (D.x - C.x)*Mu + C.x
//              ==> (B.x -A.x)*Lambda + (C.x - D.x)*Mu  = C.x - A.x
//                  (B.y -A.y)*Lambda + (C.y - D.y)*Mu  = C.y - A.y
// according to Cramer rule to solve this equations set
//     |(B.x -A.x)  (C.x - D.x)|         |(C.x - A.x)  (C.x - D.x)|        |(B.x -A.x)  (C.x - A.x)|
// D = |(B.y -A.y)  (C.y - D.y)|     D1= |(C.y - A.y)  (C.y - D.y)|    D2= |(B.y -A.y)  (C.y - A.y)|
// so: Lambda= D1/D;   Mu= D2/D;
// if( Lambda > 1 || Lambda < 0 ) no intersect
// if( Mu > 1 || Mu < 0 ) no intersect
//
*****************************/
bool IntersectionSegments::IntersectSegmentsCramer(
        QPointF Line1_A,QPointF Line1_B,QPointF Line2_C,QPointF Line2_D)
{
    //  D = |A11  B12|
    //      |A12  B22|

    double D = Determinant2(Line1_B.x()-Line1_A.x(),Line1_B.y()-Line1_A.y(), Line2_C.x() - Line2_D.x(), Line2_C.y() - Line2_D.y());
    if ( qAbs(D-0)<= 0.00001)  // D=0，two segments are overlaped
    {
        return false;
    }
    else
    {
        double Lambda = Determinant2(Line2_C.x() - Line1_A.x(), Line2_C.y() - Line1_A.y(),Line2_C.x() - Line2_D.x(), Line2_C.y()-Line2_D.y()) / D;

        double Mu = Determinant2(Line1_B.x() - Line1_A.x(), Line1_B.y() - Line1_A.y(), Line2_C.x() - Line1_A.x(), Line2_C.y() - Line1_A.y()) / D;

        if ( Lambda >1 || Lambda <0 || Mu >1 || Mu <0) // the intersection is out of AB or CD
        {
            return false;
        }

        if (Lambda >= 0 && Lambda <= 1)
        {
            if(Mu >= 0 && Mu <= 1)
            {
                Line1_Lambda = Lambda;
                Line2_Mu = Mu;
                //the case of the intersection is one of vertexes of segments
                if(qAbs(Lambda-0) <= 0.00001 || qAbs(Lambda-1) <= 0.00001
                     ||qAbs(Mu-0) <= 0.00001 || qAbs(Mu-1) <= 0.00001)
                {
                    commonIntersection++;
                    return false;
                }
                else   //  0<Lamada<1 and 0<Mu<1 AB and CD are intersected
                {
                    return true;
                }
            }
        }
    }
}


bool IntersectionSegments::PointInPolygon (
        QPointF pDetermined, QVector<QPointF> pointsPolygon, QPointF pOutPolygon)
{

    int numberInsection =0;
    bool isInPolygon;
    for(int i=0;i<pointsPolygon.count();i++)
    {
        isInPolygon = IntersectSegmentsArea(
                      pDetermined,pOutPolygon,pointsPolygon[i],pointsPolygon[(i+1)%pointsPolygon.count()]);
       // isInPolygon = IntersectSegmentsCramer(
      //                pDetermined,pOutPolygon,pointsPolygon[i],pointsPolygon[(i+1)%pointsPolygon.count()]);
        if(isInPolygon)
        {
            numberInsection++;
        }
    }

    intersectionNumber = numberInsection;

    //the number of intersection is odd,it means point is in the polygon
    if(intersectionNumber%2==1)
    {
        return true;
    }
    else
        return false;
}

/******************************
//Name: Intersections_of_EachHatchLine
//Author: Xufeng
//Description: find the intersections between hatching line and polygon
//             when draw hatching line each time
//Input: two vertexes of hactching line A and B, (A is the intersection of Y axis and hactching line
//       B is the intersection of X axis and hactching line) and Polygon vertex
//Output: intersections for each line
//Date: 2014-01-23
//Version: 1.00
*****************************/
QVector<LineSegments> IntersectionSegments::Intersections_of_EachHatchLine(
        QPointF pHatchA_onY, QPointF pHatchB_onX, QVector<QPointF> pointsPolygon)
{
    QVector<QPointF> intersections;
    QVector<LineSegments> hatchingLineSegments;
    for(int i=0;i< pointsPolygon.count();i++)
    {
        bool isIntersected = IntersectSegmentsCramer(
                pHatchA_onY,pHatchB_onX,pointsPolygon[i],pointsPolygon[(i+1)%pointsPolygon.count()]);

        if(isIntersected)
        {
            double Lambda = Line1_Lambda;
            double Mu= Line2_Mu;
            double xIntersection = (1-Lambda)*pHatchA_onY.x() + Lambda*pHatchB_onX.x();
            double yIntersection = (1-Lambda)*pHatchA_onY.y() + Lambda*pHatchB_onX.y();
            intersections.append(QPointF(xIntersection,yIntersection));
        }
    }

    if(intersections.count()>0)
    {
        SortVertexbyCoordinate sortIntersection;
        QVector<QPointF> intersectionsSorted = sortIntersection.SortPoints(intersections);

        for( int j=0; j < intersectionsSorted.count()-1;j+=2)
        {
            LineSegments tempSeg(intersectionsSorted[j],intersectionsSorted[j+1]);
            hatchingLineSegments.append(tempSeg);
        }
    }
        return hatchingLineSegments;

}

/******************************
//hatch polygon from the minX and MinY of minimum rectangle
//first hactching line: A(0, ymin + xmin*tan(alpha))
//                      B(xmin + ymin/(tan(alpha)), 0)
//following :           Ai(0, xmin + (ymin/(tan(alpha))+ i*d/(sin(alpha)))*tan(alpha))
//                      Bi(xmin + ymin/(tan(alpha))+ i*d/(sin(alpha)), 0)
//Description: find the intersections between hatching line and polygon
//             when draw hatching line each time
//Input: two vertexes of hactching line A and B, A is the intersection of Y axis and hactching line
//       B is the intersection of X axis and hactching line
//Output: intersections for each line
//Date: 2014-01-23
//Version: 1.00
*****************************/

QVector<LineSegments> IntersectionSegments::CalculateAllHatchLine( QVector<QPointF> pointsPolygon,
        double alpha, double distanceHatchLine,QRectF minRectofPolygon)
{
    double xmin = minRectofPolygon.bottomLeft().x();
    double ymin = minRectofPolygon.bottomLeft().y();

    QPointF A1,B1;
    A1.setX(0);
    A1.setY(ymin + xmin* (qTan(alpha*PI/180)));

    B1.setX(xmin + ymin/(qTan(alpha*PI/180)));
    B1.setY(0);

    ComputeTriArea ComputeArea;
    QVector<LineSegments> allHatchLines;
    QVector<QPointF> tempTriangle;


    for(int n =0; ;n++)
    {
        QPointF Ai,Bi;
        Bi.setX(B1.x()+ n*distanceHatchLine/qSin(alpha*PI/180));
        Bi.setY(0);
        Ai.setX(0);
        Ai.setY(Bi.x()*qTan(alpha*PI/180));

        tempTriangle.clear();
        tempTriangle.append(Ai);
        tempTriangle.append(Bi);
        tempTriangle.append(minRectofPolygon.topRight());

        if(ComputeArea.ComputeDet(tempTriangle) > 0)
        {
            QVector<LineSegments> tempHatchLines;
            tempHatchLines = Intersections_of_EachHatchLine(Ai,Bi,pointsPolygon);
            for(int i=0;i<tempHatchLines.count();i++)
            {
                allHatchLines.append(tempHatchLines[i]);
            }
        }
        else
        {
            break;
        }


    }
        return allHatchLines;

}

