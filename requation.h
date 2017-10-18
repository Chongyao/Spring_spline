#ifndef REQUATION_H
#define REQUATION_H
#include<vector>
#include<map>
#include<iostream>
using namespace std;
struct CSC
{
  vector<int>column;
  vector<int>index;
  vector<double>value;
};
//重载了矢量的加、减、乘、数乘
vector<double> operator +(const vector<double> &a,const vector<double> &b);
vector<double> operator *(const vector<double> &a,double m);
vector<double> operator -(const vector<double> &a,const vector<double> &b);
vector<double> operator *(double m,const vector<double> &a);
double operator *(const vector<double> &a,const vector<double> &b);
vector<double> operator *(const CSC &A,vector<double>V);
//求解Ax=b
int eqsolver(const CSC A, vector<double> &x, const vector<double>b, const vector<double> x0);
int eqsolver1(const CSC A, vector<double> &x, const vector<double>b, const vector<double> x0);
int eqsolver2(const CSC A, vector<double> &x, const vector<double>b, const vector<double> x0);
void eqsolver_R(const vector<double> &A,vector<double> &x,const vector<double>b, const vector<double> x0);
//CSS+CSC
void MplusM(const CSC M1,const CSC M2,double a,double b,CSC &outcome);

//not CSC
void MxV(const vector<double> M, const vector<double>V, const int m, const int n, vector<double> &MV);
vector<double> VXV(const vector<double> &a,const vector<double> &b);
#endif // REQUATION_H
