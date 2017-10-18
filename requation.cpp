 #include "requation.h"
#include "math.h"

using namespace std;

//  conjugate gradient method求解Ax=b;
int eqsolver(const CSC A,  vector<double> &x, const vector<double> b,const vector<double> x0)
{
    vector<double> r,p,Ap;
    double rsold,alpha,rsnew;
    x=x0;//取迭代初始点
    r=b-A*x;
    p=r;
    rsold=r*r;
    rsnew=rsold;
    int count=0;//记数用，可删除
    for(unsigned long i=0;i<b.size();i++)
    {
       Ap=A*p;
       alpha=rsold/(p*Ap);
       x=x+alpha*p;
       r=r-alpha*Ap;
       rsnew=r*r;
       if(rsnew<1e-13)
           break;
       p=r+(rsnew/rsold)*p;
       rsold=rsnew;
       count++;
    }
    return count;
}
//the preconditioned conjugate gradient method
int eqsolver1(const CSC A, vector<double> &x, const vector<double>b, const vector<double> x0)
{
    //提取主对角线元素形成preconditioning M matrix(use vector to express)
    unsigned long n=b.size();
   CSC M;
   M.column=vector<int>(n);
   M.index=vector<int>(n+1);
   M.value=vector<double>(n);
   auto it=A.index.begin();
   for(unsigned long i=0;i<n;i++)
   {
       M.column[i]=i;
       for(int j=*it;j<*(it+1);j++)
       if(A.column[j]==i) M.value[i]=1/A.value[j];
       ++it;
   }
   M.index=M.column;
   M.index.push_back(n);


    vector<double> r,p,z;
    double rsold,alpha,rsnew;
    x=x0;//取迭代初始点
    r=b-A*x;
    z=M*r;
    p=z;
    rsold=z*r;
    int count=0;//记数用，可删除
//    double deltold,deltnew=1;
    for(unsigned long i=0;i<n;i++)
    {
//        deltold=deltnew;
       alpha=rsold/(p*(A*p));
       x=x+alpha*p;
       r=r-alpha*(A*p);
//       deltnew=x*x;
       if(r*r<1e-10)
           break;
       z=M*r;
       rsnew=z*r;
       p=z+(rsnew/rsold)*p;
       rsold=rsnew;
       count++;
    }
    return count;
}
//the flexible preconditioned conjugate gradient method
int eqsolver2( const CSC A, vector<double> &x, const vector<double>b, const vector<double> x0)
{
    //提取主对角线元素形成preconditioning M matrix(use vector to express)
    unsigned long n=b.size();
   CSC M;
   M.column=vector<int>(n);
   M.index=vector<int>(n+1);
   M.value=vector<double>(n);
   auto it=A.index.begin();
   for(unsigned long i=0;i<n;i++)
   {
       M.column[i]=i;
       for(int j=*it;j<*(it+1);j++)
       if(A.column[j]==i) M.value[i]=1/A.value[j];
       ++it;
   }
   M.index=M.column;
   M.index.push_back(n);


    vector<double> r,p,z,rold,zold;
    double alpha;
    x=x0;//取迭代初始点
    r=b-A*x;
    z=M*r;
    p=z;
    rold=r;zold=z;
    int count=0;//记数用，可删除
    for(unsigned long i=0;i<n;i++)
    {
       alpha=z*r/(p*(A*p));
       x=x+alpha*p;
       r=r-alpha*(A*p);
       if(r*r<1e-13)
           break;
       z=M*r;
       p=z+(z*(r-rold)/(zold*rold))*p;
       rold=r;zold=z;
       count++;
    }
    return count;
}
void MplusM(const CSC M1,const CSC M2,double a,double b,CSC &outcome)
{    
//清空outcome
outcome.column.clear();outcome.index.clear();outcome.value.clear();
if (M1.index.size()!=M2.index.size()) cout<<"Dimension of M1 is not equal to M2";
//按照每列计算矩阵和
int dim=M1.index.size()-1,count=0;
auto it1=M1.column.begin(),it2=M2.column.begin();
for(int i=0;i<dim;i++)
   {
   outcome.index.push_back(count);
   bool ret1=1,ret2=1;
   while(1){
       if(ret1&&ret2)
       {
           if(*it1>*it2){
             outcome.column.push_back(*it2);
             outcome.value.push_back(b*M2.value[it2-M2.column.cbegin()]);
             ++it2;++count;}
           else if(*it1<*it2){
               outcome.column.push_back(*it1);
               outcome.value.push_back(a*M1.value[it1-M1.column.cbegin()]);
               ++it1;++count;}
           else {outcome.column.push_back(*it1);
               outcome.value.push_back(a*M1.value[it1-M1.column.cbegin()]+b*M2.value[it2-M2.column.cbegin()]);
               ++it1;++it2;++count;}
       }
       else if(ret1&&!ret2)
       {
           outcome.column.push_back(*it1);
           outcome.value.push_back(a*M1.value[it1-M1.column.cbegin()]);
           ++it1;++count;
       }
       else if(!ret1&&ret2)
       {
           outcome.column.push_back(*it2);
           outcome.value.push_back(b*M2.value[it2-M2.column.cbegin()]);
           ++it2;++count;
       }
       else break;
         ret1=it1!=M1.column.cbegin()+M1.index[i+1];
         ret2=it2!=M2.column.cbegin()+M2.index[i+1];}
   }
outcome.index.push_back(count);
}

//计算csc矩阵乘向量

vector<double> operator+(const vector<double> &a,const vector<double> &b)
{
    int n=a.size();
    vector<double>outcome(n);
    for(int i=0;i<n;i++)
        outcome[i]=(a[i]+b[i]);
    return outcome;
}
vector<double> operator *(const vector<double> &a,double m)
{
    int n=a.size();
    vector<double>outcome(n);
    for(int i=0;i<n;i++)
        outcome[i]=(a[i]*m);
    return outcome;
}
vector<double>operator -(const vector<double> &a,const vector<double> &b)
{   
    int n=a.size();
    vector<double>outcome(n);
    for(int i=0;i<n;i++)
        outcome[i]=(a[i]-b[i]);
    return outcome;
}
vector<double> operator *(double m,const vector<double> &a)
{
    int n=a.size();
    vector<double>outcome(n);
    for(int i=0;i<n;i++)
        outcome[i]=(a[i]*m);
    return outcome;
}
double operator *(const vector<double> &a,const vector<double> &b)
{
double result=0;
int n=a.size();
for(int i=0;i<n;i++)
    result+=a[i]*b[i];
return result;
}
vector<double> operator *(const CSC &A,vector<double>V)
{
    int diA=V.size(),row=0;
    vector<double>result(diA);
    auto it=A.column.begin();
    while(it!=A.column.end())
    {
       result[*it]+=A.value[it-A.column.cbegin()]*V[row];
       it++;
       if (it-A.column.cbegin()==A.index[row+1]) ++row;
    }
    return result;
}


void eqsolver_R(const vector<double> &A, vector<double> &x, const vector<double>b, const vector<double> x0)
{      //提取主对角线元素形成preconditioning M matrix(use vector to express)
    unsigned long n=b.size();
   vector<double>M(n);
   for(unsigned long i=0;i<n;i++)
   {
      M[i]=1/A[i*n+i];
   }
    vector<double> r,p,z,temp;
    double rsold,alpha,rsnew;
    x=x0;//取迭代初始点
    MxV(A,x,n,n,temp);
    r=b-temp;
    z=VXV(M,r);
    p=z;
    rsold=z*r;
    int count=0;//记数用，可删除
//    double deltold,deltnew=1;
    for(unsigned long i=0;i<n;i++)
    {
//       deltold=deltnew;
       MxV(A,p,n,n,temp);
       alpha=rsold/(p*temp);
       x=x+alpha*p;
       r=r-alpha*temp;
//       deltnew=x*x;
       if(r*r<1e-10)
           break;
       z=VXV(M,r);
       rsnew=z*r;
       p=z+(rsnew/rsold)*p;
       rsold=rsnew;
       count++;
    }

}

void MxV(const vector<double> M,const vector<double>V, const int m, const int n,vector<double>&MV)
{
    MV.clear();
    MV=vector<double>(V.size());
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++)
            MV[i] += M[i*n+j]* V[j];
     }
}

vector<double> VXV(const vector<double> &a,const vector<double> &b)
{
    unsigned n=b.size();
    vector<double>outcome(n);
    for(unsigned i=0;i<n;i++)
       outcome[i]=a[i]*b[i];
    return outcome;
}
