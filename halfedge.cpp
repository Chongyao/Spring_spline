#include "halfedge.h"
#include<fstream>
#include<sstream>
#include<cmath>
#include <iostream>
#include<map>
#include"zjucad/matrix/include/io.h"
using namespace std;
using namespace zjucad::matrix;
bool operator<(const H_edge &a, const H_edge &b)
{
  return a.length<b.length;
}


halfedge::halfedge()
{
}

int halfedge::read_data(const string &input_file){

  filename = input_file;
  ifstream fin(filename);
  if (fin.is_open()==false)
   return -1;

  string keyword;
  fin>>keyword;

  while(!fin.eof()){  //read the data line by line
    if(keyword=="#")
      read_anno(fin,keyword);

    else if(keyword=="v")
      read_vertex(fin,keyword);

    else if(keyword=="f")
      read_face(fin,keyword);

    else if(keyword=="vn")
      read_anno(fin,keyword);
    
    else if(keyword == "s")
      read_anno(fin,keyword);
    else if(keyword == "vt")
      read_anno(fin,keyword);
    else
      cout<<"this identity is not exit.\n";

  }


  cout<<"data is read.\n";
  fin.close();
  return 0;
}


void halfedge::read_vertex( ifstream &fin,string &keyword){
  H_vertex vertex_temp;
  vertex_temp.position.resize(4,1);
  fin >> vertex_temp.position(0);
  fin >> vertex_temp.position(1);
  fin >> vertex_temp.position(2);
  vertex_temp.position(3) = 1;
  vertex_temp.Kp.resize(4,4);
  vertexs_.push_back(vertex_temp);
  fin >> keyword;
  
  
}

void halfedge::read_face(ifstream &fin,string &keyword){
  fin >> keyword;
  int position = keyword.find("/",0);
  if (position != -1){
    while(keyword != "f" && !fin.eof()){
      int vert_id; { // atof(keyword)
        keyword.erase(keyword.begin()+keyword.find("/",0),keyword.end());
        stringstream temp;
        temp << keyword;
        temp >> vert_id;
        fin >> keyword;
      }
      init_faces.push_back(vert_id);
    }
  }
  else{
    int vert_id; { // atof(keyword)
      stringstream temp;
      temp << keyword;
      temp >> vert_id;
      init_faces.push_back(vert_id);
      fin >> vert_id;
      init_faces.push_back(vert_id);
      fin >> vert_id;
      init_faces.push_back(vert_id);
    }
      fin >> keyword;


  }
}
void halfedge::read_anno(ifstream &fin, string &keyword){
  string temp;
  getline(fin,temp);
  fin >> keyword;

}
void halfedge::construct_half_edges(){
  size_t num = init_faces.size();

  map<vertex_pair,size_t> pairs;
  faces_ = vector<H_face>(num/3);

  half_edges_ = vector<H_edge> (num);

  for(size_t i = 0;i < num/3;i++){
    
    faces_[i].edge_ = i*3;
    for(size_t k = i*3;k < (i+1)*3;k++){
      half_edges_[k].vertex_ = init_faces[k]-1;

      vertex_pair v_pair_temp;
      v_pair_temp.second = init_faces[k]-1;
     
     vertexs_[init_faces[k]-1].edge_ = k;

      half_edges_[k].face_ = i;

      if(k == i*3){
        half_edges_[k].prev_ = k+2;
        v_pair_temp.first = init_faces[k+2]-1;
        
      }
      else{
        half_edges_[k].prev_= k-1;
        v_pair_temp.first = init_faces[k-1]-1;
      }
      if(k == i*3+2)
        half_edges_[k].next_ = k-2;
      else
        half_edges_[k].next_ = k+1;


      pairs[v_pair_temp] = k;
      
   }

  }


  //find the opposite by map
  
  for(auto it = pairs.begin();it != pairs.end(); it++){
    auto it_oppo = pairs.find({it->first.second, it->first.first});
    if(half_edges_[it->second].oppo_ == -1 && it_oppo != pairs.end()){
      half_edges_[it->second].oppo_ = it_oppo->second;
      half_edges_[it_oppo->second].oppo_ = it->second;
    
    }
  } 

  //cal the faces kp
  size_t num_faces = faces_.size();
  for(size_t i = 0; i < num_faces; i++){
    cal_Kp_face(faces_[i]);
  }
  

  //cal the vertexs_ Kp

  
    size_t num_vertexs_ = vertexs_.size();
    for (size_t i = 0; i < num_vertexs_; i++){
      cal_Kp_vertex(vertexs_[i]);
    }
    cout << "kP of vertexs_ is calculated.\n";
  
  

  cout<<"the data has been converted to halfedge constructure.\n";
}





void halfedge::halfedge_to_obj( const string &outfile){

  

  ofstream fout(outfile);
  map<size_t,size_t> turn;
  size_t num_vertex = vertexs_. size();{
    size_t count=1;
    for (size_t i = 0;i < num_vertex; i++){
      if (vertexs_[i]. is_exist){
        fout<<"v "<<vertexs_[i].position(0)<<" "<<vertexs_[i].position(1)<<" "<<vertexs_[i].position(2)<<"\n";
        turn[i] = count;
        ++count;
      }
      // else {
      //   cout << "Vertex " << i+1 << " is deleted\n";
      // }
    }
  }
  size_t num_faces = faces_. size();{
    for(size_t i = 0; i < num_faces; ++i){
      if(faces_[i]. is_exist){
        const size_t edge_id = faces_[i]. edge_;
        size_t edge_c = edge_id;
        fout<<"f ";
        do{
          size_t vert = half_edges_ [edge_c]. vertex_;
          fout << turn [vert] << " ";
          edge_c = half_edges_ [edge_c]. next_;
        }while(edge_id != edge_c);
        fout<<"\n";
      }
      // else{
      //   cout << "Face " << i << " is deleted.\n";
      // }
    }
  }
  fout.close();
  cout << outfile <<" end of writing.\n";  
}

void halfedge::cal_Kp_face(H_face &face_){
  face_.Kp.resize(4,4);
  vector<size_t> vertex_id;{
  size_t edge_id = face_.edge_;
  vertex_id.push_back(half_edges_[edge_id].vertex_);
  edge_id = half_edges_[edge_id].next_;
  vertex_id.push_back(half_edges_[edge_id].vertex_);
  edge_id = half_edges_[edge_id].next_;
  vertex_id.push_back(half_edges_[edge_id].vertex_);
  }

   matrix<double> A(3,1),B(3,1);{
       for (size_t i = 0;i < 3;i++){
           A[i] = vertexs_[vertex_id[0]].position[i]-vertexs_[vertex_id[1]].position[i];
           B[i] = vertexs_[vertex_id[0]].position[i]-vertexs_[vertex_id[2]].position[i];
       }
   }
   matrix<double> face_normal(4,1);{
       matrix<double> face_normal_temp = cross(A,B);
       double det = norm(face_normal_temp);
       face_normal_temp /= det;
       face_normal[0] = face_normal_temp[0];
       face_normal[1] = face_normal_temp[1];
       face_normal[2] = face_normal_temp[2];
       face_normal[3] = -dot(face_normal,vertexs_[vertex_id[0]].position);
   }

   //Kp
   face_.Kp = face_normal*trans(face_normal);
}


void halfedge::cal_Kp_vertex(H_vertex &vertex_){
  size_t edge_id = vertex_. edge_,
      edge_end_id = vertex_.edge_,
      face_id = half_edges_[edge_id]. face_;
  vertex_. Kp.resize(4,4);
  do{
    vertex_.Kp += faces_[face_id].Kp;
    edge_id = half_edges_[edge_id]. next_;
    edge_id = half_edges_[edge_id]. oppo_;
    face_id = half_edges_[edge_id]. face_;
  }while(edge_id != edge_end_id);


}
