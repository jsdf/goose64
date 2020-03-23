-- UTILITIES
function err(str)
	if(str) then print("ERROR: "..str) end
	os.exit(1)
end

function intToBin(x)
	ret=""
	while x~=1 and x~=0 do
		ret=tostring(x%2)..ret
		x=math.modf(x/2)
	end
	ret=tostring(x)..ret
	return ret
end

function readFile(path)
    local file = io.open(path, "rb") -- r read mode and b binary mode
    if not file then return nil end
    local content = file:read "*a" -- *a or *all reads the whole file
    file:close()
    return content
end

function padBinaryLeft(s,l)
	local outstring = s
	for _= #s + 1,l do
		outstring = "0"..outstring
	end
	return outstring
end

function padStringLeft(s,l)
	local outstring = "                          "
	outstring = string.sub(outstring,0,l-#tostring(s))..s
	return outstring
end

function inTable(tab, val)
    for index, value in ipairs(tab) do
        if value == val then
            return true
        end
    end
    return false
end

function appendToOutput(stringValue)
	table.insert(final_file_output, stringValue)
end

function printf(s,...)
   return print(s:format(...))
end

function htmlOutput(s)
	io.output(htmlFile)
	io.write(s)
end

function initHtml()
	htmlFile = io.open("table_output.html","w")
	io.output(htmlFile)
	io.write("")
	io.close(htmlFile)
	htmlFile = io.open("table_output.html","a")
end

function htmlClose()
	io.close(htmlFile)
end

function tprint (tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
    formatting = string.rep("  ", indent) .. k .. ": "
    if type(v) == "table" then
      print(formatting)
      tprint(v, indent+1)
    elseif type(v) == 'boolean' then
      print(formatting .. tostring(v))      
    else
      print(formatting .. v)
    end
  end
end

function boolToInt(bool)
	if(bool) then
		return 1
	else
		return 0
	end
end

function getLocationOfItem(haystack,needle)
	for i=1,#haystack do
		if(haystack[i] == needle) then
			return i
		end
	end
	return false
end

function formatVert(floatString, object_scale)
	return padStringLeft(math.floor(object_scale*floatString),5)
end


function tableLength(tbl)
  local count = 0
  for _ in pairs(tbl) do count = count + 1 end

  return count
end
