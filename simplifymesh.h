#ifndef SIMPLIFYMESH_H
#define SIMPLIFYMESH_H
#include"halfedge.h"
#include<set>
struct ident{
  size_t id;
  double  value;
};

class simplify_mesh
{
 public:
  simplify_mesh(halfedge &HE):mesh_init(HE),zero_({0,0}){}
  halfedge mesh_init;

  void Simp_shorstest(const size_t &iter_times);
  // int main_();

 private:
  std::set<ident> priority;
  const ident zero_;
  void make_priority();
  void pop_priority(const size_t &edge_id);
  void modify_priority (const size_t &edge_id, const double &value_new);
  
  void change_topology (const size_t &edge_id, const int &edge_oppo_id, const int &result);

  int check_manifold( size_t &edge_id,  int &edge_oppo_id);
};

#endif // SIMPLIFYMESH_0
