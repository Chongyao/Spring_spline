#ifndef SIMPLIFYMESH_H
#define SIMPLIFYMESH_H
#include"halfedge.h"

struct ident{
  size_t id;
  double  value;
};

class simplify_mesh
{
 public:
  simplify_mesh(halfedge &HE):mesh_init(HE){}
  halfedge mesh_init;

  void Simp_shorstest(const size_t &iter_times);
  // int main_();

 private:
  std::vector<ident> priority;
  void make_priority();
  void change_topology (const size_t &edge_id, const int &edge_oppo_id, const int &result);
  void pop_priority();
  int check_manifold(const size_t &edge_id, const int &edge_oppo_id);
};

#endif // SIMPLIFYMESH_0
