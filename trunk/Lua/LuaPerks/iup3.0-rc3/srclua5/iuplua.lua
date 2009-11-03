-- This file is executed with the "iup" table already as the globalindex

------------------------------------------------------------------------------
-- Callback handler  
------------------------------------------------------------------------------

callbacks = {}

function iupCallMethod(name, ...)
  local handle = arg[1] -- always the handle
  local func = handle[name]
  if (not func) then
    return
  end
  
  if type(func) == "function" then
    return func(unpack(arg))
  elseif type(func) == "string" then  
    local temp = self
    self = handle
    local result = iup.dostring(func)
    self = temp
    return result
  else
    return iup.ERROR
  end
end

function RegisterCallback(name, func, type)
  if not callbacks[name] then callbacks[name] = {} end
  local cb = callbacks[name]
  if type then
    cb[type] = func
  else
    cb[1] = func
  end
end

------------------------------------------------------------------------------
-- Meta Methods 
------------------------------------------------------------------------------


local widget_gettable = function(object, index)
  local p = object
  local v
  while 1 do
    v = rawget(p, index)
    if v then return v end
    p = rawget(p, "parent")
      if not p then return nil end
  end
end

iupNewClass("iup widget")
iupSetMethod("iup widget", "__index", widget_gettable)


local ihandle_gettable = function(handle, index)
  local INDEX = string.upper(index)
  if (callbacks[INDEX]) then 
   local object = iupGetWidget(handle)
   if (not object or type(object)~="table") then error("invalid iup handle") end
   return object[index]
  else
    local value = GetAttribute(handle, INDEX)
    if (not value) then
      local object = iupGetWidget(handle)
      if (not object or type(object)~="table") then error("invalid iup handle") end
      return object[index]
    elseif type(value)== "number" or type(value) == "string"  then
      local ih = GetHandle(value)
      if ih then return ih
      else return value end
    else
      return value 
    end
  end
end

local ihandle_settable = function(handle, index, value)
  local ti = type(index)
  local tv = type(value)
  local object = iupGetWidget(handle)
  if (not object or type(object)~="table") then error("invalid iup handle") end
  if ti == "number" or ti == "string" then -- check if a valid C name
    local INDEX = string.upper(index)
    local cb = callbacks[INDEX]
    if (cb) then -- if a callback name
      local func = cb[1]
      if (not func) then
        func = cb[GetClassName(handle)]
      end
      iupSetCallback(handle, INDEX, func, value) -- register the pre-defined C callback
      object[index] = value -- store also in Lua
    elseif iupGetClass(value) == "iup handle" then -- if a iup handle
      local name = ihandle_setname(value)
      SetAttribute(handle, INDEX, name)
      object[index] = nil -- if there was something in Lua remove it
    elseif tv == "string" or tv == "number" or tv == "nil" then -- if a common value
      SetAttribute(handle, INDEX, value)
      object[index] = nil -- if there was something in Lua remove it
    else
      object[index] = value -- store also in Lua
    end
  else
    object[index] = value -- store also in Lua
  end
end

iupNewClass("iup handle")
iupSetMethod("iup handle", "__index", ihandle_gettable)
iupSetMethod("iup handle", "__newindex", ihandle_settable)
iupSetMethod("iup handle", "__tostring", ihandle_tostring)
iupSetMethod("iup handle", "__eq", ihandle_compare) -- implemented in C


------------------------------------------------------------------------------
-- Utilities 
------------------------------------------------------------------------------

function ihandle_setname(v)  -- used also by radio and zbox
  local name = GetName(v)
  if not name then
    local autoname = string.format("_IUPLUA_NAME(%s)", tostring(v))
    SetHandle(autoname, v)
    return autoname
  end
  return name
end

function iupRegisterWidget(ctrl) -- called by all the controls initialization functions
  iup[ctrl.nick] = function(arg)
    return ctrl:constructor(arg)
  end
end

function RegisterHandle(handle, typename)

  iupSetClass(handle, "iup handle")
  
  local object = iupGetWidget(handle)
  if not object then

    local class = iup[string.upper(typename)]
    if not class then
      class = WIDGET
    end

    local object = { parent=class, handle=handle }
    iupSetClass(object, "iup widget")
    iupSetWidget(handle, object)
  end
  
  return handle
end

------------------------------------------------------------------------------
-- Widget class (top class) 
------------------------------------------------------------------------------

WIDGET = {
  callback = {}
}

function WIDGET.show(object)
  Show(object.handle)
end

function WIDGET.hide(object)
  Hide(object.handle)
end

function WIDGET.map(object)
  Map(object.handle)
end

function WIDGET.constructor(class, arg)
  local handle = class:createElement(arg)
  local object = { 
    parent = class,
    handle = handle
  }
  iupSetClass(handle, "iup handle")
  iupSetClass(object, "iup widget")
  iupSetWidget(handle, object)
  object:setAttributes(arg)
  return handle
end

function WIDGET.setAttributes(object, arg)
  local handle = object.handle
  for i,v in pairs(arg) do 
    if type(i) == "number" and iupGetClass(v) == "iup handle" then
      -- We should not set this or other elements (such as iuptext)
      -- will erroneosly inherit it
      rawset(object, i, v)
    else
      -- this will call settable metamethod
      handle[i] = v
    end
  end
end

-- all the objects in the hierarchy must be "iup widget"
-- Must repeat this call for every new widget
iupSetClass(WIDGET, "iup widget")


------------------------------------------------------------------------------
-- Box class (inherits from WIDGET) 
------------------------------------------------------------------------------

BOX = {
  parent = WIDGET
}

function BOX.setAttributes(object, arg)
  local handle = rawget(object, "handle")
  local n = #arg
  for i = 1, n do
    if iupGetClass(arg[i]) == "iup handle" then 
      Append(handle, arg[i]) 
    end
  end
  WIDGET.setAttributes(object, arg)
end

iupSetClass(BOX, "iup widget")


------------------------------------------------------------------------------
-- Compatibility functions.
------------------------------------------------------------------------------

error_message_popup = nil

function _ERRORMESSAGE(err,traceback)
  err = err..(traceback or "")
  if (error_message_popup) then
    error_message_popup.value = err
  else  
    local bt = button{title="Ok", size="60", action="error_message_popup = nil; return iup.CLOSE"}
    local ml = multiline{expand="YES", readonly="YES", value=err, size="300x150"}
    local vb = vbox{ml, bt; alignment="ACENTER", margin="10x10", gap="10"}
    local dg = dialog{vb; title="Error Message",defaultesc=bt,defaultenter=bt,startfocus=bt}
    error_message_popup = ml
    dg:popup(CENTER, CENTER)
    dg:destroy()
    error_message_popup = nil
  end
end

pack = function (...) return arg end

function protectedcall_(f, err)
  if not f then 
    _ERRORMESSAGE(err)
    return 
  end
  local ret = pack(pcall(f))
  if not ret[1] then 
    _ERRORMESSAGE(ret[2])
    return
  else  
    table.remove(ret, 1)
    return unpack(ret)
  end
end

function dostring(s) return protectedcall_(loadstring(s)) end
function dofile(f) return protectedcall_(loadfile(f)) end

function RGB(r, g, b)
  return string.format("%d %d %d", 255*r, 255*g, 255*b)
end

-- This will allow both names to be used in the same application
-- also will allow static linking to work with require
if _G.package then
   _G.package.loaded["iuplua"] = iup
   _G.package.loaded["iuplua51"] = iup
   iup._M = iup
   iup._PACKAGE = "iuplua"
end
