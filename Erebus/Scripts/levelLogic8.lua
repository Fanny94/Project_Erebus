
function LoadLogic8()
	
end
firstLoad = true
countDownToWin = 10
function UpdateLogic(dt)
	if rewinder.rewinding then
		rewinder:Update(dt)
		boss.animationController:AnimationUpdate(dt, Network)
	else
		BossStuff(dt)
	end
	if boss.combatStarted then
		--if firstLoad then 
			Transform.SetPosition(levels[8].props.WoodenFenceID.transformID, {x=289.09, y=126.67, z=479.67})
			levels[8].props.WoodenFenceID.collider:SetPos(289.09, 126.67, 479.67)
			Transform.ActiveControl(levels[8].props.WoodenFenceID.transformID, true)
			Transform.SetPosition(levels[8].props.WoodenFence1ID.transformID, {x=355.01, y=126.86, z=479.48})
			levels[8].props.WoodenFence1ID.collider:SetPos(355.01, 126.86, 479.48)
			Transform.ActiveControl(levels[8].props.WoodenFence1ID.transformID, true)
			firstLoad = false
		--end
	end
	
	if LEVEL_ROUND > 3 then
		countDownToWin = countDownToWin - dt
		if countDownToWin < 0 then
			gamestate.ChangeState(GAMESTATE_CREDITS)
			countDownToWin = 10
		end
	end
end

function BossStuff(dt)
	if not boss.loaded then LoadBoss() end

	if not boss.realDead then
		if not boss.combatStarted then
			StartingBoss()
		end
	end
	UpdateBoss(dt)
end

function ResetLevel8()
	firstLoad = true
	Transform.ActiveControl(levels[8].props.WoodenFence1ID.transformID, false)
	Transform.ActiveControl(levels[8].props.WoodenFenceID.transformID, false)
end

function StartingBoss()
	local player1BossDistance = Transform.GetDistanceBetweenTrans(player.transformID, boss.transformID)
	local player2BossDistance = Transform.GetDistanceBetweenTrans(player2.transformID, boss.transformID)
	local player2Location = Transform.GetPosition(player2.transformID)
	if player2Location.x == 0 and player2Location.y == 0 and player2Location.z == 0 then
		IS_SINGLEPLAYER = true
	end
	
	if (player1BossDistance <= 39 and player2BossDistance <= 39) or (player1BossDistance <= 39 and IS_SINGLEPLAYER) then 
		boss:Spawn()
		PlayBGM("BOSS.ogg")
		--Sound.SetVolume(BACKGROUND_MUSIC_ID, 0)
	end
end

function UnloadLogic8()
	firstLoad = true
	boss.loaded = false
end

return { Load = LoadLogic8, Unload = UnloadLogic8, Update = UpdateLogic }
