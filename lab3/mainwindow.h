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
#include <QVarLengthArray>

#include "computetriarea.h"
#include "computetricenter.h"
#include "triangle.h"
#include "incircle.h"
#include "linesegments.h"
#include "computecircumcirclecenter.h"
#include "qpointf3.h"

#define PI 3.1415926
#include <vector>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;


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

    enum { Margin = 80, RightMargin = 30 ,LeftMargin = 60,TopMargin = 10,BottomMargin = 25,
           PixMapLeftMargin = 5, PixMapTopMargin = 5,PixMapRightMargin = 5, PixMapBottomMargin = 5,
           LabelWidth = 100,RigntButtonWidth = 80,ChinaMapBottomMargin=20,
           YMaxTicks=16};
     //The QPixmap class is an off-screen image representation that can be used as a paint device
     QPixmap pixmap;

     QMap<int, QVector<QPointF3> > QMap_coordinate; //store the coordinate of vertex of polygon
     QMap<int, QVector<QPointF3> > QMap_GlobalTriangle; //store the global triangle
     QVector<QPointF3> QVec_DataFile;

     QVector<QPointF3> QVec_CoordTransform;
     QMap<int, QPointF3> QMap_CoordTransform;

     double MinX, MaxX, MinY, MaxY, MinZ, MaxZ; //the min and max
     double RangeX; // the range of coordinate for display the polygon
     double RangeY;
     double RangeZ;
     double approximateMinX,approximateMinY,approximateMinZ;
     double approximateMaxX,approximateMaxY,approximateMaxZ;



     QMap<int, QPointF3> QMap_coordinateVertex;//store the clicked numberID and coordinate
     QMap<int, QPointF3> QMap_GlobalTriVertex;
     QMap<int, QVector<Triangle> > QMap_Triangle;//QMap for storing all triangles
     QMap<int, QVector<Triangle> > QMap_TriangleWalked;//QMap for storing walked triangles
     QVector<QPointF3> QVec_VertexCoord;     // store the clicked and inserted point position in pixmap
     QVector<QPointF3> QVec_GlobalTriangle;  //store global triangle
     QVector<Triangle> QVec_Triangles;  //store all triangle
     //QVector<Triangle> QVec_TrianglesWalked;  //store all walked triangle
     QVector<int> WalkedTriangleID;           //record the walked triangle IDs
     //QVector<int> QVec_TableTopology[9];// TriID, vertex1,vertex2,vertex3,TriAdjacent1,TriAdjacent1,TriAdjacent2,TriAdjacent3,center
     QVector< QVector<int> > QVec_TableTopology;
     QVector <int> QVec_StackOptim; // build a stack for optimization


     bool isDisplayCoordinate; // display coordinate
     bool isDisplayDelaunay;


     int numberVertex;//the ID of inserted point

     bool readTextFile(QString fileName,QString SplitSign);
     double Find_Min(double num1,double num2); // find min value
     double Find_Max(double num1,double num2); // find max value

     void setDataSource(int id, const QVector<QPointF3> &data, QString flag); // organize the coordinate into QMap
     void setDataSource(int id, const QVector<Triangle> &data, QString flag); // organize the triangle into QMap
     void setDataSource(int id, const QVector<LineSegments> &data, QString flag);

     void drawSign(double x,double y,QPainter *painter,int size); // draw the sign on point of polygon
     void drawCoordinateText(double x,double y,QPainter *painter,QString text); // draw the coordiante value for vertex
     void drawGrid(QPainter *painter); // draw the coordiante axis
     void drawCurves(QPainter *painter); // conntect vertex as polygon
     void refreshPixmap(); //refresh the pixmap

     void Init();
     void preProcessData(QVector<QPointF3> QVec_VertexCoord); // compute the range of Frame Triangle(Global triangle)
     void Frame(QMap<int, QPointF3> GlobalTriVertex);
     int  Walk(int newPointID,QMap<int, QPointF3> &QMap_coordinateVertex);
     void Insert(int newPointID,int TriID_WalkResult);
     QVector<double> ComputeDetTri_andNewPoint(int triID,int newPointID,QMap<int, QPointF3> &QMap_coordinateVertex);
     double isInCircle(int currentTri,int fourthPointID, QMap<int, QPointF3> &QMap_coordinateVertex);
     void DisplayWalkedTriangles(QVector<int> WalkedTriangleID);
     int Find_NP_PNC(int adjacentTriID,int currentTriID,int flag);
     void Optimize(int NewPointID,QVector<int> &QVec_StackOptim, QMap<int, QPointF3> &QMap_coordinateVertex);
     int Swap(int currentTriID,int newPointID,int adjacentTriID,int PointNotCommon ,QVector<QVector<int> > &QVec_TableTopology);
     double DisplayAllTriangles(QVector<QVector<int> > &QVec_TableTopology,QMap<int, QPointF3> &QMap_coordinateVertex);


     Eigen::MatrixX4d matrixPoints;
     Eigen::MatrixX4d matrixPointsTranslation;
     double rotationAngleZ;
     double rotationAngleY;
     double rotationAngleX;
     double distance;

     double MaxX_recorded;
     double MaxY_recorded;
     double MinX_recorded;
     double MinY_recorded;


     void preProcessData(Eigen::MatrixX4d matrix);
     void removeGlobalVertex(QVector< QVector<int> > &QVec_TableTopology);
     Eigen::MatrixX4d Translation(MatrixX4d points,double deltX,double deltY,double deltZ);
     Eigen::MatrixX4d Scaling(Eigen::MatrixX4d points,double sX,double sY,double sZ);
     Eigen::MatrixX4d Rotation(Eigen::MatrixX4d points,double angle,QString sign);
     Eigen::MatrixX4d LoadPointsToMatrix( QMap<int, QPointF3> &QMap_coordinateVertex);
     QMap<int, QPointF3> LoadMatrixToQMap(Eigen::MatrixX4d matrix);
     Eigen::Vector4d PerspectiveProjectionPointOnXZ(Eigen::Vector4d points,double d);
     Eigen::MatrixX4d Projection3DModel(Eigen::MatrixX4d points,double d);

     void DisplayMatrix(MatrixX4d result);
     void DisplayMatrix(Matrix4d result);

protected:   //the overload function
     void paintEvent(QPaintEvent *event);
     void resizeEvent(QResizeEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);



private slots:

    void on_actionOpen_files_triggered();

    void on_pushButtonTranslation_clicked();

    void on_pushButtonRotationZ_clicked();

    void on_pushButtonRotationY_clicked();

    void on_pushButtonRotationX_clicked();

    void on_pushButtonScaling_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
