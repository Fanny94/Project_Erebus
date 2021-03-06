levelScripts = {}
local scripts = {}
LEVEL_ROUND = 1
local scriptFiles =
{
	"Scripts/camera.lua",
	"Scripts/particle.lua",
	"Scripts/ProjectileType.lua",
	"Scripts/fireEffect.lua",
	"Scripts/timeSlowEffect.lua",
	"Scripts/chronoBall.lua",
	"Scripts/timeOrbWave.lua",
	"Scripts/orbWaveType.lua",
	"Scripts/sunRay.lua",
	"Scripts/Animation_Controllers/playerController.lua",
	"Scripts/Animation_Controllers/meleeGoblinController.lua",
	"Scripts/Animation_Controllers/bossController.lua",
	"Scripts/slowEffect.lua",
	"Scripts/iceGrenade.lua",
	"Scripts/grenadeType.lua",
	"Scripts/hellPillar.lua",
	"Scripts/rayType.lua",
	"Scripts/staticAoEType.lua",
	"Scripts/player.lua",
	"Scripts/player2.lua",
	"Scripts/enemies.lua",
	"Scripts/spellList.lua",
	"Scripts/HUD.lua",
	"Scripts/spellUtility.lua",
	"Scripts/boss.lua",
	"Scripts/fireball.lua",
	"Scripts/lifeStealEffect.lua",
	"Scripts/siphon.lua",
	"Scripts/polymorphEffect.lua",
	"Scripts/polymorph.lua",
	"Scripts/tumbleThorns.lua",
	"Scripts/windknockback.lua",
	"Scripts/knockbackEffect.lua",
	"Scripts/revive.lua",
	"Scripts/TimeLaser.lua",
	"Scripts/healthOrb.lua",
	"Scripts/reusable.lua",
	"Scripts/sluice.lua",
	"Scripts/rewinder.lua",
	"Scripts/blackHole.lua"
}

loadedLevels = {}

gameplayStarted = false
loadedGameplay = false

function LoadGameplay()
	--print("LOADING GAMEPLAY")
	-- run scripts
	for i=1, #scriptFiles do
		scripts[i] = dofile(scriptFiles[i])
	end
	for i = 1, 8 do
		levelScripts[i] = dofile("Scripts/levelLogic"..i..".lua")
	end
end

function CreateIM()
	for i = 1, 8 do
		levels[i].load()
		loadedLevels[i] = true
	end

	AI.CreateIM()--,#heightmaps,widthTest,heightTest)

	for i = 1, 8 do
		levels[i].unload()
		loadedLevels[i] = false
	end
	loadedLevels = {}
end

function UnloadGameplay()
	--print("unloading gameplay")
	if loadedGameplay then		
		LEVEL_ROUND = 0

		BOSS_DEAD = false

		Rewind()

		for i = 1, #levelScripts do
			levelScripts[i].Unload()
		end

		-- unload all the loaded levels
		for levelIndex,level in pairs(levels) do
			if loadedLevels[levelIndex] then
				level.unload()
			end
		end

		-- unload all the scripts
		for key,value in pairs(scripts) do
			if value.Unload then
				value.Unload()
			end
		end

		UnInitPolymorphs()
		UnInitTimeSlows()
		UnInitFireEffectParticles()

		loadedGameplay = false
		gameplayStarted = false
		loadedLevels = {}

		Transform.ResetTransforms()
		Gear.ResetAnimations()
		CollisionHandler.Reset()

		openSluices = {}

		enemies = {}

		collectgarbage()
	end
end

function UpdateGameplay(dt)
	if Inputs.KeyReleased(SETTING_KEYBIND_MENU) then
		--print(SETTING_KEYBIND_MENU)
		gamestate.ChangeState(GAMESTATE_PAUSEMENU)
	end

	if not player.isAlive and not player2.isAlive then
		gamestate.ChangeState(GAMESTATE_DEATH)
	end

	for key,value in pairs(scripts) do
		value.Update(dt)
	end
	
	levelScripts[player.levelIndex].Update(dt)

	--if SETTING_DEBUG then 
	--	CollisionHandler.DrawHitboxes()
	--end
	
	
	local newEndEventValue, endEventId = Network.GetEndEventPacket()
	if newEndEventValue == true then
		if endEventId == 0 then -- other player died
			gamestate.ChangeState(GAMESTATE_DEATH)
		elseif endEventId == 1 then -- other player quit to main menu
			--gamestate.ChangeState(GAMESTATE_MAIN_MENU) 
			gamestate.ChangeState(GAMESTATE_DISCONNECTED)
			UnloadGameplay()
			Erebus.ShutdownNetwork()
		elseif endEventId == 2 then -- player win!
			boss.health = 0
			BOSS_DEAD = true
			gamestate.ChangeState(GAMESTATE_DEATH)
		end
	end
end

function EnterGameplay()
	--print("entering gameplay")
	if loadedGameplay == false then 
		InitPolymorphs()
		InitTimeSlows()
		InitFireEffectParticles()

		for key,value in pairs(scripts) do
			if value.Load then value.Load() end
		end

		dofile( "Scripts/Max_Oscar_test.lua" )

		CreateIM()

		levels[1].load()
		loadedLevels[1] = true
		for _,v in pairs(levels[1].surrounding) do
			levels[v].load()
			loadedLevels[v] = true
		end
		--levels[1].load()
		
		loadedGameplay = true

		------------------
		local tempTable = TILE_ATMOSPHERE_TABLE[player.levelIndex]-- = {AMBIENCECOLOR = {r = 0,g=0,b=0}, FOGCOLOR ={r=0,g=0,b=0},SKYBOX = 0}

		Sky.SetAmbient(tempTable.AMBIENCECOLOR.r + OVEREALAMBIENCE.r,tempTable.AMBIENCECOLOR.g+ OVEREALAMBIENCE.g,tempTable.AMBIENCECOLOR.b+ OVEREALAMBIENCE.b, true)
		Sky.SetFogColor(tempTable.FOGCOLOR.r+ OVEREALAMBIENCE.r,tempTable.FOGCOLOR.g+ OVEREALAMBIENCE.g,tempTable.FOGCOLOR.b+ OVEREALAMBIENCE.b)
		Sky.SetBlend(tempTable.SKYBOX, true)
		Sky.SetSunColor(tempTable.SUNCOLOR.r,tempTable.SUNCOLOR.g,tempTable.SUNCOLOR.b)
		Sky.SetSunAngle(tempTable.SUNANGLE,true)

		----------------------

		LEVEL_ROUND = 1
	end

	Gear.QueueModels(true)
	CollisionHandler.Enable()
	Gear.CursorVisible(false)
	Erebus.EnableControls(true)
	player.controlsEnabled = true
	gameplayStarted = true
end

function ExitGameplay()
	player.controlsEnabled = false
end

return { Load = LoadGameplay, Unload = UnloadGameplay, Update = UpdateGameplay, Enter = EnterGameplay, Exit = ExitGameplay }