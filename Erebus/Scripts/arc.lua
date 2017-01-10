local ARC_LIFETIME = 10
local ARC_GRAVITY = 49.1

function CreateArc()
	local arc = dofile( "Scripts/basespell.lua" )
	print(arc.BaseUpdate)
	arc.speed = 40
	arc.upSpeed = 30
	arc.currentUpSpeed = 0
	arc.lifeTime = ARC_LIFETIME
	local fireball = createFireball()
	arc.damage = 5
	arc.explodeTime = 1
	local arcModel = Assets.LoadModel( "Models/testGuy.model" )
	arc.sphereCollider = SphereCollider.Create(arc.transformID)
	CollisionHandler.AddSphere(arc.sphereCollider)
	SphereCollider.SetRadius(arc.sphereCollider, 5)
	local fireball = createFireball()

	Gear.AddStaticInstance(arcModel, arc.transformID)

	function arc:Cast()
		self.position = Transform.GetPosition(player.transformID)
		self.velocity = Transform.GetLookAt(player.transformID)
		self.velocity.y = self.upSpeed
		self.velocity.x = self.velocity.x * 50
		self.velocity.z = self.velocity.z * 50
		self.alive = true
		self.lifeTime = ARC_LIFETIME
		self.currentUpSpeed = self.upSpeed
		fireball.cast()
		Transform.SetPosition(self.transformID, self.position)
		fireball.cast()
	end


	function arc:Update(dt)
		self.velocity.y = self.velocity.y - self.speed*dt
		fireball.update(self.position.x, self.position.y, self.position.z)
		local height = heightmap:GetHeight(self.position.x, self.position.z)
		if self.position.y <= height then
			fireball.die(self.position.x, self.position.y, self.position.z)
			arc.exploding = true
			--self:Kill()
		end

		self.lifeTime = self.lifeTime - dt
		if self.lifeTime<=0 then
			fireball.die(self.position.x, self.position.y, self.position.z)
			arc.exploding = true
			--self:Kill()
		end
		
		if arc.exploding == true then
			local collisionIDs = self.sphereCollider:GetCollisionIDs()
			for curID = 1, #collisionIDs do
			for curEnemy=1, #enemies do
					if collisionIDs[curID] == enemies[curEnemy].sphereCollider:GetID() then
						print ("Tr�ff f�r fan")
					fireball.die(self.position.x, self.position.y, self.position.z)
						enemies[curEnemy]:Hurt(self.damage)
						--self:Kill()						
					end

					--if not self.alive then break end
				end

				--if not self.alive then break end
			end
		end
		if arc.exploding == true then
			arc.explodeTime = arc.explodeTime - dt
			if arc.explodeTime < 0.0 then
				self:Kill()
			end
		end
	end

	return arc
end