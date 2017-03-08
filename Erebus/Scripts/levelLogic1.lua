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
	pos.y = pos.y - (dt * 8)
	TUTORIAL_COUNTER = TUTORIAL_COUNTER + dt
	Transform.SetPosition(object.transformID,pos)
	if TUTORIAL_COUNTER > 0.85 then
		OBBCollider.SetActive(object.collider,false)
		TUTORIAL_START_ANIM=false
	end
end


function TutorialBarrier(TutorialObject,dt)
	
	--this is triggered upon both players
	if TUTORIAL_DONE == false then
	pos = Transform.GetPosition(TutorialObject.transformID,pos) 
	showCombineBarrierImage(dt,pos.x,pos.y,pos.z)
		if player2.position.x==0 then
			
			TUTORIAL_DONE = true
			TUTORIAL_START_ANIM = true
			TUTORIAL_OBJECT = TutorialObject
			hideCombinationImage()
		end

		local colID = TutorialObject.collider:GetID()
		local collisionIDs = TutorialObject.collider:GetCollisionIDs()

		for i = 1, #collisionIDs do 
			for curID = 1, 3 do
				if player.spells[curID]:GetCollider()[1] == collisionIDs[i] then				
					if #player.spells[curID].effects == 2 then 
						
						TUTORIAL_DONE = true
						TUTORIAL_START_ANIM = true
						TUTORIAL_OBJECT = TutorialObject

						hideCombinationImage()
					end
				end
				if player2.spells[curID]:GetCollider()[1] == collisionIDs[i] then
					if #player2.spells[curID].effects == 2 then 
						
						TUTORIAL_DONE = true
						TUTORIAL_START_ANIM = true
						TUTORIAL_OBJECT = TutorialObject
						hideCombinationImage()
					end
				end
			end
		end
	end
end
--TUTORIAL_DONE = true
return { Load = LoadLogic, Unload = UnloadLogic, Update = UpdateLogic }