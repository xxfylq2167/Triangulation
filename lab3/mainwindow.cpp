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
    isDisplayCoordinate=true;
    isDisplayDelaunay =true;

     MinX =0;
     MaxX =0;
     MinY=0;
     MaxY=0;
     MinZ=0;
     MaxZ=0;
     RangeX=0;
     RangeY=0;
     approximateMinX=0;
     approximateMinY=0;
     approximateMinZ=0;;
     approximateMaxX=0;
     approximateMaxY=0;
     approximateMaxZ=0;

     rotationAngleZ =0;
     rotationAngleY =0;
     rotationAngleX =0;

     //distance = 590800000;
     distance = 1000000;

     ui->pushButtonTranslation->setEnabled(false);
     ui->pushButtonRotationZ->setEnabled(false);
     ui->pushButtonRotationY->setEnabled(false);
     ui->pushButtonRotationX->setEnabled(false);
     ui->pushButtonScaling->setEnabled(false);
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
void MainWindow::setDataSource(int id, const QVector<QPointF3> &data, QString flag)
{
    // store the coordiante of vertexes
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
void MainWindow::Frame(QMap<int, QPointF3> GlobalTriVertex)
{
    int VertexNum=0;
    QVector<int> templine;
    templine.append(1);
    templine.append(1);
    templine.append(2);
    templine.append(3);
    templine.append(0);
    templine.append(0);
    templine.append(0);


    QVec_TableTopology.append(templine);

//        for(int j=0;j< QVec_TableTopology.count() ;j++)
//        {
//            qDebug()<< QVec_TableTopology[j][0]<<" "\
//                    << QVec_TableTopology[j][1]<<" "\
//                    << QVec_TableTopology[j][2]<<" "\
//                    << QVec_TableTopology[j][3]<<" "\
//                    << QVec_TableTopology[j][4]<<" "\
//                    << QVec_TableTopology[j][5]<<" "\
//                    << QVec_TableTopology[j][6];
//        }
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
QVector<double> MainWindow::ComputeDetTri_andNewPoint(int triID,int newPointID,
                                            QMap<int, QPointF3> &QMap_coordinateVertex)
{
    // construct the new three triangle, the new point is common vertex
    int j = triID;
    ComputeTriArea triDet;
    QPointF3 newPoint;
    QVector<QPointF> triangle1;
    QVector<QPointF> triangle2;
    QVector<QPointF> triangle3;

    newPoint  = QMap_coordinateVertex.value(newPointID);
    QPointF3 V1= QMap_coordinateVertex.value(QVec_TableTopology[j][1]);
    QPointF3 V2= QMap_coordinateVertex.value(QVec_TableTopology[j][2]);
    QPointF3 V3= QMap_coordinateVertex.value(QVec_TableTopology[j][3]);

    triangle1.append(QPointF(V1.x,V1.y));
    triangle1.append(QPointF(V2.x,V2.y));
    triangle1.append(QPointF(newPoint.x,newPoint.y));

    triangle2.append(QPointF(V2.x,V2.y));
    triangle2.append(QPointF(V3.x,V3.y));
    triangle2.append(QPointF(newPoint.x,newPoint.y));

    triangle3.append(QPointF(V3.x,V3.y));
    triangle3.append(QPointF(V1.x,V1.y));
    triangle3.append(QPointF(newPoint.x,newPoint.y));

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
int MainWindow::Walk(int newPointID, QMap<int, QPointF3> &QMap_coordinateVertex)
{
   // qDebug()<<"walk start";

    double zero = 0.0001;
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
        // qDebug()<<"result det"<<Result1_12P<<" "<<Result2_23P<<" "<<Result3_31P;
         //qDebug()<<"compute det end";

         // find the adjacent of TA3 that is on the right of
         // the first edge V1-V2(the opposite adjacent triangle of V3)

         if(qAbs(Result1_12P)<zero || Result1_12P < 0) //12P <=0
         {

             if(qAbs(Result1_12P)<zero ) //12P=0
             {
                 //qDebug()<<"Result1_12P = 0";
                 if(qAbs(Result2_23P) < zero) //23P=0, so P on the vertex 2
                 {
                     break;
                 }
                 if(qAbs(Result3_31P) < zero) //31P=0, P on the vertex 1
                 {
                     break;
                 }
                 if(Result2_23P < 0) //23P<0 go to TA1
                 {
                     if(QVec_TableTopology[triCurrentID][4] !=0)
                     {
                         triCurrentID = QVec_TableTopology[triCurrentID][4] -1;
                         WalkedTriangleID.append(triCurrentID);
                         continue;
                     }
                 }

                 if(Result2_23P > 0)
                 {
                     //qDebug()<<"Result2_23P > 0";
                     //23P > 0 and 31P > 0,  p on the edge 12, between 1 and 2
                     if(Result3_31P >0)
                     {
                        break;
                     }
                     if(Result3_31P <0) // go to TA2
                     {
                         if(QVec_TableTopology[triCurrentID][5] !=0)
                         {
                            triCurrentID = QVec_TableTopology[triCurrentID][5]-1;
                            WalkedTriangleID.append(triCurrentID);
                            continue;
                         }
                     }
                 }


             }
             else  //12P < 0,  go to TA3
             {
                 if(QVec_TableTopology[triCurrentID][6] !=0)
                 {
                     //update the current triID
                     triCurrentID =  QVec_TableTopology[triCurrentID][6] -1 ;
                     WalkedTriangleID.append(triCurrentID);
                     continue;
                 }
             }

         }
         else      //12P > 0
         {
             // find the adjacent of TA1 that is on the right of the second edge V2-V3
             if(qAbs(Result2_23P ) < zero || Result2_23P <0) //23P <= 0
             {   
                 if(qAbs(Result2_23P) < zero)  //23P = 0
                 {
                     if(qAbs(Result3_31P) < zero) //31P=0, so P on the vertex 3
                     {
                         break;
                     }

                     //31P < 0 go to TA2, P is after 3 in the direction of 23,
                     //but the case before 2 have been processed in 12P < 0
                     if(Result3_31P < 0)
                     {
                         if(QVec_TableTopology[triCurrentID][5] !=0)
                         {
                            triCurrentID = QVec_TableTopology[triCurrentID][5]-1;
                            WalkedTriangleID.append(triCurrentID);
                            continue;
                         }
                     }
                     if(Result3_31P > 0) //31P > 0   p on the edge 23, between 2 and 3
                     {
                         break;
                     }
                 }
                 else //23P < 0 go to TA1
                 {
                     if(QVec_TableTopology[triCurrentID][4] !=0)
                     {
                         triCurrentID = QVec_TableTopology[triCurrentID][4] -1;
                         WalkedTriangleID.append(triCurrentID);
                         continue;
                     }
                 }

             }
             else  //23P > 0
             {  
                 // find the adjacent of TA2 that is on the right of the second edge V3-V1
                 if(qAbs(Result3_31P) < zero || Result3_31P < 0) //31P <= 0
                 {
                     if(qAbs(Result3_31P) < zero ) //31P =0
                     {
                        // qDebug()<<"31P is 0";
                         break;
                     }
                     else //31P<0
                     {
                         if(QVec_TableTopology[triCurrentID][5] !=0)
                         {
                            triCurrentID = QVec_TableTopology[triCurrentID][5]-1;
                            WalkedTriangleID.append(triCurrentID);
                            continue;
                         }
                     }
                 }
                 else //31P>0
                 {  // find the target triangle that contains the new point
                     break;
                 }
             }
         }
         resultDet.clear();
     }

   // qDebug()<<"walk end";

    return QVec_TableTopology[triCurrentID][0];
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
   // qDebug()<<"QVec_TableTopology[0].size()"<<QVec_TableTopology[0].size();

   // qDebug()<< "Insert start";
    int NumberOfTriangle = QVec_TableTopology.count();

    //int NumberOfTriangle = QVec_TableTopology.at();

    QVector<int> triangleNew1;
    QVector<int> triangleNew2;
    //add a new lines for new triangle N+1

    triangleNew1.append(NumberOfTriangle+1);
    triangleNew1.append(newPointID);
    triangleNew1.append(QVec_TableTopology[TriID_WalkResult -1][3]);
    triangleNew1.append(QVec_TableTopology[TriID_WalkResult -1][1]);
    triangleNew1.append(QVec_TableTopology[TriID_WalkResult -1][5]);
    triangleNew1.append(NumberOfTriangle+2);
    triangleNew1.append(QVec_TableTopology[TriID_WalkResult -1][0]);

    QVec_TableTopology.append(triangleNew1);
    triangleNew1.clear();

//    QVec_TableTopology[0].append(NumberOfTriangle+1);
//    QVec_TableTopology[1].append(newPointID);
//    QVec_TableTopology[2].append(QVec_TableTopology[3][TriID_WalkResult -1]);
//    QVec_TableTopology[3].append(QVec_TableTopology[1][TriID_WalkResult -1]);
//    QVec_TableTopology[4].append(QVec_TableTopology[5][TriID_WalkResult -1]);
//    QVec_TableTopology[5].append(NumberOfTriangle+2);
//    QVec_TableTopology[6].append(QVec_TableTopology[0][TriID_WalkResult -1]);

    //add a new lines for new triangle N+1

        triangleNew2.append(NumberOfTriangle+2);
        triangleNew2.append(newPointID);
        triangleNew2.append(QVec_TableTopology[TriID_WalkResult -1][1]);
        triangleNew2.append(QVec_TableTopology[TriID_WalkResult -1][2]);
        triangleNew2.append(QVec_TableTopology[TriID_WalkResult -1][6]);
        triangleNew2.append(QVec_TableTopology[TriID_WalkResult -1][0]);
        triangleNew2.append(NumberOfTriangle+1);

        QVec_TableTopology.append(triangleNew2);
        triangleNew2.clear();

//    QVec_TableTopology[0].append(NumberOfTriangle+2);
//    QVec_TableTopology[1].append(newPointID);
//    QVec_TableTopology[2].append(QVec_TableTopology[1][TriID_WalkResult -1]);
//    QVec_TableTopology[3].append(QVec_TableTopology[2][TriID_WalkResult -1]);
//    QVec_TableTopology[4].append(QVec_TableTopology[6][TriID_WalkResult -1]);
//    QVec_TableTopology[5].append(QVec_TableTopology[0][TriID_WalkResult -1]);
//    QVec_TableTopology[6].append(NumberOfTriangle+1);

    //update the other two adjacent triangle of current triangle
    //find the TA2 and TA3
    int adjacent2 = QVec_TableTopology[TriID_WalkResult -1][5];
    int adjacent3 = QVec_TableTopology[TriID_WalkResult -1][6];

    int indexTriIDinTA2,indexTriIDinTA3;
    if(adjacent2!= 0 || adjacent3!= 0)
    {
        for(int i=4;i<7;i++)
        {
            if(QVec_TableTopology[adjacent2 -1][i] == QVec_TableTopology[TriID_WalkResult-1][0])
            {
                indexTriIDinTA2 = i;
            }
            if(QVec_TableTopology[adjacent3 -1][i] == QVec_TableTopology[TriID_WalkResult-1][0])
            {
                indexTriIDinTA3 = i;
            }
        }
        // update the adjacent of TA2 and TA3
        QVec_TableTopology[adjacent2 -1][indexTriIDinTA2] = NumberOfTriangle+1;
        QVec_TableTopology[adjacent3 -1][indexTriIDinTA3] = NumberOfTriangle+2;
    }

    //update the current triangle contains the new point
    QVec_TableTopology[TriID_WalkResult -1][1]  = newPointID;
    QVec_TableTopology[TriID_WalkResult -1][5]  = NumberOfTriangle+1;
    QVec_TableTopology[TriID_WalkResult -1][6]  = NumberOfTriangle+2;

    // put the new inserted triangles into stack for optimization of Delaunay
    QVec_StackOptim.clear();
    QVec_StackOptim.append(QVec_TableTopology[TriID_WalkResult -1][0]);
    QVec_StackOptim.append(QVec_TableTopology[NumberOfTriangle][0]);
    QVec_StackOptim.append(QVec_TableTopology[NumberOfTriangle+1][0]);

   // qDebug()<< "Insert end";
}

// display all triangle in pixmap and tableWidget_Topology
double MainWindow::DisplayAllTriangles(QVector<QVector<int> > &QVec_TableTopology,
                                       QMap<int, QPointF3> &QMap_coordinateVertex )
{
    QVec_Triangles.clear();
    //display all the triangle after Insert
    for(int j=0;j< QVec_TableTopology.count() ;j++)
    {
        Triangle tempTri;
        tempTri.Vertex1 = QMap_coordinateVertex.value(QVec_TableTopology[j][1]);
        tempTri.Vertex2 = QMap_coordinateVertex.value(QVec_TableTopology[j][2]);
        tempTri.Vertex3 = QMap_coordinateVertex.value(QVec_TableTopology[j][3]);


        QVec_Triangles.append(tempTri);
    }
    setDataSource(0,QVec_Triangles,"Delaunay_Triangles");
    refreshPixmap();

    //display topological table in ui->tableWidget_Topology
//    ui->tableWidget_Topology->setRowCount(QVec_TableTopology[0].count());
//    ui->tableWidget_Topology->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    for(int i=0;i< QVec_TableTopology[0].count();i++)
//    {
//        for(int j=0;j< 7 ;j++)
//        {
//            QTableWidgetItem *tmpItem;
//            tmpItem = new QTableWidgetItem(QString::number(QVec_TableTopology[j][i]));
//            ui->tableWidget_Topology->setItem(i,j , tmpItem);
//        }
//    }
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

double MainWindow::isInCircle(int currentTri, int fourthPointID, QMap<int, QPointF3> &QMap_coordinateVertex)
{
    InCircle inCir;
    QPointF3 fourthPoint = QMap_coordinateVertex.value(fourthPointID);
    QPointF3 V1= QMap_coordinateVertex.value(QVec_TableTopology[currentTri-1][1]);
    QPointF3 V2= QMap_coordinateVertex.value(QVec_TableTopology[currentTri-1][2]);
    QPointF3 V3= QMap_coordinateVertex.value(QVec_TableTopology[currentTri-1][3]);

    double resultInCircle = inCir.ComputeDetInCircle(QPointF(fourthPoint.x,fourthPoint.y),QPointF(V1.x,V1.y),QPointF(V2.x,V2.y),QPointF(V3.x,V3.y));
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
                          QMap<int, QPointF3> &QMap_coordinateVertex)
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
            if(QVec_TableTopology[currentTriID -1][i] == NewPointID)
            {
                positionNewP = i;
                break;
            }
        }
        int adjacentTri = QVec_TableTopology[currentTriID -1][positionNewP + 3];

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
int MainWindow::Swap(int currentTriID,int newPointID,int adjacentTriID,int PointNotCommon ,
                     QVector<QVector<int> > &QVec_TableTopology)
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
        if(QVec_TableTopology[currentTriID-1][i] == newPointID)
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

    PC1 = QVec_TableTopology[currentTriID-1][positionPC1T1];
    PC2 = QVec_TableTopology[currentTriID-1][positionPC2T1];

    //in adjacent triangle, PC1 and PC2 are changed
    for(int i=1;i<4;i++)
    {
        if(QVec_TableTopology[adjacentTriID-1][i] == PointNotCommon)
        {
            positionPointNotCommon = i;
        }
        if(QVec_TableTopology[adjacentTriID-1][i] == PC2 )
        {
            positionPC2T2 = i;
        }
        if(QVec_TableTopology[adjacentTriID-1][i] == PC1 )
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
    newCurrentTri.append(QVec_TableTopology[adjacentTriID - 1][positionPC1T2+3]);//adjacent triangle opposite to PC1 in old T2
    newCurrentTri.append(QVec_TableTopology[currentTriID - 1][positionPC1T1+3]); // the adjacent triangle opposite to PC1 in old T1
    newCurrentTri.append(QVec_TableTopology[currentTriID - 1][positionNewPoint+3]); // T2

    //record new changed newAdjacentTri T2
    newAdjacentTri.append(adjacentTriID);
    newAdjacentTri.append(newPointID);
    newAdjacentTri.append(PC1);
    newAdjacentTri.append(PointNotCommon);
    newAdjacentTri.append(QVec_TableTopology[adjacentTriID - 1][positionPC2T2+3]); //adjacent triangle opposite to PC2 in old T2
    newAdjacentTri.append(QVec_TableTopology[adjacentTriID - 1][positionPointNotCommon+3]);//T1
    newAdjacentTri.append(QVec_TableTopology[currentTriID - 1][positionPC2T1+3]); //adjacent triangle opposite to PC2 in old T1

    //find the adjacent triangle T4 opposite PC1 in T1
    int T1_PC2_Adj = QVec_TableTopology[currentTriID - 1][positionPC2T1+3];

    //find the adjacent triangle T6 opposite PC2 in T2
    int T2_PC1_Adj = QVec_TableTopology[adjacentTriID - 1][positionPC1T2+3];

    //update T4 and T6
    //update T1 in the T4 and replaced by T2
     if(T1_PC2_Adj!=0)
     {
        int n;
        for(int i=4; i< 7; i++)
        {
            if(QVec_TableTopology[T1_PC2_Adj - 1][i] == currentTriID)
            {
                n=i;
                break;
            }
        }
        QVec_TableTopology[T1_PC2_Adj - 1][n] = adjacentTriID;
    }

    //update T2 in the T6 and replaced by T1
     if(T2_PC1_Adj!=0)
     {
        int m;
        for(int i=4; i< 7; i++)
        {
            if(QVec_TableTopology[T2_PC1_Adj - 1][i] == adjacentTriID)
            {
                m=i;
                break;
            }
        }

        QVec_TableTopology[T2_PC1_Adj - 1][m] = currentTriID;
    }

    //update T1 and T2
    for(int i=0;i<newCurrentTri.count();i++)
    {
        QVec_TableTopology[currentTriID - 1][i] = newCurrentTri[i];
        QVec_TableTopology[adjacentTriID - 1][i] = newAdjacentTri[i];
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

    vertexCurrTri.append(QVec_TableTopology[currentTriID -1][1]);
    vertexCurrTri.append(QVec_TableTopology[currentTriID -1][2]);
    vertexCurrTri.append(QVec_TableTopology[currentTriID -1][3]);

    vertexAdjTri.append(QVec_TableTopology[adjacentTriID -1][1]);
    vertexAdjTri.append(QVec_TableTopology[adjacentTriID -1][2]);
    vertexAdjTri.append(QVec_TableTopology[adjacentTriID -1][3]);

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
    painter.setBackground(Qt::black);
    painter.drawPixmap(ui->Pixmapframe->pos().x()+PixMapLeftMargin,
         ui->Pixmapframe->pos().y()+ui->menuBar->height()+ui->mainToolBar->height()+PixMapTopMargin,  pixmap);

}

void MainWindow::resizeEvent(QResizeEvent *)
{
    int RightMarginforButton = 250;

    ui->Pixmapframe->setGeometry(10,5 + PixMapTopMargin,this->width() - RightMarginforButton , height()-PixMapTopMargin-50);
    int x=ui->Pixmapframe->width()+ 25 + PixMapTopMargin;

    ui->groupBox1->setGeometry(x,PixMapTopMargin,200,140);
    ui->groupBox2->setGeometry(x,10+ui->groupBox1->height(),200,50);
    ui->groupBox3->setGeometry(x,20+ui->groupBox1->height()+ui->groupBox2->height(),200,50);
    ui->groupBox4->setGeometry(x,30+ui->groupBox1->height()+2*ui->groupBox2->height(),200,50);
    ui->groupBox5->setGeometry(x,40 + ui->groupBox1->height() +ui->groupBox2->height()+ui->groupBox3->height()+ui->groupBox4->height()+ PixMapTopMargin,200, 170);

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
    pixmap.fill(Qt::black);//background colour

    QPainter painter(&pixmap);
    painter.initFrom(this);

    if(RangeX > 0 && RangeY > 0)
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

    int numXTicks = 10;
    int numYTicks = 10;

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

        painter->setPen(QPen(Qt::white));
        painter->drawText(x-30, rect.bottom() + 8, 60, 15,
                          Qt::AlignHCenter | Qt::AlignTop | Qt::AlignJustify,
                          QString::number(label,'d',0));


    }


    //draw y axis
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
        painter->setPen(QPen(Qt::white));
        painter->drawText(rect.left() - Margin, y - 10, Margin - 10, 20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(label,'d',2));
    }

    painter->setPen(QPen(Qt::blue,1));
    painter->drawRect(rect.adjusted(0, 0, -1, -1));


    painter->setPen(QPen(Qt::white));
    //axis Z and arrow
    painter->drawLine(rect.left()+10, rect.top()+10, rect.left()+10, rect.top()+50);
    painter->drawLine(rect.left()+5, rect.top()+15, rect.left()+10, rect.top()+10);
    painter->drawLine(rect.left()+15, rect.top()+15, rect.left()+10, rect.top()+10);
    painter->drawText(rect.left()+12, rect.top()+10, 10, 10,
                      Qt::AlignRight | Qt::AlignVCenter,"Z");

    //axis X and arrow
    painter->drawLine(rect.left()+10, rect.top()+50, rect.left()+50, rect.top()+50);
    painter->drawLine(rect.left()+45, rect.top()+45, rect.left()+50, rect.top()+50);
    painter->drawLine(rect.left()+45, rect.top()+55, rect.left()+50, rect.top()+50);
    painter->drawText(rect.left()+50, rect.top()+50, 10, 10,
                      Qt::AlignRight | Qt::AlignVCenter,"X");

    //axis Y and arrow
    painter->drawLine(rect.left()+10, rect.top()+50, rect.left()+50, rect.top()+30);
    painter->drawLine(rect.left()+45, rect.top()+28, rect.left()+50, rect.top()+30);
    painter->drawLine(rect.left()+47, rect.top()+35, rect.left()+50, rect.top()+30);
    painter->drawText(rect.left()+50, rect.top()+30, 10, 10,
                      Qt::AlignRight | Qt::AlignVCenter,"Y");

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

    //draw points
    QMapIterator <int, QPointF3> i1(QMap_CoordTransform);
    QPolygonF polyline;
    while (i1.hasNext())
    {
        i1.next();
        int id = i1.key();
        const QPointF3 &data = i1.value();

        double dx = data.x- approximateMinX;
        double dy = data.y - approximateMinY;
        double x = rect.left() + (dx * (rect.width()-1 )
                                     / RangeX);
        double y = rect.bottom() - (dy * (rect.height() -1)
                                       / RangeY);

//        double x = rect.left()+  (rect.width()-1)*(dx/ RangeX + 0.5);
//        double y = rect.bottom() - (rect.height()-1)*(dy/ RangeY + 0.5);


        polyline.append(QPointF(x, y)) ;
        painter->setPen(QPen(Qt::red));

        //draw pointID
       // painter->setPen(QPen(Qt::blue));
       // painter->drawText(QPointF(x, y),QString::number(id));

        //painter->setPen(QPen(Qt::red,5));
        //drawSign(x,y,painter,2);

    }
    //draw points
    painter->setPen(QPen(Qt::red,2));
    painter->drawPoints(polyline);




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

                double dx1 = data[j].Vertex1.x - approximateMinX;
                double dy1 = data[j].Vertex1.y - approximateMinY;
               // double dz1 = data[j].Vertex1.y - approximateMinZ;
                double x1 = rect.left() + (dx1 * (rect.width() -1)
                                             / RangeX);
                double y1 = rect.bottom() - (dy1 * (rect.height() -1)
                                               / RangeY);

                double dx2 = data[j].Vertex2.x - approximateMinX;
                double dy2 = data[j].Vertex2.y - approximateMinY;
                double x2 = rect.left() + (dx2 * (rect.width() -1)
                                             / RangeX);
                double y2 = rect.bottom() - (dy2 * (rect.height() -1)
                                               / RangeY);

                double dx3 = data[j].Vertex3.x - approximateMinX;
                double dy3 = data[j].Vertex3.y - approximateMinY;
                double x3 = rect.left() + (dx3 * (rect.width() -1)
                                             / RangeX);
                double y3 = rect.bottom() - (dy3 * (rect.height() -1)
                                               / RangeY);
                //draw polygon
                painter->setPen(QPen(Qt::white));

                painter->drawLine(QPointF(x1,y1),QPointF(x2,y2));
                painter->drawLine(QPointF(x2,y2),QPointF(x3,y3));
                painter->drawLine(QPointF(x3,y3),QPointF(x1,y1));

            }
        }
    }



}


// process the inputed files, to evaluate the range and global triangle
void MainWindow::preProcessData(QVector<QPointF3> QVec_VertexCoord)
{
    QStringList X;
    QStringList Y;
    QStringList Z;

    for(int i=0; i< QVec_VertexCoord.count();i++)
    {
        X.append(QString::number(QVec_VertexCoord[i].x,'d',3));
        Y.append(QString::number(QVec_VertexCoord[i].y,'d',3));
        Z.append(QString::number(QVec_VertexCoord[i].z,'d',3));
    }

    //qDebug()<<"z"<<Z;
    if(X.count()==Y.count() && X.count()==Z.count())
    {
        if(X.count() == 1)
        {
            MinX= X[0].toDouble();
            MaxX= X[0].toDouble();

            MinY= Y[0].toDouble();
            MaxY= Y[0].toDouble();

            MinZ= Z[0].toDouble();
            MaxZ= Z[0].toDouble();
        }
        else if(X.count() == 2)
        {
            MinX=Find_Min(X[0].toDouble(),X[1].toDouble());
            MaxX=Find_Max(X[0].toDouble(),X[1].toDouble());

            MinY=Find_Min(Y[0].toDouble(),Y[1].toDouble());
            MaxY=Find_Max(Y[0].toDouble(),Y[1].toDouble());

            MinZ=Find_Min(Z[0].toDouble(),Z[1].toDouble());
            MaxZ=Find_Max(Z[0].toDouble(),Z[1].toDouble());
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
            //find min and max value from Y
            MinY= Y[0].toDouble();
            MaxY= Y[0].toDouble();
            for(int j=1;j < Y.count();j++)
            {
                MinY=Find_Min(MinY,Y[j].toDouble());
                MaxY=Find_Max(MaxY,Y[j].toDouble());
            }

            //find min and max value from Z
            MinZ= Z[0].toDouble();
            MaxZ= Z[0].toDouble();
            for(int j=1;j < Z.count();j++)
            {
                MinZ=Find_Min(MinZ,Z[j].toDouble());
                MaxZ=Find_Max(MaxZ,Z[j].toDouble());
            }
        }

       // qDebug()<<"MinX,MaxX,MinY,MaxY"<<QString::number(MinX,'d',3)<<" "<<QString::number(MaxX,'d',3)<<" "<<QString::number(MinY,'d',3)<<" "<<QString::number(MaxY,'d',3);

        double MinX_Approximate = floor(MinX/100)*100 -100;
                //- qAbs( 0.1*floor(MinX/100)*100);
        double MaxX_Approximate = ceil(MaxX/100)*100 + 100 ;
               // + qAbs( 0.1*ceil(MaxX/100)*100);

        double MinY_Approximate = floor(MinY/100)*100 -100 ;
                //- qAbs(0.1*floor(MinY/100)*100);
        double MaxY_Approximate = ceil(MaxY/100)*100 + 100 ;
                //+   qAbs( 0.1*ceil(MaxY/100)*100);

        double MinZ_Approximate = floor(MinZ/10)*10 ;
                //- qAbs(floor(MinZ/100))*10;
        double MaxZ_Approximate = ceil(MaxZ/10)*10 ;
                //+ ceil(MaxZ/100)*10;



//        qDebug()<<"Approximate MinX,MaxX,MinY,MaxY"<< \
//                  QString::number(MinX_Approximate,'d',3)<<" "<< QString::number(MaxX_Approximate,'d',3) \
//               <<" "<<QString::number(MinY_Approximate,'d',3)<<" "<<QString::number(MaxY_Approximate,'d',3);

        double x1 =  (MaxX_Approximate + MinX_Approximate)/2;
        double y1 =  MaxY_Approximate + ((MaxX_Approximate - MinX_Approximate)/2) * qTan(PI/3);

        double x2 = MinX_Approximate - qAbs((MaxY_Approximate - MinY_Approximate)) / (qTan(PI/3));
        double y2 = MinY_Approximate;

        double x3 = MaxX_Approximate + (MaxY_Approximate - MinY_Approximate) / (qTan(PI/3));
        double y3 = MinY_Approximate;

//        approximateMinX = floor((x2 -100)/100)*100;
//        approximateMaxX = ceil((x3 +100)/100)*100;

//        approximateMinY = floor((y2-100)/100)*100;
//        approximateMaxY = ceil((y1 +100)/100)*100;

        approximateMinX = MinX_Approximate;
        approximateMaxX = MaxX_Approximate;

        approximateMinY = MinY_Approximate;
        approximateMaxY = MaxY_Approximate;

        approximateMinZ = MinZ_Approximate;
        approximateMaxZ = MaxZ_Approximate;

        RangeX = approximateMaxX - approximateMinX;
        RangeY = approximateMaxY - approximateMinY;
        RangeZ = approximateMaxZ - approximateMinZ;

        //qDebug()<<"RangeX"<<RangeX<<" "<<RangeY;
        QVec_GlobalTriangle.clear();
        QVec_GlobalTriangle.append(QPointF3(x1,y1,0));
        QVec_GlobalTriangle.append(QPointF3(x2,y2,0));
        QVec_GlobalTriangle.append(QPointF3(x3,y3,0));


//        QMapIterator<int, QPointF3> i(QMap_GlobalTriVertex);
//        while (i.hasNext()) {
//            i.next();
//            int GlobalVertexID = i.key();
//            qDebug()<<GlobalVertexID<<" "<<i.value().x<<" "<<i.value().y<<" "<<i.value().z;
//        }

//        QMapIterator<int, QPointF3> i1(QMap_coordinateVertex);
//        while (i1.hasNext()) {
//            i1.next();
//            int GlobalVertexID = i1.key();
//            qDebug()<<GlobalVertexID<<" "<<i1.value().x<<" "<<i1.value().y<<" "<<i1.value().z;
//        }

        //setDataSource(0,QVec_GlobalTriangle,"Global_Triangle");
    }
}
/******************************
//Name: preProcessData
//Author: Xufeng
//Description: get the min and max of x and Y and Z
//Input: matrix
//Output: the minimum and maximum of X and Y, approximated range of X and Y for display coordiante
//Date: 2014-03-22
//Version: 1.00
*****************************/
void MainWindow::preProcessData(Eigen::MatrixX4d matrix)
{
    QStringList X;
    QStringList Y;
    QStringList Z;
    for(int i =3; i<matrix.rows();i++)
    {

        X.append(QString::number(matrix(i,0),'d',3));
        Y.append(QString::number(matrix(i,1),'d',3));
        Z.append(QString::number(matrix(i,2),'d',3));

    }

   // qDebug()<<"z"<<Z;

    if(X.count()==Y.count() && X.count()==Z.count())
    {
        if(X.count() == 1)
        {
            MinX= X[0].toDouble();
            MaxX= X[0].toDouble();

            MinY= Y[0].toDouble();
            MaxY= Y[0].toDouble();

            MinZ= Z[0].toDouble();
            MaxZ= Z[0].toDouble();
        }
        else if(X.count() == 2)
        {
            MinX=Find_Min(X[0].toDouble(),X[1].toDouble());
            MaxX=Find_Max(X[0].toDouble(),X[1].toDouble());

            MinY=Find_Min(Y[0].toDouble(),Y[1].toDouble());
            MaxY=Find_Max(Y[0].toDouble(),Y[1].toDouble());

            MinZ=Find_Min(Z[0].toDouble(),Z[1].toDouble());
            MaxZ=Find_Max(Z[0].toDouble(),Z[1].toDouble());
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
            //find min and max value from Y
            MinY= Y[0].toDouble();
            MaxY= Y[0].toDouble();
            for(int j=1;j < Y.count();j++)
            {
                MinY=Find_Min(MinY,Y[j].toDouble());
                MaxY=Find_Max(MaxY,Y[j].toDouble());
            }

            //find min and max value from Z
            MinZ= Z[0].toDouble();
            MaxZ= Z[0].toDouble();
            for(int j=1;j < Z.count();j++)
            {
                MinZ=Find_Min(MinZ,Z[j].toDouble());
                MaxZ=Find_Max(MaxZ,Z[j].toDouble());
            }
        }

//        qDebug()<<"11 MinX,MaxX,MinY,MaxY,MinZ,MaxZ"<<QString::number(MinX,'d',3)<<" "<<QString::number(MaxX,'d',3) \
//               <<" "<<QString::number(MinY,'d',3)<<" "<<QString::number(MaxY,'d',3)\
//               <<" "<<QString::number(MinZ,'d',3)<<" "<<QString::number(MaxZ,'d',3);

//        double MinX_Approximate = floor(MinX/100)*100 -1000;
//        double MaxX_Approximate = ceil(MaxX/100)*100 + 1000;

//        double MinY_Approximate = floor(MinZ/10)*10 ;
//        double MaxY_Approximate = ceil(MaxZ/10)*10 ;

//        double MinZ_Approximate = floor(MinY/100)*100 -100 ;
//        double MaxZ_Approximate = ceil(MaxY/100)*100 + 100 ;

                double MinX_Approximate = MinX ;
                double MaxX_Approximate = MaxX ;

                double MinY_Approximate = MinY ;
                double MaxY_Approximate = MaxY ;

                double MinZ_Approximate = MinZ ;
                double MaxZ_Approximate = MaxZ ;

//        qDebug()<<"22 Approximate MinX,MaxX,MinY,MaxY,MinZ,MaxZ"<< \
//                  QString::number(MinX_Approximate,'d',3)<<" "<< QString::number(MaxX_Approximate,'d',3) \
//               <<" "<<QString::number(MinY_Approximate,'d',3)<<" "<<QString::number(MaxY_Approximate,'d',3)\
//               <<" "<<QString::number(MinZ_Approximate,'d',3)<<" "<<QString::number(MaxZ_Approximate,'d',3);


        approximateMinX = MinX_Approximate;
        approximateMaxX = MaxX_Approximate;

        approximateMinY = MinY_Approximate;
        approximateMaxY = MaxY_Approximate;

        approximateMinZ = MinZ_Approximate;
        approximateMaxZ = MaxZ_Approximate;

        RangeX = approximateMaxX - approximateMinX;
        RangeY = approximateMaxY - approximateMinY;
        RangeZ = approximateMaxZ - approximateMinZ;
    }
}
/******************************
//Name: readTextFile
//Author: Xufeng
//Description: get coordinate from text file
//Input: fileName,the sign for split text
//Output: the coordiante, the minimum and maximum of X and Y, approximated range of X and Y for display coordiante
//Date: 2014-03-22
//Version: 1.00
*****************************/
bool MainWindow::readTextFile(QString fileName,QString SplitSign)
{
    QStringList line_content;

    QStringList X;
    QStringList Y;
    QStringList Z;


    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream readin(&file);
        while (!readin.atEnd())
        {
            QString readline = readin.readLine();
            if(readline!="")
            {
                line_content =  readline.split(SplitSign,QString::SkipEmptyParts);
                if(line_content.count()== 4)
                {
                    if(!line_content.contains("ID"))
                    {
                      //  qDebug()<< line_content[1]<<" "<<line_content[2]<<" "<<line_content[3];
                        X.append(line_content[1]);
                        Y.append(line_content[2]);
                        Z.append(line_content[3]);
                        QPointF3 p;
                        p.x = line_content[1].toDouble();
                        p.y = line_content[2].toDouble();
                        p.z = line_content[3].toDouble();

                       // qDebug()<<QString::number(p.x,'d',3) <<" "<<QString::number(p.y,'d',3)<<" "<<QString::number(p.z,'d',3);
                        QVec_DataFile.append(p);

                    }
                }
                else
                {
                    return false;
                }
            }
        }
    }
    else
    {
         QMessageBox::warning(this,"Tips",fileName+" did not open");
    }
}
/******************************
//Name: removeGlobalVertex
//Author: Xufeng
//Description: remove the global vertexes for delaunay triangulation
//Input: Topological table
//Output:Topological table
//Date: 2014-03-22
//Version: 1.00
*****************************/
void MainWindow::removeGlobalVertex(QVector< QVector<int> > &QVec_TableTopology)
{
    QVector <int> vertexRemove;
    for(int i=0;i< QVec_TableTopology.count();i++)
    {
        QStringList vertexes;
        vertexes.append(QString::number(QVec_TableTopology[i][1]));
        vertexes.append(QString::number(QVec_TableTopology[i][2]));
        vertexes.append(QString::number(QVec_TableTopology[i][3]));

        if(vertexes.contains("1") || vertexes.contains("2") || vertexes.contains("3"))
        {
            vertexRemove.append(QVec_TableTopology[i][0]);
        }
    }

    for(int i=0;i< QVec_TableTopology.count();++i)
    {
        for(int j=0; j< vertexRemove.count();++j)
        {
            if(QVec_TableTopology[i][0] != vertexRemove[j])
            {
                for(int k=4;k<7;++k)
                {
                    if(vertexRemove.contains(QVec_TableTopology[i][k]))
                    {
                        QVec_TableTopology[i][k] =0;
                    }
                }
            }
        }
    }
    for(int i=0;i< QVec_TableTopology.count();++i)
    {
        for(int j=0; j< vertexRemove.count();++j)
        {
            if(QVec_TableTopology[i][0] == vertexRemove[j])
            {
                QVec_TableTopology.remove(i);
            }

        }

    }

}

/******************************
//Name: on_actionOpen_files_triggered
//Author: Xufeng
//Description: open data file and display 3D models
//Input:
//Output:
//Date: 2014-04-02
//Version: 1.00
*****************************/

void MainWindow::on_actionOpen_files_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("Textfiles (*.txt)"));
    if(!fileName.isNull() && readTextFile(fileName," "))
    {
        preProcessData(QVec_DataFile);

        for(int i=0;i<QVec_GlobalTriangle.count();i++)
        {
            QMap_GlobalTriVertex.insert(i+1, QVec_GlobalTriangle[i]);
            QMap_coordinateVertex.insert(i+1, QVec_GlobalTriangle[i]);
        }

        Frame(QMap_GlobalTriVertex);
        numberVertex= 4;

        for(int i=0;i<QVec_DataFile.count();i++)
        {
            QMap_coordinateVertex.insert(numberVertex, QVec_DataFile[i]);
            numberVertex++;
        }

        QMapIterator<int, QPointF3> i1(QMap_coordinateVertex);
        while (i1.hasNext())
        {
            i1.next();
            int NoVex= i1.key();
            if(NoVex>3)
            {
                int triID = Walk(NoVex,QMap_coordinateVertex);
                Insert(NoVex,triID);
                if(QVec_TableTopology.count()>3)
                {
                    Optimize(NoVex,QVec_StackOptim,QMap_coordinateVertex);
                }

            }
        }

        removeGlobalVertex(QVec_TableTopology);
        matrixPoints = LoadPointsToMatrix(QMap_coordinateVertex);
       // DisplayMatrix(matrixPoints);
        preProcessData(matrixPoints);

        Eigen::MatrixX4d matrixPointsProjected = Projection3DModel(matrixPoints,distance);


        preProcessData(matrixPointsProjected);
        QMap_CoordTransform = LoadMatrixToQMap(matrixPointsProjected);
        DisplayAllTriangles(QVec_TableTopology,QMap_CoordTransform);

        ui->pushButtonTranslation->setEnabled(true);
        ui->pushButtonRotationZ->setEnabled(true);
        ui->pushButtonRotationY->setEnabled(true);
        ui->pushButtonRotationX->setEnabled(true);
        ui->pushButtonScaling->setEnabled(true);
    }
}
/******************************
//Name: LoadPointsToMatrix
//Author: Xufeng
//Description: organize coordiantes into matrix
//Input: QMap_coordinateVertex
//Output:MatrixX4d
//Date: 2014-03-22
//Version: 1.00
*****************************/
Eigen::MatrixX4d MainWindow::LoadPointsToMatrix( QMap<int, QPointF3> &QMap_coordinateVertex)
{
    MatrixX4d matrixPoints(QMap_coordinateVertex.count(),4);
    int row = 0;
    QMapIterator<int, QPointF3> i2(QMap_coordinateVertex);
    while (i2.hasNext()) {
        i2.next();
        int GlobalVertexID = i2.key();

//        qDebug()<<GlobalVertexID<<" "<<QString::number(i2.value().x,'d',3) \
//               <<" "<<QString::number(i2.value().y,'d',3)\
//               <<" "<<QString::number(i2.value().z,'d',3);

        matrixPoints(row,0) = i2.value().x;
        matrixPoints(row,1) = i2.value().z;
        matrixPoints(row,2) = i2.value().y;
        matrixPoints(row,3) = 1;

        ++row;
    }

    return matrixPoints;
}

/******************************
//Name: LoadMatrixToQMap
//Author: Xufeng
//Description: organize matrix into coordiantes for displaying 3D models
//Input: MatrixX4d
//Output:QMap_coordinateVertex
//Date: 2014-03-22
//Version: 1.00
*****************************/
QMap<int, QPointF3> MainWindow::LoadMatrixToQMap(Eigen::MatrixX4d matrix)
{
    QMap<int, QPointF3 > QMap_CoordTransform;
    for(int i=0;i<matrix.rows();i++)
    {

        QMap_CoordTransform.insert(i+1,QPointF3(matrix(i,0),matrix(i,1),matrix(i,2)));
    }
    return QMap_CoordTransform;

}

/******************************
//Name: Translation
//Author: Xufeng
//Description: Translation of 3D models
//Input: MatrixX4d,deltX, deltY,deltZ
//Output:MatrixX4d
//Date: 2014-03-28
//Version: 1.00
*****************************/

Eigen::MatrixX4d MainWindow::Translation(Eigen::MatrixX4d points,double deltX,double deltY,double deltZ)
{
    Matrix4d matrixTran;
    matrixTran<< 1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 deltX,deltY,deltZ,1;

    MatrixX4d tranResult = points *  matrixTran;
    return tranResult;

}

/******************************
//Name: Scaling
//Author: Xufeng
//Description: Translation of 3D models
//Input: MatrixX4d,sX, sY,sZ
//Output:MatrixX4d
//Date: 2014-03-28
//Version: 1.00
*****************************/
Eigen::MatrixX4d MainWindow::Scaling(Eigen::MatrixX4d points,double sX,double sY,double sZ)
{
    Matrix4d matrixScaling;
    matrixScaling<< sX,0,0,0,
                    0,sY,0,0,
                    0,0,sZ,0,
                    0,0, 0,1;

    MatrixX4d scaleResult = points *  matrixScaling;
    return scaleResult;
}

/******************************
//Name: Rotation
//Author: Xufeng
//Description: Rotation of 3D models
//Input: MatrixX4d,angle,sign for selecting axis
//Output:MatrixX4d
//Date: 2014-03-28
//Version: 1.00
*****************************/
Eigen::MatrixX4d MainWindow::Rotation(Eigen::MatrixX4d points,double angle,QString sign)
{
    double cosAngle = qCos((angle/180)*PI);
    double sinAngle = qSin((angle/180)*PI);
    Matrix4d matrixRotationZ;
    matrixRotationZ<< cosAngle, sinAngle, 0, 0,
                      -sinAngle ,cosAngle, 0,0,
                      0, 0, 1, 0,
                      0, 0, 0, 1;

    Matrix4d matrixRotationY;
    matrixRotationY<< cosAngle, 0, -sinAngle, 0,
                      0, 1, 0, 0,
                      sinAngle, 0, cosAngle, 0,
                      0, 0, 0, 1;

    Matrix4d matrixRotationX;
    matrixRotationX<< 1, 0, 0, 0,
                      0, cosAngle, sinAngle,  0,
                      0, -sinAngle, cosAngle, 0,
                      0, 0, 0, 1;
    if(sign == "x")
    {
        MatrixX4d rotationResult_X = points *  matrixRotationX;
        return rotationResult_X;
    }
    else if(sign == "y")
    {
        MatrixX4d rotationResult_Y = points *  matrixRotationY;
        return rotationResult_Y;
    }
    else if(sign == "z")
    {
        MatrixX4d rotationResult_Z = points *  matrixRotationZ;
        return rotationResult_Z;
    }
}

/******************************
//Name: PerspectiveProjectionPointOnXZ
//Author: Xufeng
//Description: perspective projection for  a point
//Input: distance d
//Output:Vector4d
//Date: 2014-03-28
//Version: 1.00
*****************************/

Eigen::Vector4d MainWindow::PerspectiveProjectionPointOnXZ(Eigen::Vector4d points,double d)
{
     //qDebug()<<"points"<<QString::number(points(0),'d',6)<<" "<<QString::number(points(1),'d',6)<<" "<<QString::number(points(2),'d',6)<<" "<<QString::number(points(3),'d',6);
      double z = points(2);
      Eigen:: Matrix4d projectedMatrix;
      projectedMatrix<< d/z, 0, 0, 0,
                        0, d/z, 0, 0,
                        0, 0, d/z, 0,
                        0, 0,  0,  1;

//      Eigen:: Matrix4d coordinateSystemMatrix;
//      coordinateSystemMatrix<< 1, 0, 0, 0,
//                               0, 1, 0, 0,
//                               0, 0,-1, 0,
//                               0, 0, 0, 1;

      Eigen::Vector4d result = points.transpose() * projectedMatrix ;
     // qDebug()<<"result Project"<<QString::number(result(0),'d',6)<<" "<<QString::number(result(1),'d',6)<<" "<<QString::number(result(2),'d',6)<<" "<<QString::number(result(3),'d',6);
      return result;
}

/******************************
//Name: Projection3DModel
//Author: Xufeng
//Description: perspective projection for  all points
//Input:MatrixX4d points, distance d
//Output:MatrixX4d
//Date: 2014-03-28
//Version: 1.00
*****************************/
Eigen::MatrixX4d MainWindow::Projection3DModel(Eigen::MatrixX4d points,double d)
{
    MatrixX4d matrixPointsProjected(points.rows(),4);
    Eigen::Vector4d pointProjectedResult;
    for(int i =0; i < points.rows();i++)
    {
        Eigen::Vector4d pointProjected(points(i,0),points(i,1),points(i,2),points(i,3));
        pointProjectedResult = PerspectiveProjectionPointOnXZ(pointProjected,d);

        for(int j=0;j<4;j++ )
        {
            matrixPointsProjected(i,j)= pointProjectedResult(j);
        }
    }

    return matrixPointsProjected;
}


void MainWindow::DisplayMatrix(MatrixX4d result)
{
    for(int i=0;i<result.rows();i++)
    {
        QString l;
        for(int j=0;j<result.cols();j++)
        {
            l+= QString::number(result(i,j),'d',4)+" ";
        }
        qDebug()<<l;
    }
}

void MainWindow::DisplayMatrix(Matrix4d result)
{
    for(int i=0;i<result.rows();i++)
    {
        QString l;
        for(int j=0;j<result.cols();j++)
        {
            l+= QString::number(result(i,j),'d',4)+" ";
        }
        qDebug()<<l;
    }
}

/******************************
//Name: on_pushButtonTranslation_clicked
//Author: Xufeng
//Description: operation of translation
//Input:
//Output:
//Date: 2014-04-03
//Version: 1.00
*****************************/
void MainWindow::on_pushButtonTranslation_clicked()
{
    double deltX, deltY,deltZ;
    QString pattern("(\\-)?\\d+(.\\d+){0,1}");
    QRegExp rx(pattern);

        if(ui->lineEditTransX->text()=="")
        {
            deltX = 0;
        }
        else
        {
            bool isMatchDeltX = rx.exactMatch(ui->lineEditTransX->text());
            if(isMatchDeltX)
            {
                deltX = ui->lineEditTransX->text().toDouble();
            }
        }

        if(ui->lineEditTransY->text()=="")
        {
            deltY = 0;
        }
        else
        {
            bool isMatchDeltY = rx.exactMatch(ui->lineEditTransY->text());
            if(isMatchDeltY)
            {
                deltY = ui->lineEditTransY->text().toDouble();
            }
        }

        if(ui->lineEditTransZ->text()=="")
        {
            deltZ = 0;
        }
        else
        {
            bool isMatchDeltZ = rx.exactMatch(ui->lineEditTransZ->text());
            if(isMatchDeltZ)
            {
                deltZ= ui->lineEditTransZ->text().toDouble();
            }
        }

    MatrixX4d translationMatrix = Translation(matrixPoints,deltX,deltZ,deltY);
    Eigen::MatrixX4d matrixPointsProjected = Projection3DModel(translationMatrix,distance);

    if(qAbs(deltY-0.00001) > 0)
    {
        double MinX_recorded = approximateMinX;
        double MinY_recorded = approximateMinY;

        double MaxX_recorded = approximateMaxX;
        double MaxY_recorded = approximateMaxY;

        preProcessData(matrixPointsProjected);

        if(approximateMaxX < MaxX_recorded)
        {
            approximateMaxX = MaxX_recorded;
        }
        if(approximateMaxY < MaxY_recorded)
        {
            approximateMaxY = MaxY_recorded;
        }
        if(approximateMinY > MinY_recorded)
        {
            approximateMinY = MinY_recorded;
        }
        if(approximateMinX > MinX_recorded)
        {
            approximateMinX = MinX_recorded;
        }

        RangeX = approximateMaxX - approximateMinX;
        RangeY = approximateMaxY - approximateMinY;
        RangeZ = approximateMaxZ - approximateMinZ;

    }

    QMap_CoordTransform = LoadMatrixToQMap(matrixPointsProjected);
    DisplayAllTriangles(QVec_TableTopology,QMap_CoordTransform);

}

/******************************
//Name: on_pushButtonRotationZ_clicked
//Author: Xufeng
//Description: operation of rotation around axis Z
//Input:
//Output:
//Date: 2014-04-03
//Version: 1.00
*****************************/
void MainWindow::on_pushButtonRotationZ_clicked()
{
    QString pattern("(\\-)?\\d+(.\\d+){0,1}");
    QRegExp rx(pattern);

    if(ui->lineEditRotationAngleZ->text()=="")
    {
        QMessageBox::information(this,"Tips","please input the rotation parameter");
    }
    else
    {
        bool isMatchRotationZ = rx.exactMatch(ui->lineEditRotationAngleZ->text());
        if(isMatchRotationZ)
        {
            rotationAngleZ += ui->lineEditRotationAngleZ->text().toDouble();
            //qDebug()<<"rotationAngleZ"<<rotationAngleZ;
        }

        MatrixX4d RotationZMatrix = Rotation(matrixPoints,rotationAngleZ,"x");
        Eigen::MatrixX4d matrixPointsProjected =Projection3DModel(RotationZMatrix,distance);

        preProcessData(matrixPointsProjected);
        QMap_CoordTransform = LoadMatrixToQMap(matrixPointsProjected);
        DisplayAllTriangles(QVec_TableTopology,QMap_CoordTransform);
    }



}

/******************************
//Name: on_pushButtonRotationY_clicked
//Author: Xufeng
//Description: operation of rotation around axis Y
//Input:
//Output:
//Date: 2014-04-03
//Version: 1.00
*****************************/
void MainWindow::on_pushButtonRotationY_clicked()
{

    QString pattern("(\\-)?\\d+(.\\d+){0,1}");
    QRegExp rx(pattern);

    if(ui->lineEditRotationAngleY->text()=="")
    {
        QMessageBox::information(this,"Tips","please input the rotation parameter");
    }
    else
    {
        bool isMatchRotationY = rx.exactMatch(ui->lineEditRotationAngleY->text());
        if(isMatchRotationY)
        {
            rotationAngleY += ui->lineEditRotationAngleY->text().toDouble();
            //qDebug()<<"rotationAngleY"<<rotationAngleY;
        }

        MatrixX4d RotationYMatrix = Rotation(matrixPoints,rotationAngleY,"z");
        Eigen::MatrixX4d matrixPointsProjected =Projection3DModel(RotationYMatrix,distance);

        preProcessData(matrixPointsProjected);
        QMap_CoordTransform = LoadMatrixToQMap(matrixPointsProjected);
        DisplayAllTriangles(QVec_TableTopology,QMap_CoordTransform);
    }


}

/******************************
//Name: on_pushButtonRotationX_clicked
//Author: Xufeng
//Description: operation of rotation around axis X
//Input:
//Output:
//Date: 2014-04-03
//Version: 1.00
*****************************/
void MainWindow::on_pushButtonRotationX_clicked()
{
    QString pattern("(\\-)?\\d+(.\\d+){0,1}");
    QRegExp rx(pattern);

    if(ui->lineEditRotationAngleX->text()=="")
    {
        QMessageBox::information(this,"Tips","please input the rotation parameter");
    }
    else
    {
        bool isMatchRotationX = rx.exactMatch(ui->lineEditRotationAngleX->text());
        if(isMatchRotationX)
        {
            rotationAngleX += ui->lineEditRotationAngleX->text().toDouble();
            //qDebug()<<"rotationAngleX"<<rotationAngleX;
        }


        MatrixX4d RotationXMatrix = Rotation(matrixPoints,rotationAngleX,"y");
        Eigen::MatrixX4d matrixPointsProjected =Projection3DModel(RotationXMatrix,distance);

        preProcessData(matrixPointsProjected);
        QMap_CoordTransform = LoadMatrixToQMap(matrixPointsProjected);
        DisplayAllTriangles(QVec_TableTopology,QMap_CoordTransform);

    }



}
/******************************
//Name: on_pushButtonScaling_clicked
//Author: Xufeng
//Description: operation of scaling trigered by scaling button
//Input:
//Output:
//Date: 2014-04-03
//Version: 1.00
*****************************/
void MainWindow::on_pushButtonScaling_clicked()
{
    QString pattern("(\\-)?\\d+(.\\d+){0,1}");
    QRegExp rx(pattern);
    double scalingX,scalingY,scalingZ;

    if(ui->lineEditScaleX->text()=="")
    {
        scalingX = 1;
    }
    else
    {
        bool isMatchscalingX = rx.exactMatch(ui->lineEditScaleX->text());
        if(isMatchscalingX)
        {
            scalingX = ui->lineEditScaleX->text().toDouble();
           // qDebug()<<"scalingX"<<scalingX;
        }
    }

    if(ui->lineEditScaleY->text()=="")
    {
        scalingY = 1;
    }
    else
    {
        bool isMatchscalingY = rx.exactMatch(ui->lineEditScaleY->text());
        if(isMatchscalingY)
        {
            scalingY = ui->lineEditScaleY->text().toDouble();
            //qDebug()<<"scalingY"<<scalingY;
        }
    }

    if(ui->lineEditScaleZ->text()=="")
    {
        scalingZ = 1;
    }
    else
    {
        bool isMatchscalingZ = rx.exactMatch(ui->lineEditScaleZ->text());
        if(isMatchscalingZ)
        {
            scalingZ= ui->lineEditScaleZ->text().toDouble();
            //qDebug()<<"scalingZ"<<scalingZ;
        }
    }

    double centerX = (approximateMaxX + approximateMinX)/2;
    double centerY = (approximateMaxY + approximateMinY)/2;
    double centerZ = (approximateMaxZ + approximateMinZ)/2;
    MatrixX4d translationMatrix = Translation(matrixPoints,- centerX,- centerY,- centerZ);
    MatrixX4d scalingMatrix = Scaling(translationMatrix,scalingX,scalingZ,scalingY);
    MatrixX4d after_scalingMatrix = Translation(scalingMatrix, centerX, centerY, centerZ);
    Eigen::MatrixX4d matrixPointsProjected =Projection3DModel(after_scalingMatrix,distance);


    QMap_CoordTransform = LoadMatrixToQMap(matrixPointsProjected);
    DisplayAllTriangles(QVec_TableTopology,QMap_CoordTransform);

}
