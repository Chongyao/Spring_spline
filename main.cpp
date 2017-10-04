#include <iostream>
#include"halfedge.h"
#include"simplifymesh.h"
#define iter_times 1000
using namespace std;

int main()
{
    halfedge data;
    string Input="../data/input/bunny.obj";
    data.ReadData(Input);
    data.ConstructHalfedge();
    simplify_mesh data_simp(data);
    data_simp. Simp_shorstest(iter_times);
    string Output = "../data/output/bunny_out.obj";
    data. halfedge_to_obj(Output);
    return 0;
}
