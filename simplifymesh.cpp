g#include "simplifymesh.h"
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

  //change the topology
  change_topology(priority[0].id, priority[1].id);
}

    void simplify_mesh::change_topology( const size_t &edge_id, const size_t &edge_oppo_id){
      size_t  edge_r_id = edge_id;{//delete the edges
      do{
        mesh_init.HalfEdges[edge_r_id].is_exist = false;
        edge_r_id = mesh_init.HalfEdges[edge_id].next_;
      }while(edge_id == edge_r_id)
 
        edge_r_id = mesh_init.HalfEdges[edge_id].oppo_;
      do{
        mesh_init.HalfEdges[edge_r_id].is_exist = false;
        edge_r_id = mesh_init.HalfEdges[edge_id].next_;
      }while(edge_id == edge_r_id)
        mesh_init.HalfEdges[edge_oppo_id].is_exist=false;
    }
    size_t vertex_r_id = mesh_init.HalfEdges [ edge_oppo_id ].vertex_;{
      //delete the vertex
      mesh_init.Vertexs [ vertex_r_id ].is_exist = false;
    }

    size_t face_r_id = mesh_init.HalfEdges[ edge_id ].face_;{
      //delete the face
      mesh_init. Faces[ face_r_id ]. is_exist = false;
      face_r_id = mesh_init. HalfEdges[ edge_oppo_id ];
      mesh_init. Faces[ face_r_id ]. is_exist = false;
    }


    {        //change the vertex
      const size_t edge_end_id=mesh_init. HalfEdges[ edge_id]. prev_;
      const size_t vertex_ur_id=mesh_init.HalfEdges[edge_id];
      size_t edge_change_id = mesh_init.HalfEdges[ edge_oppo_id]. next_;
      do{
        edge_change_id = mesh_init. HalfEdges[ edge_change_id]. oppo_;
        mesh_init. HalfEdges[ edge_change_id]. vertex = vertex_ur_id;
        edge_change_id = mesh_init. HalfEdges[ edge_change_id]. next;
      }while ( edge_change_id == edge_end_id)
        }


}
void simplify_mesh::make_priority(){
  size_t num = mesh_init.HalfEdges.size();
  priority = vector<ident>(num);
  for (size_t i = 0; i < num; i++){
    priority[i].id = i;
    priority[i].ident = mesh_init.HalfEdges[i].length;
  }
  make_heap(priority.begin(),priority.end(),operator <());
  sort_heap(priority.begin(),priority.end());


  if(priority[1].id != mesh_init.HalfEdges[edge_id].oppo_)
    cout<<"error in sort";
  
}
