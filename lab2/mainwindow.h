#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QLabel>
#include <QDate>
#include <QStylePainter>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <cmath>
#include <QTime>
#include <QFileDialog>
#include <QMouseEvent>
#include "qdebug.h"

#include "computetriarea.h"
#include "computetricenter.h"
#include "triangle.h"
#include "incircle.h"
#include "linesegments.h"
#include "computecircumcirclecenter.h"

#define PI 3.1415926


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:

    enum { Margin = 80, RightMargin = 35 ,LeftMargin = 40,TopMargin = 10,BottomMargin = 25,
           PixMapLeftMargin = 5, PixMapTopMargin = 5,PixMapRightMargin = 5, PixMapBottomMargin = 5,
           LabelWidth = 100,RigntButtonWidth = 80,ChinaMapBottomMargin=20,
           YMaxTicks=16};
     //The QPixmap class is an off-screen image representation that can be used as a paint device
     QPixmap pixmap;

     QMap<int, QVector<QPointF> > QMap_coordinate; //store the coordinate of vertex of polygon
     QMap<int, QVector<QPointF> > QMap_GlobalTriangle; //store the global triangle

     double MinX, MaxX, MinY, MaxY; //the min and max of polygon,include X and Y
     double RangeX; // the range of coordinate for display the polygon
     double RangeY;
     double approximateMinX,approximateMinY;


     QMap<int, QPointF> QMap_coordinateVertex;//store the clicked numberID and coordinate
     QMap<int, QPointF> QMap_GlobalTriVertex;
     QMap<int, QVector<Triangle> > QMap_Triangle;//QMap for storing all triangles
     QMap<int, QVector<Triangle> > QMap_TriangleWalked;//QMap for storing walked triangles
     QVector<QPointF> QVec_VertexCoord;     // store the clicked and inserted point position in pixmap
     QVector<QPointF> QVec_GlobalTriangle;  //store global triangle
     QVector<Triangle> QVec_Triangles;  //store all triangle
     //QVector<Triangle> QVec_TrianglesWalked;  //store all walked triangle
     QVector<int> WalkedTriangleID;           //record the walked triangle IDs
     QVector<int> QVec_TableTopology[9];// TriID, vertex1,vertex2,vertex3,TriAdjacent1,TriAdjacent1,TriAdjacent2,TriAdjacent3,center
     QVector <int> QVec_StackOptim; // build a stack for optimization


     QVector<QPointF> selectedVertex; // store coordinate selected by mouse click
     QVector<QPointF> nearestNeignbors_selectedVertex; //store all nearest vertex of selected points
     QMap<int, QVector<QPointF> > QMap_SelectedPoint ;
     QMap<int, QVector<QPointF> > QMap_NearestNeighbors ;//store all nearest vertex of selected points for displaying

     QMap<int, QPointF> QMap_coordinateCircumcircleCenter; //store circumcircle of triangle, int record the TriID
     QMap<int, QVector<LineSegments> > QMap_VoronoiLines; //store the voronoi line segments
     QVector<LineSegments> QVec_VoronoiLines;


     QVector<QPointF> selectedVertexVoronoi;
     QVector<QPointF> QVec_VoronoiCellPloygon;
     QVector <int> QVec_VoronoiCell_NeigbhorTriangles;
     QMap<int, QVector<QPointF> >  QMap_coordinateCircumcircleCenterOfVoronoiCell;

     bool isDisplayCoordinate; // display coordinate
     bool isSelectNeighbor;
     bool isInsertPoint;
     bool isDisplayVoronoi;
     bool isDisplayDelaunay;
     bool isDisplayWalkedTriangels;
     bool isDisplayVoronoiCell;

     int numberVertex;//the ID of inserted point

     double Find_Min(double num1,double num2); // find min value
     double Find_Max(double num1,double num2); // find max value

     void setDataSource(int id, const QVector<QPointF> &data, QString flag); // organize the coordinate into QMap
     void setDataSource(int id, const QVector<Triangle> &data, QString flag); // organize the triangle into QMap
     void setDataSource(int id, const QVector<LineSegments> &data, QString flag);

     void drawSign(double x,double y,QPainter *painter,int size); // draw the sign on point of polygon
     void drawCoordinateText(double x,double y,QPainter *painter,QString text); // draw the coordiante value for vertex
     void drawGrid(QPainter *painter); // draw the coordiante axis
     void drawCurves(QPainter *painter); // conntect vertex as polygon
     void refreshPixmap(); //refresh the pixmap

     void Init();
     void preProcessData(QVector<QPointF> QVec_VertexCoord); // compute the range of Frame Triangle(Global triangle)
     void Frame(QMap<int, QPointF> GlobalTriVertex);
     int  Walk(int newPointID,QMap<int, QPointF> &QMap_coordinateVertex);
     int  Walk(QPointF newPoint, QMap<int, QPointF> &QMap_coordinateVertex);
     void Insert(int newPointID,int TriID_WalkResult);
     QVector<double> ComputeDetTri_andNewPoint(int triID,int newPointID,QMap<int, QPointF> &QMap_coordinateVertex);
     QVector<double> ComputeDetTri_andNewPoint(int triID,QPointF newPoint,QMap<int, QPointF> &QMap_coordinateVertex);
     double isInCircle(int currentTri,int fourthPointID, QMap<int, QPointF> &QMap_coordinateVertex);

     void DisplayWalkedTriangles(QVector<int> WalkedTriangleID);
     QVector<double> RestrictNewPointInGlobalTriangle( double x, double y);

     int Find_NP_PNC(int adjacentTriID,int currentTriID,int flag);

     void Optimize(int NewPointID,QVector<int> &QVec_StackOptim, QMap<int, QPointF> &QMap_coordinateVertex);
     int Swap(int currentTriID,int newPointID,int adjacentTriID,int PointNotCommon ,QVector<int> (&QVec_TableTopology)[9]);

     double DisplayAllTriangles(QVector<int> (&QVec_TableTopology)[9],QMap<int, QPointF> &QMap_coordinateVertex);

     double DistanceTwoPoints(QPointF P1,QPointF P2);
     QVector<int> SelectNearestVertexID_TriID(QPointF newPoint, int triID,QMap<int, QPointF> &QMap_coordinateVertex);
     QVector<int> SelectNearestVertexes(int SelectedPointID, int currentTriID, QMap<int, QPointF> &QMap_coordinateVertex);

     QPointF CalculateCenterOfCircumcircle(int TriID);
     void CalculateAllCenterOfCircumcircle(QVector<int> (&QVec_TableTopology)[9]);
     void CalculateVoronoiCellCenterOfCircumcircle(QVector<int> celltriangleID,QVector<int> (&QVec_TableTopology)[9]);
     void CreateVoronoiLineSegments(QVector<int> (&QVec_TableTopology)[9]);


protected:   //the overload function
     void paintEvent(QPaintEvent *event);
     void resizeEvent(QResizeEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);



private slots:

    void on_pushButtonCreateCoordinateAxis_clicked();

    void on_checkBox_NearestNeighbor_clicked();

    void on_checkBox_DisplayVoronoi_clicked();

    void on_checkBox_DisplayWalkedTriangles_clicked();

    void on_radioButtonCoordinate_clicked();

    void on_checkBox_VoronoiCell_clicked();

    void on_checkBox_DisplayDelaunay_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
