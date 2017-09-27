#ifndef HALFEDGE_H
#define HALFEDGE_H
#include<memory>
#include<vector>
#include<string>

using namespace std;
struct H_vertex ;
struct H_face ;
struct H_edge;
struct H_vertex{
  double x;
  double y;
  double z;
  size_t edge_;
//  shared_ptr<H_edge> edge;
};
struct H_face{
//  shared_ptr<H_edge> edge;
    size_t edge_;
};
struct H_edge{ // read_only to others
//    H_edge(const halfedge &HE)
//        :HE_(HE) {}

//  shared_ptr<H_vertex> vertex;
//  shared_ptr<H_face> face;
//  shared_ptr<H_edge> next;
//  shared_ptr<H_edge> prev;
//  shared_ptr<H_edge> opposite;
  size_t next_, prev_, oppo_,face_,vertex_;
//  const halfedge &HE_;
  double length;
};

class halfedge
{
public:
    halfedge();
    int ReadData(const string &InputFile);
    void ConstructHalfedge();

    vector<H_edge> HalfEdges;
//    vector<H_edge>
    vector<H_face> Faces;
    vector<H_vertex> Vertexs;
    vector<int> InitFaces;

private:
    void ReadVertex(ifstream &fin, string &keyword);
    void ReadFace(ifstream &fin, string &keyword);
    void ReadAnno(ifstream &fin, string &keyword);




};
#endif // HALFEDGE_H
