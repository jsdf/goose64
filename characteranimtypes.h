
#ifndef CHARACTERANIMTYPES_H
#define CHARACTERANIMTYPES_H 1

typedef enum CharacterMeshType {
  characterbicep_l_characterbicep_lmesh,
  characterbicep_r_characterbicep_rmesh,
  characterfoot_l_characterfoot_lmesh,
  characterfoot_r_characterfoot_rmesh,
  characterhand_l_characterhand_lmesh,
  characterhand_r_characterhand_rmesh,
  characterhead_characterheadmesh,
  charactershin_l_charactershin_lmesh,
  charactershin_r_charactershin_rmesh,
  characterspine_characterspinemesh,
  characterthigh_l_characterthigh_lmesh,
  characterthigh_r_characterthigh_rmesh,
  MAX_CHARACTER_MESH_TYPE,
} CharacterMeshType;

typedef enum CharacterAnimType {
  character_idle_anim,
  character_walk_anim,
  MAX_CHARACTER_ANIM_TYPE,
} CharacterAnimType;

#endif /* CHARACTERANIMTYPES_H */
