local screenImages = {}
local imageTextures = {}

function LoadPauseMenuUI()
	imageTextures["background"] = Assets.LoadTexture("Textures/menuBackground.png");
	imageTextures["return"] = Assets.LoadTexture("Textures/buttonReturn.png");
	imageTextures["options"] = Assets.LoadTexture("Textures/buttonOptions.png");
	imageTextures["exit"] = Assets.LoadTexture("Textures/buttonExit.png");

	screenImages["background"] = UI.load(0, 0, 1280, 720);
	screenImages["return"] = UI.load(465, 240, 350, 60);
	screenImages["options"] = UI.load(465, 310, 350, 60);
	screenImages["exit"] = UI.load(465, 380, 350, 60);

end

function UnloadPauseMenuUI()

end

function UpdatePauseMenuUI(dt)

	DrawPauseMenuUI()
	if Inputs.ButtonReleased(Buttons.Left) then
		x,y = Inputs.GetMousePos()
		if UI.mousePick(screenImages["return"], x,y) then
			gamestate.ChangeState(GAMESTATE_GAMEPLAY)
		end

		if UI.mousePick(screenImages["options"], x,y) then
			OPTIONS_RETURN_STATE = GAMESTATE_PAUSEMENU
			gamestate.ChangeState(GAMESTATE_OPTIONS)
		end

		if UI.mousePick(screenImages["exit"], x,y) then
			gamestate.ChangeState(GAMESTATE_MAIN_MENU)
			Erebus.ShutdownNetwork()
		end
	end
end

function DrawPauseMenuUI()
	UI.drawImage(screenImages["background"], imageTextures["background"]);
	UI.drawImage(screenImages["return"], imageTextures["return"]);
	UI.drawImage(screenImages["options"], imageTextures["options"]);
	UI.drawImage(screenImages["exit"], imageTextures["exit"]);
end

return { Load = LoadPauseMenuUI, Unload = UnloadPauseMenuUI, Update = UpdatePauseMenuUI }