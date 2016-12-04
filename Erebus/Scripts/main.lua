local scripts = {}
local scriptFiles = 
{
	"Scripts/player.lua",
	"Scripts/enemies.lua",
	"Scripts/bullets.lua"
}

function Load()
	-- run scripts
	for i=1, #scriptFiles do
		scripts[i] = dofile(scriptFiles[i])
	end

	-- call their load function
	for key,value in pairs(scripts) do
		value.Load()
	end
end

function Unload()
	for key,value in pairs(scripts) do
		value.Unload()
	end
end

function Update(dt)
	for key,value in pairs(scripts) do
		value.Update(dt)
	end
end