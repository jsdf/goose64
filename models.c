#include <nusys.h>

#include "models.h"

#include "modeltype.h"

// models
#include "book.h"
#include "bush.h"
#include "characterrig.h"
#include "gooserig.h"
#include "ground.h"
#include "lilypad.h"
#include "planter.h"
#include "reed.h"
#include "rocks.h"
#include "testingCube.h"
#include "wall.h"
#include "water.h"
#include "watergrass.h"

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
      return bush_model_meshes[subtype];
    case BookItemModel:
      return book_model_meshes[subtype];
    case WallModel:
      return wall_model_meshes[subtype];
    case PlanterModel:
      return planter_model_meshes[subtype];
    case GroundModel:
      return ground_model_meshes[subtype];
    case RockModel:
      return rocks_model_meshes[subtype];
    case WaterModel:
      return water_model_meshes[subtype];
    case WatergrassModel:
      return watergrass_model_meshes[subtype];
    case ReedModel:
      return reed_model_meshes[subtype];
    case LilypadModel:
      return lilypad_model_meshes[subtype];
    default:
      return testingCube_model_meshes[0];
  }
}
