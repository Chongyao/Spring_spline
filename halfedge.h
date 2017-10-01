#ifndef HALFEDGE_H
#define HALFEDGE_H
#include<memory>
#include<vector>
#include<string>


struct H_vertex ;
struct H_face ;
struct H_edge;
struct H_vertex{
  double x;
  double y;
  double z;
  size_t edge_;
  bool is_exist;
};
struct H_face{
    size_t edge_;
    bool is_exist;
};
struct H_edge{ // read_only to others
//    H_edge(const halfedge &HE)
//        :HE_(HE) {}

  size_t next_, prev_, oppo_,face_,vertex_;
//  const halfedge &HE_;
  double length;
  bool is_exist;
};


class halfedge
{
public:
    halfedge();

    std::vector<int> InitFaces;
    int ReadData(const std::string &InputFile);

    void ConstructHalfedge();

    //core structure
    std::vector<H_edge> HalfEdges;
    std::vector<H_face> Faces;
    std::vector<H_vertex> Vertexs;


    //get information

    size_t Get_edge_next(size_t edge_id);



private:
    void ReadVertex(std::ifstream &fin, std::string &keyword);
    void ReadFace(std::ifstream &fin, std::string &keyword);
    void ReadAnno(std::ifstream &fin, std::string &keyword);




};
#endif // HALFEDGE_H
