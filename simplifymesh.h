#ifndef SIMPLIFYMESH_H
#define SIMPLIFYMESH_H
#include"halfedge.h"
#include<map>
#include"zjucad/matrix/include/matrix.h"
struct ident{
  size_t id;
  double  value;
};

class simplify_mesh
{
 public:
  simplify_mesh(halfedge &HE):mesh_init(HE),zero_({0,0}){}
  halfedge mesh_init;

  void Simp_shorstest(const size_t &iter_times, const std::string &outfile);
  // int main_();

 private:
  std::map<ident,zjucad::matrix::matrix<double>> priority;
  const ident zero_;
  void make_priority();
  void pop_priority(const size_t &edge_id);
  void modify_priority (const size_t &edge_id, const double &value_new, const zjucad::matrix::matrix<double> &V);
  
  void change_topology (const zjucad::matrix::matrix<double> &new_V, const size_t &edge_id, const int &edge_oppo_id, const int &result);

  int check_manifold( size_t &edge_id,  int &edge_oppo_id, zjucad::matrix::matrix<double>&new_V);

  void cal_error(const size_t &edge_id, double &error, zjucad::matrix::matrix<double> &V);


  void delete_halfedges(const size_t &edge_id, const size_t &edge_oppo_id);
  void delete_vertex(const size_t &edge_oppo_id);
  void delete_faces(const size_t &edge_id, const size_t &edge_oppo_id);
  size_t change_vertex(const size_t &edge_id, const size_t &edge_oppo_id, const zjucad::matrix::matrix<double> &new_V, const int &result);
  void change_oppoedge(const size_t &edge_id, const size_t &edge_oppo_id);
  void change_face_kp(const size_t &vertex_ur_id);
  void change_vertex_kp(const size_t &vertex_ur_id);
  void change_priority(const size_t &vertex_ur_id);
  
};

#endif // SIMPLIFYMESH_0
