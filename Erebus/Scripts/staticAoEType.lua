function CreateStaticAoEType()
	local type = {}
	type.transformID = Transform.Bind()
	
	type.sphereCollider = SphereCollider.Create(type.transformID)
	CollisionHandler.AddSphere(type.sphereCollider, 1)
	SphereCollider.SetActive(type.sphereCollider, false)
	Transform.ActiveControl(type.transformID, false)
	type.maxradius = 0
	type.duration = 0
	type.timer = 0

	function type:Cast(duration, radius, position)
		Transform.ActiveControl(type.transformID, true)
		SphereCollider.SetActive(self.sphereCollider, true)
		Transform.SetPosition(self.transformID, position)
		self.maxradius = radius
		self.duration = duration
		self.timer = 0
	end

	function type:Update(dt)
		local result = {}
		self.timer = self.timer + dt
		local factor = math.min(self.timer / self.duration,1)
		local scale = factor * self.maxradius
		Transform.SetScale(self.transformID, scale)
		SphereCollider.SetRadius(self.sphereCollider, scale)
		local collisionIDs = self.sphereCollider:GetCollisionIDs()
		for curID = 1, #collisionIDs do
			for curEnemy=1, #enemies do
				if collisionIDs[curID] == enemies[curEnemy].sphereCollider:GetID() then
					table.insert(result, enemies[curEnemy])
				end
			end
		end
		return result
	end

	function type:Kill()
		Transform.ActiveControl(self.transformID, false)
		SphereCollider.SetActive(self.sphereCollider, false)
		Transform.SetPosition(self.transformID, {x=0,y=0,z=0})
	end

	return type
end