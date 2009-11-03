------------------------------------------------------------------------------
-- Tree class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "tree",
  parent = WIDGET,
  creation = "",
  callback = {
    selection_cb = "nn",
    multiselection_cb = "nn",  -- fake definition to be replaced by treefuncs module
    branchopen_cb = "n",
    branchclose_cb = "n",
    executeleaf_cb = "n",
    renamenode_cb = "ns",
    noderemoved_cb = "ns",  -- fake definition to be replaced by treefuncs module
    rename_cb = "ns",
    showrename_cb = "n",
    rightclick_cb = "n",
    dragdrop_cb = "nnnn",
  },
  extrafuncs = 1,
}

local function TreeSetAttributeHandle(handle, name, value)
   if iupGetClass(value) == "iup handle" then value = ihandle_setname(value) end
   SetAttribute(handle, name, value)
end

function TreeSetNodeAttrib(handle, node, id)
  if node.color then SetAttribute(handle, "COLOR"..id, node.color) end
  if node.state then SetAttribute(handle, "STATE"..id, node.state) end
  if node.titlefont then SetAttribute(handle, "TITLEFONT"..id, node.titlefont) end
  if node.marked then SetAttribute(handle, "MARKED"..id, node.marked) end
  if node.image then TreeSetAttributeHandle(handle, "IMAGE"..id, node.image) end
  if node.imageexpanded then TreeSetAttributeHandle(handle, "IMAGEEXPANDED"..id, node.imageexpanded) end
  if node.userid then TreeSetUserId(handle, id, node.userid) end
end

function TreeSetValueRec(handle, t, id)
  if t == nil then return end
  local cont = #t
  while cont >= 0 do
    local node = t[cont]
    if type(node) == "table" then
      if node.branchname then
        SetAttribute(handle, "ADDBRANCH"..id, node.branchname)
        TreeSetNodeAttrib(handle, node, id+1)
        TreeSetValueRec(handle, node, id+1)
      elseif node.leafname then
        SetAttribute(handle, "ADDLEAF"..id, node.leafname)
        TreeSetNodeAttrib(handle, node, id+1)
      end
    else
      if node then
        SetAttribute(handle, "ADDLEAF"..id, node)
      end
    end
    cont = cont - 1
   end
end

function TreeSetValue(handle, t, id)
  if (not id) then
    id = 0  -- default is the root
    if t.branchname then SetAttribute(handle, "TITLE0", t.branchname) end
    TreeSetNodeAttrib(handle, t, 0)
  end
  TreeSetValueRec(handle, t, id)
end

function ctrl.createElement(class, arg)
  return Tree()
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
