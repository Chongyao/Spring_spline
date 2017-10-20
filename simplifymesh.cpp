#include<cmath>
#include "simplifymesh.h"
#include<algorithm>
#include<iostream>
#include"requation.h"
#define INF 99999999；

using namespace std;

// bool operator>(const ident &a, const ident &b){
//   return a. value > b. value;

// }
bool operator <(const ident &a,const ident &b){
  if (a.value != b.value)
    return a. value < b. value;
  else
    return a. id < b. id;
}



void simplify_mesh::Simp_shorstest(const size_t &iter_times){
  //find the shortest edge
  make_priority();
  cout <<"the size is "<< priority.size() << "\n";
  
  for(size_t i = 0; i < iter_times; i++){
    cout << i << " iteration:\n";
    
    size_t edge_id ;
    vector<double> new_V;{
      auto iter = priority.upper_bound(zero_);
      edge_id = iter->first.id;
      new_V = iter->second;
    }
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
    change_topology(new_V,edge_id,edge_oppo_id,result);
  
    
    cout<<"\n\n";
  
  }
}

void simplify_mesh::change_topology( const vector<double> &new_V, const size_t &edge_id, const int &edge_oppo_id, const int &result){
  size_t  edge_r_id = edge_id;{//delete the edges

    do{
      mesh_init.HalfEdges[edge_r_id].is_exist = false;
      size_t vertex_id = mesh_init.HalfEdges[edge_r_id].vertex_;
      if (mesh_init.Vertexs[vertex_id].edge_ == edge_r_id ){
        size_t edge_c_id;
        edge_c_id = mesh_init.HalfEdges[edge_r_id].oppo_;
        edge_c_id = mesh_init.HalfEdges[edge_c_id].prev_;
        edge_c_id = mesh_init.HalfEdges[edge_c_id].oppo_;
        edge_c_id = mesh_init.HalfEdges[edge_c_id].prev_;
        mesh_init.Vertexs[vertex_id].edge_ = edge_c_id;
      }
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
    mesh_init.Vertexs [ vertex_r_id ].is_exist = false;

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

 
    const size_t vertex_ur_id = mesh_init.HalfEdges[ edge_id]. vertex_; { //change the vertex
    mesh_init.Vertexs[vertex_ur_id].x = new_V[0];
    mesh_init.Vertexs[vertex_ur_id].y = new_V[1];
    mesh_init.Vertexs[vertex_ur_id].z = new_V[2];
    
    size_t edge_change_id, edge_end_id;
    if (result == -2){
       edge_change_id = edge_oppo_id;
       edge_end_id = mesh_init. HalfEdges [edge_id]. prev_;
    }
    else {
      edge_change_id = result;
      edge_end_id = -1;
    }
    do{

      mesh_init. HalfEdges[ edge_change_id]. vertex_ = vertex_ur_id;
      edge_change_id = mesh_init. HalfEdges [edge_change_id]. next_;


      edge_change_id = mesh_init. HalfEdges [edge_change_id]. oppo_;

      // modify_priority(edgeo_change_id,length);
      // mesh_init. HalfEdges [edge_change_id]. length = length;
    }while ( edge_change_id != edge_end_id);
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

  cout << "the opposite edges have been changed.\n";
  }


  //change Kp of the face
  {
    size_t edge_c_id  = mesh_init.Vertexs[vertex_ur_id].edge_,
        edge_end_id = edge_c_id;
    do{
      size_t face_id = mesh_init.HalfEdges[edge_c_id].face_;
      mesh_init.cal_Kp_face(mesh_init.Faces[face_id]);
      edge_c_id = mesh_init.HalfEdges[edge_c_id].next_;
      edge_c_id = mesh_init.HalfEdges[edge_c_id].oppo_;
    }while(edge_c_id != edge_end_id);    
  }

  //change kp of vertexs
  {
    mesh_init.cal_Kp_vertex(mesh_init.Vertexs[vertex_ur_id]);
    size_t edge_c_id = mesh_init.Vertexs[vertex_ur_id].edge_;
    edge_c_id = mesh_init.HalfEdges[edge_c_id].next_;
    size_t edge_end_id = edge_c_id;
    do{
      size_t vertex_c_id = mesh_init.HalfEdges[edge_c_id].vertex_;
      mesh_init.cal_Kp_vertex(mesh_init.Vertexs[vertex_c_id]);
      edge_c_id = mesh_init.HalfEdges[edge_c_id]. prev_;
      edge_c_id = mesh_init.HalfEdges[edge_c_id].oppo_;
    }while(edge_c_id != edge_end_id);
    cout << "kp of vertexs have been changed.\n";
  }

  //calculate error and change the priority
  {
    size_t edge_c_id = mesh_init.Vertexs[vertex_ur_id].edge_;
    edge_c_id = mesh_init.HalfEdges[edge_c_id].oppo_;
    size_t edge_last_id = edge_c_id;
    do{
      size_t edge_end_id = edge_c_id;     
      do{
        double error = 0;
        vector<double> V(4);

        cal_error(edge_c_id,error,V);
        modify_priority(edge_c_id, error, V);
        mesh_init.HalfEdges[edge_c_id].length = error;
        
        edge_c_id = mesh_init.HalfEdges[edge_c_id].next_;

        cal_error(edge_c_id,error,V);
        modify_priority(edge_c_id, error, V);
        mesh_init.HalfEdges[edge_c_id].length = error;
        edge_c_id = mesh_init.HalfEdges[edge_c_id].oppo_;
        
      }while(edge_c_id != edge_end_id);
      edge_c_id = mesh_init.HalfEdges[edge_c_id].prev_;
      edge_c_id = mesh_init.HalfEdges[edge_c_id].oppo_;
    }while(edge_c_id != edge_last_id);
    cout << "priority has been changed.\n";
  }
  
 



  

  
  cout << "the vertex have been changed.\n";

}
void simplify_mesh::make_priority(){
  size_t num = mesh_init.HalfEdges.size();
  for (size_t i = 0; i < num; i++){
    double value;
    vector<double> V(4);
    cal_error(i,value,V);
    ident A = {i,value};
    mesh_init.HalfEdges[i]. length = value;
    priority.insert({A,V});
  {
     
    size_t num_prio = priority.size();
    cout << "edge id is " << i <<" num of priority is " << num_prio <<"\n";
    for (auto it = priority.begin();it != priority.end();it++){
      cout << "id is " << it->first.id <<" value is " << it->first.value <<"\n";
    }
    cout << "\n\n";
  }
    
  }

  
}
void simplify_mesh::pop_priority(const size_t &edge_id){//pop before the halfedges changed 
  size_t edge_oppo_id = mesh_init. HalfEdges[edge_id].oppo_;
  if (edge_oppo_id == -1){
    priority.erase({edge_id,mesh_init.HalfEdges[edge_id].length});
    priority.erase({edge_oppo_id,mesh_init.HalfEdges[edge_oppo_id].length});
  }
  else{
        priority.erase({edge_id,mesh_init.HalfEdges[edge_id].length});
  }
  
}
int simplify_mesh::check_manifold(size_t &edge_id,  int &edge_oppo_id){
  int edge_bound_id=-2;
  bool is_cllap = true;

  if(!mesh_init. HalfEdges [edge_id].is_exist) {
    is_cllap = false;
    goto pop;
  }


  
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
      is_cllap = false;
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
    auto iter = priority.upper_bound(zero_);
    edge_id = iter->first.id;
    edge_oppo_id = mesh_init. HalfEdges [edge_id]. oppo_;
  }
  return edge_bound_id;
}
void simplify_mesh::modify_priority (const size_t &edge_id, const double &value_new ,const vector<double> &V){//You must modify priority before modify the Halfedges vector
  pop_priority(edge_id);
  ident temp_ = {edge_id,mesh_init.HalfEdges[edge_id].length}; 
  priority.insert({temp_,V});
 
}
void simplify_mesh::cal_error(const size_t &edge_id, double &error, vector<double>V){
  vector<double> Q(10);{//calculate Q
    size_t vertex_id_1 = mesh_init.HalfEdges[edge_id].vertex_,
        edge_id_prev = mesh_init.HalfEdges[edge_id].prev_,
        vertex_id_2 = mesh_init.HalfEdges[edge_id_prev].vertex_;
    mesh_init.plus_vector(mesh_init. Vertexs[vertex_id_1]. Kp, mesh_init. Vertexs[vertex_id_2].Kp, Q);
    }


    //vector<double>(4);
    {// get the V
    double delt  = -(pow(Q[2],2)*Q[4]) + 2*Q[1]*Q[2]*Q[5] - Q[0]*pow(Q[5],2) - pow(Q[1],2)*Q[7] + Q[0]*Q[4]*Q[7];

   
      if( delt > 1e-18){
        V[0] = (-(Q[2]*Q[5]*Q[6]) + Q[1]*Q[6]*Q[7] + Q[3]*(pow(Q[5],2) - Q[4]*Q[7]) + Q[2]*Q[4]*Q[8] - Q[1]*Q[5]*Q[8])/delt;
        V[1] = (pow(Q[2],2)*Q[6] + Q[1]*Q[3]*Q[7] - Q[0]*Q[6]*Q[7] + Q[0]*Q[5]*Q[8] - Q[2]*(Q[3]*Q[5] + Q[1]*Q[8]))/delt;
        V[2] = (Q[2]*Q[3]*Q[4] - Q[1]*Q[3]*Q[5] - Q[1]*Q[2]*Q[6] + Q[0]*Q[5]*Q[6] + pow(Q[1],2)*Q[8] - Q[0]*Q[4]*Q[8])/delt;
        V[3] = 1;
      }
      else{
        cout << "error";//!!!!!!!!!!!!!!!!!! consider Q is alwats invetable
      }
      
    }


    //    double error;{//calcylate the error
      error = Q[0]*pow(V[0],2) + 2*Q[1]*V[0]*V[1] + Q[4]*pow(V[1],2) + 2*Q[2]*V[0]*V[2] + 2*Q[5]*V[1]*V[2] + Q[7]*pow(V[2],2) + 2*Q[3]*V[0]*V[3] + 2*Q[6]*V[1]*V[3] + 2*Q[8]*V[2]*V[3] + Q[9]*pow(V[3],2);

    
  
}
