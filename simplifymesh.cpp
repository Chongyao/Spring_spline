
#include"requation.h"
#include<cmath>
#include"simplifymesh.h"
#include<sstream>
#define INF 99999999；
#include"zjucad/matrix/include/io.h"
using namespace std;
using namespace zjucad::matrix;

// bool operator>(const ident &a, const ident &b){
//   return a. value > b. value;

// }
bool operator <(const ident &a,const ident &b){
  if (a.value != b.value)
    return a. value < b. value;
  else
    return a. id < b. id;
}



void simplify_mesh::simp_shorstest(const size_t &iter_times, const string &outfile){
  //find the shortest edge
  make_priority();
  cout <<"the size is "<< priority.size() << "\n";

   
  for(size_t i = 0; i < iter_times; i++){
    cout << i << " iteration:\n";
   //     cout << "priority is: " << endl; 
   // for (auto iter = priority.begin();iter != priority.end(); iter++){
   //     cout << "edge_id is " << iter->first.id << "value is " << iter->first.value << endl;
   //     cout << iter->second << endl;
   // }
         
    size_t edge_id ;
    matrix<double> new_V;{
      auto iter = priority.upper_bound(zero_);
      edge_id = iter->first.id;
      new_V = iter->second;
    }
    int   edge_oppo_id = mesh_init.half_edges_[ edge_id ]. oppo_;
    

    //check the manifold
    int result;
    size_t count = 0;
    do{
      result = check_manifold(edge_id, edge_oppo_id,new_V);
      cout << "result of check of manifold is "<< result <<"\n";
      count ++;
      if (count > 500) 
        goto terminate;
    }while(result == -1);

    if(priority.size() == 0){
      count = 501;
      goto terminate;
    }
    terminate:
    if(count > 500) break;
    
    //pop the priority
    pop_priority(edge_id);
     
    //change the topology
    cout << "the edge to be collapsed is " << edge_id <<"\n";
    change_topology(new_V,edge_id,edge_oppo_id,result);
  
   
    cout<<"\n\n";
    if( i<30000 &&(i+1)%1000 == 0) {
      string out_ ,temp;
      stringstream _str_;
      _str_ << i+1;
      _str_ >> temp;
      out_.append(outfile);
      out_.append(temp);
      out_.append(".obj");
      mesh_init.halfedge_to_obj(out_);
    }
    if( i>30000 && (i+1)%200 == 0){
      string out_ ,temp;
      stringstream _str_;
      _str_ << i+1;
      _str_ >> temp;
      out_.append(outfile);
      out_.append(temp);
      out_.append(".obj");
      mesh_init.halfedge_to_obj(out_);
    }
  }
}

void simplify_mesh::change_topology( const matrix<double> &new_V, const size_t &edge_id, const int &edge_oppo_id, const int &result){
  delete_half_edges(edge_id,edge_oppo_id);
  delete_vertex(edge_oppo_id);
  delete_faces(edge_id,edge_oppo_id);
  size_t vertex_ur_id = change_vertex(edge_id, edge_oppo_id, new_V, result);
  change_oppoedge(edge_id,edge_oppo_id);
  change_face_kp (vertex_ur_id);
  change_vertex_kp(vertex_ur_id);
  change_priority(vertex_ur_id);
}

void simplify_mesh::delete_half_edges(const size_t &edge_id,const size_t &edge_oppo_id){
size_t  edge_r_id = edge_id;

    do{
      mesh_init.half_edges_[edge_r_id].is_exist = false;
      size_t vertex_id = mesh_init.half_edges_[edge_r_id].vertex_;
      
      {
        if (mesh_init.vertexs_[vertex_id].edge_ == edge_r_id ){
          size_t edge_c_id = edge_r_id;
          do{
            edge_c_id = mesh_init.half_edges_[edge_c_id].oppo_;
            edge_c_id = mesh_init.half_edges_[edge_c_id].prev_;
            if (mesh_init.half_edges_[edge_c_id].is_exist)
              mesh_init.vertexs_[vertex_id].edge_ = edge_c_id;
          }while(!mesh_init.half_edges_[edge_c_id].is_exist);
        }
      }
      edge_r_id = mesh_init.half_edges_[edge_r_id].next_;
    }while(edge_id != edge_r_id);

    edge_r_id = edge_oppo_id;
    do{
      mesh_init.half_edges_[edge_r_id].is_exist = false;
      size_t vertex_id = mesh_init.half_edges_[edge_r_id].vertex_;
      if (mesh_init.vertexs_[vertex_id].edge_ == edge_r_id ){
        size_t edge_c_id = edge_r_id;
        do{
          edge_c_id = mesh_init.half_edges_[edge_c_id].oppo_;
          edge_c_id = mesh_init.half_edges_[edge_c_id].prev_;
          if (mesh_init.half_edges_[edge_c_id].is_exist)
            mesh_init.vertexs_[vertex_id].edge_ = edge_c_id;
        }while(!mesh_init.half_edges_[edge_c_id].is_exist);
      }
      edge_r_id = mesh_init.half_edges_[edge_r_id].next_;
    }while(edge_oppo_id != edge_r_id);
    cout <<"the edges have been deleted.\n";
}

  
      
   
void simplify_mesh::delete_vertex(const size_t &edge_oppo_id){
  size_t vertex_r_id = mesh_init.half_edges_ [ edge_oppo_id ].vertex_;
    mesh_init.vertexs_ [ vertex_r_id ].is_exist = false;
    cout<<"the vertex " << vertex_r_id <<" has been deleted.\n";
  
}
void simplify_mesh::delete_faces(const size_t &edge_id,const size_t &edge_oppo_id){

  size_t face_r_id = mesh_init.half_edges_[ edge_id ].face_;
    mesh_init.faces_[ face_r_id ]. is_exist = false;
    if( edge_oppo_id != -1) {
    face_r_id = mesh_init.half_edges_[ edge_oppo_id ]. face_;
    mesh_init.faces_[ face_r_id ]. is_exist = false;
    }
    cout<<"the face has been deleted.\n";
}


size_t simplify_mesh::change_vertex(const size_t &edge_id, const size_t &edge_oppo_id,const matrix<double> &new_V,const int &result){
   const size_t vertex_ur_id = mesh_init.half_edges_[edge_id]. vertex_;
   mesh_init.vertexs_[vertex_ur_id].position = new_V;
    size_t edge_change_id, edge_end_id;
    if (result == -2){
       edge_change_id = edge_oppo_id;
       edge_end_id = mesh_init.half_edges_ [edge_id]. prev_;
    }
    else {
      edge_change_id = result;
      edge_end_id = -1;
    }
    do{
      mesh_init.half_edges_[ edge_change_id]. vertex_ = vertex_ur_id;
      edge_change_id = mesh_init.half_edges_ [edge_change_id]. next_;
      
      edge_change_id = mesh_init.half_edges_ [edge_change_id]. oppo_;
    }while ( edge_change_id != edge_end_id);
    cout << "vertex has been changed.\n";
    return vertex_ur_id;
}
void simplify_mesh::change_oppoedge(const size_t &edge_id, const size_t &edge_oppo_id) {  
  size_t edge_oppo_ur = mesh_init.half_edges_ [edge_id]. next_;
  edge_oppo_ur = mesh_init.half_edges_ [edge_oppo_ur]. oppo_;
   
  size_t edge_change_id = mesh_init.half_edges_ [edge_id]. prev_;
  edge_change_id = mesh_init.half_edges_ [edge_change_id]. oppo_;
  mesh_init.half_edges_ [edge_change_id]. oppo_ = edge_oppo_ur;
  mesh_init.half_edges_ [edge_oppo_ur]. oppo_ = edge_change_id;
  
  edge_oppo_ur = mesh_init.half_edges_ [edge_oppo_id]. prev_;
  edge_oppo_ur = mesh_init.half_edges_ [edge_oppo_ur]. oppo_;

  edge_change_id = mesh_init.half_edges_ [edge_oppo_id]. next_;
  edge_change_id = mesh_init.half_edges_ [edge_change_id]. oppo_;
  mesh_init.half_edges_ [edge_change_id]. oppo_ = edge_oppo_ur;
  mesh_init.half_edges_ [edge_oppo_ur]. oppo_ = edge_change_id;

  cout << "the opposite edges have been changed.\n";
}

void simplify_mesh::change_face_kp(const size_t &vertex_ur_id){
  size_t edge_c_id  = mesh_init.vertexs_[vertex_ur_id].edge_,
      edge_end_id = edge_c_id;
  do{
    size_t face_id = mesh_init.half_edges_[edge_c_id].face_;
    mesh_init.cal_Kp_face(mesh_init.faces_[face_id]);
    edge_c_id = mesh_init.half_edges_[edge_c_id].next_;
    edge_c_id = mesh_init.half_edges_[edge_c_id].oppo_;
  }while(edge_c_id != edge_end_id);

  cout << "faces kp have been changed.\n";
}

void simplify_mesh::change_vertex_kp(const size_t &vertex_ur_id)  {
  mesh_init.cal_Kp_vertex(mesh_init.vertexs_[vertex_ur_id]);
  size_t edge_c_id = mesh_init.vertexs_[vertex_ur_id].edge_;
  edge_c_id = mesh_init.half_edges_[edge_c_id].next_;
  size_t edge_end_id = edge_c_id;
  do{
    size_t vertex_c_id = mesh_init.half_edges_[edge_c_id].vertex_;
    mesh_init.cal_Kp_vertex(mesh_init.vertexs_[vertex_c_id]);
    edge_c_id = mesh_init.half_edges_[edge_c_id]. prev_;
    edge_c_id = mesh_init.half_edges_[edge_c_id].oppo_;
  }while(edge_c_id != edge_end_id);
  cout << "kp of vertexs have been changed.\n";
}


void simplify_mesh::change_priority(const size_t &vertex_ur_id){
  size_t edge_c_id = mesh_init.vertexs_[vertex_ur_id].edge_;
  edge_c_id = mesh_init.half_edges_[edge_c_id].oppo_;
  size_t edge_last_id = edge_c_id;
  do{
    size_t edge_end_id = edge_c_id;     
    do{
      double error = 0;
      matrix<double> V(4,1);

      cal_error(edge_c_id,error,V);
      modify_priority(edge_c_id, error, V);
      mesh_init.half_edges_[edge_c_id].length = error;
        
      edge_c_id = mesh_init.half_edges_[edge_c_id].next_;

      cal_error(edge_c_id,error,V);
      modify_priority(edge_c_id, error, V);
      mesh_init.half_edges_[edge_c_id].length = error;
      edge_c_id = mesh_init.half_edges_[edge_c_id].oppo_;
        
    }while(edge_c_id != edge_end_id);
    edge_c_id = mesh_init.half_edges_[edge_c_id].prev_;
    edge_c_id = mesh_init.half_edges_[edge_c_id].oppo_;
  }while(edge_c_id != edge_last_id);
  cout << "priority has been changed.\n";
}



void simplify_mesh::make_priority(){
  size_t num = mesh_init.half_edges_.size();
  for (size_t i = 0; i < num; i++){
    double value;
    matrix<double> V(4,1);
    cal_error(i,value,V);
    ident A = {i,value};
    mesh_init.half_edges_[i]. length = value;
    priority.insert({A,V});
    
  }

  
}
void simplify_mesh::pop_priority(const size_t &edge_id){//pop before the half_edges_ changed
  size_t edge_oppo_id = mesh_init.half_edges_[edge_id].oppo_;


  size_t count = 0;
  
  if (edge_oppo_id != -1){
count = priority.erase({edge_id,mesh_init.half_edges_[edge_id].length});
count = priority.erase({edge_oppo_id,mesh_init.half_edges_[edge_oppo_id].length});
  }
  else{
      count = priority.erase({edge_id,mesh_init.half_edges_[edge_id].length});
  }


  if (count == 0)
    cout <<"the element is not exist!\n";
  
}
int simplify_mesh::check_manifold(size_t &edge_id,  int &edge_oppo_id, matrix<double>&new_V){
  int edge_bound_id=-2;
  bool is_cllap = true;

  if(!mesh_init.half_edges_ [edge_id].is_exist) {
    is_cllap = false;
    goto pop;
  }


  
  if ( edge_oppo_id != -1 ){
    bool is_bound_p = false;{ //check if p is on the boundry
      int edge_r = edge_id; 
      do{
        edge_r = mesh_init.half_edges_ [edge_r]. next_;
        edge_r = mesh_init.half_edges_ [edge_r]. oppo_;

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
        edge_r_2 = mesh_init.half_edges_ [edge_r_1]. prev_;
        edge_r_1 = mesh_init.half_edges_ [edge_r_2]. oppo_;
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
    int edge_r = mesh_init.half_edges_ [edge_id]. next_;
    edge_r = mesh_init.half_edges_ [edge_r]. oppo_;
    edge_r = mesh_init.half_edges_ [edge_r]. prev_;
    edge_r = mesh_init.half_edges_ [edge_r]. oppo_;
    edge_r = mesh_init.half_edges_ [edge_r]. prev_;
    edge_r = mesh_init.half_edges_ [edge_r]. oppo_;
    edge_r = mesh_init.half_edges_ [edge_r]. next_;
    if (edge_r == edge_id) {
      is_cllap = false;
      goto pop;
    }
    
    if (edge_oppo_id != -1){
      edge_r = mesh_init.half_edges_ [edge_oppo_id]. next_;
      edge_r = mesh_init.half_edges_ [edge_r]. oppo_;
      edge_r = mesh_init.half_edges_ [edge_r]. prev_;
      edge_r = mesh_init.half_edges_ [edge_r]. oppo_;
      edge_r = mesh_init.half_edges_ [edge_r]. prev_;
      edge_r = mesh_init.half_edges_ [edge_r]. oppo_;
      edge_r = mesh_init.half_edges_ [edge_r]. next_;
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
    ident temp_ = {edge_id,mesh_init.half_edges_[edge_id].length};
    double new_value = mesh_init.half_edges_[edge_id].length*999999999;
    matrix<double>V = priority[temp_];
    modify_priority(edge_id, new_value, V);
    mesh_init.half_edges_[edge_id].length *= 999999999;
    edge_bound_id = -1;
    auto iter = priority.upper_bound(zero_);
    edge_id = iter->first.id;
    new_V = iter->second;
    edge_oppo_id = mesh_init.half_edges_ [edge_id]. oppo_;
  }
  return edge_bound_id;
}
void simplify_mesh::modify_priority (const size_t &edge_id, const double &value_new ,const matrix<double> &V){//You must modify priority before modify the half_edges vector
  //pop_priority(edge_id);
  size_t is_erase =  priority.erase({edge_id,mesh_init.half_edges_[edge_id].length});
  if (is_erase == 0) cout <<"erase fail!\n";
  ident temp_ = {edge_id,value_new}; 
  priority.insert({temp_,V});
 
}
void simplify_mesh::cal_error(const size_t &edge_id, double &error, matrix<double>&V){
  matrix<double> Q;{//calculate Q
    size_t vertex_id_1 = mesh_init.half_edges_[edge_id].vertex_,
        edge_id_prev = mesh_init.half_edges_[edge_id].prev_,
        vertex_id_2 = mesh_init.half_edges_[edge_id_prev].vertex_;
    
    Q = mesh_init.vertexs_[vertex_id_1].Kp + mesh_init.vertexs_[vertex_id_2].Kp;
  }
    //calculateinverse the V
  double delt = -(pow(Q(0,2),2)*Q(1,1)) + 2*Q(0,1)*Q(0,2)*Q(1,2) - Q(0,0)*pow(Q(1,2),2) - pow(Q(0,1),2)*Q(2,2) + Q(0,0)*Q(1,1)*Q(2,2); 

      if( delt > 1e-18){
        V(0) = (-(Q(0,2)*Q(1,2)*Q(1,3)) + Q(0,1)*Q(1,3)*Q(2,2) + Q(0,3)*(pow(Q(1,2),2) - Q(1,1)*Q(2,2)) + Q(0,2)*Q(1,1)*Q(2,3) - Q(0,1)*Q(1,2)*Q(2,3))/delt;
        V(1) = (pow(Q(0,2),2)*Q(1,3) + Q(0,1)*Q(0,3)*Q(2,2) - Q(0,0)*Q(1,3)*Q(2,2) + Q(0,0)*Q(1,2)*Q(2,3) - Q(0,2)*(Q(0,3)*Q(1,2) + Q(0,1)*Q(2,3)))/delt;
        V(2) = (Q(0,2)*Q(0,3)*Q(1,1) - Q(0,1)*Q(0,3)*Q(1,2) - Q(0,1)*Q(0,2)*Q(1,3) + Q(0,0)*Q(1,2)*Q(1,3) + pow(Q(0,1),2)*Q(2,3) - Q(0,0)*Q(1,1)*Q(2,3))/delt;
        V(3) = 1;
      }
      else{
        cout << "error";//!!!!!!!!!!!!!!!!!! consider Q is alwats invetable
      }
      error = dot(trans(V),Q * V);
      //       cout <<"edge is " << edge_id <<  "error is " << error <<endl;
      //      cout << "V is " << V[0]<< " " << V[1] << " " << V[2] << endl;
      //cout << Q << endl;

      
}
  
