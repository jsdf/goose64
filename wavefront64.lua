bitmap = require("wavefront64deps.bitmap")
obj_loader = require("wavefront64deps.obj_loader")
require("wavefront64deps.BinDecHex")
require("wavefront64deps.util")
inspect = require("wavefront64deps.inspect")

interactive_input = false

helptext = [[

WAVEFRONT 64
============
Usage:
	OBJECT TO C
	"lua main.lua obj <path_to_obj>"
	e.g. `lua main.lua obj Celebi.obj`

	SPRITE TO C
	"lua main.lua spr <path_to_bmp>"
	e.g. `lua main.lua spr Celebi.bmp`

Outputs to a C header file of the same name.
e.g. `Celebi.h`	

Please note that this currently only works with objects that:
* have only a single mesh
* have one bitmap texture with a max size of 32x32 (sprites can be any size!)
And also vertex colours don't work yet. I'll work on that!
]]



function Vec3d_normalise(x,y,z)
  if (x == 0 and y == 0) then
    return x, y, z
  end

  magnitude = math.sqrt(x * x + y * y + z * z)
  x = x / magnitude
  y = y / magnitude
  z = z / magnitude
  return x, y, z
end

function w64_main()
	print("WAVEFRONT64")
	filename = ""
	local final_file_output = {}
	if(arg[1] == nil or arg[2] == nil) then err(helptext) end

	if(arg[1]:lower()=="obj") then
		-- OBJECT PARSING
		-- Main functional pipeline
		local objname, objtable, texturename, bmp, objscale, fast3d = w64_initTexturedObject(arg[2])
		local bmptable, previewtable 								= w64_bmpFileToC(bmp)
		local facetable, verttable, verttexttable 					= w64_VFFormat(objtable, objscale)
		local facesinpacks, facesinpacksrefs						= w64_VFSort(facetable)

		-- File output pipeline (push to final_file_output)
		local metaString, textureString 	= w64_outputTexture(objname,texturename,previewtable,bmptable)
		local vertexString 					= w64_outputVerts(facesinpacks,facesinpacksrefs,verttexttable,objname)
		local faceString 					= w64_outputTriangles(facesinpacks, facesinpacksrefs, fast3d, objname)
		local displayListString 			= w64_outputDisplayList(objname,texturename)
		final_file_output = {metaString,textureString,vertexString,faceString,displayListString}
		filename = objname
	elseif(arg[1]:lower()=="spr") then
		-- SPRITE ONLY PARSING
		-- functional pipeline
		local objname, bmp 				= w64_initSprite(arg[2])
		local bmptable, previewtable 	= w64_bmpFileToC(bmp)
		local metastring, datastring 	= w64_outputTexture(0,objname,previewtable,bmptable)
		final_file_output = {metastring, datastring}
		filename = objname
	else
		err("Improper operation type: "..arg[1].."!")
	end

	-- write file
	file = io.open(filename..".h","w")
	io.output(file)
	io.write(table.concat(final_file_output,"\n\n"))
	io.close(file)
	print("=================================")
	print(string.format("DONE! Output file: %s.h",filename))
	print("=================================")
end

function w64_initSprite(argument)
	local sprfilename = string.match(argument, "([A-Za-z0-9]+)")
	local bmp = w64_loadBitmap(sprfilename..".bmp")
	return sprfilename, bmp 
end

function w64_loadBitmap(image_file_name)
	bmp = bitmap.from_file(image_file_name)
	if(bmp==nil) then
		err("ERROR: File "..image_file_name.." doesn't exist, or file isn't a bitmap")
	else
		print("BMP File "..image_file_name.." found.")
	end
	return bmp 
end

function w64_initTexturedObject(argument)
	-- object init stuff
	local obj_Name = string.match(argument, "([^.\\]+).obj$")

	local mtl_file = readFile(obj_Name..".mtl")
	if(mtl_file == nil) then err("ERROR: No MTL file found for "..obj_Name..".mtl") else print("MTL file "..obj_Name..".mtl found.") end

	-- init object
	local obj_Table = obj_loader.load(obj_Name..".obj")
	if(obj_Table==nil) then
		err(obj_Name..".obj not found")
	else
		print("OBJ File "..obj_Name..".obj found.")
	end

	-- load bitmap
	local image_file_name, name_of_texture = string.match(mtl_file, "map_Kd (([A-Za-z_-]+).[A-Za-z_-]+)")
	local bmp = w64_loadBitmap(image_file_name)

	-- asks user what tf they want
	print("Vertex scale? [int. default:30]" )
	io.write(">")
	local object_scale = interactive_input and tonumber(io.read()) or 30  -- TODO figure out what this should be
	print("Object Scale set to "..object_scale)

	print("Use gsSP1Triangle? (for Fast3D) [y/n. default:n]" )
	io.write(">")
	local one_tri = interactive_input and (io.read():lower() == "y") or false
	print("Fast3D on: "..tostring(one_tri))


	return obj_Name, obj_Table, name_of_texture, bmp, object_scale, one_tri
end

function w64_bmpFileToC(bmp_file)
	--[[ 
	====================================
	 BITMAP PARSING TO BIG HEX-Y CHUNKS
	====================================
	TODO:
	* use this section to create sprite header files
	* sort this entire program out, god damn!!!!!!!!

	--]] 
	print("Parsing bitmap...")
	local table_of_bytes = {}
	local table_preview = {}
	preview_tokens = {" ",".",":","-","=","+","*","#","%","@"}
	for i=0,bmp_file.width-1 do
		for j=bmp_file.height-1,0,-1 do
			r,g,b = bmp_file:get_pixel(i,j)
			if(j==bmp_file.height-1) then
				table.insert(table_preview,"\n\t")
			end
			table.insert(table_preview,preview_tokens[math.floor((((r+g+b)/(255*3))*#preview_tokens-1))+1])

			local binstring = padBinaryLeft(intToBin(math.floor(r/8)),5)..
				padBinaryLeft(intToBin(math.floor(g/8)),5)..
				padBinaryLeft(intToBin(math.floor(b/8)),5).."1"
				-- the "1" on the end is the alpha, i'll add
				-- features to let this be changed once i add
				-- PNG support and stuff

			local outstring = "0x"..string.lower(Bin2Hex(binstring))..","
			if(j==bmp_file.height-1) then outstring = "\t"..outstring end
			if(j==0) then outstring = outstring.."\n" end
			table.insert(table_of_bytes,outstring)
		end
	end
	print("Success parsing bitmap!")
	return table_of_bytes, table_preview
end

function w64_VFFormat(obj_Table, object_scale)
	--[[ 
	======================================
	 FACES AND VERTS OUTPUT
	======================================

	TABLE STRUCTURES REFERENCE
	==========================
	vertexTable {
		"3/5":
			"index": 4
			"content": "{ 30, -30, 30, 0, 0, 990, 130, 130, 130, 0},"
		"4/6"
			"index": 5 
			...

	faceTable {
		1: {2,5,3}
		2: {4,0,2}
		3: ...

	--]] 

	print("Creating faces and verts...")

	local faceTable = {}
	local vertexTable = {}
	local vertexOutputTable = {}

	local vertsCreated = 0

	-- generate faces and verts arrays
	for i=1,#obj_Table.f do
		local faceVertReference = {}
		for j=1,3 do
			-- "4/6", etc
			local vert_ref_string = string.format("%i/%i",obj_Table.f[i][j].v,obj_Table.f[i][j].vt)

			-- if the vertex DOESN'T already exist within a table, then create and assign it
			if(vertexTable[vert_ref_string] == nil) then
				--[[ 
					make vert STRING for THIS unique combination
					TODO: change 130 to the actual vert colors (if any)
				--]] 

				normX,normY,normZ = Vec3d_normalise(
					obj_Table.vn[obj_Table.f[i][j].vn].x ,
					obj_Table.vn[obj_Table.f[i][j].vn].y ,
					obj_Table.vn[obj_Table.f[i][j].vn].z
				)

				local vertString = string.format(
					"\t{%s, %s, %s, 0, %s, %s, %s, %s, %s, 0},", 
					-- was "\t{%s, %s, %s, 0, %s, %s, 130, 130, 130, 0}, //id: %i", for debugging
					-- the id being vertsCreated

					-- output spacial coordinates
					formatVert( obj_Table.v[obj_Table.f[i][j].v].x , object_scale),
					formatVert( obj_Table.v[obj_Table.f[i][j].v].y , object_scale),
					formatVert( obj_Table.v[obj_Table.f[i][j].v].z , object_scale),
					-- output texture coordinates
					padStringLeft(math.floor(2*object_scale*(bmp.height+1)*obj_Table.vt[obj_Table.f[i][j].vt].v),6),
					padStringLeft(math.floor(2*object_scale*(bmp.width+1)*obj_Table.vt[obj_Table.f[i][j].vt].u),6),
					-- output normals
					formatVert( normX, 128 ),
					formatVert( normY, 128 ),
					formatVert( normZ , 128 )
				)
				-- print(inspect(obj_Table.vn[obj_Table.f[i][j].v]))

				-- put vertex in vertexTable
				-- At the index, eg. "3/6" (vertex 3, tex vertex 6) create a unique vert
				vertexTable[vert_ref_string] = {
					index = vertsCreated,
					content = vertString
				}

				faceVertReference[j] = vertexTable[vert_ref_string].index
				vertsCreated = vertsCreated + 1
				if(vertexTable[vert_ref_string].content ~= nil) then
					table.insert(vertexOutputTable,vertexTable[vert_ref_string].content)
				end
			else
			-- else, just assign the already-existing vertex
				faceVertReference[j] = vertexTable[vert_ref_string].index
			end
		end
		faceTable[i] = {faceVertReference[1],faceVertReference[2],faceVertReference[3]}
	end
	return faceTable, vertexTable, vertexOutputTable
end

function w64_VFSort(faceTable)
	--[[ 
	=================================================
	 SORT FACES AND VERTS INTO PACKS OF 32, BY INDEX
	=================================================

	TABLE STRUCTURES REFERENCE
	==========================
	facesPackRefs {
		1: { 1, 2, 3...30,31,32}
		2: {33,34,35...62,63,64}
		...

	facesInPacks {
		1:
			1: {2,5,3}
			2: {4,0,2}
		2:
			1: ...


	MAIN PACKAGING ALGORITHM
	=========================

	for all verts
		loop through packs until:
			current pack allows for additional vert eg. (0,1,2)
			32 - [length of current pack] - [verts not already in pack] > 0
			then
				insert face into current pack
		endloop
	endfor

	Hopefully this accomodates for verts that are close together in an actual object, otherwise
	it won't be very optimised in terms of memory, but it'll still work.
	--]] 

    -- withholds the actual vert / face STRINGS per pack
	local facesInPacks = {}
	-- small buffer to check which references are in each pack
	local facesPackRefs = {}

	local packerrors = 0
	for i=1, #faceTable do
		local inserted = false
		for j=1,10 do
			local unique_references = {}
			-- if pack doesn't exist yet, create it
			if(type(facesPackRefs[j]) ~= "table") then
				facesInPacks[j] = {}
				facesPackRefs[j] = {}
			end
			-- count up the amount of unique references that already exist in the current pack
			for h=1,3 do
				if(not inTable(facesPackRefs[j], faceTable[i][h]+1)) then
					table.insert(unique_references,faceTable[i][h]+1)
				end
			end
			-- if they can fit in the pack, put it in
			if(32 - #facesPackRefs[j] - #unique_references >= 0) then
				for z=1, #unique_references do
					table.insert(facesPackRefs[j],unique_references[z])
				end
				table.insert(facesInPacks[j],faceTable[i])
				inserted = true
				break;
			end
		end
	end

	return facesInPacks, facesPackRefs
end
	
function w64_outputTriangles(facesInPacks, facesPackRefs, one_tri, objname)
	-- WRITE FACES TO FILE / DISPLAY LIST
	faceOutputTable = {}
	for packNumber=1,#facesInPacks do
		-- for all packs, start by loading the verts
		if(#facesInPacks[packNumber] > 0) then
			table.insert(faceOutputTable,"gsSPVertex(&Vtx_"..objname.."_mesh01_"..(packNumber-1).."[0], "..#facesPackRefs[packNumber]..", 0)")
		end
		local step = 0
		if(one_tri) then step = 1 else step = 2 end
		for k=1,#facesInPacks[packNumber], step do
			-- real strange backwards-referencing thing
			-- draws faces using positions of vertices that have
			-- the associative ID number. Searches for ID, and
			-- returns the POSITION within the array, which is what
			-- we'll put in the gsSP2Triangles function. 
			if(#facesInPacks[packNumber] - k > 0 and not one_tri) then
				table.insert(
					faceOutputTable,
					string.format(
						"gsSP2Triangles(%i,%i,%i,0,%i,%i,%i,0)",
						getLocationOfItem(facesPackRefs[packNumber], facesInPacks[packNumber][k][1]+1)-1,
						getLocationOfItem(facesPackRefs[packNumber], facesInPacks[packNumber][k][2]+1)-1,
						getLocationOfItem(facesPackRefs[packNumber], facesInPacks[packNumber][k][3]+1)-1,
						getLocationOfItem(facesPackRefs[packNumber], facesInPacks[packNumber][k+1][1]+1)-1,
						getLocationOfItem(facesPackRefs[packNumber], facesInPacks[packNumber][k+1][2]+1)-1,
						getLocationOfItem(facesPackRefs[packNumber], facesInPacks[packNumber][k+1][3]+1)-1
					)
				)
			else
				table.insert(
					faceOutputTable,
					string.format(
						"gsSP1Triangle(%i,%i,%i,0)",
						getLocationOfItem(facesPackRefs[packNumber], facesInPacks[packNumber][k][1]+1)-1,
						getLocationOfItem(facesPackRefs[packNumber], facesInPacks[packNumber][k][2]+1)-1,
						getLocationOfItem(facesPackRefs[packNumber], facesInPacks[packNumber][k][3]+1)-1
					)
				)
			end
		end
	end
	-- output faces
	print("Success creating faces and verts!")
	return string.format(
		"Gfx Vtx_%s_mesh01_dl[] = {\n\t%s,\n\tgsSPEndDisplayList(),\n};",
		objname,
		table.concat(faceOutputTable,",\n\t")
	)
end

function w64_outputDisplayList(obj_Name,name_of_texture)
	-- output final display list
	-- This isn't customisable at the moment at all but until I 
	-- actually figure out what'd need changing, I'll leave this!
	return string.format(
		"Gfx Wtx_%s[] = {\n"..
		"\t  gsDPLoadTextureBlock(Text_%s_%s_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, \n"..
		"\t  \t  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), \n"..
		"\t  gsSPDisplayList(Vtx_%s_mesh01_dl),\n"..
		"\t  gsSPEndDisplayList()\n"..
		"};",
		obj_Name,
		obj_Name,
		name_of_texture,
		obj_Name
	)
end

function w64_outputTexture(objname,texturename,previewtable,bmptable)
	-- Output texture metadata
	local metastring = string.format(
		"/*\nName: %s\nTexture preview:%s\n*/",
		objname,
		table.concat(previewtable)
	)
	-- Output texture byte data
	local datastring = ""
	if(objname==0) then
		datastring = string.format(
			"unsigned short Sprite_%s[] = {\n%s};",
			texturename,
			table.concat(bmptable)
		)
	else
		datastring = string.format(
			"unsigned short Text_%s_%s_diff[] = {\n%s};",
			objname,
			texturename,
			table.concat(bmptable)
		)
	end
	return metastring, datastring
end

function w64_outputVerts(facesinpacks,facesinpacksrefs,verttexttable,objname)
	-- output verts in their packs
	local outputTable = {}
	for packNumber=1, #facesinpacks do
		local vertPrintTable = {}
		for i=1,#facesinpacksrefs[packNumber] do
			table.insert(vertPrintTable,verttexttable[facesinpacksrefs[packNumber][i]])
			-- old append (debugging):
			-- ", direct reference: ["..(packNumber-1).."]["..(i-1).."]"
		end
		table.insert(outputTable, string.format(
			"Vtx_tn Vtx_%s_mesh01_%i[%i] = {\n%s\n};",
			objname,
			packNumber-1,
			#facesinpacksrefs[packNumber],
			table.concat(vertPrintTable,"\n")
		))
	end
	return table.concat(outputTable)
end

--  ˚   　　　*   　  ·  ✧ 
-- 　　  ·  .　 　　 ˚ 
-- .  ·  .  .   ⊹ 　  ·
w64_main()
--     *  .  ✺ 　  　     ✹   ✫ 
--  · .　　.     　  　 
--    . ✵