#include "glgoose.hpp"
#include "debug_gui.hpp"

#include "glm/glm.hpp"

#include "game.h"
#include "gameutils.h"
#include "option.h"

static bool debugZSortForeground = true;

void drawZSortListUI()
{
    RendererZSortList *list = debugZSortForeground ? &zSortListFG : &zSortListBG;

    ImGui::Begin("ZSortList"); // Create a window

    ImGui::Checkbox("Foreground", &debugZSortForeground);

    int bucketIdx = 0;
    for (bucketIdx = list->count - 1; bucketIdx >= 0; --bucketIdx)
    {
        RendererZSortBucket *bucket = &list->buckets[bucketIdx];
        if (bucket->count == 0)
        {
            continue;
        }
        ImGui::Text("-- bucket %d / %d items / %.1f-%.1f ---", bucketIdx,
                    bucket->count, list->near + list->bucketSize * bucketIdx,
                    list->near + list->bucketSize * bucketIdx + 1);
        for (int i = 0; i < bucket->count; ++i)
        {
            RendererZSortItem *item = &bucket->items[i];
            GameObject *obj = item->obj;

            Vec3d pos;
            pos = obj->position;

            glm::vec3 triCentroid = getTriCentroidWorld(obj, item->meshtri);
            Vec3d *viewPos = &Game_get()->viewPos;

            char itemLabel[96];
            sprintf(itemLabel, "%d %s: %.1f {%5.1f,%5.1f,%5.1f} {%5.1f,%5.1f,%5.1f}",
                    obj->id, ModelTypeStrings[obj->modelType],
                    glm::distance(glm::vec3(viewPos->x, viewPos->y, viewPos->z),
                                  triCentroid),
                    triCentroid[0], triCentroid[1], triCentroid[2], obj->position.x,
                    obj->position.y, obj->position.z);
            if (ImGui::Selectable(itemLabel, selectedObject == obj))
            {
                selectedObject = obj;
            }
        }
    }

    ImGui::End();
}

void DebugGUI::drawGUI()
{
    Game *game = Game_get();
    GameObject *obj = selectedObject;
    Character *selectedCharacter =
        obj == Game_get()->characters->obj ? Game_get()->characters : NULL;
    ImGuiInputTextFlags inputFlags =
        ImGuiInputTextFlags_EnterReturnsTrue; // only update on blur

    ImGui::Begin("Objects"); // Create a window
    for (int i = 0; i < game->worldObjectsCount; i++)
    {
        GameObject *listObj = game->worldObjects + i;
        if (ImGui::Selectable(
                (std::to_string(i) + " " + ModelTypeStrings[listObj->modelType])
                    .c_str(),
                listObj == obj))
        {
            selectedObject = listObj;
        }
    }
    ImGui::End();

#if RENDER_ZSORT
    drawZSortListUI();
#endif

    ImGui::Begin("Object Inspector"); // Create a window

    // Display some text (you can use a format strings too)
    ImGui::Text("Selected object: %d (%s)", obj ? obj->id : -1,
                obj ? ModelTypeStrings[obj->modelType] : "none");

    if (obj)
    {
        int spatialHashResults[100];
        Vec3d objCenter;
        Game_getObjCenter(obj, &objCenter);

        if (ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat3("Position", (float *)&obj->position, "%.3f",
                               inputFlags);
            ImGui::InputFloat3("Rotation", (float *)&obj->rotation, "%.3f",
                               inputFlags);
            ImGui::InputFloat3(
                "centroidOffset",
                (float *)&modelTypesProperties[obj->modelType].centroidOffset, "%.3f",
                inputFlags);

            ImGui::InputFloat("radius",
                              (float *)&modelTypesProperties[obj->modelType].radius,
                              0.1, 1.0, "%.3f", inputFlags);
            ImGui::InputInt("subtype", (int *)&obj->subtype, 0, 1,
                            ImGuiInputTextFlags_ReadOnly);

            AABB worldAABB = Renderer_getWorldAABB(localAABBs, obj);

            ImGui::InputFloat3("worldAABB.min", (float *)&worldAABB.min, "%.3f",
                               ImGuiInputTextFlags_ReadOnly);
            ImGui::InputFloat3("worldAABB.max", (float *)&worldAABB.max, "%.3f",
                               ImGuiInputTextFlags_ReadOnly);
        }
        if (obj->physBody)
        {
            if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::InputInt("physBody", (int *)&obj->physBody->id, 0, 1,
                                ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat3("phys Velocity",
                                   (float *)&obj->physBody->nonIntegralVelocity, "%.3f",
                                   ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat3("phys Acceleration",
                                   (float *)&obj->physBody->nonIntegralAcceleration,
                                   "%.3f", ImGuiInputTextFlags_ReadOnly);

                ImGui::InputFloat3("phys position", (float *)&obj->physBody->position,
                                   "%.3f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat3("phys prevPosition",
                                   (float *)&obj->physBody->prevPosition, "%.3f",
                                   ImGuiInputTextFlags_ReadOnly);
                ImGui::InputInt("phys enabled", (int *)&obj->physBody->enabled, 0, 1,
                                ImGuiInputTextFlags_ReadOnly);
                ImGui::InputInt("phys controlled", (int *)&obj->physBody->controlled, 0,
                                1, ImGuiInputTextFlags_ReadOnly);
            }
        }
        if (obj->animState)
        {
            if (ImGui::CollapsingHeader("Animation",
                                        ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::InputInt("state", (int *)&obj->animState->state, 0, 1,
                                ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat("progress", (float *)&obj->animState->progress, 0.1,
                                  1.0, "%.3f", ImGuiInputTextFlags_ReadOnly);
            }
        }
        if (selectedCharacter)
        {
            if (ImGui::CollapsingHeader("Character",
                                        ImGuiTreeNodeFlags_DefaultOpen))
            {
                Vec3d heading;
                GameUtils_directionFromTopDownAngle(obj->rotation.y, &heading);
                ImGui::InputFloat3("heading", (float *)&heading, "%.3f",
                                   ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat3("targetLocation",
                                   (float *)&selectedCharacter->targetLocation, "%.3f",
                                   ImGuiInputTextFlags_ReadOnly);
                ImGui::Text("state: %s",
                            CharacterStateStrings[selectedCharacter->state]);

                ImGui::InputFloat("speedMultiplier",
                                  (float *)&selectedCharacter->speedMultiplier, 0.1, 1.0,
                                  "%.3f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat("heading speed scale",
                                  (float *)&selectedCharacter->speedScaleForHeading, 0.1,
                                  1.0, "%.3f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat(
                    "heading turn scale",
                    (float *)&selectedCharacter->turningSpeedScaleForHeading, 0.1, 1.0,
                    "%.3f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat("arrival speed scale",
                                  (float *)&selectedCharacter->speedScaleForArrival, 0.1,
                                  1.0, "%.3f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputInt("pathProgress", (int *)&selectedCharacter->pathProgress,
                                0, 1, ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat("pathSegmentProgress",
                                  (float *)&selectedCharacter->pathSegmentProgress, 0.1,
                                  1.0, "%.3f", ImGuiInputTextFlags_ReadOnly);
                if (selectedCharacter->pathfindingResult)
                {
                    ImGui::Text("path:");
                    int i;
                    int *pathNodeID;

                    for (i = 0,
                        pathNodeID = selectedCharacter->pathfindingResult->result; //
                         i < selectedCharacter->pathfindingResult->resultSize;     //
                         i++, pathNodeID++)
                    {
                        ImGui::Text("%d: %d", i, *pathNodeID);
                    }
                }
            }
        }

        if (ImGui::CollapsingHeader("Collision"))
        {
            ImGui::InputInt("testCollisionResult", (int *)&testCollisionResult, 0, 1,
                            inputFlags);

            std::string collKeys;
            if (testCollisionResults.size())
            {
                std::map<int, SphereTriangleCollision>::iterator collIter =
                    testCollisionResults.begin();

                while (collIter != testCollisionResults.end())
                {
                    collKeys += std::to_string(collIter->first) + ",";

                    collIter++;
                }
            }
            ImGui::Text("colliding tris=%s", collKeys.c_str());

            int spatialHashResultsCount = SpatialHash_getTriangles(
                &objCenter, Game_getObjRadius(obj),
                physWorldData.worldMeshSpatialHash, spatialHashResults,
                /*maxResults*/ 100);

            std::string bucketKeys;
            int i;
            for (i = 0; i < spatialHashResultsCount; i++)
            {
                bucketKeys += std::to_string(spatialHashResults[i]) + ",";
            }

            ImGui::Text("grid pos x=%f, y=%f",
                        SpatialHash_unitsToGridFloatForDimension(
                            objCenter.x, physWorldData.worldMeshSpatialHash),
                        SpatialHash_unitsToGridFloatForDimension(
                            objCenter.z, physWorldData.worldMeshSpatialHash));

            ImGui::Text("current bucket tris=%s", bucketKeys.c_str());
        }
    }

    if (ImGui::CollapsingHeader("Global", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::InputFloat("physWorldData.gravity", (float *)&physWorldData.gravity,
                          1.0, 10.0, "%.3f", inputFlags);

        ImGui::InputInt("updateSkipRate", (int *)&updateSkipRate, 1, 10, inputFlags);
        updateSkipRate = MAX(updateSkipRate, 1);
        for (int i = 0; i < MAX_OPTION; i++)
        {
            ImGui::InputInt(OptionStrings[i], (int *)&options + i, 1, 10, inputFlags);
        }
    }

    if (ImGui::CollapsingHeader("Camera",
#if DEBUG_FRUSTUM
                                ImGuiTreeNodeFlags_DefaultOpen
#else
                                0
#endif
                                ))
    {
        ImGui::InputFloat3("viewPos", (float *)&game->viewPos, "%.3f");
        ImGui::InputFloat3("viewTarget", (float *)&game->viewTarget, "%.3f");
        ImGui::InputFloat3("freeViewPos", (float *)&freeViewPos, "%.3f");
        ImGui::Checkbox("enableControlsInFreeView", &enableControlsInFreeView);
        ImGui::Combo("plane to test", &frustumPlaneToTest, FrustumPlanesStrings,
                     NUM_FRUSTUM_PLANES);

        if (obj)
        {
            {
                ImGui::Text("frustum test results for obj=%d", obj->id);

                AABB worldAABB = Renderer_getWorldAABB(localAABBs, obj);

                ImGui::Text("boxInFrustum: %s",
                            FrustumTestResultStrings[Frustum_boxInFrustum(&frustum,
                                                                          &worldAABB)]);
                ImGui::Text("boxInFrustumNaive: %s",
                            FrustumTestResultStrings[Frustum_boxInFrustumNaive(
                                &frustum, &worldAABB)]);
                ImGui::Text("Per plane tests:");
                for (int i = 0; i < NUM_FRUSTUM_PLANES; ++i)
                {
                    ImGui::Text("%s: %s", FrustumPlanesStrings[i],
                                FrustumTestResultStrings[Frustum_boxFrustumPlaneTestPN(
                                    &frustum, &worldAABB, i)]);
                }
            }

#if DEBUG_PAINTERS_ALGORITHM_SEPARATING_PLANE
            if (obj)
            {
                GameObject *a = game->player.goose;
                GameObject *b = selectedObject;
                RendererSortDistance sortDistA, sortDistB;
                Plane separatingPlane;
                Vec3d aCenter, bCenter, aClosestPoint, bClosestPoint,
                    aReallyClosestPoint, bReallyClosestPoint;
                Game_getObjCenter(a, &aCenter);
                Game_getObjCenter(b, &bCenter);
                AABB aabbA = Renderer_getWorldAABB(localAABBs, a);
                AABB aabbB = Renderer_getWorldAABB(localAABBs, b);
                sortDistA = (RendererSortDistance){
                    /*obj*/ game->player.goose,
                    /*distance*/ 0,
                    /*worldAABB*/
                    Renderer_getWorldAABB(localAABBs, game->player.goose)};
                sortDistB = (RendererSortDistance){
                    /*obj*/ selectedObject,
                    /*distance*/ 0,
                    /*worldAABB*/ Renderer_getWorldAABB(localAABBs, selectedObject)};
                int aCloser = Renderer_isCloserBySeparatingPlane(&sortDistA, &sortDistB,
                                                                 &game->viewPos);

                Renderer_closestPointOnAABB(&aabbA, &bCenter, &aClosestPoint);
                Renderer_closestPointOnAABB(&aabbB, &aCenter, &bClosestPoint);

                Renderer_closestPointOnAABB(&aabbA, &bClosestPoint,
                                            &aReallyClosestPoint);
                Renderer_closestPointOnAABB(&aabbB, &aClosestPoint,
                                            &bReallyClosestPoint);

                Renderer_getSeparatingPlane(&aReallyClosestPoint, &bReallyClosestPoint,
                                            &separatingPlane);

                float planeToADist = Plane_distance(&separatingPlane, &aCenter);
                float planeToViewDist =
                    Plane_distance(&separatingPlane, &game->viewPos);

                ImGui::InputFloat3("aCenter", (float *)&aCenter, "%.3f");
                ImGui::InputFloat3("separatingPlane.point",
                                   (float *)&separatingPlane.point, "%.3f");
                ImGui::InputFloat("planeToADist", (float *)&planeToADist, 0.1, 1.0,
                                  "%.3f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat("planeToViewDist", (float *)&planeToViewDist, 0.1, 1.0,
                                  "%.3f", ImGuiInputTextFlags_ReadOnly);
                ImGui::Text(aCloser == -1 ? "goose is closer than selectedObject"
                                          : "goose is NOT closer than selectedObject");
            }
#endif
        }

        if (ImGui::CollapsingHeader("Camera++"))
        {
            ImGui::InputFloat3("ntl", (float *)&frustum.ntl, "%.3f");
            ImGui::InputFloat3("ntr", (float *)&frustum.ntr, "%.3f");
            ImGui::InputFloat3("nbl", (float *)&frustum.nbl, "%.3f");
            ImGui::InputFloat3("nbr", (float *)&frustum.nbr, "%.3f");
            ImGui::InputFloat3("ftl", (float *)&frustum.ftl, "%.3f");
            ImGui::InputFloat3("ftr", (float *)&frustum.ftr, "%.3f");
            ImGui::InputFloat3("fbl", (float *)&frustum.fbl, "%.3f");
            ImGui::InputFloat3("fbr", (float *)&frustum.fbr, "%.3f");
            ImGui::InputFloat("nearD", (float *)&frustum.nearD, 1, 1, "%.3f",
                              ImGuiInputTextFlags_ReadOnly);
            ImGui::InputFloat("farD", (float *)&frustum.farD, 1, 1, "%.3f",
                              ImGuiInputTextFlags_ReadOnly);
            ImGui::InputFloat("aspect", (float *)&frustum.aspect, 1, 1, "%.3f",
                              ImGuiInputTextFlags_ReadOnly);
            ImGui::InputFloat("fovy", (float *)&frustum.fovy, 1, 1, "%.3f",
                              ImGuiInputTextFlags_ReadOnly);
            ImGui::InputFloat("tang", (float *)&frustum.tang, 1, 1, "%.3f",
                              ImGuiInputTextFlags_ReadOnly);
            ImGui::InputFloat("nw", (float *)&frustum.nw, 1, 1, "%.3f",
                              ImGuiInputTextFlags_ReadOnly);
            ImGui::InputFloat("nh", (float *)&frustum.nh, 1, 1, "%.3f",
                              ImGuiInputTextFlags_ReadOnly);
            ImGui::InputFloat("fw", (float *)&frustum.fw, 1, 1, "%.3f",
                              ImGuiInputTextFlags_ReadOnly);
            ImGui::InputFloat("fh", (float *)&frustum.fh, 1, 1, "%.3f",
                              ImGuiInputTextFlags_ReadOnly);
        }
    }

    if (ImGui::CollapsingHeader("Profiling",
#if DEBUG_PROFILING
                                ImGuiTreeNodeFlags_DefaultOpen
#else
                                0
#endif
                                ))
    {
        ImGui::Text("Frametime %.3f ms (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Text("Phys=%.3fms, Char=%.3f ms, Draw=%.3f ms, Path=%.3f ms",
                    profAvgPhysics, profAvgCharacters, profAvgDraw, profAvgPath);
        ImGui::InputInt("objsFrustumCulled", (int *)&objsFrustumCulled, 0, 10,
                        ImGuiInputTextFlags_ReadOnly);
        ImGui::InputInt("backgroundTrisCulled", (int *)&backgroundTrisCulled, 0, 10,
                        ImGuiInputTextFlags_ReadOnly);
    }

    Vec3d *goosePos = &Game_get()->player.goose->position;
    if (ImGui::CollapsingHeader("Pathfinding"))
    {
#if DEBUG_PATHFINDING
        ImGui::InputInt("debugPathfindingFrom", (int *)&debugPathfindingFrom, 1, 10,
                        inputFlags);
        ImGui::InputInt("debugPathfindingTo", (int *)&debugPathfindingTo, 1, 10,
                        inputFlags);

        if (ImGui::Button("print pos"))
        {
            printf("{%.3f, %.3f, %.3f}\n", goosePos->x, goosePos->y, goosePos->z);
        }

#endif
    }

#if ENABLE_NODEGRAPH_EDITOR
    drawNodeGraphGUI(nodeGraph, goosePos, "garden_map_graph", "garden_map_graph",
                     selectedNode);
#endif

    ImGui::End();
}
