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

    connect(ui->radioButtonCoordinate, SIGNAL(clicked()), this, SLOT(on_radioButton_Coordinate_clicked()));

    isDisplayCoordinate=false;
    fileName="";
}

MainWindow::~MainWindow()
{
    delete ui;
}

/******************************
//Name: readTextFile
//Author: Xufeng
//Description: get coordinate from text file
//Input: fileName,the sign for split text
//Output: the coordiante, the minimum and maximum of X and Y, approximated range of X and Y for display coordiante
//Date: 2014-01-23
//Version: 1.00
*****************************/
bool MainWindow::readTextFile(QString fileName,QString SplitSign)
{
    QStringList line_content;

    QStringList X;
    QStringList Y;


    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream readin(&file);
        while (!readin.atEnd())
        {
            QString readline = readin.readLine();

            line_content =  readline.split(SplitSign,QString::SkipEmptyParts);
           // qDebug()<< line_content;

            if(line_content.count()== 3)
            {

                int ID = line_content[0].toShort();
                double x = line_content[1].toDouble();
                double y = line_content[2].toDouble();

                X.append(line_content[1]);
                Y.append(line_content[2]);

                //organize x and y as QPointF
                QVec_PolygonVertex.append(QPointF(x,y));
            }
            else
            {
                return false;
            }

        }
       // qDebug()<< X;
       // qDebug()<< Y;

        if(X.count()==Y.count())
        {
            //find min and max value from X
            MinX= X[0].toDouble();
            MaxX= X[0].toDouble();
            for(int i=1;i < X.count();i++)
            {
                MinX=Find_Min(MinX,X[i].toDouble());
                MaxX=Find_Max(MaxX,X[i].toDouble());
            }
            //find min and max value from X
            MinY= Y[0].toDouble();
            MaxY= Y[0].toDouble();
            for(int j=1;j < Y.count();j++)
            {
                MinY=Find_Min(MinY,Y[j].toDouble());
                MaxY=Find_Max(MaxY,Y[j].toDouble());
            }

            double MinX_fixed=0;
            double MaxX_Approximate = ceil(MaxX/10)*10 + 2*ceil(MaxX/100)*10;
            double MinY_fixed =0;
            double MaxY_Approximate= ceil(MaxY/10)*10+ 2*ceil(MaxY/100)*10;

            approximateMinX =0;
            approximateMinY =0;
            RangeX = MaxX_Approximate - MinX_fixed;
            RangeY = MaxY_Approximate - MinY_fixed;

          //  qDebug()<< RangeX<<" " << RangeY<<" ";
            setPolygonData(0,QVec_PolygonVertex,1);
        }
        return true;

    }
    else
    {
         QMessageBox::warning(this,"Tips",fileName+" did not open");
    }

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


void MainWindow::setPolygonData(int id, const QVector<QPointF> &data, int flag)
{
    if(flag==1)
    {
        QMap_coordinate[id] = data;
    }
    else if(flag==2)
    {
        QMap_PolyCenter[id] = data;
    }
    else if(flag==3)
    {
        QMap_PolyOutPoint[id] = data;
    }

}

void MainWindow::setHatchLineData(int id, const QVector<LineSegments> &data, int flag)
{
    if(flag==1)
    {
        QMap_HatchLines[id] = data;
    }
}

void MainWindow::paintEvent(QPaintEvent * /*event */)
{

    QStylePainter painter(this);
    painter.drawPixmap(ui->Pixmapframe->pos().x()+PixMapLeftMargin,
         ui->Pixmapframe->pos().y()+ui->menuBar->height()+ui->mainToolBar->height()+PixMapTopMargin,  pixmap);

}

void MainWindow::resizeEvent(QResizeEvent *)
{

    int RightMarginforButton = 250;

    int DisVerticalButtons = 40;

    ui->Pixmapframe->setGeometry(10,5 + PixMapTopMargin,this->width() - RightMarginforButton , height()-PixMapTopMargin-50);
    int x=ui->Pixmapframe->width()+ 25 + PixMapTopMargin;
    ui->radioButtonCoordinate->setGeometry(x,10+PixMapTopMargin,120,30);
    ui->groupBoxArea->setGeometry(x,10 + DisVerticalButtons+PixMapTopMargin,200,90);
    ui->groupBoxCenter->setGeometry(x,10+10+DisVerticalButtons+ ui->groupBoxArea->height() + PixMapTopMargin,200,130);
    ui->groupBoxHatch->setGeometry(x,10+20+DisVerticalButtons+ ui->groupBoxCenter->height() + ui->groupBoxArea->height()+ PixMapTopMargin,200,150);


    refreshPixmap();
}

void MainWindow::refreshPixmap()
{
    //qDebug()<<"refreshPixmap";
    pixmap = QPixmap(ui->Pixmapframe->size().width()-PixMapLeftMargin -PixMapRightMargin,
                     ui->Pixmapframe->size().height()-PixMapTopMargin -PixMapBottomMargin);
    pixmap.fill(this, 0, 0);
    pixmap.fill(Qt::white);//background colour

    QPainter painter(&pixmap);
    painter.initFrom(this);


    if(fileName!="")
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
    for (int i = 0; i <= numXTicks; i++)
    {
        int x = rect.left() + (i * (rect.width() - 1)
                                 / numXTicks);
        double label = approximateMinX + (i * RangeX/numXTicks);

        painter->setPen(QPen(Qt::black));
        painter->setPen(Qt::DotLine);
        painter->drawLine(x, rect.top(), x, rect.bottom());

        //draw scales
        painter->setPen(QPen(Qt::black,2));
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

        if(qAbs(label-0)<0.000001){label=0;}
        painter->setPen(quiteDark);
        painter->setPen(Qt::DotLine);
        painter->drawLine(rect.left(), y, rect.right(), y);

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
    QPolygonF polyCenter(1);
    QPolygonF polyOutPoint(1);
    //the range for draw polygon
    QRect rect(LeftMargin, TopMargin ,
               ui->Pixmapframe->width() - RightMargin - LeftMargin  - PixMapRightMargin - PixMapLeftMargin,
               ui->Pixmapframe->height() - TopMargin - BottomMargin - PixMapTopMargin - PixMapBottomMargin);

    if (!rect.isValid())
        return;

    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

    //draw polygon
    QMapIterator<int, QVector<QPointF> > i1(QMap_coordinate);
    while (i1.hasNext())
    {
        i1.next();

       //int id = i.key();
        const QVector<QPointF> &data = i1.value();
        QPolygonF polyline(data.count());

        // transfer the coordinate to the position on the pixmap
        for (int j = 0; j < data.count(); ++j) {

            double dx = data[j].x() - approximateMinX;
            double dy = data[j].y() - approximateMinY;
            double x = rect.left() + (dx * (rect.width()-1 )
                                         / RangeX);
            double y = rect.bottom() - (dy * (rect.height() -1)
                                           / RangeY);
            polyline[j] = QPointF(x, y);
            painter->setPen(QPen(Qt::red));

            drawSign(x,y,painter,3);

            if(isDisplayCoordinate)
            {
                drawCoordinateText(x,y,painter,"("+QString::number(data[j].x())+","+QString::number(data[j].y())+")");
            }
        }
        //draw points
        painter->setPen(QPen(Qt::black,3));
        painter->drawPoints(polyline);

        //draw polygon
        painter->setPen(QPen(Qt::black));

        for (int j = 0; j < data.count(); ++j)
        {
            painter->drawLine(polyline[j],polyline[(j+1)%data.count()]);
        }
        painter->drawLine(polyline[data.count()-1],polyline[0]);
    }

    if(isDisplayCenter)
    {
        //draw center of gravity
        QMapIterator<int, QVector<QPointF> > i2(QMap_PolyCenter);
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

                drawSign(x,y,painter,6);

                if(isDisplayCoordinate)
                {
                    drawCoordinateText(x,y,painter,"("+QString::number(data[j].x(),'g',4)+","+QString::number(data[j].y(),'g',4)+")");
                }
            }
            //draw points
            painter->setPen(QPen(Qt::darkBlue,4));
            painter->drawPoints(polyline);
            polyCenter = polyline;
        }

        //draw a point out of polygon randomly
        QMapIterator<int, QVector<QPointF> > i3(QMap_PolyOutPoint);
        while (i3.hasNext())
        {
            i3.next();
            const QVector<QPointF> &data = i3.value();
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
                painter->setPen(QPen(Qt::blue,3));

                drawSign(x,y,painter,6);

                if(isDisplayCoordinate)
                {
                    drawCoordinateText(x,y,painter,"("+QString::number(data[j].x(),'g',4)+","+QString::number(data[j].y(),'g',4)+")");
                }
            }
            //draw points
            painter->setPen(QPen(Qt::red,4));
            painter->drawPoints(polyline);
            polyOutPoint = polyline;
        }
        painter->setPen(QPen(Qt::blue,1));
        painter->drawLine(polyCenter[0], polyOutPoint[0]);
    }


    if(isDisplayHatching)
    {
        //draw hatching lines of polygon
        QMapIterator<int, QVector<LineSegments> > i4(QMap_HatchLines);
        while (i4.hasNext())
        {
            i4.next();
            const QVector<LineSegments> &data = i4.value();
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

                painter->setPen(QPen(Qt::blue,2));


                painter->setPen(QPen(Qt::blue,1));
                painter->drawLine(polylineStart[j],polylineEnd[j]);
            }
            //draw points
            painter->setPen(QPen(Qt::blue,3));
            painter->drawPoints(polylineStart);
            painter->drawPoints(polylineEnd);
        }
    }

 }


double MainWindow::ComputePolyArea(QVector<QPointF> QVec_points)
{
    QVector<double> areaTri;
    ComputeTriArea Tri_Area;

    for(int i=1; i+1< QVec_points.count(); i++)
    {
        QVector<QPointF> temp;
        //organize triangle from polygon, the first vertex is the starting vertex
        temp.append(QVec_points[0]);
        temp.append(QVec_points[i]);
        temp.append(QVec_points[i+1]);
        double area = Tri_Area.ComputeTri_Area(temp);
        areaTri.append(area);
        temp.clear();

    }

    //qDebug()<<areaTri;
   // qDebug()<<areaTri.count();

    double totalArea=0;
    for(int j=0; j< areaTri.count();j++)
    {
        totalArea = totalArea + areaTri[j];
    }
    return totalArea;
   // qDebug()<<"total polygon area is: "<<totalArea;

}

QPointF MainWindow::ComputePolyCenter(QVector<QPointF> QVec_points)
{
    QVector<double> areaTri;
    QVector<QPointF> centerTri;

    ComputeTriArea Tri_Area;
    ComputeTriCenter Tri_Center;

    for(int i=1; i+1< QVec_points.count(); i++)
    {
        QVector<QPointF> temp;
        //organize triangle from polygon, the first vertex is the starting vertex
        temp.append(QVec_points[0]);
        temp.append(QVec_points[i]);
        temp.append(QVec_points[i+1]);
        double area = Tri_Area.ComputeTri_Area(temp);
        QPointF center = Tri_Center.ComputeTri_Center(temp);
        areaTri.append(area);
        centerTri.append(center);
        //qDebug()<<temp<<center<<area;
        temp.clear();
    }

    double totalArea=0;
    for(int j=0; j< areaTri.count();j++)
    {
        totalArea += areaTri[j];
    }

    double xCenter = 0,yCenter = 0;
    for(int k=0; k< centerTri.count() && centerTri.count()== areaTri.count() ;k++)
    {
        xCenter += (centerTri[k].x()*(areaTri[k]/totalArea));
        yCenter += (centerTri[k].y()*(areaTri[k]/totalArea));
    }
    return QPointF(xCenter,yCenter);

}


void MainWindow::on_pushButton_ComputeArea_clicked()
{
    double polygonArea = ComputePolyArea(QVec_PolygonVertex);

    ui->textEditResultArea->setText("The area of polygon:  "+QString::number(polygonArea));

}
void MainWindow::on_radioButton_Coordinate_clicked()
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

void MainWindow::on_pushButton_Center_clicked()
{
    isDisplayCenter=true;
    isDisplayHatching=false;

    QPointF PolyCenter = ComputePolyCenter(QVec_PolygonVertex);
    ui->textEditResultCenter->setText(
                "1.  The coordinate of the center of gravity for polygon:\n("
                +QString::number(PolyCenter.x())+","+QString::number(PolyCenter.y())+")");

    QVector<QPointF> polyCenterGravity;
    polyCenterGravity.append(PolyCenter);
    setPolygonData(0,polyCenterGravity,2);


    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int minX = static_cast<int>(MinX);
    int maxX = static_cast<int>(MaxX);
    int minY = static_cast<int>(MinY);
    int maxY = static_cast<int>(MaxY);

    int x1 = qrand()%(minX-1);
    int x2 = maxX+1 + qrand()%(static_cast<int>(RangeX)- (maxX+1));
    int y = minY-1 + qrand()%(static_cast<int>(RangeY)- (minY-1));

    int y1 = qrand()%(minY-1);
    int y2 = maxY+1 + qrand()%(static_cast<int>(RangeY)- (maxY+1));
    int x = minX-1 + qrand()%(static_cast<int>(RangeX)- (minX-1));

    int nx = qrand()%(2);
    int ny = qrand()%(2);
    int n = qrand()%(2);

    QVector<int> X12_random;
    QVector<int> Y12_random;
    X12_random.clear();
    Y12_random.clear();

    X12_random.append(x1);
    X12_random.append(x2);
    Y12_random.append(y1);
    Y12_random.append(y2);


    QVector<QPointF> polyOutPointSelection;
    polyOutPointSelection.append(QPointF(x,Y12_random[ny]));
    polyOutPointSelection.append(QPointF(X12_random[nx],y));

    QVector<QPointF> polyOutPoint;
    polyOutPoint.append(polyOutPointSelection[n]);
   // polyOutPoint.append(QPointF(21,7));
    setPolygonData(0,polyOutPoint,3);
    refreshPixmap();

    IntersectionSegments SegIntersect;
    bool isInPolygon = SegIntersect.PointInPolygon(PolyCenter,QVec_PolygonVertex,polyOutPointSelection[n]);
    //bool isInPolygon = SegIntersect.PointInPolygon(PolyCenter,QVec_PolygonVertex,QPointF(21,7));

    //the intersection is on the vertex
    if(SegIntersect.commonIntersection > 0)
    {

        QMessageBox::information(this,"Tips","The point selected out of polygon is invalid");
        ui->textEditResultCenter->append("2.  The point selected out of polygon is invalid");
    }
    else if(isInPolygon)
    {
        ui->textEditResultCenter->append("2.  The center of gravity is in the polygon");
        ui->textEditResultCenter->append("3.  The intersection number: "+QString::number(SegIntersect.intersectionNumber));
    }

    else
    {
        ui->textEditResultCenter->append("2.  The center of gravity is out of the polygon");
    }
}

void MainWindow::on_pushButtonHatch_clicked()
{
    isDisplayCenter=false;
    isDisplayHatching=true;
    IntersectionSegments hatchPoly;
    double angle,dis;
    QString pattern("\\d+(.\\d+){0,1}");
    QRegExp rx(pattern);

    if(ui->lineEditAngle->text()=="" && ui->lineEditDistance->text()=="")
    {
        angle = 45;
        dis = 1;

        QVector<LineSegments> HatchLines = hatchPoly.CalculateAllHatchLine
                (QVec_PolygonVertex,angle,dis,
                 QRectF(QPointF(MinX,MaxY),QPointF(MaxX,MinY)));
        setHatchLineData(0,HatchLines,1);
        refreshPixmap();
    }
    else
    {
        if(ui->lineEditAngle->text()=="" || ui->lineEditDistance->text()=="")
        {
            QMessageBox::information(this,"Tips","please input the angle and inclination");
        }
        else
        {
            bool matchAngle = rx.exactMatch(ui->lineEditAngle->text());
            bool matchDis = rx.exactMatch(ui->lineEditDistance->text());

            if(matchAngle && matchDis)
            {
                angle = ui->lineEditAngle->text().toDouble();
                dis = ui->lineEditDistance->text().toDouble();

                if(angle > 90 || angle < 0)
                {
                    ui->lineEditDistance->clear();
                    ui->lineEditAngle->clear();
                    QMessageBox::information(this,"Tips","please input the angle between 0 and 90");
                }
                else if(qAbs(angle-0)< 0.1)
                {
                    angle=0.1;
                }
                else if(qAbs(angle-90)< 0.5)
                {
                    angle=89.5;
                }

                QVector<LineSegments> HatchLines =
                        hatchPoly.CalculateAllHatchLine(QVec_PolygonVertex,angle,dis,
                                                        QRectF(QPointF(MinX,MaxY),QPointF(MaxX,MinY)));
                setHatchLineData(0,HatchLines,1);
                refreshPixmap();
            }
            else
            {
                ui->lineEditDistance->clear();
                ui->lineEditAngle->clear();
                QMessageBox::information(this,"Tips","The format of Inclination or Distance is incorrect");
            }
        }
    }

}

void MainWindow::on_actionOpen_file_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("Textfiles (*.txt)"));
    if(!fileName.isNull() && readTextFile(fileName," "))
    {
        refreshPixmap();
    }

}
