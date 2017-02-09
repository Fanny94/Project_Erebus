FIREBALL_SPELL_TEXTURE = Assets.LoadTexture("Textures/firepillar.dds");
FIRESPAM_COOLDOWN = 0.6
FIRESPAM_SPEED = 120
FIREBALL_SPEED = 70
MIN_CHARGETIME_FIREBALL = 1.5

function CreateFireball(entity)
	function initSmallFireball()
		local tiny = {}
		tiny.type = CreateProjectileType()
		tiny.damage = 1
		tiny.alive = false
		tiny.lifeTime = 1.8
		tiny.hits = {}
		local model = Assets.LoadModel( "Models/grenade.model" )
		Gear.AddForwardInstance(model, tiny.type.transformID)
		tiny.particles = createIceGrenadeParticles()
		return tiny
	end
	--General variables
	local spell = {}
	spell.isActiveSpell = false		spell.aSmallIsActive = 0
	spell.cooldown = FIRESPAM_COOLDOWN		spell.maxcooldown = 2
	spell.chargedTime = 0	spell.maxChargeTime = 3
	spell.caster = entity.transformID
	spell.owner = entity
	spell.chargeCooldown = 8
	--Small spamming fireballs
	spell.smallFB = {}		spell.currentFB = 1
	for i = 1, 4 do	table.insert(spell.smallFB, initSmallFireball())	end
	--Big fireball
	spell.bigBallActive = false
	spell.bigBallID = Transform.Bind()	
	spell.ballParticles = createChargeParticles()
	spell.sphereCollider = SphereCollider.Create(spell.bigBallID)
	CollisionHandler.AddSphere(spell.sphereCollider, 1)	
	spell.position = {x = 0, y = 0, z = 0}
	SphereCollider.SetActive(spell.sphereCollider, false)
	Transform.ActiveControl(spell.bigBallID, false)
	local model = Assets.LoadModel("Models/projectile1.model")
	Gear.AddStaticInstance(model, spell.bigBallID)
	spell.effects = {}		table.insert(spell.effects, FIRE_EFFECT_INDEX)

	spell.hudtexture = FIREBALL_SPELL_TEXTURE

	function spell:Update(dt)
		self.cooldown = self.cooldown - dt
		if self.aSmallIsActive > 0 then
			self:UpdateSmallFBs(dt)
		end
		if self.bigBallActive then
			self:BigBallUpdate(dt)
		else	self.chargeCooldown = self.chargeCooldown - dt end
	end
	
	function spell:UpdateSmallFBs(dt)
		for i = 1, 4 do 
			if self.smallFB[i].alive then 
				self.smallFB[i].particles.update(self.smallFB[i].type.position)
				local hits = self.smallFB[i].type:Update(dt) 
				for index = 1, #hits do
					if hits[index].Hurt then		
						hits[index]:Hurt(self.smallFB[i].damage)
						self:Kill(i)
					end
				end
				self.smallFB[i].lifeTime = self.smallFB[i].lifeTime - dt		
				if(self.smallFB[i].lifeTime < 0) then 
					self:Kill(i)
				end
			end		
		end	
	end

	function spell:Cast(entity)
		if self.cooldown < 0 and not self.bigBallActive then
			self.cooldown = FIRESPAM_COOLDOWN
			self.aSmallIsActive = self.aSmallIsActive + 1
			self.smallFB[self.currentFB].type:Shoot(self.owner.position, Camera.GetDirection(), FIRESPAM_SPEED)
			self.smallFB[self.currentFB].particles.cast()
			self.smallFB[self.currentFB].lifeTime = 2.1	
			self.smallFB[self.currentFB].alive = true
			if self.currentFB < 4 then		
				self.currentFB = self.currentFB + 1
			else	self.currentFB = 1
			end
		end
	end

	function spell:ChargeCast(entity)
		if self.bigBallActive then
			self:KillFireball()
		end
		if self.chargeCooldown < 0.0 and MIN_CHARGETIME_FIREBALL < self.chargedTime and not self.bigBallActive then			
			self.scale = self.chargedTime	
			self.bigBallActive = true
			self.position = entity.position
			SphereCollider.SetRadius(self.sphereCollider, self.scale)
			SphereCollider.SetActive(self.sphereCollider, true)
			Transform.ActiveControl(self.bigBallID, true)
			Transform.SetPosition(self.bigBallID, self.position)
			Transform.SetScale(self.bigBallID, self.scale)
			self.damage = 50
			self.ballParticles.cast()
		end
		self.chargedTime = 0
	end

	function spell:BigBallUpdate(dt)
		local direction = Transform.GetLookAt(self.caster)
		self.position.x = self.position.x + direction.x * FIREBALL_SPEED * dt
		self.position.y = self.position.y + direction.y * FIREBALL_SPEED * dt
		self.position.z = self.position.z + direction.z * FIREBALL_SPEED * dt
		Transform.SetPosition(self.bigBallID, self.position)
		self.damage = self.damage + 5 * dt
		self.ballParticles.update(self.position)
		local hm = GetHeightmap(self.position)
		if hm then
			if self.position.y < hm.asset:GetHeight(self.position.x, self.position.z) then self:KillFireball() end
		end
		if self.position.x > 1000 and self.position.x < -1000 and self.position.y > 1000 and self.position.z < -1000 and self.position.z > 1000 then
			self:KillFireball()
		end
	end

	spell.Charge = BaseCharge

	function spell:Change()
		self.isActiveSpell = not self.isActiveSpell
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

	function spell:Kill(index)
		self.smallFB[index].particles.die(self.smallFB[index].type.position)
		self.smallFB[index].type:Kill() 
		self.smallFB[index].alive = false 
		self.aSmallIsActive = self.aSmallIsActive - 1
	end

	function spell:KillFireball()
		self.bigBallActive = false
		self.ballParticles.die()
		SphereCollider.SetActive(self.sphereCollider, false)
		Transform.ActiveControl(self.bigBallID, false)
		self.cooldown = FIRESPAM_COOLDOWN
		self.chargeCooldown = 8
	end
	return spell
end