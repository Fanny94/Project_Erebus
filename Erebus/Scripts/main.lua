local scripts = {}
local scriptFiles = 
{
	"Scripts/player.lua",
	"Scripts/enemies.lua",
	"Scripts/camera.lua",
	"Scripts/particle.lua",
	"Scripts/projectile.lua",
	"Scripts/arc.lua"
}
local drawHitboxes = false

function Load()
	heightmap = Assets.LoadHeightmap("Textures/scale1c.png")
	--heightmap = Assets.LoadHeightmap("Textures/sclae2.png")
	Gear.AddStaticInstance(heightmap:GetModel(), heightmap.transformID)
	local model = Assets.LoadModel("Models/niclasland_scale1.model")
	--local model = Assets.LoadModel("Models/niclasland_sclae2.model")
	-- run scripts
	for i=1, #scriptFiles do
		scripts[i] = dofile(scriptFiles[i])
	end

	-- call their load function
	for key,value in pairs(scripts) do
		if value.Load then value.Load() end
	end

	dofile( "Scripts/level.lua" )
end

function Unload()
	for key,value in pairs(scripts) do
		if value.Unload then
			value.Unload()
		end
	end
end

function Update(dt)
	for key,value in pairs(scripts) do
		value.Update(dt)
	end

	--if Controls[Keys.FourPressed] then drawHitboxes = not drawHitboxes end
	--if drawHitboxes then CollisionHandler.DrawHitboxes() end
end