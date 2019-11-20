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

#include "computetriarea.h"
#include "computetricenter.h"
#include "intersectionsegments.h"


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
     QMap<int, QVector<QPointF> > QMap_PolyCenter; //store the center of polygon
     QMap<int, QVector<LineSegments> > QMap_HatchLines; //store the hatching line segments
     QMap<int, QVector<QPointF> > QMap_PolyOutPoint; //store the point out of polygon to help to judge if the center is in the polygon
     double MinX, MaxX, MinY, MaxY; //the min and max of polygon,include X and Y
     double RangeX; // the range of coordinate for display the polygon
     double RangeY;
     double approximateMinX,approximateMinY;
     QVector<QPointF> QVec_PolygonVertex; //organize the data in text file to vertex of polygon

     bool isDisplayCoordinate; // display coordinate
     bool isDisplayCenter, isDisplayHatching;
     QString fileName;


     double Find_Min(double num1,double num2); // find min value
     double Find_Max(double num1,double num2); // find max value

     void setPolygonData(int id, const QVector<QPointF> &data, int flag); // organize the coordinate into QMap
     void setHatchLineData(int id, const QVector<LineSegments> &data, int flag); // organize the line segments into QMap
     void drawSign(double x,double y,QPainter *painter,int size); // draw the sign on point of polygon
     void drawCoordinateText(double x,double y,QPainter *painter,QString text); // draw the coordiante value for vertex
     void drawGrid(QPainter *painter); // draw the coordiante axis
     void drawCurves(QPainter *painter); // conntect vertex as polygon

     bool readTextFile(QString fileName,QString SplitSign); // get coordinate from text file
     double ComputePolyArea(QVector<QPointF> QVec_points); // compute the area of polygon
     QPointF ComputePolyCenter(QVector<QPointF> QVec_points);
     void refreshPixmap(); //refresh the pixmap

protected:   //overload the function
     void paintEvent(QPaintEvent *event);
     void resizeEvent(QResizeEvent *event);



private slots:
    void on_pushButton_ComputeArea_clicked(); // click the button of Area
    void on_pushButton_Center_clicked();      //click the button of Center
    void on_pushButtonHatch_clicked();

    void on_actionOpen_file_triggered();

public slots:
    void on_radioButton_Coordinate_clicked(); // clich the button of Display coordinate


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
