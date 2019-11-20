#ifndef PLOTSETTINGS_H
#define PLOTSETTINGS_H

class PlotSettings
{
public:
   // PlotSettings();
public:
   //PlotSettings();
    PlotSettings(): minX(0), maxX(10000), numXTicks(10), minY(0), maxY(100),numYTicks(5){};
    PlotSettings(double XMax,double XMin,int nXTicks,double YMax,double YMin,int nYTicks)
        : minX(XMin), maxX(XMax), numXTicks(nXTicks), minY(YMin), maxY(YMax),numYTicks(nYTicks)
    {}

    void scroll(double dx, double dy);
    void adjust();
    double spanX() const { return maxX - minX; }
    double spanY() const { return maxY - minY; }

    double minX;
    double maxX;
    int numXTicks;
    double minY;
    double maxY;
    int numYTicks;

private:
    static void adjustAxis(double &min, double &max, int &numTicks);
};

#endif // PLOTSETTINGS_H
