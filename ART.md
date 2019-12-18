# Art asset authoring

# Single-body Models
For objects that have no moving parts ('single-body'), you can simply move them to the origin (though probably aligning the bottom to the origin is best, so they are 'sitting on the ground'), and then export as OBJ.

The model should be textured with a single UV-mapped 32x32 pixel BMP texture.

Single-body models should be exported with the following settings enabled:
- Write Normals
- Include UVs

plus any other ones which make sense for your scene/workflow (eg. Apply Modifiers, Selection Only, etc)

Run `./rebuild_models.sh` to turn your .obj file into a .h file containing displaylists for rendering the model.


## Level Editing
Place all the objects you want in your game level. They should have the same name as the ModelType defined to represent them in the ModelTypes enum.


To export the level, run the script `export_positions.py` using Blender's text editor (or however you prefer to run Blender Python scripts)

## Character Animation

Create a rigid body animation in Blender. There should be one Blender object per animatable body. Using an armature and parenting all the bodies to bones makes this easier, but is not essential.

There is a required naming scheme for the parts of the rigid body character. The parts must each be made up of an object containing a mesh which is named with the convention ${object_name}mesh, eg. an object called 'gooseneck' should have a mesh called 'gooseneckmesh' and when exported as .obj the resultant object section is called 'gooseneck_gooseneckmesh'.

Character models should be exported with the following settings enabled:
- Objects as OBJ objects
- Write Normals
- Include UVs

as well as any other flags that make sense for your workflow.

To export the animation, run the script `export_animation.py` in Blender which will generate `${modelname}_anim.h`

Manually create a `${modelname}animtypes.h` file defining a `${ModelName}MeshType` enum which includes all the enum values used to refer to model parts in `${modelname}_anim_data` of the generated `${modelname}_anim.h` file. Also define another enum which includes all the animation names in the order they are defined in `${modelname}_anim_ranges`. (maybe we can just output these enums into the generated header file in future?)

To export the character model, run the script `export_character_model.py` in Blender which will generate `${modelname}rig.obj`. Running `./rebuild_models.sh` will generate a .h file containing displaylists for rendering the model (one displaylist per model part).
 
Somewhere (stage00.c ??) define an array like `Gfx* ${modelname}MeshList[] = {...}` containing pointers to the displaylists to render for each model part, in the same order as the corresponding parts appear in the `${ModelName}MeshType` enum. This mapping will be used to determine which displaylist to render for each AnimationFrame struct in the animation frame data.