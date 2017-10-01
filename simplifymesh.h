#ifndef SIMPLIFYMESH_H
#define SIMPLIFYMESH_H
#include"halfedge.h"

struct ident{
        size_t id;
        double  ident;
};

class simplify_mesh
{
public:
    simplify_mesh(halfedge &HE):mesh_init(HE){}
    halfedge mesh_init;

    void Simp_shorstest();

private:
  vector<idnt> priority;
  void change_topology (const size_t &edge_id, const size_t &edge_oppo_id);

};

#endif // SIMPLIFYMESH_0







