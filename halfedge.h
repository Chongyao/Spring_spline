#ifndef HALFEDGE_H
#define HALFEDGE_H
#include<memory>
#include<vector>
#include<string>


struct H_vertex ;
struct H_face ;
struct H_edge;
struct H_vertex{
  H_vertex():
      is_exist(true){};
  double x;
  double y;
  double z;
  size_t edge_;
  bool is_exist;
  std::vector<double> Kp;
};
struct H_face{
  H_face():
      is_exist(true){};
  size_t edge_;
  bool is_exist;
  std::vector<double> Kp;
};
struct H_edge{ // read_only to others
  H_edge():
      oppo_(-1),  is_exist(true) {};

  size_t next_, prev_, face_, vertex_;
  int oppo_;
  //  const halfedge &HE_;
  double length;
  bool is_exist;
};
// struct vertex_pair{
//   size_t p , q ;
// };

typedef std::pair<size_t, size_t> vertex_pair;

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
  void halfedge_to_obj( const std::string & outfile);
 private:
  void ReadVertex(std::ifstream &fin, std::string &keyword);
  void ReadFace(std::ifstream &fin, std::string &keyword);
  void ReadAnno(std::ifstream &fin, std::string &keyword);


  void cal_Kp_face(H_face & face_);
  void cal_Kp_vertex(H_vertex &vertex_);

  std::string filename;
};
#endif // HALFEDGE_H
