

## geometry modes for color/shading
http://ultra64.ca/files/documentation/online-manuals/man-v5-1/tutorial/graphics/2/2_7.htm

So, for example, to color a surface using primitive color you would turn off the G_SHADE, G_SHADING_SMOOTH and G_LIGHTING flags. Then, as explained in the next section, by setting the combine mode to G_CC_PRIMITIVE you can add color using primitive color.

To color a surface using vertex color, you turn on G_SHADE and turn off G_LIGHTING. Then if G_SHADING_SMOOTH is turned off, flat shading is performed, and if it is turned on, smooth shading is performed.

For lighting, you turn on both G_SHADE and G_LIGHTING. As with vertex color, if G_SHADING_SMOOTH is turned off, then flat shading is used for rendering, and if it is turned on, then smooth shading is used.


## combine mode
set differently for single ('primitive') color vs vertex color/textured modes
http://ultra64.ca/files/documentation/online-manuals/man-v5-1/tutorial/graphics/2/2_7.htm

G_CC_PRIMITIVE: Output primitive color (shading/lighting & texture are ignored)
G_CC_SHADE: Output shading color (texture is ignored)
G_CC_DECALRGB: only texture is used (shading/lighting is ignored)
G_CC_MODULATERGB: Multiply texture color and shading color

