#include <nusys.h>

#include "models.h"

#include "constants.h"
#include "ed64io_usb.h"
#include "modeltype.h"

#include "modellist.h"

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
    Wtx_characterrig_gkbicep_l_gkbicep_lrmesh,  //    characterbicep_l_characterbicep_lmesh
    Wtx_characterrig_gkbicep_r_gkbicep_rmesh,  //    characterbicep_r_characterbicep_rmesh
    Wtx_characterrig_gkfoot_l_gkfoot_lrmesh,  //    characterfoot_l_characterfoot_lmesh
    Wtx_characterrig_gkfoot_r_gkfoot_rmesh,  //    characterfoot_r_characterfoot_rmesh
    Wtx_characterrig_gkforearm_l_gkforearm_lrmesh,  //    characterforearm_l_characterforearm_lmesh
    Wtx_characterrig_gkforearm_r_gkforearm_rmesh,  //    characterforearm_r_characterforearm_rmesh
    Wtx_characterrig_gkhead_gkheadmesh,  //    characterhead_characterheadmesh
    Wtx_characterrig_gkshin_l_gkshin_lmesh,  // charactershin_l_charactershin_lmesh
    Wtx_characterrig_gkshin_r_gkshin_rmesh,  //    charactershin_r_charactershin_rmesh
    Wtx_characterrig_gktorso_gktorsomesh,  // charactertorso_charactertorsomesh
    Wtx_characterrig_gkthigh_l_gkthigh_lmesh,  //    characterthigh_l_characterthigh_lmesh
    Wtx_characterrig_gkthigh_r_gkthigh_rmesh,  //    characterthigh_r_characterthigh_rmesh
};

Gfx* getMeshDisplayListForModelMeshPart(ModelType modelType, int meshPart) {
  switch (modelType) {
    case GooseModel:
      return GooseMeshList[meshPart];
    case GardenerCharacterModel:
      return CharacterMeshList[meshPart];
    default:
      return testingCube_model_meshes[0];
  }
}

Gfx* getModelDisplayList(ModelType modelType, int subtype) {
  switch (modelType) {
    case BushModel:
      invariant(subtype < BUSH_MODEL_MESH_COUNT);
      return bush_model_meshes[subtype];
    case BookItemModel:
      invariant(subtype < BOOK_MODEL_MESH_COUNT);
      return book_model_meshes[subtype];
    case WallModel:
      invariant(subtype < WALL_MODEL_MESH_COUNT);
      return wall_model_meshes[subtype];
    case PlanterModel:
      invariant(subtype < PLANTER_MODEL_MESH_COUNT);
      return planter_model_meshes[subtype];
    case GroundModel:
      invariant(subtype < GROUND_MODEL_MESH_COUNT);
      return ground_model_meshes[subtype];
    case RockModel:
      invariant(subtype < ROCKS_MODEL_MESH_COUNT);
      return rocks_model_meshes[subtype];
    case WaterModel:
      invariant(subtype < WATER_MODEL_MESH_COUNT);
      return water_model_meshes[subtype];
    case WatergrassModel:
      invariant(subtype < WATERGRASS_MODEL_MESH_COUNT);
      return watergrass_model_meshes[subtype];
    case ReedModel:
      invariant(subtype < REED_MODEL_MESH_COUNT);
      return reed_model_meshes[subtype];
    case LilypadModel:
      invariant(subtype < LILYPAD_MODEL_MESH_COUNT);
      return lilypad_model_meshes[subtype];
    default:
      return testingCube_model_meshes[0];
  }
}
