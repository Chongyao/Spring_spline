#include "simplifymesh.h"
#include<algorithm>
#include<iostream>
#define INF 99999999；

using namespace std;

bool operator>(const ident &a, const ident &b){
  return a. value > b. value;

}
bool operator <(const ident &a,const ident &b){
    return a. value < b. value;
}

void simplify_mesh::Simp_shorstest(const size_t &iter_times){
  //find the shortest edge
  make_priority();
  //  string Output = "../data/output/cube_tu_",fix = ".obj";
  for(size_t i = 0; i < iter_times; i++){

     for (size_t j = 0;j < 10; j++){
       cout<<"edge id is " << priority[j]. id << "  length is " << priority[j]. value <<"\n";
     }
     cout<<"\n";
  
    cout << i << " iteration:\n";
    nsize_t edge_id = priority[ 0 ]. id;
    int   edge_oppo_id = mesh_init. HalfEdges[ edge_id ]. oppo_;
    

    //check the manifold
    int result;
    do{
      result = check_manifold(edge_id, edge_oppo_id);
      cout << "result of check of manifold is "<< result <<"\n";
    }while(result == -1);

    //pop the priority
    pop_priority(edge_id);
      
    //change the topology
    cout << "the edge to be collapsed is " << edge_id <<"\n";
    change_topology(edge_id,edge_oppo_id,result);
  
    // //output
    // stringstream temp;
    // string temp_;
    // temp<<i;
    // temp>>temp_;
    // Output. append(temp_);
    // Output. append(fix);
    // cout << Output <<"\n";
    // mesh_init. halfedge_to_obj(Output);
    
    cout<<"\n\n";
  
  }
}

void simplify_mesh::change_topology( const size_t &edge_id, const int &edge_oppo_id, const int &result){
  size_t  edge_r_id = edge_id;{//delete the edges

    do{
      mesh_init.HalfEdges[edge_r_id].is_exist = false;
      edge_r_id = mesh_init.HalfEdges[edge_r_id].next_;
    }while(edge_id != edge_r_id);
    edge_r_id = edge_oppo_id;
    do{
      mesh_init.HalfEdges[edge_r_id].is_exist = false;
      edge_r_id = mesh_init.HalfEdges[edge_r_id].next_;
    }while(edge_oppo_id != edge_r_id);
    cout <<"the edges have been deleted.\n";
  }
  
  size_t vertex_r_id = mesh_init.HalfEdges [ edge_oppo_id ].vertex_;{    //delete the vertex
    mesh_init.Vertexs [ vertex_r_id-1 ].is_exist = false;
    cout<<"the vertex " << vertex_r_id <<" has been deleted.\n";



  }

  size_t face_r_id = mesh_init.HalfEdges[ edge_id ].face_;{    //delete the face
    mesh_init. Faces[ face_r_id ]. is_exist = false;
    if( edge_oppo_id != -1) {
    face_r_id = mesh_init. HalfEdges[ edge_oppo_id ]. face_;
    mesh_init. Faces[ face_r_id ]. is_exist = false;
    }
    cout<<"the face has been deleted.\n";
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
      edge_change_id_2 = mesh_init. HalfEdges [edge_change_id_1]. next_;
      double length;{ //change the length
        size_t vertex_pre_id = mesh_init. HalfEdges [edge_change_id_2]. vertex_;
        length =  sqrt(
            pow((mesh_init.Vertexs[vertex_ur_id-1].x-mesh_init.Vertexs[vertex_pre_id-1].x),2)+
            pow((mesh_init.Vertexs[vertex_ur_id-1].y-mesh_init.Vertexs[vertex_pre_id-1].y),2)+
            pow((mesh_init.Vertexs[vertex_ur_id-1].z-mesh_init.Vertexs[vertex_pre_id-1].z),2));
        mesh_init. HalfEdges [edge_change_id_1]. length = length;
        priority [edge_change_id_1]. value = length;

        mesh_init. HalfEdges [edge_change_id_2]. length = length;
        priority [edge_change_id_2]. value = length;
      } 


      edge_change_id_1 = mesh_init. HalfEdges [edge_change_id_2]. oppo_;
    }while ( edge_change_id_1 != edge_end_id);
  }

  {  //change the opposite edge
  size_t edge_oppo_ur = mesh_init. HalfEdges [edge_id]. next_;
  edge_oppo_ur = mesh_init. HalfEdges [edge_oppo_ur]. oppo_;
   
  size_t edge_change_id = mesh_init. HalfEdges [edge_id]. prev_;
  edge_change_id = mesh_init. HalfEdges [edge_change_id]. oppo_;
  mesh_init. HalfEdges [edge_change_id]. oppo_ = edge_oppo_ur;
  mesh_init. HalfEdges [edge_oppo_ur]. oppo_ = edge_change_id;
  
  edge_oppo_ur = mesh_init. HalfEdges [edge_oppo_id]. prev_;
  edge_oppo_ur = mesh_init. HalfEdges [edge_oppo_ur]. oppo_;

  edge_change_id = mesh_init. HalfEdges [edge_oppo_id]. next_;
  edge_change_id = mesh_init. HalfEdges [edge_change_id]. oppo_;
  mesh_init. HalfEdges [edge_change_id]. oppo_ = edge_oppo_ur;
  mesh_init. HalfEdges [edge_oppo_ur]. oppo_ = edge_change_id;
  }

  cout << "the vertex have been changed.\n";

}
void simplify_mesh::make_priority(){
  size_t num = mesh_init.HalfEdges.size();
  priority = vector<ident>(num);
  for (size_t i = 0; i < num; i++){
    priority[i].id = i;
    priority[i].value = mesh_init.HalfEdges[i].length;
  }

  make_heap(priority.begin(),priority.end(),greater<ident>());
  
}
void simplify_mesh::pop_priority(const size_t &edge_id){

  if (mesh_init. HalfEdges[edge_id]. oppo_ !=-1){
    pop_heap (priority.begin(), priority.end(),greater<ident>());
    priority.pop_back();

    pop_heap (priority.begin(), priority.end(),greater<ident>());
    priority.pop_back();
  }
  else{
    pop_heap (priority.begin(), priority.end(),greater<ident>());
    priority.pop_back();
  }
  
}
int simplify_mesh::check_manifold(size_t &edge_id,  int &edge_oppo_id){
  int edge_bound_id=-2;
  bool is_cllap = true;

  if(!mesh_init. HalfEdges [edge_id].is_exist) {
    is_cllap = false;
    goto pop;
  }


   // if (edge_id == 1406){
   //   size_t id = edge_id;
   //   do{

   //     cout << "the edge is " << id <<" is exist "<< mesh_init. HalfEdges[id].is_exist<< " the vertex is "<< mesh_init. HalfEdges [id]. vertex_ << "\n";
   //   id = mesh_init. HalfEdges [id].next_;

   //   cout << "the edge is " << id <<"\n ";
   //   id = mesh_init. HalfEdges [id].oppo_;
   //   }while(id != edge_id);
   // }
    


  
  if ( edge_oppo_id != -1 ){
    bool is_bound_p = false;{ //check if p is on the boundry
      int edge_r = edge_id; 
      do{
        edge_r = mesh_init. HalfEdges [edge_r]. next_;
        edge_r = mesh_init. HalfEdges [edge_r]. oppo_;

        if (edge_r == -1) {
          is_bound_p = true;
          break;
        }
      }while(edge_r != edge_id);
    }
    if(! is_bound_p) cout<<"p is not on the bound\n";
    
    bool is_bound_q = false;{ // check if q is on the boundry
      int edge_r_1 = edge_id,edge_r_2;
      do{
        edge_r_2 = mesh_init. HalfEdges [edge_r_1]. prev_;
        edge_r_1 = mesh_init. HalfEdges [edge_r_2]. oppo_;
        if (edge_r_1 == -1) {
          edge_bound_id = edge_r_2;
          is_bound_q = true;
          break;
        }
      }while(edge_r_1 != edge_id);
    }

    if (!is_bound_q) cout << "q is not on the bound\n";
    if (is_bound_q && is_bound_p){
      is_cllap == false;
      cout << "the p and q is on the bound.\n";
      goto pop;
    }}
  
 {  // check if this edge is in three triangles
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
        cout << "the edge has three triangles.\n";
        goto pop;
      }
    }
  }
  
    if (is_cllap) cout<< "the edge is collapsable\n";  
pop:
  if (is_cllap == false) {
    pop_priority(edge_id);
    edge_bound_id = -1;
    edge_id = priority[0]. id;
    edge_oppo_id = mesh_init. HalfEdges [edge_id]. oppo_;
  }
  return edge_bound_id;
}
void simplify_mesh::modify_priority (const ident &new_ident){//You must modify priority before modify the Halfedges vector
  double length_old = mesh_init. HalfEdges[new_ident.id]. length;
  for ()
  



  
}
