#include "plotsettings.h"
#include <cmath>
#define MinScale 10

void PlotSettings::scroll(double dx, double dy)
{
    double stepX = spanX() / numXTicks;
    minX += dx * stepX;
    maxX += dx * stepX;

    double stepY = spanY() / numYTicks;
    minY += dy * stepY;
    maxY += dy * stepY;
}

void PlotSettings::adjust()
{
    adjustAxis(minX, maxX, numXTicks);
    adjustAxis(minY, maxY, numYTicks);
}

void PlotSettings::adjustAxis(double &min, double &max,int &numTicks)
{
    const int MinTicks = MinScale;
    double grossStep = (max - min) / MinTicks;
   // qDebug()<<"grossStep:"<<grossStep;

    double step = pow(10.0, floor(log10(grossStep)));
    // qDebug()<<"step:"<<step;

    if (5 * step < grossStep) {
        step *= 5;

    } else if (2 * step < grossStep) {
        step *= 2;

    }

    numTicks = int(ceil(max / step) - floor(min / step));
     //qDebug()<<"numTicks:"<<numTicks;
    if (numTicks < MinTicks)
        numTicks = MinTicks;
    if (numTicks > ceil(1.5*MinTicks))
        numTicks = ceil(1.5*MinTicks);


}
