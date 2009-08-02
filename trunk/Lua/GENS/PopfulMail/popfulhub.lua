
-- this script is just a minimal "base of operations" to start the others from

--[[

-- this is the movie I'm using
moviename = "Popful MAIL.gmv"
-- open it if a movie isn't already playing
if not movie.active() then
	movie.play(moviename)
end

]]


-- start up the other scripts
gens.openscript("popfulmacro.lua")
gens.openscript("popfulinfo.lua")
gens.openscript("popfulghost.lua")

