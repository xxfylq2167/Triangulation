#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    setBackgroundRole(QPalette::Dark);  //when enlarge the widget and redraw it,provie Qt a default range for fill color in new area
    setAutoFillBackground(true);

    //make widgets can be scaledon the horizontal and vertical directions
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setAttribute(Qt::WA_PaintOutsidePaintEvent);

    setFocusPolicy(Qt::StrongFocus);

    Init();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/******************************
//Name: Init
//Author: Xufeng
//Description: initalize some variables
//Input:
//Output:
//Date: 2014-02-10
//Version: 1.00
*****************************/

void MainWindow::Init()
{
    ui->radioButtonCoordinate->setChecked(true);
    ui->checkBox_DisplayDelaunay->setChecked(true);
    ui->checkBox_NearestNeighbor->setChecked(false);
    ui->checkBox_VoronoiCell->setChecked(false);
    ui->checkBox_DisplayVoronoi->setChecked(false);
    ui->checkBox_DisplayWalkedTriangles->setChecked(false);

    isDisplayCoordinate=true;
    isSelectNeighbor = false;
    isInsertPoint = true;
    isDisplayVoronoi = false;
    isDisplayDelaunay = true;
    isDisplayWalkedTriangels = false;
    isDisplayVoronoiCell= false;

    numberVertex= 4;

    approximateMinX = 0;
    approximateMinY = 0;
    RangeX = 100 ;
    RangeY = 100 ;

    double x1 =  (RangeX - approximateMinX)/2;
    double y1 =  RangeY-5;

    double x2 = 5;
    double y2 = 5;

    double x3 = RangeY-5;
    double y3 = 5;

    // set the vertex for Global triangle
    QVec_GlobalTriangle.clear();
    QVec_GlobalTriangle.append(QPointF(x1,y1));
    QVec_GlobalTriangle.append(QPointF(x2,y2));
    QVec_GlobalTriangle.append(QPointF(x3,y3));

    // store the three vertex of global vertexes into QMap_GlobalTriVertex and QMap_coordinateVertex
    for(int i=0;i<QVec_GlobalTriangle.count();i++)
    {
        QMap_GlobalTriVertex.insert(i+1, QVec_GlobalTriangle[i]);
        QMap_coordinateVertex.insert(i+1, QVec_GlobalTriangle[i]);

    }
    // draw the global triangle
    setDataSource(0,QVec_GlobalTriangle,"Global_Triangle");

    // build Frame for triangulation
    Frame(QMap_GlobalTriVertex);

    //initialize tableWidget_Topology for display the topologies
    for(int j=0;j< 7 ;j++)
    {
        ui->tableWidget_Topology->setColumnWidth(j,30);
    }
    QStringList header;
    header<<tr("TriID")<<tr("S1")<<tr("S2")<<tr("S3")<<tr("TA1")<<tr("TA2")<<tr("TA3");
    ui->tableWidget_Topology->setHorizontalHeaderLabels(header);

}


double MainWindow::Find_Min(double num1,double num2)
{
    double Min = qMin(num1,num2);
    return Min;
}

double MainWindow::Find_Max(double num1,double num2)
{
    double Max = qMax(num1,num2);
    return Max;
}

/******************************
//Name: setDataSource
//Author: Xufeng
//Description: overlapped, according different data structre and flag to set data for displaying
//Input: data structure and flag
//Output:
//Date: 2014-02-12
//Version: 1.00
*****************************/
void MainWindow::setDataSource(int id, const QVector<QPointF> &data, QString flag)
{
    // store the coordiante of vertexes clicked by mouse
    if(flag=="Vertex_Clicked")
    {
        QMap_coordinate[id] = data;
    }
    //store the coordiante of vertexes of global triangle
    else if(flag=="Global_Triangle")
    {
        QMap_GlobalTriangle[id] = data;
    }
    //store the all ID of points and corresponding coordinate
    else if(flag=="Vertex_All")
    {
        if(data.count()>0)
        {
            for(int j=0;j< data.count();j++)
            {
                QMap_coordinateVertex.insert(j+1+3, data[j]);
            }
        }
        else
        {
            QMap_coordinateVertex.clear();
        }
    }
    // store the coordinates of nearest neighbors
    else if(flag=="Nearest_Neighbors")
    {
        QMap_NearestNeighbors[id] =  data;
    }
    // store the coordinates of selected point
    else if(flag == "Selected_Point")
    {
        QMap_SelectedPoint[id] = data;
    }
    //store the voronoi cell pologon
    else if(flag=="Voronoi_Cell")
    {
        QMap_coordinateCircumcircleCenterOfVoronoiCell[id] =data;
    }

    refreshPixmap();
}

void MainWindow::setDataSource(int id, const QVector<Triangle> &data, QString flag)
{
    if(flag== "Delaunay_Triangles") //store  all triangles
    {
        QMap_Triangle[id]=data;
    }
    if(flag== "Walked_Triangle_Delaunay") //walked triangle Delaunay
    {
        QMap_TriangleWalked[id]=data;
    }

    refreshPixmap();
}

void MainWindow::setDataSource(int id, const QVector<LineSegments> &data, QString flag)
{
    //store the linesegments of voronoi graph
    if(flag=="Voronoi_Graph")
    {
        QMap_VoronoiLines[id] = data;
    }
}

// process the inputed files, to evaluate the range and global triangle
void MainWindow::preProcessData(QVector<QPointF> QVec_VertexCoord)
{
    QStringList X;
    QStringList Y;

    for(int i=0; i< QVec_VertexCoord.count();i++)
    {
        X.append(QString::number(QVec_VertexCoord[i].x()));
        Y.append(QString::number(QVec_VertexCoord[i].y()));
    }
    if(X.count()==Y.count())
    {
        if(X.count() == 1)
        {
            MinX= X[0].toDouble();
            MaxX= X[0].toDouble();
            MinY= Y[0].toDouble();
            MaxY= Y[0].toDouble();
        }
        else if(X.count() == 2)
        {
            MinX=Find_Min(X[0].toDouble(),X[1].toDouble());
            MaxX=Find_Max(X[0].toDouble(),X[1].toDouble());
            MinY=Find_Min(Y[0].toDouble(),Y[1].toDouble());
            MaxY=Find_Max(Y[0].toDouble(),Y[1].toDouble());
        }
        else
        {
            //find min and max value from X
            MinX= X[0].toDouble();
            MaxX= X[0].toDouble();
            for(int i=1;i < X.count();i++)
            {
                MinX=Find_Min(MinX,X[i].toDouble());
                MaxX=Find_Max(MaxX,X[i].toDouble());
            }
            //find min and max value from y
            MinY= Y[0].toDouble();
            MaxY= Y[0].toDouble();
            for(int j=1;j < Y.count();j++)
            {
                MinY=Find_Min(MinY,Y[j].toDouble());
                MaxY=Find_Max(MaxY,Y[j].toDouble());
            }
        }

        qDebug()<<"MinX,MaxX,MinY,MaxY"<<MinX<<" "<<MaxX<<" "<<MinY<<" "<<MaxY;

        double MinX_Approximate= floor(MinX/10)*10 - qAbs(floor(MinX/100))*10;
        double MaxX_Approximate = ceil(MaxX/10)*10 + ceil(MaxX/100)*10;
        double MinY_Approximate = floor(MinY/10)*10 - qAbs(floor(MinY/100))*10;
        double MaxY_Approximate= ceil(MaxY/10)*10 + ceil(MaxY/100)*10;

         qDebug()<<"Approximate MinX,MaxX,MinY,MaxY"<<MinX_Approximate<<" "<<MaxX_Approximate<<" "<<MinY_Approximate<<" "<<MaxY_Approximate;
        double x1 =  (MaxX_Approximate + MinX_Approximate)/2;
        double y1 =  MaxY_Approximate + ((MaxX_Approximate - MinX_Approximate)/2) * qTan(PI/3);

        double x2 = MinX_Approximate - (MaxY_Approximate - MinY_Approximate) / (qTan(PI/3));
        double y2 = MinY_Approximate;

        double x3 = MaxX_Approximate + (MaxY_Approximate - MinY_Approximate) / (qTan(PI/3));
        double y3 = MinY_Approximate;

        approximateMinX = floor((x2 -10)/10)*10;
        approximateMinY = floor((y2 -10)/10)*10;
        RangeX = ceil((x3 +10)/10)*10 - approximateMinX;
        RangeY = ceil((y1 +10)/10)*10 - approximateMinY;

        QVec_GlobalTriangle.clear();
        QVec_GlobalTriangle.append(QPointF(x1,y1));
        QVec_GlobalTriangle.append(QPointF(x2,y2));
        QVec_GlobalTriangle.append(QPointF(x3,y3));
        //qDebug()<< RangeX<<" " << RangeY<<" ";

        for(int i=0;i<QVec_GlobalTriangle.count();i++)
        {
            QMap_GlobalTriVertex.insert(i+1, QVec_GlobalTriangle[i]);
            QMap_coordinateVertex.insert(i+1, QVec_GlobalTriangle[i]);

        }
       // qDebug()<< " end" ;
        setDataSource(0,QVec_GlobalTriangle,"Global_Triangle");
    }
}

/******************************
//Name: Frame
//Author: Xufeng
//Description: Build Frame for insert new point
//Input: QMap<int, QPointF> GlobalTriVertex
//Output:
//Date: 2014-02-12
//Version: 1.00
*****************************/
void MainWindow::Frame(QMap<int, QPointF> GlobalTriVertex)
{
    int VertexNum=0;
    QVec_TableTopology[0].append(1);

    QMapIterator<int, QPointF> i(GlobalTriVertex);
    while (i.hasNext()) {
        i.next();
        int GlobalVertexID = i.key();
        QVec_TableTopology[VertexNum+1].append(GlobalVertexID);
        QVec_TableTopology[VertexNum+4].append(0);
        VertexNum++;
    }
}
/******************************
//Name: ComputeDetTri_andNewPoint
//Author: Xufeng
//Description: compure if the new point is in this triangles
//Input: the ID of current triangle,ID of new point, the structure storing coordinates
//Output: the results of the determinant value of three triangles
//Date: 2014-02-14
//Version: 1.00
*****************************/
QVector<double> MainWindow::ComputeDetTri_andNewPoint(int triID,int newPointID,QMap<int, QPointF> &QMap_coordinateVertex)
{
    // construct the new three triangle, the new point is common vertex
    int j = triID;
    ComputeTriArea triDet;
    QPointF newPoint;
    QVector<QPointF> triangle1;
    QVector<QPointF> triangle2;
    QVector<QPointF> triangle3;

    newPoint  = QMap_coordinateVertex.value(newPointID);
    QPointF V1= QMap_coordinateVertex.value(QVec_TableTopology[1][j]);
    QPointF V2= QMap_coordinateVertex.value(QVec_TableTopology[2][j]);
    QPointF V3= QMap_coordinateVertex.value(QVec_TableTopology[3][j]);

    triangle1.append(V1);
    triangle1.append(V2);
    triangle1.append(newPoint);

    triangle2.append(V2);
    triangle2.append(V3);
    triangle2.append(newPoint);

    triangle3.append(V3);
    triangle3.append(V1);
    triangle3.append(newPoint);

    double Result1_12P = triDet.ComputeDet(triangle1);
    double Result2_23P = triDet.ComputeDet(triangle2);
    double Result3_31P = triDet.ComputeDet(triangle3);
    triangle1.clear();
    triangle2.clear();
    triangle3.clear();

    QVector<double> result;
    result.clear();
    result.append(Result1_12P);
    result.append(Result2_23P);
    result.append(Result3_31P);
    return result;
}

QVector<double> MainWindow::ComputeDetTri_andNewPoint(int triID,QPointF newPoint,QMap<int, QPointF> &QMap_coordinateVertex)
{
    int j = triID;
    ComputeTriArea triDet;
    QVector<QPointF> triangle1;
    QVector<QPointF> triangle2;
    QVector<QPointF> triangle3;
    triangle1.clear();
    triangle2.clear();
    triangle3.clear();

    QPointF V1= QMap_coordinateVertex.value(QVec_TableTopology[1][j]);
    QPointF V2= QMap_coordinateVertex.value(QVec_TableTopology[2][j]);
    QPointF V3= QMap_coordinateVertex.value(QVec_TableTopology[3][j]);

    triangle1.append(V1);
    triangle1.append(V2);
    triangle1.append(newPoint);

    triangle2.append(V2);
    triangle2.append(V3);
    triangle2.append(newPoint);

    triangle3.append(V3);
    triangle3.append(V1);
    triangle3.append(newPoint);

    double Result1_12P = triDet.ComputeDet(triangle1);
    double Result2_23P = triDet.ComputeDet(triangle2);
    double Result3_31P = triDet.ComputeDet(triangle3);

    QVector<double> result;
    result.clear();
    result.append(Result1_12P);
    result.append(Result2_23P);
    result.append(Result3_31P);
    return result;
}

/******************************
//Name: Walk
//Author: Xufeng
//Description: objectve to find the triangle contains the new point
               (1)from the startting triangle, calculate the triangle formed by edge of starrting triangle,
               (2)judge the first triangle constructed by target(new point) and the first edge(P1 and P2),
               (3)if the determinant of triangle are negetive, find the adjacent triangle that owns the common edge with the starrting triangle
               (4)walk to the found adjacent triangle
               (5)else,  judge the triangle formed by new point and second edge(P2 and P3)
               (6)similiarly, continue judge the third triangle

the data structure of topological table:

ID  V1   V2   V3   TA1  TA2   TA3

TA1 is the adjacent triangle that is opposite to the V1
TA2 is the adjacent triangle that is opposite to the V2
TA3 is the adjacent triangle that is opposite to the V3
//Input:ID of new point, the structure storing coordinates
//Output: ID of triangle containing the new point
//Date: 2014-02-14
//Version: 1.00
*****************************/
int MainWindow::Walk(int newPointID, QMap<int, QPointF> &QMap_coordinateVertex)
{
    WalkedTriangleID.clear();
    // strating triangle is the number 1
    int triCurrentID = 0;

    WalkedTriangleID.append(triCurrentID);
    while(1)
    {
         QVector<double> resultDet =  ComputeDetTri_andNewPoint(triCurrentID,newPointID,QMap_coordinateVertex);

         double Result1_12P = resultDet[0];
         double Result2_23P = resultDet[1];
         double Result3_31P = resultDet[2];

         // find the adjacent of TA3 that is on the right of
         // the first edge V1-V2(the opposite adjacent triangle of V3)
         if(Result1_12P < 0)
         {
             if(QVec_TableTopology[6][triCurrentID] !=0)
             {
                 //update the current triID
                 triCurrentID =  QVec_TableTopology[6][triCurrentID] -1 ;
                 WalkedTriangleID.append(triCurrentID);
                 continue;
             }
         }
         else
         {
             // find the adjacent of TA1 that is on the right of the second edge V2-V3
             if(Result2_23P < 0)
             {   
                 if(QVec_TableTopology[4][triCurrentID] !=0)
                 {
                     triCurrentID = QVec_TableTopology[4][triCurrentID] -1;
                     WalkedTriangleID.append(triCurrentID);
                     continue;
                 }
             }
             else
             {  
                 // find the adjacent of TA2 that is on the right of the second edge V3-V1
                 if(Result3_31P < 0)
                 {
                     if(QVec_TableTopology[5][triCurrentID] !=0)
                     {
                        triCurrentID = QVec_TableTopology[5][triCurrentID]-1;
                        WalkedTriangleID.append(triCurrentID);
                        continue;
                     }
                 }
                 else
                 {  // find the target triangle that contains the new point
                     break;
                 }
             }
         }
     }
    return QVec_TableTopology[0][triCurrentID];
}

int MainWindow::Walk(QPointF newPoint, QMap<int, QPointF> &QMap_coordinateVertex)
{
    WalkedTriangleID.clear();
    int triCurrentID = 0;

    WalkedTriangleID.append(triCurrentID);
    while(1)
    {
        QVector<double> resultDet =  ComputeDetTri_andNewPoint(triCurrentID,newPoint,QMap_coordinateVertex);

         double Result1_12P = resultDet[0];
         double Result2_23P = resultDet[1];
         double Result3_31P = resultDet[2];

         // find the adjacent of TA3 that is on the right of the first edge V1-V2
         if(Result1_12P < 0)
         {
             if(QVec_TableTopology[6][triCurrentID] !=0)
             {
                 triCurrentID =  QVec_TableTopology[6][triCurrentID] -1 ;
                 WalkedTriangleID.append(triCurrentID);
                 continue;
             }
         }
         else
         {
             // find the adjacent of TA1 that is on the right of the second edge V2-V3
             if(Result2_23P < 0)
             {
                 if(QVec_TableTopology[4][triCurrentID] !=0)
                 {
                     triCurrentID = QVec_TableTopology[4][triCurrentID] -1;
                     WalkedTriangleID.append(triCurrentID);
                     continue;
                 }
             }
             else
             {
                 // find the adjacent of TA2 that is on the right of the second edge V3-V1
                 if(Result3_31P < 0)
                 {
                     if(QVec_TableTopology[5][triCurrentID] !=0)
                     {
                        triCurrentID = QVec_TableTopology[5][triCurrentID]-1;
                        WalkedTriangleID.append(triCurrentID);
                        continue;
                     }
                 }
                 else
                 {  // find the triangle ID
                     break;
                 }
             }
         }
     }
    return QVec_TableTopology[0][triCurrentID];

}

/******************************
//Name: Insert
//Author: Xufeng
//Description: update the topologies after inserting a new point
//Input:ID of new point, the ID of triangle that contains the new point
//Output: updated topological table
//Date: 2014-02-27
//Version: 1.00
*****************************/

// use the table to maintant the update of topology
// | TirID | vertex1 | vertex2 | vertex3 | TA1  | TA2  |TA3 |
//
// TA1 is the adjacent of the opposite of vertex1
// TA2 is the adjacent of the opposite of vertex2
// TA3 is the adjacent of the opposite of vertex3

// the index of current triangle is [i] and the number of triangles is [N]
/* | TirID | vertex1 | vertex2 | vertex3 |  TA1   |   TA2  |  TA3 |
 *    i        A        B          C         Ta        Tb      Tc
 *
 *  (1) add new two triangles
 *  N+1        P        C          A        Tb        N+2     i
 *  N+2        P        A          B        Tc        i     N+1
 *
 *  (2) update the Tb and Ta, find the adjacent (i), but the i+1 is not in fix position
 *  find (i) in Tb and replace it with [N+1]
 *  Tb         ..       ..         ..         i
 *
 *  find (i) in Tc and replace it with [N+2]
 *  Tc         ..       ..         ..         i
 *  (3)//update the current triangle contained the new point ever
 *   i        P        B          C        Ta        N+1      N+2
 *
 */
void MainWindow::Insert(int newPointID,int TriID_WalkResult)
{
    int NumberOfTriangle = QVec_TableTopology[0].count();

    //add a new lines for new triangle N+1
    QVec_TableTopology[0].append(NumberOfTriangle+1);
    QVec_TableTopology[1].append(newPointID);
    QVec_TableTopology[2].append(QVec_TableTopology[3][TriID_WalkResult -1]);
    QVec_TableTopology[3].append(QVec_TableTopology[1][TriID_WalkResult -1]);
    QVec_TableTopology[4].append(QVec_TableTopology[5][TriID_WalkResult -1]);
    QVec_TableTopology[5].append(NumberOfTriangle+2);
    QVec_TableTopology[6].append(QVec_TableTopology[0][TriID_WalkResult -1]);

    //add a new lines for new triangle N+1
    QVec_TableTopology[0].append(NumberOfTriangle+2);
    QVec_TableTopology[1].append(newPointID);
    QVec_TableTopology[2].append(QVec_TableTopology[1][TriID_WalkResult -1]);
    QVec_TableTopology[3].append(QVec_TableTopology[2][TriID_WalkResult -1]);
    QVec_TableTopology[4].append(QVec_TableTopology[6][TriID_WalkResult -1]);
    QVec_TableTopology[5].append(QVec_TableTopology[0][TriID_WalkResult -1]);
    QVec_TableTopology[6].append(NumberOfTriangle+1);

    //update the other two adjacent triangle of current triangle
    //find the TA2 and TA3
    int adjacent2 = QVec_TableTopology[5][TriID_WalkResult -1];
    int adjacent3 = QVec_TableTopology[6][TriID_WalkResult -1];

    int indexTriIDinTA2,indexTriIDinTA3;
    if(adjacent2!= 0 || adjacent3!= 0)
    {
        for(int i=4;i<7;i++)
        {
            if(QVec_TableTopology[i][adjacent2 -1] == QVec_TableTopology[0][TriID_WalkResult-1])
            {
                indexTriIDinTA2 = i;
            }
            if(QVec_TableTopology[i][adjacent3 -1] == QVec_TableTopology[0][TriID_WalkResult-1])
            {
                indexTriIDinTA3 = i;
            }
        }
        // update the adjacent of TA2 and TA3
        QVec_TableTopology[indexTriIDinTA2][adjacent2 -1] = NumberOfTriangle+1;
        QVec_TableTopology[indexTriIDinTA3][adjacent3 -1] = NumberOfTriangle+2;
    }

    //update the current triangle contains the new point
    QVec_TableTopology[1][TriID_WalkResult -1]  = newPointID;
    QVec_TableTopology[5][TriID_WalkResult -1]  = NumberOfTriangle+1;
    QVec_TableTopology[6][TriID_WalkResult -1]  = NumberOfTriangle+2;

    // put the new inserted triangles into stack for optimization of Delaunay
    QVec_StackOptim.clear();
    QVec_StackOptim.append(QVec_TableTopology[0][TriID_WalkResult -1]);
    QVec_StackOptim.append(QVec_TableTopology[0][NumberOfTriangle]);
    QVec_StackOptim.append(QVec_TableTopology[0][NumberOfTriangle+1]);

}

// display all triangle in pixmap and tableWidget_Topology
double MainWindow::DisplayAllTriangles(QVector<int> (&QVec_TableTopology)[9],QMap<int, QPointF> &QMap_coordinateVertex )
{
    QVec_Triangles.clear();
    //display all the triangle after Insert
    for(int j=0;j< QVec_TableTopology[0].count() ;j++)
    {
        Triangle tempTri;
        tempTri.Vertex1 = QMap_coordinateVertex.value(QVec_TableTopology[1][j]);
        tempTri.Vertex2 = QMap_coordinateVertex.value(QVec_TableTopology[2][j]);
        tempTri.Vertex3 = QMap_coordinateVertex.value(QVec_TableTopology[3][j]);
        QVec_Triangles.append(tempTri);
    }
    setDataSource(0,QVec_Triangles,"Delaunay_Triangles");
    refreshPixmap();

    //display topological table in ui->tableWidget_Topology
    ui->tableWidget_Topology->setRowCount(QVec_TableTopology[0].count());
    ui->tableWidget_Topology->setEditTriggers(QAbstractItemView::NoEditTriggers); 
    for(int i=0;i< QVec_TableTopology[0].count();i++)
    {
        for(int j=0;j< 7 ;j++)
        {
            QTableWidgetItem *tmpItem;
            tmpItem = new QTableWidgetItem(QString::number(QVec_TableTopology[j][i]));
            ui->tableWidget_Topology->setItem(i,j , tmpItem);
        }
    }
}
/******************************
//Name: isInCircle
//Author: Xufeng
//Description: judge if the fouth point is in the circumcircle derived by triangle
//Input:ID of current triangle,ID of fourth point, the structure storing coordinates
//Output: the result of determinant value
//Date: 2014-02-28
//Version: 1.00
*****************************/

double MainWindow::isInCircle(int currentTri, int fourthPointID, QMap<int, QPointF> &QMap_coordinateVertex)
{
    InCircle inCir;
    QPointF fourthPoint = QMap_coordinateVertex.value(fourthPointID);
    QPointF V1= QMap_coordinateVertex.value(QVec_TableTopology[1][currentTri-1]);
    QPointF V2= QMap_coordinateVertex.value(QVec_TableTopology[2][currentTri-1]);
    QPointF V3= QMap_coordinateVertex.value(QVec_TableTopology[3][currentTri-1]);

    double resultInCircle = inCir.ComputeDetInCircle(fourthPoint,V1,V2,V3);
    return resultInCircle;
}

/******************************
//Name: Optimize
//Author: Xufeng
//Description: optimize triangle in the light of Delaunay Rule through swap the diagonal of two adjacent triangles
              (1) store the triangles after inserting a new point into a stack
              (2) pop-up the top triangle
              (3) find the adjacent triangle opposite to the new inserted point
              (4) calculate the H
              (5) if H<0, swap the two triangles
              (6) push the ID of two triangle into stack
              (7) repeat, until the stack is empty

//Input:ID of new point,stack, the structure storing coordinates
//Output: updated topologies
//Date: 2014-03-10
//Version: 1.00
*****************************/

void MainWindow::Optimize(int NewPointID,QVector<int> &QVec_StackOptim,
                          QMap<int, QPointF> &QMap_coordinateVertex)
{
    while(!QVec_StackOptim.isEmpty())
    {
        int i = QVec_StackOptim.count()-1;
        int currentTriID = QVec_StackOptim[i];
        QVec_StackOptim.remove(i);
        // find the position of new point and find the opposite adjacent triangle
        int positionNewP;
        for(int i=1;i<4;i++)
        {
            if(QVec_TableTopology[i][currentTriID -1] == NewPointID)
            {
                positionNewP = i;
                break;
            }
        }
        int adjacentTri = QVec_TableTopology[positionNewP + 3][currentTriID -1];

        if(adjacentTri!=0)
        {
            // find the PNC and NP for swap
            int PNC = Find_NP_PNC(adjacentTri,currentTriID,1);
            int NP = Find_NP_PNC(adjacentTri,currentTriID,2);
            double H = isInCircle(currentTriID,PNC,QMap_coordinateVertex);

            if(H<0)
            {
                //swap the two adjacent triangles
                Swap(currentTriID,NP,adjacentTri,PNC,QVec_TableTopology);
                //push the swaped triangles into stack
                QVec_StackOptim.append(adjacentTri);
                QVec_StackOptim.append(currentTriID);
            }
        }
    }
}


/*judge if PNC is in the circumcricle formed by NP,PC1,PC2
 *           NP
 *            /\
 *      T4   /  \ T3
 *          /    \
 *         / T1   \
 *    PC1 /........\PC2
 *        \  T2    /
 *         \      /
 *      T5  \    / T6
 *           \  /
 *            \/
 *            PNC
 *
 *
 *
 */
/*             NP
 *             .
 *            /.\
 *     T4    / . \  T3
 *          /  .  \
 *         /   .   \
 *    PC1 / T2 .T1  \PC2
 *        \    .    /
 *         \   .   /
 *          \  .  /
 *      T5   \ . /  T6
 *            \./
 *             .
 *            PNC
 *
 **/
// T1 ---> currentTriID
// T2 ---> adjacentTriID
// T1: NP,  PC1, PC2  T2  T3  T4
// T2: PNC, PC2, PC1  T1  T5  T6
/******************************
//Name: swap
//Author: Xufeng
//Description: (1)Decide the position of NP, PC1 and PC2 in T1, and the position of PNC, PC1 and PC2 in T2, find the corresponding vertex of NP, PC1, PC2,PNC.
               (2)Record the new T1 and T2
                    Where T6 is the adjacent triangle opposite PC1 in original T2
                    T3 is the adjacent triangle opposite PC1 in original T1
                    T5 is the adjacent triangle opposite PC2 in original T2
                    T4 is the adjacent triangle opposite PC2 in original T1
               (3)Update the topologies of T4 and T6 because the original adjacent triangle of T4 is T1, but now it is T2, similarly, the same case for T6.
                    Find the adjacent triangle in T4 which is T1 and replace it by T2
                    Find the adjacent triangle in T6 which is T1 and replace it by T1
               (4)Update the T1 and T2 by the recorded new T1 and T2

//Input:ID of current triangle, ID of new point,adjTriangle,PNC, the structure storing coordinates
//Output: updated topologies
//Date: 2014-03-10
//Version: 1.00
*****************************/
int MainWindow::Swap(int currentTriID,int newPointID,int adjacentTriID,int PointNotCommon ,QVector<int> (&QVec_TableTopology)[9])
{
    // record the position of vertexes for swap
    int PC1,PC2;
    int positionPointNotCommon;
    int positionNewPoint;
    int positionPC1T1;
    int positionPC2T1;
    int positionPC1T2;
    int positionPC2T2;

    for(int i=1;i<4;i++)
    {
        if(QVec_TableTopology[i][currentTriID-1] == newPointID)
        {
            positionNewPoint = i;
        }
    }

    if(positionNewPoint==1)
    {
        positionPC1T1 = 2;
        positionPC2T1 = 3;
    }
    if(positionNewPoint==2)
    {
        positionPC1T1 = 3;
        positionPC2T1 = 1;
    }
    if(positionNewPoint==3)
    {
        positionPC1T1 = 1;
        positionPC2T1 = 2;
    }

    PC1 = QVec_TableTopology[positionPC1T1][currentTriID-1];
    PC2 = QVec_TableTopology[positionPC2T1][currentTriID-1];

    //in adjacent triangle, PC1 and PC2 are changed
    for(int i=1;i<4;i++)
    {
        if(QVec_TableTopology[i][adjacentTriID-1] == PointNotCommon)
        {
            positionPointNotCommon = i;
        }
        if(QVec_TableTopology[i][adjacentTriID-1] == PC2 )
        {
            positionPC2T2 = i;
        }
        if(QVec_TableTopology[i][adjacentTriID-1] == PC1 )
        {
            positionPC1T2 = i;
        }
    }

    QVector<int> newCurrentTri;
    QVector<int> newAdjacentTri;

    //record new changed currentTri T1
    newCurrentTri.append(currentTriID);
    newCurrentTri.append(newPointID);
    newCurrentTri.append(PointNotCommon);
    newCurrentTri.append(PC2);
    newCurrentTri.append(QVec_TableTopology[positionPC1T2+3][adjacentTriID - 1]);//adjacent triangle opposite to PC1 in old T2
    newCurrentTri.append(QVec_TableTopology[positionPC1T1+3][currentTriID - 1]); // the adjacent triangle opposite to PC1 in old T1
    newCurrentTri.append(QVec_TableTopology[positionNewPoint+3][currentTriID - 1]); // T2

    //record new changed newAdjacentTri T2
    newAdjacentTri.append(adjacentTriID);
    newAdjacentTri.append(newPointID);
    newAdjacentTri.append(PC1);
    newAdjacentTri.append(PointNotCommon);
    newAdjacentTri.append(QVec_TableTopology[positionPC2T2+3][adjacentTriID - 1]); //adjacent triangle opposite to PC2 in old T2
    newAdjacentTri.append(QVec_TableTopology[positionPointNotCommon+3][adjacentTriID - 1]);//T1
    newAdjacentTri.append(QVec_TableTopology[positionPC2T1+3][currentTriID - 1]); //adjacent triangle opposite to PC2 in old T1

    //find the adjacent triangle T4 opposite PC1 in T1
    int T1_PC2_Adj = QVec_TableTopology[positionPC2T1+3][currentTriID - 1];

    //find the adjacent triangle T6 opposite PC2 in T2
    int T2_PC1_Adj = QVec_TableTopology[positionPC1T2+3][adjacentTriID - 1];

    //update T4 and T6
    //update T1 in the T4 and replaced by T2
     if(T1_PC2_Adj!=0)
     {
        int n;
        for(int i=4; i< 7; i++)
        {
            if(QVec_TableTopology[i][T1_PC2_Adj - 1] == currentTriID)
            {
                n=i;
                break;
            }
        }
        QVec_TableTopology[n][T1_PC2_Adj - 1] = adjacentTriID;
    }

    //update T2 in the T6 and replaced by T1
     if(T2_PC1_Adj!=0)
     {
        int m;
        for(int i=4; i< 7; i++)
        {
            if(QVec_TableTopology[i][T2_PC1_Adj - 1] == adjacentTriID)
            {
                m=i;
                break;
            }
        }

        QVec_TableTopology[m][T2_PC1_Adj - 1] = currentTriID;
    }

    //update T1 and T2
    for(int i=0;i<newCurrentTri.count();i++)
    {
        QVec_TableTopology[i][currentTriID - 1] = newCurrentTri[i];
        QVec_TableTopology[i][adjacentTriID - 1] = newAdjacentTri[i];
    }
}


/******************************
//Name: Find_NP_PNC
//Author: Xufeng
//Description: find the NewPoint and PointNonCommon
//Input:ID of adjacent triangle, ID of current triangle,flag
//Output: flag =1 return pointNonCommon, it belongs to adjacentTriID
          flag =2 return newpoint, it belongs to currentTriID
//Date: 2014-03-02
//Version: 1.00
*****************************/

int MainWindow::Find_NP_PNC(int adjacentTriID,int currentTriID,int flag)
{

    int NewPointID,pointNonCommon;
    QVector<int> vertexCurrTri;
    QVector<int> vertexAdjTri;

    vertexCurrTri.append(QVec_TableTopology[1][currentTriID -1]);
    vertexCurrTri.append(QVec_TableTopology[2][currentTriID -1]);
    vertexCurrTri.append(QVec_TableTopology[3][currentTriID -1]);

    vertexAdjTri.append(QVec_TableTopology[1][adjacentTriID -1]);
    vertexAdjTri.append(QVec_TableTopology[2][adjacentTriID -1]);
    vertexAdjTri.append(QVec_TableTopology[3][adjacentTriID -1]);

    for( int i= 0; i< vertexCurrTri.count(); i++)
    {
        if(!vertexAdjTri.contains(vertexCurrTri[i]))
        {
            NewPointID = vertexCurrTri[i];
        }
    }

    for( int j= 0; j< vertexAdjTri.count(); j++)
    {
        if(!vertexCurrTri.contains(vertexAdjTri[j]))
        {
            pointNonCommon = vertexAdjTri[j];
        }
    }

    vertexCurrTri.clear();
    vertexAdjTri.clear();

    if(flag==1)
    {
        return pointNonCommon;
    }
    if(flag==2)
    {
        return NewPointID;
    }
}



void MainWindow::paintEvent(QPaintEvent * /*event */)
{

    QStylePainter painter(this);
    painter.setBackground(Qt::white);
    painter.drawPixmap(ui->Pixmapframe->pos().x()+PixMapLeftMargin,
         ui->Pixmapframe->pos().y()+ui->menuBar->height()+ui->mainToolBar->height()+PixMapTopMargin,  pixmap);

}

void MainWindow::resizeEvent(QResizeEvent *)
{
    int RightMarginforButton = 250;

    ui->Pixmapframe->setGeometry(10,5 + PixMapTopMargin,this->width() - RightMarginforButton , height()-PixMapTopMargin-50);
    int x=ui->Pixmapframe->width()+ 25 + PixMapTopMargin;

    ui->groupBox1->setGeometry(x,10 + PixMapTopMargin,200,180);
    ui->groupBox3->setGeometry(x,10+10+ ui->groupBox1->height() + PixMapTopMargin,200, height()-PixMapTopMargin-70 - ui->groupBox1->height());

    refreshPixmap();
}

void MainWindow::DisplayWalkedTriangles(QVector<int> WalkedTriangleID)
{
    QMapIterator<int, QVector<Triangle> > i(QMap_TriangleWalked);
    while (i.hasNext())
    {
        i.next();
        int id = i.key();
        QMap_TriangleWalked.remove(id);
    }

    QVector<Triangle> QVec_TrianglesWalked;
    //QVec_TrianglesWalked.clear();
    for(int i=0; i< WalkedTriangleID.count();i++ )
    {
        Triangle tempTri;
        tempTri.Vertex1 = QMap_coordinateVertex.value(QVec_TableTopology[1][WalkedTriangleID[i]]);
        tempTri.Vertex2 = QMap_coordinateVertex.value(QVec_TableTopology[2][WalkedTriangleID[i]]);
        tempTri.Vertex3 = QMap_coordinateVertex.value(QVec_TableTopology[3][WalkedTriangleID[i]]);
        QVec_TrianglesWalked.append(tempTri);
    }

    setDataSource(0,QVec_TrianglesWalked,"Walked_Triangle_Delaunay");
    QVec_TrianglesWalked.clear();
    refreshPixmap();
}


// judge the clicked point to make sure all the points in the global triangle
QVector<double>  MainWindow::RestrictNewPointInGlobalTriangle( double x, double y)
{
    QVector<QPointF> triangle1;
    QVector<QPointF> triangle2;
    QVector<QPointF> triangle3;
    triangle1.clear();
    triangle2.clear();
    triangle3.clear();

    QPointF newPoint  = QPointF(x,y);
    QPointF V1= QVec_GlobalTriangle[0];
    QPointF V2= QVec_GlobalTriangle[1];
    QPointF V3= QVec_GlobalTriangle[2];

    triangle1.append(V1);
    triangle1.append(V2);
    triangle1.append(newPoint);

    triangle2.append(V2);
    triangle2.append(V3);
    triangle2.append(newPoint);

    triangle3.append(V3);
    triangle3.append(V1);
    triangle3.append(newPoint);

    ComputeTriArea triDet;
    double Result1_12P = triDet.ComputeDet(triangle1);
    double Result2_23P = triDet.ComputeDet(triangle2);
    double Result3_31P = triDet.ComputeDet(triangle3);

    QVector<double> result;
    result.append(Result1_12P);
    result.append(Result2_23P);
    result.append(Result3_31P);
    return result;
}

/******************************
//Name: SelectNearestVertexID_TriID
//Author: Xufeng
//Description: find the nearest point that is nearest to clicked point from the triangle contains newpoint and its adjacent
//Input:new point, ID of current triangle, structure for storing coordinates
//Output: return the ID of nearest vertex and triangle contains it
//Date: 2014-03-05
//Version: 1.00
*****************************/
QVector<int> MainWindow::SelectNearestVertexID_TriID(QPointF newPoint, int triID, QMap<int, QPointF> &QMap_coordinateVertex)
{
    int j = triID -1 ;
    int V1ID = QVec_TableTopology[1][j];
    int V2ID = QVec_TableTopology[2][j];
    int V3ID = QVec_TableTopology[3][j];

    QPointF V1= QMap_coordinateVertex.value(QVec_TableTopology[1][j]);
    QPointF V2= QMap_coordinateVertex.value(QVec_TableTopology[2][j]);
    QPointF V3= QMap_coordinateVertex.value(QVec_TableTopology[3][j]);


    QVector<QPointF> VertexTmp;
    QVector<int> VertexTmpID;
    QVector<int> TriID_NearestIn;

    TriID_NearestIn.append(QVec_TableTopology[0][j]);

    VertexTmp.append(V1);
    VertexTmp.append(V2);
    VertexTmp.append(V3);

    VertexTmpID.append(V1ID);
    VertexTmpID.append(V2ID);
    VertexTmpID.append(V3ID);


    int AdjIDTri1 = QVec_TableTopology[4][j];
    int AdjIDTri2 = QVec_TableTopology[5][j];
    int AdjIDTri3 = QVec_TableTopology[6][j];

    if(AdjIDTri1 != 0)
    {
        int PNC1 = Find_NP_PNC(AdjIDTri1,triID,1);
        QPointF PA1= QMap_coordinateVertex.value(PNC1);
        VertexTmp.append(PA1);
        VertexTmpID.append(PNC1);
        TriID_NearestIn.append(AdjIDTri1);
    }
    if(AdjIDTri2 != 0)
    {
        int PNC2 = Find_NP_PNC(AdjIDTri2,triID,1);
        QPointF PA2= QMap_coordinateVertex.value(PNC2);
        VertexTmp.append(PA2);
        VertexTmpID.append(PNC2);
        TriID_NearestIn.append(AdjIDTri2);
    }
    if(AdjIDTri3 != 0)
    {
        int PNC3 = Find_NP_PNC(AdjIDTri3,triID,1);
        QPointF PA3= QMap_coordinateVertex.value(PNC3);
        VertexTmp.append(PA3);
        VertexTmpID.append(PNC3);
        TriID_NearestIn.append(AdjIDTri3);
    }

    QVector<double> distance;
    double d1 = DistanceTwoPoints(newPoint,VertexTmp[0]);
    double MinDis=0;
    int MinPosition=0;
    MinDis = d1;
    int nearestTriID =0;
    for(int i=0; i< VertexTmp.count(); ++i)
    {
        distance.append( DistanceTwoPoints(newPoint,VertexTmp[i]) );
    }

    for(int i=1; i< distance.count(); ++i)
    {
        if( distance[i] < MinDis)
        {
            MinDis = distance[i];
            MinPosition = i;
        }
    }

    int MinDisVertexID = VertexTmpID[MinPosition];

    if(MinPosition < 3)
    {
        nearestTriID = TriID_NearestIn[0];
    }
    else
    {
        nearestTriID = TriID_NearestIn[MinPosition - 2];
    }

    VertexTmp.clear();
    VertexTmpID.clear();
    TriID_NearestIn.clear();

    QVector<int> result;
    result.clear();
    result.append(MinDisVertexID);
    result.append(nearestTriID);
    return result;
}
/******************************
//Name: SelectNearestVertexes
//Author: Xufeng
//Description: (1) find the position of selected point V1
               (2) find the point V2 after the selected point according to anticlockwise
               (3) find the adjacent triangle opposite to this point V2
               (4) continue
               (5) if the adjacent trinagle equals to the orginal currient triangle, break and stop

//Input:ID of selected point, ID of current triangle, structure for storing coordinates
//Output: return the ID of nearest neighboring vertexes
//Date: 2014-03-05
//Version: 1.00
*****************************/
QVector<int> MainWindow::SelectNearestVertexes(int SelectedPointID, int currentTriID, QMap<int, QPointF> &QMap_coordinateVertex)
{
    int originalTriID = currentTriID ;
    int currTri = currentTriID;

    QVector<int> nearestNeigborVeterxID;
    int positionOfSelectedPoint;

    QVec_VoronoiCell_NeigbhorTriangles.clear();
    QVec_VoronoiCell_NeigbhorTriangles.append(originalTriID);

    while(1)
    {
        QVector<int> VeterxID;

        int V1ID = QVec_TableTopology[1][currTri -1];
        int V2ID = QVec_TableTopology[2][currTri -1];
        int V3ID = QVec_TableTopology[3][currTri -1];

        VeterxID.append(V1ID);
        VeterxID.append(V2ID);
        VeterxID.append(V3ID);

        for(int i= 0; i< VeterxID.count();i++)
        {
            if(VeterxID[i] == SelectedPointID)
            {
                positionOfSelectedPoint = i;
                break;
            }
        }

        int PositionAfterSelectedPoint_1 = (positionOfSelectedPoint + 1)%3 + 1;
        nearestNeigborVeterxID.append(QVec_TableTopology[PositionAfterSelectedPoint_1][currTri -1]);

        int adjacentTri = QVec_TableTopology[PositionAfterSelectedPoint_1 + 3][currTri -1];
        if(adjacentTri == originalTriID)
        {
            break;
        }
        else
        {
            QVec_VoronoiCell_NeigbhorTriangles.append(adjacentTri);
            currTri = adjacentTri;
        }

        VeterxID.clear();
    }
    return nearestNeigborVeterxID;
}

double MainWindow::DistanceTwoPoints(QPointF P1,QPointF P2)
{
    double d = qSqrt(qPow((P1.x() - P2.x()),2) + qPow((P1.y() - P2.y()),2) );
    return d;
}

/******************************
//Name: CalculateCenterOfCircumcircle
//Author: Xufeng
//Description: calculate the center of circumcircle of Delaunay triangles
//Input:ID of selected point, ID of current triangle, structure for storing coordinates
//Output: the QPoint of center
//Date: 2014-03-06
//Version: 1.00
*****************************/
QPointF MainWindow::CalculateCenterOfCircumcircle(int TriID)
{
    ComputeCircumcircleCenter CircumCenter;
    int j = TriID -1;
    int V1ID = QVec_TableTopology[1][j];
    int V2ID = QVec_TableTopology[2][j];
    int V3ID = QVec_TableTopology[3][j];

    QPointF V1= QMap_coordinateVertex.value(QVec_TableTopology[1][j]);
    QPointF V2= QMap_coordinateVertex.value(QVec_TableTopology[2][j]);
    QPointF V3= QMap_coordinateVertex.value(QVec_TableTopology[3][j]);

    QPointF Center = CircumCenter.ComputeCenterCircumcircle(V1,V2,V3);
    return Center;
}
/******************************
//Name: CalculateAllCenterOfCircumcircle
//Author: Xufeng
//Description: calculate all the center of circumcircle of Delaunay triangles
//Input:ID of selected point, ID of current triangle, structure for storing coordinates
//Output:
//Date: 2014-03-06
//Version: 1.00
*****************************/
void MainWindow::CalculateAllCenterOfCircumcircle(QVector<int> (&QVec_TableTopology)[9])
{
    QMapIterator <int, QPointF> i(QMap_coordinateCircumcircleCenter);
    while (i.hasNext())
    {
        i.next();
        int id = i.key();
        QMap_coordinateCircumcircleCenter.remove(id);
    }

    for(int i=0;i<QVec_TableTopology[0].count();i++)
    {
        QPointF Pcenter = CalculateCenterOfCircumcircle(QVec_TableTopology[0][i]);
        QMap_coordinateCircumcircleCenter.insert(QVec_TableTopology[0][i], Pcenter);
    }
}
/******************************
//Name: CalculateVoronoiCellCenterOfCircumcircle
//Author: Xufeng
//Description: calculate the center of circumcircle of Delaunay triangles belong to Voronoi cell
//Input:ID of selected point, ID of current triangle, structure for storing coordinates
//Output:
//Date: 2014-03-06
//Version: 1.00
*****************************/
void MainWindow::CalculateVoronoiCellCenterOfCircumcircle(QVector<int> celltriangleID,QVector<int> (&QVec_TableTopology)[9])
{
    QVec_VoronoiCellPloygon.clear();
    for(int i=0;i<celltriangleID.count();i++)
    {
        QPointF Pcenter = CalculateCenterOfCircumcircle(celltriangleID[i]);
        //QMap_coordinateCircumcircleCenterOfVoronoiCell.insert(celltriangleID[i], Pcenter);
        QVec_VoronoiCellPloygon.append(Pcenter);
    }
}
/******************************
//Name: CreateVoronoiLineSegments
//Author: Xufeng
//Description: connect the Voronoi edge according to the topologies of Delaunay triangulation
//Input:ID of selected point, ID of current triangle, structure for storing coordinates
//Output:
//Date: 2014-03-06
//Version: 1.00
*****************************/
void MainWindow::CreateVoronoiLineSegments(QVector<int> (&QVec_TableTopology)[9])
{
    for(int i=0;i<QVec_TableTopology[0].count();i++)
    {
        int triID = QVec_TableTopology[0][i];

        int adj1ID = QVec_TableTopology[4][i];
        int adj2ID = QVec_TableTopology[5][i];
        int adj3ID = QVec_TableTopology[6][i];

        if(adj1ID!=0 )
        {
            LineSegments l1;
            l1.LineSegments_StartVertex = QMap_coordinateCircumcircleCenter.value(triID);
            l1.LineSegments_EndVertex = QMap_coordinateCircumcircleCenter.value(adj1ID);
            QVec_VoronoiLines.append(l1);

        }
        if(adj2ID!=0)
        {
            LineSegments l2;
            l2.LineSegments_StartVertex = QMap_coordinateCircumcircleCenter.value(triID);
            l2.LineSegments_EndVertex = QMap_coordinateCircumcircleCenter.value(adj2ID);
            QVec_VoronoiLines.append(l2);
        }
        if(adj3ID!=0)
        {
            LineSegments l3;
            l3.LineSegments_StartVertex = QMap_coordinateCircumcircleCenter.value(triID);
            l3.LineSegments_EndVertex = QMap_coordinateCircumcircleCenter.value(adj3ID);
            QVec_VoronoiLines.append(l3);
        }
    }
    setDataSource(0, QVec_VoronoiLines, "Voronoi_Graph");
    QVec_VoronoiLines.clear();
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QRect rect(ui->Pixmapframe->pos().x() + PixMapLeftMargin + LeftMargin,
               ui->Pixmapframe->pos().y()+ui->menuBar->height()+ui->mainToolBar->height()+ TopMargin + PixMapTopMargin,
               ui->Pixmapframe->width() - RightMargin - LeftMargin - PixMapLeftMargin - PixMapRightMargin + TopMargin + PixMapTopMargin,
               ui->Pixmapframe->height() - TopMargin - BottomMargin - PixMapTopMargin - PixMapBottomMargin);
    if (event->button() == Qt::LeftButton) {  //left button
        if (rect.contains(event->pos()))
        {
            setCursor(Qt::CrossCursor);//set mouse shape as Cross
            int posX = event->pos().x();
            int posY = event->pos().y();

            double xcoord = approximateMinX + ((event->pos().x() - (ui->Pixmapframe->pos().x() + PixMapLeftMargin + LeftMargin))*(RangeX)
                                                      /(ui->Pixmapframe->width() - RightMargin - LeftMargin-PixMapLeftMargin-PixMapRightMargin)) ;

            double ycoord = approximateMinY + (rect.top()+ rect.height() - event->pos().y() )*(RangeY)
                                                           /(ui->Pixmapframe->height() - TopMargin - BottomMargin -PixMapTopMargin -PixMapBottomMargin);

            QVector<double>  result = RestrictNewPointInGlobalTriangle(xcoord, ycoord);

            //make sure the inserted point  is in the global triangle
            if(result[0] >0 && result[1] >0 && result[2] >0)
            {
                if(isInsertPoint)
                {
                    QMap_coordinateVertex.insert(numberVertex, QPointF(xcoord,ycoord));
                    QVec_VertexCoord.append(QPointF(xcoord,ycoord));

                    int triID = Walk(numberVertex,QMap_coordinateVertex);

                    if(isDisplayWalkedTriangels)
                    {
                        DisplayWalkedTriangles(WalkedTriangleID);
                    }

                    Insert(numberVertex,triID);

                    if(QVec_TableTopology[0].count()>3)
                    {
                      Optimize(numberVertex,QVec_StackOptim,QMap_coordinateVertex);
                    }

                    if(isDisplayDelaunay)
                    {
                        DisplayAllTriangles(QVec_TableTopology,QMap_coordinateVertex);
                    }

                    setDataSource(0, QVec_VertexCoord, "Vertex_All");
                    numberVertex++;

                }


                if(isSelectNeighbor)
                {
                    if(QVec_TableTopology[0].count()>3)
                    {
                        QPointF newPoint = QPointF(xcoord,ycoord);
                        //walk to the triangle contains the clicked point
                        int triID = Walk(newPoint,QMap_coordinateVertex);

                        if(isDisplayWalkedTriangels)
                        {
                            DisplayWalkedTriangles(WalkedTriangleID);
                        }
                        //get nearest point of clicked point and included TriID
                        QVector<int> result;
                        result = SelectNearestVertexID_TriID(newPoint,triID,QMap_coordinateVertex);
                        int selectedNearestPointID = result[0];
                        int vertexInTriID = result[1];
                        result.clear();

                        //make sure the selected point is not one of the vertex of global triangle
                        if(selectedNearestPointID!=1 && selectedNearestPointID!=2 && selectedNearestPointID!=3)
                        {
                            //get the coordinate of nearest points from QMap_coordinateVertex
                            QPointF nearestPoint = QMap_coordinateVertex.value(selectedNearestPointID);
                            //display the "nearestPoint" selected by mouse
                            selectedVertex.clear();
                            selectedVertex.append(nearestPoint);
                            setDataSource(0,selectedVertex,"Selected_Point");


                            //display the "nearest neighboring of selected points
                            QVector<int> NearestPoints = SelectNearestVertexes(selectedNearestPointID, vertexInTriID, QMap_coordinateVertex);
                            for(int i=0;i< NearestPoints.count();i++)
                            {
                                nearestNeignbors_selectedVertex.append(QMap_coordinateVertex.value(NearestPoints[i]));
                            }
                            setDataSource(0,nearestNeignbors_selectedVertex,"Nearest_Neighbors");
                            nearestNeignbors_selectedVertex.clear();
                            NearestPoints.clear();
                        }
                        else
                        {
                            QMessageBox::warning(this,"Tips"," The vertex number 1,2 and 3 are not effective ones for find nearest vertexes\n if you select the checkBox for displaying nearest neighbor, please cancle it");
                        }
                    }


                }
                if(isDisplayVoronoi)
                {
                    CalculateAllCenterOfCircumcircle(QVec_TableTopology);
                    CreateVoronoiLineSegments(QVec_TableTopology);
                }

                if(isDisplayVoronoiCell)
                {
                    QPointF newPoint = QPointF(xcoord,ycoord);
                    //walk to the triangle contains the clicked point
                    int triID = Walk(newPoint,QMap_coordinateVertex);
                    if(isDisplayWalkedTriangels)
                    {
                        DisplayWalkedTriangles(WalkedTriangleID);
                    }
                    //get nearest point of clicked point and included TriID
                    QVector<int> result;
                    result = SelectNearestVertexID_TriID(newPoint,triID,QMap_coordinateVertex);

                    int selectedNearestPointID = result[0];
                    int vertexInTriID = result[1];
                    result.clear();
                    //make sure the selected point is not one of the vertex of global triangle
                    if(selectedNearestPointID!=1 && selectedNearestPointID!=2 && selectedNearestPointID!=3)
                    {
                        //get the coordinate of nearest points from QMap_coordinateVertex
                        QPointF nearestPoint = QMap_coordinateVertex.value(selectedNearestPointID);
                        //display the "nearestPoint" selected by mouse

                        selectedVertexVoronoi.append(nearestPoint);
                        setDataSource(0,selectedVertexVoronoi,"Selected_Point");
                        selectedVertexVoronoi.clear();

                        //display the "nearest neighbors of selected points
                        QVector<int> NearestPoints = SelectNearestVertexes(selectedNearestPointID, vertexInTriID, QMap_coordinateVertex);

                        CalculateVoronoiCellCenterOfCircumcircle(QVec_VoronoiCell_NeigbhorTriangles,QVec_TableTopology);
                        setDataSource(0,QVec_VoronoiCellPloygon,"Voronoi_Cell");

                        QVec_VoronoiCell_NeigbhorTriangles.clear();
                        QVec_VoronoiCellPloygon.clear();
                        NearestPoints.clear();
                        refreshPixmap();
                    }
                    else
                    {
                        QMessageBox::warning(this,"Tips"," The vertex number 1,2 and 3 are not effective ones for find nearest vertexes\n if you select the checkBox for displaying nearest neighbor, please cancle it");
                    }
                }

            }
        }
    }

}



void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{

        if (event->button() == Qt::RightButton)  //erase all the points and init
        {
            QVec_VertexCoord.clear();
            setDataSource(0, QVec_VertexCoord, "Vertex_All");

            QVec_Triangles.clear();
            setDataSource(0,QVec_Triangles,"Delaunay_Triangles");

            for(int i = 0; i< 9;i++)
            {
                QVec_TableTopology[i].clear();
            }

           // QVec_TrianglesWalked.clear();
           // setDataSource(0,QVec_TrianglesWalked,"Walked_Triangles");

            ui->tableWidget_Topology->clear();

            //clear data structures for nearest neighbors
            selectedVertex.clear();
            setDataSource(0,selectedVertex,"Selected_Point");

            nearestNeignbors_selectedVertex.clear();
            setDataSource(1,nearestNeignbors_selectedVertex,"Nearest_Neighbors");

           //clear data structures for Voronoi
            QVec_VoronoiLines.clear();
            setDataSource(0, QVec_VoronoiLines, "Voronoi_Graph");

            QMapIterator <int, QPointF> i(QMap_coordinateCircumcircleCenter);
            while (i.hasNext())
            {
                i.next();
                int id = i.key();
                QMap_coordinateCircumcircleCenter.remove(id);
            }

            QVec_VoronoiCellPloygon.clear();
            setDataSource(0,QVec_VoronoiCellPloygon,"Voronoi_Cell");

            Init();
            refreshPixmap();
        }
        else
        {
            setCursor(Qt::ArrowCursor);//set mouse shape as Arrow
        }
}

void MainWindow::refreshPixmap()
{
    pixmap = QPixmap(ui->Pixmapframe->size().width()-PixMapLeftMargin -PixMapRightMargin,
                     ui->Pixmapframe->size().height()-PixMapTopMargin -PixMapBottomMargin);
    pixmap.fill(this, 0, 0);
    pixmap.fill(Qt::white);//background colour

    QPainter painter(&pixmap);
    painter.initFrom(this);

    if(RangeX >= 10 && RangeY >= 10)
    {
        drawGrid(&painter);
        drawCurves(&painter);
    }

    update(); //update the pixmap
}



void MainWindow::drawGrid(QPainter *painter)
{
    QRect rect(LeftMargin, TopMargin ,
               ui->Pixmapframe->width() - RightMargin - LeftMargin  - PixMapRightMargin - PixMapLeftMargin,
               ui->Pixmapframe->height() - TopMargin - BottomMargin - PixMapTopMargin - PixMapBottomMargin);

    if (!rect.isValid())
        return;

    int numXTicks = RangeX/10;
    int numYTicks = RangeY/10;

    QPen quiteDark = QPen(Qt::black);

    //Draw X Axis
    for (int i = 0; i <= numXTicks; ++i)
    {
        int x = rect.left() + (i * (rect.width() - 1)
                                 / numXTicks);
        double label = approximateMinX + (i * RangeX/numXTicks);
        if(qAbs(label-0)<0.000001)
        {
            label=0;
            painter->setPen(QPen(Qt::blue,1));
            painter->drawLine(x,rect.top(), x, rect.bottom());//draw Y axis
        }

//        painter->setPen(QPen(Qt::black));
//        painter->setPen(Qt::DotLine);
//        painter->drawLine(x, rect.top(), x, rect.bottom());

        //draw scales
        painter->setPen(QPen(Qt::blue,2));
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);//draw more 5 pixeles downward
        painter->setPen(QPen(Qt::black));

        painter->drawText(x-30, rect.bottom() + 8, 60, 15,
                          Qt::AlignHCenter | Qt::AlignTop | Qt::AlignJustify,
                          QString::number(label));
    }


    //draw Y axis
    for (int j = 0; j <= numYTicks; ++j) {
        int y = rect.bottom() - (j * (rect.height() - 1)
                                   / numYTicks);
        double label = approximateMinY + (j * RangeY/ numYTicks);
       // qDebug()<<"y"<<label;
        if(qAbs(label-0)<0.000001)
        {
            label=0;
            painter->setPen(QPen(Qt::blue,1));
            painter->drawLine(rect.left() - 5, y, rect.right(), y);//draw Y axis

        }
//        painter->setPen(quiteDark);
//        painter->setPen(Qt::DotLine);
//        painter->drawLine(rect.left(), y, rect.right(), y);

        //draw scale
        painter->setPen(QPen(Qt::blue,2));
        painter->drawLine(rect.left() - 5, y, rect.left(), y);//draw more 5 scale to left
        painter->drawLine(rect.right() - 5, y, rect.right(), y);
        painter->setPen(quiteDark);

        //draw the number of scale
        painter->drawText(rect.left() - Margin, y - 10, Margin - 10, 20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(label));
    }

    painter->setPen(QPen(Qt::blue,1));
    painter->drawRect(rect.adjusted(0, 0, -1, -1));

}



void MainWindow::drawSign(double x,double y,QPainter *painter,int size)
{
    painter->drawEllipse(x-size,y-size,2*size,2*size);
}
void MainWindow::drawCoordinateText(double x,double y,QPainter *painter,QString text)
{
    painter->drawText(x+5, y-7, 80, 15,
                      Qt::AlignLeft | Qt::AlignTop | Qt::AlignJustify,
                     text);
}

void MainWindow::drawCurves(QPainter *painter)
{

    static const QColor colorForIds[10] = {
        QColor(0,0,0),//black
        QColor(0,0,255),//Qt::blue,
        Qt::red,
        Qt::green,
        Qt::magenta,
        QColor(0,128,0),// dark green
        QColor(189,95,0),// brown
        QColor(243,58,100),//
        QColor(128,0,255),// voilet
        QColor(245,169,54)//green brown
    };

    //the range for draw polygon
    QRect rect(LeftMargin, TopMargin ,
               ui->Pixmapframe->width() - RightMargin - LeftMargin  - PixMapRightMargin - PixMapLeftMargin,
               ui->Pixmapframe->height() - TopMargin - BottomMargin - PixMapTopMargin - PixMapBottomMargin);

    if (!rect.isValid())
        return;

    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

    //draw points clicked by mouse
    QMapIterator <int, QPointF> i1(QMap_coordinateVertex);
    QPolygonF polyline;
    while (i1.hasNext())
    {
        i1.next();
        int id = i1.key();
        const QPointF &data = i1.value();

        double dx = data.x() - approximateMinX;
        double dy = data.y() - approximateMinY;
        double x = rect.left() + (dx * (rect.width()-1 )
                                     / RangeX);
        double y = rect.bottom() - (dy * (rect.height() -1)
                                       / RangeY);
        polyline.append(QPointF(x, y)) ;
        painter->setPen(QPen(Qt::red));

        if(isDisplayCoordinate)
        {
            painter->setPen(QPen(Qt::blue,4));
            drawCoordinateText(x,y,painter,"["+QString::number(id)+"]");
            painter->setPen(QPen(Qt::red));
            drawCoordinateText(x,y+10,painter,"("+QString::number(data.x(),'g',3)+","
                               +QString::number(data.y(),'g',3)+")");
        }
        painter->setPen(QPen(Qt::red,5));
        drawSign(x,y,painter,2);

    }
    //draw points
    painter->setPen(QPen(Qt::red,3));
    painter->drawPoints(polyline);


    //draw global triangle
    QMapIterator<int, QVector<QPointF> > i2(QMap_GlobalTriangle);
    while (i2.hasNext())
    {
        i2.next();
        const QVector<QPointF> &data = i2.value();
        QPolygonF polyline(data.count());

        // transfer the coordinate to the position on the pixmap
        for (int j = 0; j < data.count(); ++j)
        {

            double dx = data[j].x() - approximateMinX;
            double dy = data[j].y() - approximateMinY;
            double x = rect.left() + (dx * (rect.width() -1)
                                         / RangeX);
            double y = rect.bottom() - (dy * (rect.height() -1)
                                           / RangeY);
            polyline[j] = QPointF(x, y);
            painter->setPen(QPen(Qt::red,3));

        }
        //draw points

        painter->setPen(QPen(QColor(0,0,255,80),3));
        painter->drawPoints(polyline);

        //draw triangle
        for (int j = 0; j < data.count(); ++j)
        {
            painter->drawLine(polyline[j],polyline[(j+1)%data.count()]);
        }
    }


    //draw all the triangle
    if(isDisplayDelaunay)
    {
        QMapIterator<int, QVector<Triangle> > i3(QMap_Triangle);
        while (i3.hasNext())
        {
            i3.next();
            const QVector<Triangle> &data = i3.value();
            // transfer the coordinate to the position on the pixmap
            for (int j = 0; j < data.count(); ++j)
            {

                double dx1 = data[j].Vertex1.x() - approximateMinX;
                double dy1 = data[j].Vertex1.y() - approximateMinY;
                double x1 = rect.left() + (dx1 * (rect.width() -1)
                                             / RangeX);
                double y1 = rect.bottom() - (dy1 * (rect.height() -1)
                                               / RangeY);

                double dx2 = data[j].Vertex2.x() - approximateMinX;
                double dy2 = data[j].Vertex2.y() - approximateMinY;
                double x2 = rect.left() + (dx2 * (rect.width() -1)
                                             / RangeX);
                double y2 = rect.bottom() - (dy2 * (rect.height() -1)
                                               / RangeY);

                double dx3 = data[j].Vertex3.x() - approximateMinX;
                double dy3 = data[j].Vertex3.y() - approximateMinY;
                double x3 = rect.left() + (dx3 * (rect.width() -1)
                                             / RangeX);
                double y3 = rect.bottom() - (dy3 * (rect.height() -1)
                                               / RangeY);  
                //draw polygon 
                painter->setPen(QPen(Qt::black));

                painter->drawLine(QPointF(x1,y1),QPointF(x2,y2));
                painter->drawLine(QPointF(x2,y2),QPointF(x3,y3));
                painter->drawLine(QPointF(x3,y3),QPointF(x1,y1));

            }
        }
    }

    //draw all the walked triangle
    if(isDisplayWalkedTriangels)
    {

        QMapIterator<int, QVector<Triangle> > i4(QMap_TriangleWalked);
        while (i4.hasNext())
        {
            i4.next();
            const QVector<Triangle> &data = i4.value();

            // transfer the coordinate to the position on the pixmap
            for (int j = 0; j < data.count(); ++j)
            {

                double dx1 = data[j].Vertex1.x() - approximateMinX;
                double dy1 = data[j].Vertex1.y() - approximateMinY;
                double x1 = rect.left() + (dx1 * (rect.width() -1)
                                             / RangeX);
                double y1 = rect.bottom() - (dy1 * (rect.height() -1)
                                               / RangeY);

                double dx2 = data[j].Vertex2.x() - approximateMinX;
                double dy2 = data[j].Vertex2.y() - approximateMinY;
                double x2 = rect.left() + (dx2 * (rect.width() -1)
                                             / RangeX);
                double y2 = rect.bottom() - (dy2 * (rect.height() -1)
                                               / RangeY);

                double dx3 = data[j].Vertex3.x() - approximateMinX;
                double dy3 = data[j].Vertex3.y() - approximateMinY;
                double x3 = rect.left() + (dx3 * (rect.width() -1)
                                             / RangeX);
                double y3 = rect.bottom() - (dy3 * (rect.height() -1)
                                               / RangeY);

                QPolygonF polyline;
                polyline.append(QPointF(x1, y1));
                polyline.append(QPointF(x2, y2));
                polyline.append(QPointF(x3, y3));

                QPainterPath TriPath;
                TriPath.addPolygon(polyline);
                polyline.clear();

                QBrush fillBrush(QColor(255,0,0,50), Qt::SolidPattern);//set brush for filling the walked triangles
                painter->setPen(QPen(QColor(255,0,0,50),3));

                painter->setBrush(fillBrush);
                painter->drawPath(TriPath);

                painter->drawLine(QPointF(x1,y1),QPointF(x2,y2));
                painter->drawLine(QPointF(x2,y2),QPointF(x3,y3));
                painter->drawLine(QPointF(x3,y3),QPointF(x1,y1));


            }
        }

    }
   //display the selected point
    if(isSelectNeighbor || isDisplayVoronoiCell)
    {
        QMapIterator <int, QVector<QPointF> > i5_1(QMap_SelectedPoint);
        QPolygonF polyline;
        while (i5_1.hasNext())
        {
            i5_1.next();
            int id = i5_1.key();
            const QVector<QPointF> &data = i5_1.value();

            for (int j = 0; j < data.count(); ++j)
            {
                double dx = data[j].x() - approximateMinX;
                double dy = data[j].y() - approximateMinY;
                double x = rect.left() + (dx * (rect.width()-1 )
                                             / RangeX);
                double y = rect.bottom() - (dy * (rect.height() -1)
                                               / RangeY);
                polyline.append(QPointF(x, y)) ;
                painter->setPen(QPen(Qt::blue,2));

               // painter->setPen(QPen(colorForIds[(uint(id)) % 10],3));
                drawSign(x,y,painter,6);
             }
        }
        //draw points
        painter->setPen(QPen(Qt::red,5));
        painter->drawPoints(polyline);
    }

    //display the nearest points of selected points
    if(isSelectNeighbor)
    {
        QMapIterator <int, QVector<QPointF> > i5(QMap_NearestNeighbors);
        QPolygonF polyline;
        while (i5.hasNext())
        {
            i5.next();
            int id = i5.key();
            const QVector<QPointF> &data = i5.value();

            for (int j = 0; j < data.count(); ++j)
            {
                double dx = data[j].x() - approximateMinX;
                double dy = data[j].y() - approximateMinY;
                double x = rect.left() + (dx * (rect.width()-1 )
                                             / RangeX);
                double y = rect.bottom() - (dy * (rect.height() -1)
                                               / RangeY);
                polyline.append(QPointF(x, y)) ;
                //painter->setPen(QPen(Qt::green,2));

                painter->setPen(QPen(colorForIds[(uint(id)) % 10],3));
                drawSign(x,y,painter,6);
             }
        }
        //draw points
        painter->setPen(QPen(Qt::red,5));
        painter->drawPoints(polyline);
    }

    //display the circumcircle center
    if(isDisplayVoronoi)
    {
        QMapIterator <int, QPointF> i6(QMap_coordinateCircumcircleCenter);
        QPolygonF polyline;
        while (i6.hasNext())
        {
            i6.next();
            int id = i6.key();
            const QPointF &data = i6.value();


                double dx = data.x() - approximateMinX;
                double dy = data.y() - approximateMinY;
                double x = rect.left() + (dx * (rect.width()-1 )
                                             / RangeX);
                double y = rect.bottom() - (dy * (rect.height() -1)
                                               / RangeY);
                polyline.append(QPointF(x, y)) ;
                painter->setPen(QPen(Qt::darkYellow,3));

               // painter->setPen(QPen(colorForIds[(uint(id)) % 10],3));
                //drawSign(x,y,painter,6);

        }
        //draw points
        painter->setPen(QPen(Qt::black,5));
        painter->drawPoints(polyline);
    }

    //display Voronoi graph
    if(isDisplayVoronoi)
    {
        QMapIterator <int, QVector<LineSegments> > i7(QMap_VoronoiLines);
        while (i7.hasNext())
        {
            i7.next();
            int id = i7.key();
            const QVector<LineSegments> &data = i7.value();

            QPolygonF polylineStart(data.count());
            QPolygonF polylineEnd(data.count());

            // transfer the coordinate to the position on the pixmap
            for (int j = 0; j < data.count(); ++j)
            {

                double dx1 = data[j].LineSegments_StartVertex.x() - approximateMinX;
                double dy1 = data[j].LineSegments_StartVertex.y() - approximateMinY;
                double x1 = rect.left() + (dx1 * (rect.width() -1)
                                             / RangeX);
                double y1 = rect.bottom() - (dy1 * (rect.height() -1)
                                               / RangeY);

                double dx2 = data[j].LineSegments_EndVertex.x() - approximateMinX;
                double dy2 = data[j].LineSegments_EndVertex.y() - approximateMinY;
                double x2 = rect.left() + (dx2 * (rect.width() -1)
                                             / RangeX);
                double y2 = rect.bottom() - (dy2 * (rect.height() -1)
                                               / RangeY);
                polylineStart[j] = QPointF(x1, y1);
                polylineEnd[j] = QPointF(x2, y2);

                painter->setPen(QPen(Qt::red,1));
                painter->drawLine(polylineStart[j],polylineEnd[j]);
            }
        }

    }

    if(isDisplayVoronoiCell)
    {
        QMapIterator <int, QVector<QPointF> > i8(QMap_coordinateCircumcircleCenterOfVoronoiCell);
        QPolygonF polyline;
        while (i8.hasNext())
        {
            i8.next();
            int id = i8.key();
            const QVector<QPointF> &data = i8.value();
            for (int j = 0; j < data.count(); ++j)
            {

                double dx = data[j].x() - approximateMinX;
                double dy = data[j].y() - approximateMinY;
                double x = rect.left() + (dx * (rect.width()-1 )
                                             / RangeX);
                double y = rect.bottom() - (dy * (rect.height() -1)
                                               / RangeY);
                polyline.append(QPointF(x, y)) ;
                painter->setPen(QPen(Qt::green,2));

                //painter->setPen(QPen(colorForIds[(uint(id)) % 10],3));
                drawSign(x,y,painter,6);
            }

        }
        //draw points
        QPainterPath cellPath;
        cellPath.addPolygon(polyline);
        polyline.clear();

        QBrush fillBrush(QColor(0,255,0,50), Qt::SolidPattern);//set brush for filling the walked triangles
        painter->setPen(QPen(QColor(0,255,0,50),3));

        painter->setBrush(fillBrush);
        painter->drawPath(cellPath);

        painter->setPen(QPen(Qt::red,5));
        painter->drawPoints(polyline);
    }

}






void MainWindow::on_pushButtonCreateCoordinateAxis_clicked()
{
    if(ui->lineEditRangeX->text()=="")
    {
        RangeX = 100;
        approximateMinX = 0;

    }
    if(ui->lineEditRangeY->text()=="")
    {
        RangeY = 100;
        approximateMinY = 0;
    }
    else
    {
        RangeX = ui->lineEditRangeX->text().toDouble();
        RangeY = ui->lineEditRangeY->text().toDouble();
    }
    refreshPixmap();
}


void MainWindow::on_checkBox_NearestNeighbor_clicked()
{
    if(ui->checkBox_NearestNeighbor->isChecked())
    {
        isSelectNeighbor = true;
        isInsertPoint = false;
        isDisplayVoronoiCell =false;
        ui->checkBox_VoronoiCell->setChecked(false);

    }
    else
    {
        isSelectNeighbor = false;
        isInsertPoint = true;
    }

    refreshPixmap();
}



void MainWindow::on_checkBox_VoronoiCell_clicked()
{
    if(ui->checkBox_VoronoiCell->isChecked())
    {
        isSelectNeighbor = false;
        isInsertPoint = false;
        isDisplayVoronoiCell=true;
        ui->checkBox_NearestNeighbor->setChecked(false);
    }
    else
    {
        isDisplayVoronoiCell=false;
        isInsertPoint = true;
    }
    refreshPixmap();
}

void MainWindow::on_checkBox_DisplayVoronoi_clicked()
{
    if(ui->checkBox_DisplayVoronoi->isChecked())
    {
        isDisplayVoronoi=true;
        CalculateAllCenterOfCircumcircle(QVec_TableTopology);
        CreateVoronoiLineSegments(QVec_TableTopology);
    }
    else
    {
        isDisplayVoronoi=false;
    }
    refreshPixmap();
}

void MainWindow::on_checkBox_DisplayWalkedTriangles_clicked()
{
    if(ui->checkBox_DisplayWalkedTriangles->isChecked())
    {
       isDisplayWalkedTriangels = true;
    }
    else
    {
       isDisplayWalkedTriangels = false;
    }

    refreshPixmap();
}

void MainWindow::on_radioButtonCoordinate_clicked()
{
    if(ui->radioButtonCoordinate->isChecked())
    {
        isDisplayCoordinate=true;
    }
    else
    {
        isDisplayCoordinate=false;
    }
    refreshPixmap();
}



void MainWindow::on_checkBox_DisplayDelaunay_clicked()
{
    if(ui->checkBox_DisplayDelaunay->isChecked())
    {
       isDisplayDelaunay = true;
    }
    else
    {
       isDisplayDelaunay = false;
    }

    refreshPixmap();
}
