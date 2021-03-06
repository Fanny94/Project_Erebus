local PROJECTILE_LIFETIME = 2
--[[function CreateProjectile()
	local projectile  = dofile( "Scripts/basespell.lua" )
	projectile.speed = 100
	projectile.damage = 5
	projectile.lifeTime = PROJECTILE_LIFETIME
	projectile.sphereCollider = SphereCollider.Create(projectile.transformID)
	local fireball = createFireball()
	CollisionHandler.AddSphere(projectile.sphereCollider)

	local projectileModel = Assets.LoadModel( "Models/projectile1.model" )
	--Gear.AddStaticInstance(projectileModel, projectile.transformID)
	function projectile:Cast()
		self.position = Transform.GetPosition(player.transformID)
		self.direction = Camera.GetDirection()--Transform.GetLookAt(player.transformID
		self.velocity.x = self.direction.x * self.speed
		self.velocity.y = self.direction.y * self.speed
		self.velocity.z = self.direction.z * self.speed
		self.alive = true
		self.lifeTime = PROJECTILE_LIFETIME
		Transform.SetPosition(self.transformID, self.position)
		fireball.cast()
	end


	function projectile:Update(dt)
		local height = heightmaps[1]:GetHeight(self.position.x, self.position.z)
		fireball.update(self.position.x, self.position.y, self.position.z)
		if self.position.y <= height then
			fireball.die(self.position.x, self.position.y, self.position.z)
			self:Kill()
		end
		self.lifeTime = self.lifeTime - dt
		if self.lifeTime <= 0 then
			fireball.die(self.position.x, self.position.y, self.position.z)
			self:Kill()
		end

		local collisionIDs = self.sphereCollider:GetCollisionIDs()
		for curID = 1, #collisionIDs do
			for curEnemy=1, MAX_ENEMIES do
				if collisionIDs[curID] == enemies[curEnemy].sphereCollider:GetID() then
				
					fireball.die(self.position.x, self.position.y, self.position.z)
					self:Kill()
					enemies[curEnemy]:Hurt(self.damage)
				end

				if not self.alive then break end
			end

			if not self.alive then break end
		end
	end

	return projectile
end]]--


function CreateProjectile()
	projectile.speed = 100
	projectile.damage = 5
	projectile.lifeTime = PROJECTILE_LIFETIME
	projectile.type = 
	local fireball = createFireball()

	local projectileModel = Assets.LoadModel( "Models/projectile1.model" )
	Gear.AddStaticInstance(projectileModel, projectile.transformID)
	function projectile:Cast()
		self.position = Transform.GetPosition(player.transformID)
		self.direction = Camera.GetDirection()--Transform.GetLookAt(player.transformID
		self.velocity.x = self.direction.x * self.speed
		self.velocity.y = self.direction.y * self.speed
		self.velocity.z = self.direction.z * self.speed
		self.alive = true
		self.lifeTime = PROJECTILE_LIFETIME
		Transform.SetPosition(self.transformID, self.position)
		fireball.cast()
	end


	function projectile:Update(dt)
		local height = heightmap:GetHeight(self.position.x, self.position.z)
		fireball.update(self.position.x, self.position.y, self.position.z)
		if self.position.y <= height then
			fireball.die(self.position.x, self.position.y, self.position.z)
			self:Kill()
		end
		self.lifeTime = self.lifeTime - dt
		if self.lifeTime <= 0 then
			fireball.die(self.position.x, self.position.y, self.position.z)
			self:Kill()
		end

		local collisionIDs = self.sphereCollider:GetCollisionIDs()
		for curID = 1, #collisionIDs do
		for curEnemy=1, #enemies do
				if collisionIDs[curID] == enemies[curEnemy].sphereCollider:GetID() then
				
					fireball.die(self.position.x, self.position.y, self.position.z)
					self:Kill()
					enemies[curEnemy]:Hurt(self.damage)
				end

				if not self.alive then break end
			end

			if not self.alive then break end
		end
	end

	return projectile
end