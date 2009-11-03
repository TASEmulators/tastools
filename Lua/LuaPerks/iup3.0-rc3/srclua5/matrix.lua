------------------------------------------------------------------------------
-- Matrix class 
------------------------------------------------------------------------------
local ctrl = {
  nick = "matrix",
  parent = WIDGET,
  creation = "-",
  callback = {
    action_cb = "nnnns",
    click_cb = "nns",
    release_cb = "nns",
    drop_cb = "inn",
    dropcheck_cb = "nn",
    draw_cb = "nnnnnnn",  -- fake definitions to be replaced by matrixfuncs module
    dropselect_cb = "nnisnn",
    edition_cb = "nnnn",
    enteritem_cb = "nn",
    leaveitem_cb = "nn",
    mousemove_cb = "nn",
    scrolltop_cb = "nn",
    fgcolor_cb = "nn",  -- fake definitions to be replaced by matrixfuncs module
    bgcolor_cb = "nn",
    font_cb = {"nn", ret = "s"}, -- ret is return type
    value_cb = {"nn", ret = "s"}, -- ret is return type
    value_edit_cb = "nns",
    mark_cb = "nn",
    markedit_cb = "nnn",
  },
  include = "iupcontrols.h",
  extrafuncs = 1,
}

function ctrl.createElement(class, arg)
   return Matrix(arg.action)
end

function ctrl.setcell(handle, l, c, val)
  SetAttribute(handle,l..":"..c,val)
end

function ctrl.getcell(handle, l, c)
  return GetAttribute(handle,l..":"..c)
end

iupRegisterWidget(ctrl)
iupSetClass(ctrl, "iup widget")
