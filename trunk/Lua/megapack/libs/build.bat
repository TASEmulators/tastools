pushd dist
touch lua.tag
del ..\lua-megapack-dist.7z
..\7z a -mx9 -ssw ..\lua-megapack-dist.7z *
popd