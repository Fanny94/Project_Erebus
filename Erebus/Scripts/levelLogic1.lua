TUTORIAL_DONE = false
TUTORIAL_START_ANIM = false
TUTORIAL_COUNTER = 0 
TUTORIAL_OBJECT = {}

function LoadLogic()

end

function UpdateLogic(dt)
	--Sluice()
	if TUTORIAL_START_ANIM == true then
		lowerObject(TUTORIAL_OBJECT,3,dt)
	end 
end

function LoadLogic()

end





function lowerObject(object,time,dt)
	local pos = Transform.GetPosition(object.transformID)
	pos.y = pos.y - (dt * 5)
	TUTORIAL_COUNTER = TUTORIAL_COUNTER + dt
	Transform.SetPosition(object.transformID,pos)
	if TUTORIAL_COUNTER > 3 then
		TUTORIAL_START_ANIM=false
	end
end


function TutorialBarrier(TutorialObject,dt)
	if TUTORIAL_DONE == false then 

		local pos = Transform.GetPosition(TutorialObject.transformID)
		showTutorialImage(pos.x+2,pos.y+7,pos.z+15,dt)

		if player.combinedSpellIDs ~= nil then
			local colID = TutorialObject.collider:GetID()
			local collisionIDs = TutorialObject.collider:GetCollisionIDs()

			for i = 1, #collisionIDs do 
				for o = 1, #player.combinedSpellIDs do
					if collisionIDs[i] == player.combinedSpellIDs[o] then
						
						OBBCollider.SetActive(TutorialObject.collider,false)
						player.combinedSpellIDs = nil
						TUTORIAL_DONE = true
						TUTORIAL_START_ANIM = true
						TUTORIAL_OBJECT = TutorialObject
						return
					end
				end
			end
		end
	end
end
--TUTORIAL_DONE = true
return { Load = LoadLogic, Unload = UnloadLogic, Update = UpdateLogic }