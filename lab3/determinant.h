#ifndef DETERMINANT_H
#define DETERMINANT_H





typedef  double DataType;
using namespace std;

class determinant
{
public:
    determinant();

    double Left(DataType **a, int x, int y,int n);
    double ComputeDet(DataType **a,int n);


};

#endif // DETERMINANT_H
