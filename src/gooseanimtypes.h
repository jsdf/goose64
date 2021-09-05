
#ifndef GOOSEANIMTYPES_H
#define GOOSEANIMTYPES_H 1

typedef enum GooseMeshType {
  goosebody_goosebodymesh,
  goosehead_gooseheadmesh,
  gooseleg_l_gooseleg_lmesh,
  goosefoot_l_goosefoot_lmesh,
  gooseleg_r_gooseleg_rmesh,
  goosefoot_r_goosefoot_rmesh,
  gooseneck_gooseneckmesh,
  MAX_GOOSE_MESH_TYPE,
} GooseMeshType;

typedef enum GooseAnimType {
  goose_idle_anim,
  goose_walk_anim,
  MAX_GOOSE_ANIM_TYPE,
} GooseAnimType;

#endif /* GOOSEANIMTYPES_H */
