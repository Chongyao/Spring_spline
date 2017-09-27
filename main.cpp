#include <iostream>
#include"halfedge.h"
using namespace std;

int main()
{
    halfedge data;
    string Input="bunny.obj";
    data.ReadData(Input);
    data.ConstructHalfedge();



    return 0;
}
