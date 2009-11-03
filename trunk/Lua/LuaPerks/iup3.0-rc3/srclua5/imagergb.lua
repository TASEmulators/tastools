------------------------------------------------------------------------------
-- ImageRGB class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "imagergb",
  parent = WIDGET,
  creation = "nns", -- fake definition
  funcname = "ImageRGB", 
  callback = {},
  createfunc = [[ 
static int ImageRGB(lua_State *L)
{
  int w = luaL_checkint(L, 1);
  int h = luaL_checkint(L, 2);
  unsigned char *pixels = iuplua_checkuchar_array(L, 3, w*h*3);
  Ihandle *ih = IupImageRGB(w, h, pixels);
  iuplua_plugstate(L, ih);
  iuplua_pushihandle_raw(L, ih);
  free(pixels);
  return 1;
}
 
]]
}

function ctrl.createElement(class, arg)
   return ImageRGB(arg.width, arg.height, arg.pixels)
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
