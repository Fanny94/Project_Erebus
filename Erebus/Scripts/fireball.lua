--FIREBALL_SPELL_TEXTURE = Assets.LoadTexture("Textures/IconFireball.dds");
FIRESPAM_COOLDOWN = 0.2
FIREBALL_COOLDOWN = 6
FIRESPAM_SPEED = 80
FIREBALL_SPEED = 40
FIREBALL_LIFETIME = 0.8
FIREBALL_EXPLODETIME = 0.5
MIN_CHARGETIME_FIREBALL = 0.5
FIRESPAM_DAMAGE = 10
FIREBALL_BASE_DMG = 20
FIREBALL_LIGHTRADIUS = 5
FIREBALL_CASTSPEED_MULTIPLE = 1.1 + 0.1875
FIREBALL_CAST_SFX = "Effects/fireball-01.ogg"
FIREBALL_SMALL_HIT_SFX = "Effects/explosion.ogg"
FIREBALL_BIG_HIT_SFX = "Effects/explosion.ogg"

function CreateFireball(entity)
	function initSmallFireball()
		local tiny = {}
		local model = Assets.LoadModel( "Models/projectile1.model" )
		tiny.type = CreateProjectileType(model)
		tiny.damage = FIRESPAM_DAMAGE
		tiny.alive = false
		tiny.lifeTime = FIREBALL_LIFETIME
		tiny.hits = {}
		tiny.particles = CreateFireEffectParticles()
		return tiny
	end
	--General variables
	local spell = {}
	spell.element = FIRE
	spell.damage = FIREBALL_BASE_DMG
	spell.combineDamage = 10
	spell.hudtexture = Assets.LoadTexture("Textures/IconFireball.dds");
	spell.isActiveSpell = false
	spell.maxcooldown = FIREBALL_COOLDOWN		spell.minChargeTime = MIN_CHARGETIME_FIREBALL
	spell.chargedTime = 0	spell.maxChargeTime = 3
	spell.caster = entity.transformID
	spell.owner = entity
	spell.spamCooldown = FIRESPAM_COOLDOWN
	spell.position = {x = 0, y = 0, z = 0}
	--Small spamming fireballs
	spell.smallFBSoundID = {}
	spell.bigFBSoundID = -1
	spell.smallFB = {}		spell.currentFB = 1
	for i = 1, 6 do	table.insert(spell.smallFB, initSmallFireball())	end
	
	--For animation timing 
	spell.hasSpamAttack = true
	spell.cooldown = 0 --spells no longer have an internal cooldown for spam attacks. The player's castSpeed determines this.

	spell.castTimeAttack = 0.5 * FIREBALL_CASTSPEED_MULTIPLE
	spell.castAnimationPlayTime = 2 * FIREBALL_CASTSPEED_MULTIPLE --the true cast time of the animation
	spell.castTimeFirstAttack = 0.1875 * FIREBALL_CASTSPEED_MULTIPLE

	--Big fireball
	spell.bigBallActive = false
	local model = Assets.LoadModel( "Models/projectile1.model" )
	spell.ballParticles = CreateFireEffectParticles()
	spell.transformID = Gear.BindForwardInstance(model)
	spell.sphereCollider = SphereCollider.Create(spell.transformID)
	CollisionHandler.AddSphere(spell.sphereCollider, 2)	
	SphereCollider.SetActive(spell.sphereCollider, false)
	Transform.ActiveControl(spell.transformID, false)

	spell.lifeTime = FIREBALL_LIFETIME
	spell.explodeTime = 0.5
	spell.enemiesHit = {}
	
	spell.effects = {}		table.insert(spell.effects, FIRE_EFFECT_INDEX)
	spell.light = nil
	spell.lightRadius = 0
	function spell:Update(dt)
		self.spamCooldown = self.spamCooldown - dt
		self:UpdateSmallFBs(dt)
		if self.bigBallActive then
			self:BigBallUpdate(dt)
		else	self.cooldown = self.cooldown - dt end
	end
	
	function spell:UpdateSmallFBs(dt)
		for i = 1, 4 do 
			if self.smallFB[i].alive then 
				self.smallFB[i].particles:Update(self.smallFB[i].type.position)
				self.smallFB[i].type:Update(dt)

				local collisionIDs = self.smallFB[i].type.sphereCollider:GetCollisionIDs()
				for curID = 1, #collisionIDs do
					for curEnemy=1, #enemies do
						if collisionIDs[curID] == enemies[curEnemy].collider:GetID() then
							enemies[curEnemy]:Hurt(FIRESPAM_DAMAGE, self.owner, self.element)
							self:SpamFireball(i)
						end
					end
					if boss.alive and collisionIDs[curID] == boss.collider:GetID() then
						boss:Hurt(FIRESPAM_DAMAGE, self.owner, self.element)
						self:SpamFireball(i)
					end
				end
				local hm = GetHeightmap(self.smallFB[i].type.position)
				if hm then
					if self.smallFB[i].type.position.y < hm.asset:GetHeight(self.smallFB[i].type.position.x, self.smallFB[i].type.position.z) then self:SpamFireball(i) end
				end
				self.smallFB[i].lifeTime = self.smallFB[i].lifeTime - dt		
				if(self.smallFB[i].lifeTime < 0) then 
					self:SpamFireball(i)
				end
			end		
		end	
	end

	function spell:Cast(entity)
		if self.spamCooldown < 0 and not self.bigBallActive then
			self.spamCooldown = FIRESPAM_COOLDOWN
			--self.smallFB[self.currentFB].type:Shoot(self.owner.position, Transform.GetLookAt(self.caster), FIRESPAM_SPEED)
			self.smallFB[self.currentFB].type:Shoot(self.owner.position, self.owner.spellDirection, FIRESPAM_SPEED)
			self.smallFB[self.currentFB].particles:Cast()
			self.smallFB[self.currentFB].lifeTime = FIREBALL_LIFETIME
			self.smallFB[self.currentFB].alive = true
			Sound.Play(FIREBALL_CAST_SFX, 3, self.smallFB.position)
			if self.currentFB < 4 then		
				self.currentFB = self.currentFB + 1
			else	self.currentFB = 1
			end
		end
	end

	function spell:GetCollider()
		local result = {}
		table.insert(result, self.sphereCollider:GetID())
		return result
	end

	function spell:ChargeCast(entity)
		if self.bigBallActive then
			self:EngageExplode()
		end
		if self.cooldown < 0.0 and MIN_CHARGETIME_FIREBALL < self.chargedTime and not self.bigBallActive then
			if self.owner == player then
				camera.toFollow = self
				ZoomOutCamera()	
			end
			self.lifeTime = FIREBALL_LIFETIME
			self.explodeTime = FIREBALL_EXPLODETIME	
			self.cooldown = FIREBALL_COOLDOWN
			self.bigBallActive = true
			self.position = entity.position
			SphereCollider.SetActive(self.sphereCollider, true)
			Transform.ActiveControl(self.transformID, true)
			Transform.SetPosition(self.transformID, self.position)
			self.damage = FIREBALL_BASE_DMG * self.chargedTime
			self.light = Light.addLight(124, 32, 220, 1, 0, 0, FIREBALL_LIGHTRADIUS, 3, true)
			self.ballParticles:Cast()
			Sound.Play(FIREBALL_CAST_SFX, 7, self.position)
		end
		self.chargedTime = 0
	end

	function spell:BigBallUpdate(dt)
		if self.lifeTime < 0 then 
			self:Exploding(dt) 
		else
			self.lifeTime = self.lifeTime - dt
			self:BigBallFlying(dt)	
		end
	end

	function spell:BigBallFlying(dt)
		local direction = Transform.GetLookAt(self.caster)
		self.position.x = self.position.x + direction.x * FIREBALL_SPEED * dt
		self.position.y = self.position.y + direction.y * FIREBALL_SPEED * dt
		self.position.z = self.position.z + direction.z * FIREBALL_SPEED * dt
		Transform.SetPosition(self.transformID, self.position)
		Transform.SetLookAt(self.transformID, direction)
		self.damage = self.damage + 3 * dt
		self.ballParticles:Update(self.position)
		local hm = GetHeightmap(self.position)
		if hm then
			if self.position.y < hm.asset:GetHeight(self.position.x, self.position.z) then self:EngageExplode() end
		else
			self:EngageExplode()
		end
		Light.updatePos(self.light, self.position.x, self.position.y, self.position.z, true)

		if CollisionHandler.IsHitboxCollidingWithLayer(self.sphereCollider, 3) then 
			self:EngageExplode() 
		else
			local collisionIDs = self.sphereCollider:GetCollisionIDs()
			for curID = 1, #collisionIDs do
				for curEnemy=1, #enemies do
					if collisionIDs[curID] == enemies[curEnemy].collider:GetID() then
						self:EngageExplode()
						return
					end
				end
				if boss.alive and collisionIDs[curID] == boss.collider:GetID() then
					self:EngageExplode()
					return
				end
			end	
		end	
	end

	function spell:EngageExplode()
		SphereCollider.SetRadius(self.sphereCollider, 3)
		Transform.SetScale(self.transformID, 3)
		self.lifeTime = -1
		self.lightRadius = FIREBALL_LIGHTRADIUS * 5
		Light.updateRadius(self.light, self.lightRadius , true)
	end

	function spell:Exploding(dt)
		self.explodeTime = self.explodeTime - dt
		if self.owner == player then
			camera.toFollow = self.owner
		end
		if self.explodeTime < 0 then 
			self:Kill() 
			return
		end
		self.lightRadius = self.lightRadius - 10 * dt
		Light.updateRadius(self.light, self.lightRadius, true)
		local collisionIDs = self.sphereCollider:GetCollisionIDs()
		for curID = 1, #collisionIDs do
			for curEnemy=1, #enemies do
				if collisionIDs[curID] == enemies[curEnemy].collider:GetID() then
					if not self.enemiesHit[enemies[curEnemy].transformID] then
						enemies[curEnemy]:Hurt(self.damage, self.owner, self.element)
						for stuff = 1, #self.effects do
							local effect = effectTable[self.effects[stuff]](self.owner, 0.5)
							enemies[curEnemy]:Apply(effect)
						end
					end
					self.enemiesHit[enemies[curEnemy].transformID] = true
				end
			end
			if boss.alive and collisionIDs[curID] == boss.collider:GetID() then
				if not self.enemiesHit[boss.transformID] then
					boss:Hurt(self.damage, self.owner, self.element)
					for stuff = 1, #self.effects do
						local effect = effectTable[self.effects[stuff]](self.owner, 0.5)
						boss:Apply(effect)
					end
					self.enemiesHit[boss.transformID] = true
				end
			end
		end			
	end

	function spell:GetEffect()
		return self.effects[1]
	end

	function spell:Combine(effect,damage)
		if #self.effects < 2 then
			table.insert(self.effects, effect)
			self.damage = self.damage + damage
		end
	end

	function spell:Kill()
		Sound.Play(FIREBALL_BIG_HIT_SFX, 7, self.position)
		self.enemiesHit = {}
		self.bigBallActive = false
		self.ballParticles:Die(self.position)
		SphereCollider.SetRadius(self.sphereCollider, 1)
		Transform.SetScale(self.transformID, 1)
		SphereCollider.SetActive(self.sphereCollider, false)
		Transform.ActiveControl(self.transformID, false)
		self.damage = FIREBALL_BASE_DMG	
		if #self.effects > 1 then
			table.remove(self.effects)
		end
		if self.light then		Light.removeLight(self.light, true)	 self.light = nil	end
	end

	function spell:Change()
		self.isActiveSpell = not self.isActiveSpell
		if self.isActiveSpell then
			ShowCrosshair()
		else
			HideCrosshair()
		end
	end

	function spell:SpamFireball(index)
		local id = Sound.Play(FIREBALL_SMALL_HIT_SFX, 39, self.smallFB[index].type.position)
		Sound.SetVolume(id, 0.5)
		Sound.Resume(id)
		self.smallFB[index].particles:Die(self.smallFB[index].type.position)
		self.smallFB[index].type:Kill() 
		self.smallFB[index].alive = false 
	end
	spell.Charge = BaseCharge
	spell.Change = BaseChange
	return spell
end

function DestroyFireball(fireball)
	for _,v in pairs(fireball.spell.smallFB) do
		DestroyFireEffectParticles(v.particles)
		DestroyProjectileType(v.type)
		Assets.UnloadModel( "Models/projectile1.model" )
	end

	DestroyFireEffectParticles(fireball.spell.ballParticles)

	Gear.UnbindInstance(fireball.transformID)

	Assets.UnloadModel( "Models/projectile1.model" )
	Assets.UnloadTexture( "Textures/IconFireball.dds" )

	fireball = nil
end