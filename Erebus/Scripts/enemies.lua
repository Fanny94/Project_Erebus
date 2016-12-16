MAX_ENEMIES = 10
enemies = {}

function LoadEnemies()
	for i=1, MAX_ENEMIES do
		enemies[i] = {}
		enemies[i].transformID = Transform.Bind()
		enemies[i].movementSpeed = math.random(5,20)
		enemies[i].health = 20

		enemies[i].Hurt = function(self,damage)
			self.health = self.health - damage
			if self.health <= 0 then
				Transform.ActiveControl(self.transformID,false)
			end
		end

		Transform.SetPosition(enemies[i].transformID, {x = math.random(10, 255), y = math.random(15, 30), z = math.random(10, 245)})
		enemies[i].sphereCollider = SphereCollider.Create(enemies[i].transformID)
		enemies[i].sphereCollider:SetRadius(2)
		CollisionHandler.AddSphere(enemies[i].sphereCollider)
	end

	local model = Assets.LoadModel("Models/Robot.model")
	for i=1, MAX_ENEMIES do
		Gear.AddStaticInstance(model, enemies[i].transformID)
	end
end

function UnloadEnemies()
end

function UpdateEnemies(dt)
	for i=1, MAX_ENEMIES do
		if enemies[i].health > 0 then
			Transform.Follow(player.transformID, enemies[i].transformID, enemies[i].movementSpeed, dt)
			local pos = Transform.GetPosition(enemies[i].transformID)
			pos.y = heightmap:GetHeight(pos.x,pos.z)+1
			Transform.SetPosition(enemies[i].transformID, pos)
		end
		Transform.UpdateRotationFromLookVector(enemies[i].transformID);
	end
end

return { Load = LoadEnemies, Unload = UnloadEnemies, Update = UpdateEnemies }