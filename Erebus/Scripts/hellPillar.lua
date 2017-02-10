HELLPILLAR_SPELL_TEXTURE = Assets.LoadTexture("Textures/firepillar.dds");
BLEND_TERXTURE1 = Assets.LoadTexture("Textures/hellpillarAnimBurn2.dds");
BLEND_TERXTURE2 = Assets.LoadTexture("Textures/hellpillarAnimFlame.dds");

MIN_CHARGE_TIME_PILLAR = 1
COOLDOWN_PILLAR = 4
PILLAR_SFX = "Effects/explosion.wav"
HIT_SFX = "Effects/burn_ice_001.wav"

function CreateHellPillar(entity)
		
	local spell = {}
	spell.caster = entity.transformID	
	spell.owner = entity
	spell.pos = Transform.GetPosition(spell.caster)
	spell.chargedTime = 0	
	spell.Charge = BaseCharge
	spell.cooldown = 0
	spell.effects = {}
	table.insert(spell.effects, FIRE_EFFECT_INDEX)
	spell.hudtexture = HELLPILLAR_SPELL_TEXTURE
	spell.texture1 = BLEND_TERXTURE1
	spell.texture2 = BLEND_TERXTURE2
	spell.maxcooldown = COOLDOWN_PILLAR --Change to cooldown duration if it has a cooldown otherwise -1
	spell.blendValue1 = {x = 0.0, y = 0.0} spell.blendValue2 = {x = 0.0, y = 0.5}
	spell.Change = GenericChange
	spell.maxChargeTime = 3
	--Set up collider, model and transform for the pillar
	spell.riseFactor = 0.1
	spell.transformID = Transform.Bind()
	spell.sphereCollider = SphereCollider.Create(spell.transformID)
	CollisionHandler.AddSphere(spell.sphereCollider, 1)
	SphereCollider.SetRadius(spell.sphereCollider, 3)
	SphereCollider.SetActive(spell.sphereCollider, false)
	Transform.ActiveControl(spell.transformID, false)
	local model = Assets.LoadModel( "Models/hellpillarTest1.model" )
	Gear.AddBlendingInstance(model, spell.transformID)
	--Gear.AddForwardInstance(model, spell.transformID)

	Gear.SetBlendTextures(spell.modelIndex, 2, spell.texture1, spell.texture2)

	spell.effectflag = false
	spell.damage = MAX_DAMAGE_PILLAR
	spell.alive = false
	spell.attack = false
	spell.effects = {}
	table.insert(spell.effects, FIRE_EFFECT_INDEX)
	spell.aimPos = {}
	
	spell.timeSinceLastPoop = 2
	spell.startUp = true	spell.startUpTime = 1.5		spell.growAgain = true	
	spell.finishingTime = 2.0	spell.maxScale = 3
	spell.isActiveSpell = false
	--spell.lightRadius = 0	spell.light = 0
	--Set up the first model
	spell.firstModel = Transform.Bind()
	local model = Assets.LoadModel( "Models/hellpillarLoadOut2.model" )
	Gear.AddForwardInstance(model, spell.firstModel)

	function spell:Cast(entity, chargetime)
		if self.cooldown < 0 then	
			self.timeSinceLastPoop = 2
			self.cooldown = 1.5
			self.startUpTime = 0.4		self.finishingTime = 2	self.startUpScale = 1	
			self.damage = 10
			self.maxScale = 1
			Transform.SetScale(spell.transformID, 0.2)
			SphereCollider.SetRadius(self.sphereCollider, 0.8)
			ZoomInCamera()
			self:GeneralCast()	
		end
	end

	function spell:ChargeCast(entity)
		if self.cooldown < 0.0 and MIN_CHARGE_TIME_PILLAR < self.chargedTime  then		
			self.cooldown = COOLDOWN_PILLAR	
			self.startUpTime = 1.5		self.finishingTime = 4.5	self.startUpScale = 3
			self.maxScale = 3
			Transform.SetScale(spell.transformID, 1)
			SphereCollider.SetRadius(self.sphereCollider, 3)
			self.damage = 50
			self:GeneralCast()	
		end
		self.chargedTime = 0
	end
	
	function spell:GeneralCast()
		self.alive = true	self.growAgain = true
		self.pos = self.aimPos
		Transform.SetPosition(self.firstModel, self.pos)
		Transform.ActiveControl(self.firstModel, true)
		--self.lightRadius = 10
		--self.light = Light.addLight(self.pos.x, self.pos.y+3, self.pos.z, 1,0,0,self.lightRadius,10)
	end
	
	function spell:Update(dt)
		self.cooldown = self.cooldown - dt
		
		if self.alive then
			if self.startUp then
				self:StartingUp(dt)			
			elseif self.attack then
				self:PillarUpdate(dt)
			else
				self:Finishing(dt)
			end
		end		
		if self.isActiveSpell then
			self:Aim()
			self.timeSinceLastPoop = self.timeSinceLastPoop - dt
			if self.timeSinceLastPoop < 0 then
				ZoomOutCamera()
				self.timeSinceLastPoop = 1000
			end
		end
	end

	spell.someRotation = {x = 0, y = 0, z = 0}
	spell.startUpScale = 0
	function spell:StartingUp(dt)
		self.startUpTime = self.startUpTime - dt
		self.startUpScale = self.startUpScale - dt * 2
		Transform.SetScale(self.firstModel,  self.startUpScale )
		self.someRotation.y = self.someRotation.y + 8 * dt 
		Transform.SetRotation(self.firstModel, 	self.someRotation)
		--self.lightRadius = self.lightRadius - 5*dt
		--Light.updateRadius(self.light, self.lightRadius)
		if self.startUpTime < 0 then
			self.startUp = false
			self.attack = true		
			SphereCollider.SetActive(self.sphereCollider, true)
			Transform.SetPosition(self.transformID, self.pos)
			Sound.Play(PILLAR_SFX, 7, self.pos)				
			Transform.ActiveControl(self.transformID, true)
			self.startUpTime = 0.2
			--Light.updateRadius(self.light, 10)
		end
	end

	function spell:PillarUpdate(dt)			
		local collisionIDs = self.sphereCollider:GetCollisionIDs()
		for curID = 1, #collisionIDs do
			for curEnemy=1, #enemies do
				if collisionIDs[curID] == enemies[curEnemy].sphereCollider:GetID() then
					enemies[curEnemy]:Hurt(self.damage, self.owner)
					for i = 1, #self.effects do
						local effect = effectTable[self.effects[i]](self.owner)
						enemies[curEnemy]:Apply(effect)
					end	
				end
					Sound.Play(HIT_SFX, 1, self.pos)
			end
		end		
		self.startUp = false
		self.attack = false
		SphereCollider.SetActive(self.sphereCollider, false)
	end

	function spell:Finishing(dt)
		self.finishingTime = self.finishingTime - dt
		if self.finishingTime < 0 then
			self.alive = false 
			self.startUp = true
			Transform.ActiveControl(self.transformID, false)
			Transform.SetPosition(self.transformID, {x=0,y=0,z=0})
			self.blendValue1.x, self.blendValue1.y = 0, 0
			self.blendValue2.x, self.blendValue2.y = 0, 0
			self.riseFactor = 0.1
			--Light.removeLight(self.light)
		else
			--self.someRotation.y = self.someRotation.y + 15 * dt 	
			--Transform.SetRotation(self.transformID, self.someRotation)
			self.blendValue1.x = self.blendValue1.x + 0.2 * dt
			self.blendValue1.y = self.blendValue1.y + 0.6 * dt

			self.blendValue2.x = self.blendValue2.x - 0.2 * dt
			self.blendValue2.y = self.blendValue2.y - 0.3 * dt

			Gear.SetBlendUniformValue(self.modelIndex, 2, self.blendValue1, self.blendValue2)
			if self.riseFactor < 1 then self.riseFactor = self.riseFactor + math.tan(self.riseFactor) * 2 * dt end
			
			Transform.SetScaleNonUniform(self.transformID, 1, self.riseFactor, 1)
			self.startUpTime = self.startUpTime - dt
			if self.startUpTime > 0 then
				self.startUpScale = self.startUpScale + 50 * dt
				if self.startUpScale > self.maxScale  then self.startUpScale = self.maxScale  end
				Transform.SetScale(self.firstModel, self.startUpScale )
			else
				Transform.ActiveControl(self.firstModel, false)
				self.growAgain = false
			end
		end	
	end

	function spell:Aim()
		local lookAt = Transform.GetLookAt(self.caster)
		local aPos = Transform.GetPosition(self.caster)
		self.aimPos = {x = aPos.x + lookAt.x *10, y = 0, z = aPos.z + lookAt.z *10 }
		local hm = GetHeightmap(self.aimPos)
		if hm then
			self.aimPos.y = hm.asset:GetHeight(self.aimPos.x, self.aimPos.z)
			self.owner.aim:SetPos(self.aimPos)
		end
	end

	function spell:Change()
		self.isActiveSpell = not self.isActiveSpell
		Transform.ActiveControl(self.owner.aim.transformID, self.isActiveSpell)
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
	function spell:Kill() Transform.ActiveControl(self.owner.aim.transformID, false) end
	return spell
end