GRENADE_EXPLODE_TIME =3

function CreateGrenadeType(model)
	local type = {}
	--type.transformID = Transform.Bind()
	type.transformID = Gear.BindStaticInstance(model)
	
	type.sphereCollider = SphereCollider.Create(type.transformID)
	CollisionHandler.AddSphere(type.sphereCollider, 2)
	SphereCollider.SetActive(type.sphereCollider, false)
	type.sphereCollider:SetRadius(0.3)
	Transform.ActiveControl(type.transformID, false)
	
	type.direction = {x=0,y=0,z=0}
	type.positon = {x=0,y=0,z=0}
	type.falloffFactor = 0
	type.speed = 0
	type.hitflag = false
	type.radius = 0
	type.explodetime = 0
	type.firstflag = false

	function type:Cast(position, direction, falloff, speed, explosionRadius)
		self.direction = direction
		self.position = position
		self.falloffFactor = falloff
		self.speed = speed
		Transform.ActiveControl(self.transformID, true)
		SphereCollider.SetActive(self.sphereCollider, true)
		self.radius = explosionRadius
		self.explodetime = 0
		self.firstflag = true
	end
	function type:flyUpdate(dt)
		result = false
		self.direction.y = self.direction.y - self.falloffFactor * dt
		self.position.x = self.position.x + self.direction.x*self.speed*dt
		self.position.y = self.position.y + self.direction.y*self.speed*dt
		self.position.z = self.position.z + self.direction.z*self.speed*dt
		Transform.SetPosition(self.transformID, self.position)

		--[[local posx = math.floor(self.position.x/512)
		local posz = math.floor(self.position.z/512)
		local heightmapIndex = (posz*2 + posx)+1
		if heightmapIndex < 1 then heightmapIndex = 1 end
		if heightmapIndex > 4 then heightmapIndex = 4 end
		if heightmaps[heightmapIndex].asset:GetHeight(self.position.x, self.position.z) > self.position.y then
			result = true
		else
			local collisionIDs = self.sphereCollider:GetCollisionIDs()
			for curID = 1, #collisionIDs do
				for curEnemy=1, #enemies do
					if collisionIDs[curID] == enemies[curEnemy].sphereCollider:GetID() then
						result = true
					end
				end
			end
		end--]]

		local hm = GetHeightmap(self.position)
		if not hm or hm.asset:GetHeight(self.position.x, self.position.z) > self.position.y then
			result = true
		else
			local collisionIDs = self.sphereCollider:GetCollisionIDs()
			for curID = 1, #collisionIDs do
				for curEnemy=1, #enemies do
					if collisionIDs[curID] == enemies[curEnemy].collider:GetID() then
						result = true
					end
				end
				if boss.alive and collisionIDs[curID] == boss.collider:GetID() then
					result = true
				end
			end
		end

		if self.firstflag then
			self.firstflag = false
			return false
		end

		return result
	end

	function type:Update(dt)
		result = {} 
		self.explodetime = self.explodetime + dt
		local scale = (self.explodetime / GRENADE_EXPLODE_TIME)* self.radius + 1
		--Transform.SetScale(self.transformID, scale)
		--SphereCollider.SetRadius(self.sphereCollider, scale)
		local collisionIDs = self.sphereCollider:GetCollisionIDs()
		for curID = 1, #collisionIDs do
			for curEnemy=1, #enemies do
				if collisionIDs[curID] == enemies[curEnemy].collider:GetID() then
					table.insert(result, enemies[curEnemy])
				end
			end
			if boss.alive and collisionIDs[curID] == boss.collider:GetID() then
				table.insert(result, boss)
			end
		end
		return result
	end

	function type:Kill()
		Transform.SetScale(self.transformID, 1)
		self.sphereCollider:SetRadius(0.3)
		Transform.ActiveControl(self.transformID, false)
		SphereCollider.SetActive(self.sphereCollider, false)
	end

	return type
end

function DestroyGrenadeType(grenadeType)
	Gear.UnbindInstance(grenadeType.transformID)
	grenadeType = nil
end