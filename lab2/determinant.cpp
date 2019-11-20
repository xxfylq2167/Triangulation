#include "determinant.h"

using namespace std;
#include <iostream>
#include <cmath>


determinant::determinant()
{
}

// compute the determinant recursively

/******************************
/*Name: ComputeDet
/*Author: Xufeng
/*Description: Calculate the determiant
/*Input: the arbitrary determiant
/*Output: the result of determiant
/*Date: 2014-01-23
/*Version: 1.00
*****************************/
double determinant::ComputeDet(DataType **a,int n)
{
    if(n==1) return a[0][0];
    if(n==2) return a[0][0]*a[1][1]-a[1][0]*a[0][1];
    double b=0;
    for(int i=0;i<n;i++)
    {
      b = b + pow(-1.0,i)* a[0][i] * Left(a,0,i,n);
    }
    return b;
}

//compute the sub-determinant recursively
double determinant::Left(DataType **a, int x, int y, int n)
{
    DataType **leftmatrix = new DataType*[n-1];
    for(int i=0; i<n-1; i++)
    {
      leftmatrix[i]=new DataType[n-1];
    }
    // get the sub-determinant
    int x1=0,y1=0;
    for(int i=0;i<n;i++)
    {
        if(i==x) continue;
        y1=0;
        for(int j=0;j<n;j++)
        {
            if(j==y) continue;
            leftmatrix[x1][y1]=a[i][j];
            y1++;
        }
     x1++;
    }
    //compute the sub-determinant recursively
    double re = ComputeDet(leftmatrix,n-1);

    for(int i=0;i<n-1;i++)
    {
        delete[] leftmatrix[i];
    }
        delete[] leftmatrix;
    return re;
}


