local scriptsMenu = {}
local menuBGM = "GOOD.ogg"
local bgmID = -1
local scriptFilesMenu =
{
 "Scripts/MenuUI.lua"
}

function LoadMainMenu()
	-- run scripts
	for i=1, #scriptFilesMenu do
		scriptsMenu[i] = dofile(scriptFilesMenu[i])
	end

	-- call their load function
	for key,value in pairs(scriptsMenu) do
		if value.Load then value.Load() end
	end
end

function UnloadMainMenu()
end

function UpdateMainMenu(dt)
	for key,value in pairs(scriptsMenu) do
		value.Update(dt)
	end
end

function EnterMainMenu()
	Gear.QueueModels(false)
	CollisionHandler.Disable()
	Gear.CursorVisible(true)
	PlayBGM(menuBGM,0.8)
end

function ExitMainMenu()
	
end

return { Load = LoadMainMenu, Unload = UnloadMainMenu, Update = UpdateMainMenu, Enter = EnterMainMenu, Exit = ExitMainMenu }