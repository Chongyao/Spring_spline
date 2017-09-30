#include "simplifymesh.h"
#include<algorithm>
#define INF 99999999；
using namespace std;

bool operator <(const ident &a,const ident &b){
    return a.ident<b.ident;
}


simplify_mesh::SimplifyMesh() {

}


void simplify_mesh::Simp_shorstest()
{
    //find the shortest edge
    size_t num = mesh_init.HalfEdges.size();
    priority = vector<ident>(num);
    for (size_t i = 0; i < num; i++){
        priority[i].id = i;
        priority[i].ident = mesh_init.HalfEdges[i].length;
    }
    make_heap(priority.begin(),priority.end(),operator <());
    sort_heap(priority.begin(),priority.end());

    size_t edge_id=priority[0].id;
    double length_min=priority[0].length;

    if(priority[1].id!=mesh_init.HalfEdges[edge_id].oppo_)
    cout<<"error in sort";

    //change the topology
    size_t  edge_oppo_id = mesh_init.HalfEdges[edge_id].oppo_;{
        //delete the edege
        mesh_init.HalfEdges[edge_id].is_exist=false;
        mesh_init.HalfEdges[edge_oppo_id].is_exist=false;
    }

    size_t edge_prev_id = mesh_init.HalfEdges[edge_id].prev_,
           vertex_r_id = mesh_init.HalfEdges[edge_prev_id].vertex_;{
        //delete the vertex
        mesh_init.Vertexs[vertex_r_id].is_exist=false;
    }



    size_t edge_end_id=mesh_init.Vertexs[vertex_r_id].edge_;
    do{
      size_t edge_change_id=;
        mesh_init.HalfEdges[edge_prev_id];





    }while()




}

