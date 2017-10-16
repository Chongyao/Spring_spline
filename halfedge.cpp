#include "halfedge.h"
#include<fstream>
#include<sstream>
#include<cmath>
#include <iostream>
#include<memory>
#include<map>
using namespace std;

bool operator<(const H_edge &a, const H_edge &b)
{
  return a.length<b.length;
}

// bool operator<(const vertex_pair &a, const vertex_pair &b){
//   if(a.first == b.first)
//     return a.q < b.q;
//   else
//     return a.first < b.first;
  
// }
template<typename T>
void plus_vector(vector<T> &a, vector<T> &b, vector<T> &result){
  size_t num = a.size();
  for(size_t i = 0;i < num; i++){
    result[i] = a[i] + b[i];
  }
}
halfedge::halfedge()
{
}

int halfedge::ReadData(const string &InputFile){

  filename = InputFile;
  ifstream fin(filename);
  if (fin.is_open()==false)
   return -1;

  string keyword;
  fin>>keyword;

  while(!fin.eof()){  //read the data line by line
    if(keyword=="#")
      ReadAnno(fin,keyword);

    else if(keyword=="v")
      ReadVertex(fin,keyword);

    else if(keyword=="f")
      ReadFace(fin,keyword);

    else if(keyword=="vn")
      ReadAnno(fin,keyword);
    
    else if(keyword == "s")
      ReadAnno(fin,keyword);
    
    else
      cout<<"this identity is not exit.\n";

  }


  cout<<"data is read.\n";
  fin.close();
  return 0;
}


void halfedge::ReadVertex( ifstream &fin,string &keyword){
  H_vertex vertex_temp;

  fin>>vertex_temp.x;
  fin>>vertex_temp.y;
  fin>>vertex_temp.z;
  this->Vertexs.push_back(vertex_temp);

  fin>>keyword;
}

void halfedge::ReadFace(ifstream &fin,string &keyword){
  fin>>keyword;
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
      InitFaces.push_back(vert_id);
    }
  }
  else{
    int vert_id; { // atof(keyword)
      stringstream temp;
      temp << keyword;
      temp >> vert_id;
      InitFaces.push_back(vert_id);
      fin >> vert_id;
      InitFaces.push_back(vert_id);
      fin >> vert_id;
      InitFaces.push_back(vert_id);
    }
      fin >> keyword;


  }
}
void halfedge::ReadAnno(ifstream &fin, string &keyword){
  string temp;
  getline(fin,temp);
  fin>>keyword;

}
void halfedge::ConstructHalfedge(){
  size_t num=InitFaces.size();

  map<vertex_pair,size_t> pairs;
  Faces=vector<H_face>(num/3);

  HalfEdges=vector<H_edge> (num);

  for(size_t i=0;i<num/3;i++){
    
    Faces[i].edge_=i*3;
    for(size_t k=i*3;k<(i+1)*3;k++){
      HalfEdges[k].vertex_=InitFaces[k];

      vertex_pair v_pair_temp;
      v_pair_temp.second = InitFaces[k];
      
      Vertexs[InitFaces[k]-1].edge_=k;

      HalfEdges[k].face_=i;

      if(k==i*3){
        HalfEdges[k].prev_=k+2;
        HalfEdges[k].length= sqrt(pow((Vertexs[InitFaces[k+2]-1].x-Vertexs[InitFaces[k]-1].x),2)+
                                  pow((Vertexs[InitFaces[k+2]-1].y-Vertexs[InitFaces[k]-1].y),2)+
                                  pow((Vertexs[InitFaces[k+2]-1].z-Vertexs[InitFaces[k]-1].z),2));
        v_pair_temp.first = InitFaces[k+2];
        
      }
      else{
        HalfEdges[k].prev_=k-1;
        HalfEdges[k].length=sqrt(pow((Vertexs[InitFaces[k-1]-1].x-Vertexs[InitFaces[k]-1].x),2)+
                                 pow((Vertexs[InitFaces[k-1]-1].y-Vertexs[InitFaces[k]-1].y),2)+
                                 pow((Vertexs[InitFaces[k-1]-1].z-Vertexs[InitFaces[k]-1].z),2));
        v_pair_temp.first = InitFaces[k-1];
      }
      if(k==i*3+2)
        HalfEdges[k].next_= k-2;
      else
        HalfEdges[k].next_= k+1;


      pairs[v_pair_temp] = k;

    }

    cal_Kp_face(Faces[i]);
  }



  


  //find the opposite by map
  
  for(auto it = pairs.begin();it != pairs.end(); it++){
    auto it_oppo = pairs.find({it->first.second, it->first.first});
    if(HalfEdges[it->second].oppo_ == -1 && it_oppo != pairs.end()){
      HalfEdges[it->second].oppo_ = it_oppo->second;
      HalfEdges[it_oppo->second].oppo_ = it->second;
    
    }
  } 



  cout<<"the data has been converted to halfedge constructure.\n";
}

size_t halfedge::Get_edge_next(size_t edge_id)
{
  return this->HalfEdges[edge_id].next_;
}




void halfedge::halfedge_to_obj( const string &outfile){

  

  ofstream fout(outfile);
  map<size_t,size_t> turn;
  size_t num_vertex = Vertexs. size();{
    size_t count=1;
    for (size_t i = 0;i < num_vertex; i++){
      if (Vertexs[i]. is_exist){
        fout<<"v "<<Vertexs[i]. x<<" "<<Vertexs[i]. y<<" "<<Vertexs[i]. z<<"\n";
        turn[i+1] = count;
        ++count;
      }
      // else {
      //   cout << "Vertex " << i+1 << " is deleted\n";
      // }
    }
  }
  size_t num_faces = Faces. size();{
    for(size_t i = 0; i < num_faces; ++i){
      if(Faces[i]. is_exist){
        const size_t edge_id = Faces[i]. edge_;
        size_t edge_c = edge_id;
        fout<<"f ";
        do{
          size_t vert = HalfEdges [edge_c]. vertex_;
          fout << turn [vert] << " ";
          edge_c = HalfEdges [edge_c]. next_;
        }while(edge_id != edge_c);
        fout<<"\n";
      }
      // else{
      //   cout << "Face " << i << " is deleted.\n";
      // }
    }
  }
  fout.close();
  cout<<"\nend of writing";  
}

void halfedge::cal_Kp_face(H_face &face_){
  face_.Kp = vector<double>(8);
  vector<double> x(3),y(3),z(3);{//store the three vertexs' coordinate
    size_t edge_id = face_.edge_,
        vertex_id = HalfEdges[edge_id].vertex_;
    for(size_t i = 0; i < 3; i++){
      x[i] = Vertexs[vertex_id].x;
      y[i] = Vertexs[vertex_id].y;
      z[i] = Vertexs[vertex_id].z;
    
      edge_id = HalfEdges[edge_id]. next_;
      vertex_id = HalfEdges[edge_id]. vertex_;
    }
  }
  double a,b,c,d;{ // calculate a,b,c,d
    a = (y[3]*(z[1] - z[2]) + y[1]*(z[2] - z[3]) + y[2]*(-z[1] + z[3]))/
        (x[3]*y[2]*z[1] - x[2]*y[3]*z[1] - x[3]*y[1]*z[2] + x[1]*y[3]*z[2] + x[2]*y[1]*z[3] - x[1]*y[2]*z[3]),
        b = (x[2]*z[1] - x[3]*z[1] - x[1]*z[2] + x[3]*z[2] + x[1]*z[3] - x[2]*z[3])/
        (x[3]*y[2]*z[1] - x[2]*y[3]*z[1] - x[3]*y[1]*z[2] + x[1]*y[3]*z[2] + x[2]*y[1]*z[3] - x[1]*y[2]*z[3]),
        c = (x[3]*(-y[1] + y[2]) + x[2]*(y[1] - y[3]) + x[1]*(-y[2] + y[3]))/
        (-(x[3]*y[2]*z[1]) + x[2]*y[3]*z[1] + x[3]*y[1]*z[2] - x[1]*y[3]*z[2] - x[2]*y[1]*z[3] + x[1]*y[2]*z[3]);
    d = sqrt(a*a + b*b + c*c);
    a = a/d;
    b = b/d;
    c = c/d;
    d = 1/d;
  }
  face_. Kp[0] = a*a;
  face_. Kp[1] = a*b;
  face_. Kp[2] = a*c;
  face_. Kp[3] = a*d;
  face_. Kp[4] = b*b;
  face_. Kp[5] = b*c;
  face_. Kp[6] = b*d;
  face_. Kp[7] = c*c;
  face_. Kp[8] = c*d;
}



void halfedge::cal_Kp_vertex(H_vertex &vertex_){
  size_t edge_id = vertex_. edge_,
      edge_end_id = vertex_.edge_,
      face_id = HalfEdges[edge_id]. face_;
  vertex_. Kp = vector<double> (8);
  do{
    plus_vector(vertex_.Kp, Faces[face_id].Kp, vertex_.Kp);
    edge_id = HalfEdges[edge_id]. next_;
    edge_id = HalfEdges[edge_id]. oppo_;
    face_id = HalfEdges[edge_id]. face_;
  }while(edge_id != edge_end_id);

  
}



