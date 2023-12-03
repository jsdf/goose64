#ifndef GLGOOSE_HPP_
#define GLGOOSE_HPP_

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "imgui_glue.hpp"
#include "debug_gui.hpp"

#include "../renderer.h"

#define FREEVIEW_SPEED 0.2f

#define DEBUG_LOG_RENDER 0
#define DEBUG_TEXT_BASIC 0
#define DEBUG_OBJECTS 0
#define DEBUG_RAYCASTING 0
#define DEBUG_MODELS 0

#define DEBUG_ANIMATION 0
#define DEBUG_ANIMATION_MORE 0
#define DEBUG_ATTACHMENT 0

#define DEBUG_PHYSICS 0

#define DEBUG_COLLISION_MESH 0
#define DEBUG_COLLISION_MESH_MORE 0
#define DEBUG_COLLISION_SPATIAL_HASH 0
#define DEBUG_COLLISION_SPATIAL_HASH_RAYCAST 0
#define DEBUG_COLLISION_SPATIAL_HASH_TRIS 0
#define DEBUG_COLLISION_MESH_AABB 0

#define DEBUG_AABB 0
#define DEBUG_FRUSTUM 1
#define DEBUG_ZBUFFER_INTERSECTING 0
#define DEBUG_PAINTERS_ALGORITHM_SEPARATING_PLANE 0

#define DEBUG_PATHFINDING_GRAPH 0
#define DEBUG_PATHFINDING 0
#define DEBUG_PATHFINDING_AUTO 0

#define DEBUG_PROFILING 0

#if RENDERER_PAINTERS_ALGORITHM
#define RENDER_ZSORT 0
#endif
#define USE_SPRITES 0
#define USE_LIGHTING 1
#define USE_LIGHTING_STATIC_ONLY 1
#define USE_FLAT_SHADING 1
#define USE_ANIM_FRAME_LERP 1
#define ENABLE_NODEGRAPH_EDITOR 0

extern RendererZSortList zSortListBG;
extern RendererZSortList zSortListFG;
extern AABB *localAABBs;
extern PhysWorldData physWorldData;
extern int updateSkipRate;
extern Vec3d freeViewPos;
extern bool enableControlsInFreeView;
extern int frustumPlaneToTest;
extern Frustum frustum;
extern float profAvgCharacters;
extern float profAvgPhysics;
extern float profAvgDraw;
extern float profAvgPath;

extern int objsFrustumCulled;
extern int backgroundTrisCulled;

extern GameObject *selectedObject;

glm::vec3 getTriCentroidWorld(GameObject *obj, MeshTri *meshtri);

#endif // GLGOOSE_HPP_
