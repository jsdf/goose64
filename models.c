#include <nusys.h>

#include "models.h"

#include "modeltype.h"

// models
#include "book.h"
#include "bush.h"
#include "characterrig.h"
#include "flagpole.h"
#include "gooserig.h"
#include "ground.h"
#include "planter.h"
#include "rocks.h"
#include "testingCube.h"
#include "university_bldg.h"
#include "university_floor.h"
#include "wall.h"
#include "water.h"

// map the mesh type enum (used by animation frames) to the mesh displaylists
Gfx* GooseMeshList[8] = {
    Wtx_gooserig_goosebody_goosebodymesh,      // goosebody_goosebodymesh
    Wtx_gooserig_goosehead_gooseheadmesh,      // goosehead_gooseheadmesh
    Wtx_gooserig_gooseleg_l_gooseleg_lmesh,    // gooseleg_l_gooseleg_lmesh
    Wtx_gooserig_goosefoot_l_goosefoot_lmesh,  // goosefoot_l_goosefoot_lmesh
    Wtx_gooserig_gooseleg_r_gooseleg_rmesh,    // gooseleg_r_gooseleg_rmesh
    Wtx_gooserig_goosefoot_r_goosefoot_rmesh,  // goosefoot_r_goosefoot_rmesh
    Wtx_gooserig_gooseneck_gooseneckmesh,      // gooseneck_gooseneckmesh
};

Gfx* CharacterMeshList[12] = {
    Wtx_characterrig_gkbicep_l_gkbicep_lrmesh,  // characterbicep_l_characterbicep_lmesh
    Wtx_characterrig_gkbicep_r_gkbicep_rmesh,  // characterbicep_r_characterbicep_rmesh
    Wtx_characterrig_gkfoot_l_gkfoot_lrmesh,  // characterfoot_l_characterfoot_lmesh
    Wtx_characterrig_gkfoot_r_gkfoot_rmesh,  // characterfoot_r_characterfoot_rmesh
    Wtx_characterrig_gkforearm_l_gkforearm_lrmesh,  // characterforearm_l_characterforearm_lmesh
    Wtx_characterrig_gkforearm_r_gkforearm_rmesh,  // characterforearm_r_characterforearm_rmesh
    Wtx_characterrig_gkhead_gkheadmesh,      // characterhead_characterheadmesh
    Wtx_characterrig_gkshin_l_gkshin_lmesh,  // charactershin_l_charactershin_lmesh
    Wtx_characterrig_gkshin_r_gkshin_rmesh,  // charactershin_r_charactershin_rmesh
    Wtx_characterrig_gktorso_gktorsomesh,  // charactertorso_charactertorsomesh
    Wtx_characterrig_gkthigh_l_gkthigh_lmesh,  // characterthigh_l_characterthigh_lmesh
    Wtx_characterrig_gkthigh_r_gkthigh_rmesh,  // characterthigh_r_characterthigh_rmesh
};

Gfx* getMeshDisplayListForModelMeshPart(ModelType modelType, int meshPart) {
  switch (modelType) {
    case GooseModel:
      return GooseMeshList[meshPart];
    default:
      return CharacterMeshList[meshPart];
  }
}

Gfx* getModelDisplayList(ModelType modelType, int subtype) {
  switch (modelType) {
    case BushModel:
      return Wtx_bush;
    case BookItemModel:
      return Wtx_book;
    case UniBldgModel:
      return Wtx_university_bldg;
    case UniFloorModel:
      return Wtx_university_floor;
    case FlagpoleModel:
      return Wtx_flagpole;
    case WallModel:
      return Wtx_wall;
    case PlanterModel:
      return Wtx_planter;
    case GroundModel:
      switch (subtype) {
        case 0:
          return Wtx_ground_Ground_001_Grid_002;
        case 1:
          return Wtx_ground_Ground_002_Grid_003;
        case 2:
          return Wtx_ground_Ground_003_Grid_004;
        case 3:
          return Wtx_ground_Ground_004_Grid_005;
        case 4:
          return Wtx_ground_Ground_005_Grid_006;
        case 5:
          return Wtx_ground_Ground_006_Grid_007;
        case 6:
          return Wtx_ground_Ground_007_Grid_008;
      }
    case RockModel:
      switch (subtype) {
        case 0:
          return Wtx_rocks_Rock_Cube_001;
        case 1:
          return Wtx_rocks_Rock_001_Cube_002;
        case 2:
          return Wtx_rocks_Rock_002_Cube_003;
      }
    case WaterModel:
      return Wtx_water;
    default:
      return Wtx_testingCube;
  }
}
