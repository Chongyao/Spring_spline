
#include "simplifymesh.h"
#include<algorithm>
#define INF 99999999；
using namespace std;

bool operator <(const ident &a,const ident &b){
  return a. value < b. value;
}

bool operator>(const ident &a, const ident &b){
  return a. value > b. value;
}


void simplify_mesh::

Simp_shorstest(){
  //find the shortest edge
  make_priority();
  const size_t edge_id = priority[ 0 ]. id;
  const int   edge_oppo_id = mesh_init. HalfEdges[ edge_id ]. oppo_;
  
  //check the manifold
  int result;
  do{
  result = check_manifold(edge_id, edge_oppo_id);
  }while(result == -1);
    
    
    
  //change the topology


  
   
  change_topology(edge_id,edge_oppo_id,result);
  

  //pop the priority
  pop_priority();
}

void simplify_mesh::change_topology( const size_t &edge_id, const int &edge_oppo_id, const int &result){
  size_t  edge_r_id = edge_id;{//delete the edges
    do{
      mesh_init.HalfEdges[edge_r_id].is_exist = false;
      edge_r_id = mesh_init.HalfEdges[edge_id].next_;
    }while(edge_id == edge_r_id);
 
    edge_r_id = mesh_init.HalfEdges[edge_id].oppo_;
    do{
      mesh_init.HalfEdges[edge_r_id].is_exist = false;
      edge_r_id = mesh_init.HalfEdges[edge_id].next_;
    }while(edge_id == edge_r_id);
    mesh_init.HalfEdges[edge_oppo_id].is_exist=false;
  }
  size_t vertex_r_id = mesh_init.HalfEdges [ edge_oppo_id ].vertex_;{    //delete the vertex
    mesh_init.Vertexs [ vertex_r_id ].is_exist = false;
  }

  size_t face_r_id = mesh_init.HalfEdges[ edge_id ].face_;{    //delete the face
    mesh_init. Faces[ face_r_id ]. is_exist = false;
    if( edge_oppo_id != -1) {
    face_r_id = mesh_init. HalfEdges[ edge_oppo_id ]. face_;
    mesh_init. Faces[ face_r_id ]. is_exist = false;
    }
  }


  { //change the vertex
    const size_t vertex_ur_id = mesh_init.HalfEdges[ edge_id]. vertex_;
    size_t edge_change_id_1, edge_change_id_2, edge_end_id;
    if (result == -2){
       edge_change_id_1 = edge_oppo_id;
       edge_end_id = mesh_init. HalfEdges [edge_id]. prev_;
    }
    else {
      edge_change_id_1 = result;
      edge_end_id = -1;
    }
    do{

      mesh_init. HalfEdges[ edge_change_id_1]. vertex_ = vertex_ur_id;

      double length;{ //change the length
        size_t edge_pre_id = mesh_init. HalfEdges [edge_change_id_1]. prev_; 
        size_t vertex_pre_id = mesh_init. HalfEdges [edge_pre_id]. vertex_;
        length =  sqrt(
            pow((mesh_init.Vertexs[vertex_ur_id].x-mesh_init.Vertexs[vertex_pre_id].x),2)+
            pow((mesh_init.Vertexs[vertex_ur_id].y-mesh_init.Vertexs[vertex_pre_id].y),2)+
            pow((mesh_init.Vertexs[vertex_ur_id].z-mesh_init.Vertexs[vertex_pre_id].z),2));
        mesh_init. HalfEdges [edge_change_id_1]. length = length;
        priority [edge_change_id_1]. value = length;
        mesh_init. HalfEdges [edge_change_id_2]. length = length;
        priority [edge_change_id_2]. value = length;
      } 

      edge_change_id_2 = mesh_init. HalfEdges [edge_change_id_1]. next_;
      edge_change_id_1 = mesh_init. HalfEdges [edge_change_id_2]. oppo_;
    }while ( edge_change_id != edge_end_id);
  }


}
void simplify_mesh::make_priority(){
  size_t num = mesh_init.HalfEdges.size();
  priority = vector<ident>(num);
  for (size_t i = 0; i < num; i++){
    priority[i].id = i;
    priority[i].value = mesh_init.HalfEdges[i].length;
  }
  make_heap(priority.begin(), priority.end(), greater<ident>());
  sort_heap(priority.begin(),priority.end(), greater<ident>());


  
}
void simplify_mesh::pop_priority(){
  pop_heap( priority.begin(), priority.end(), greater<ident>()); 
  pop_heap( priority.begin(), priority.end(), greater<ident>());
  
}
int simplify_mesh::check_manifold(const size_t &edge_id, const int &edge_oppo_id){
  int edge_bound_id=-2;
  bool is_cllap = true;

  
  if ( edge_oppo_id != -1 ){

    bool is_bound_p = false;{ //check if p is on the boundry
      int edge_r = edge_id; 
      do{
        edge_r = mesh_init. HalfEdges [edge_r]. next_;
        edge_r = mesh_init. HalfEdges [edge_r]. oppo_;

        if (edge_r == -1) 
          is_bound_p = true;
      }while(edge_r != edge_id);
    }
    
    bool is_bound_q = false;{ // check if q is on the boundry
      
      //      int edge_r_1 = edge_oppo_id,edge_r_2;
      do{
        edge_r_2 = mesh_init. HalfEdges [edge_r_1]. prev_;
        edge_r_1 = mesh_init. HalfEdges [edge_r_2]. oppo_;
        if (edge_r_1 == -1) {
          edge_bound_id = edge_r_2;
          is_bound_q = true;
        }
      }while(edge_r_1 != edge_id);
    }
    
    if (is_bound_q && is_bound_p){
      is_cllap == false;
      goto pop;
  }
  
   // check if this edge will
    int edge_r = mesh_init. HalfEdges [edge_id]. next_;
    edge_r = mesh_init. HalfEdges [edge_r]. oppo_;
    edge_r = mesh_init. HalfEdges [edge_r]. prev_;
    edge_r = mesh_init. HalfEdges [edge_r]. oppo_;
    edge_r = mesh_init. HalfEdges [edge_r]. prev_;
    edge_r = mesh_init. HalfEdges [edge_r]. oppo_;
    edge_r = mesh_init. HalfEdges [edge_r]. next_;
    if (edge_r == edge_id) {
      is_cllap = false;
      goto pop;
    }
    
    if (edge_oppo_id != -1){
      edge_r = mesh_init. HalfEdges [edge_oppo_id]. next_;
      edge_r = mesh_init. HalfEdges [edge_r]. oppo_;
      edge_r = mesh_init. HalfEdges [edge_r]. prev_;
      edge_r = mesh_init. HalfEdges [edge_r]. oppo_;
      edge_r = mesh_init. HalfEdges [edge_r]. prev_;
      edge_r = mesh_init. HalfEdges [edge_r]. oppo_;
      edge_r = mesh_init. HalfEdges [edge_r]. next_;
      if (edge_r == edge_id) {
        is_cllap = false;
        goto pop;
      }
   
    }  
  
  
pop:
  if (is_cllap == false) {
    pop_priority();
    edge_bound_id = -1;
  }
  return edge_bound_id;
}
