# Art asset authoring

# Single-body Models
For objects that have no moving parts ('single-body'), you can simply move them to the origin (though probably aligning the bottom to the origin is best, so they are 'sitting on the ground'), and then export as OBJ.

The model should be textured with a single UV-mapped 32x32 pixel BMP texture.

Single-body models should be exported with the following settings enabled:
- Write Normals
- Include UVs

plus any other ones which make sense for your scene/workflow (eg. Apply Modifiers, Selection Only, etc)


## Level Editing
Place all the objects you want in your game level. They should have the same name as the ModelType defined to represent them in the ModelTypes enum.


To export the animation, run the script `export_positions.py` using Blender's text editor (or however you prefer to run Blender Python scripts)

## Character Animation

Create a rigid body animation in Blender. There should be one Blender object per animatable body. Using an armature and parenting all the bodies to bones makes this easier, but is not essential.

There is a required naming scheme for the parts of the rigid body character. The parts must each be made up of an object containing a mesh which is named with the convention ${object_name}mesh, eg. an object called 'gooseneck' should have a mesh called 'gooseneckmesh' and when exported as .obj the resultant object section is called 'gooseneck_gooseneckmesh'.

Character models should be exported with the following settings enabled:
- Objects as OBJ objects
- Write Normals
- Include UVs

as well as any other flags that make sense for your workflow.

To export the animation, run the script `export_animation.py` in Blender