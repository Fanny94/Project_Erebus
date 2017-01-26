SUNRAY_DURATION = 2
SUNRAY_MAX_CHARGETIME = 3
SUNRAY_DAMAGE = 3
SUNRAY_COOLDOWN = 5

function CreateSunRay()
	local sunRay = {}
	sunRay.type = CreateProjectileType()
	sunRay.effect = CreateFireEffect --reference to function
	sunRay.lifeTime = SUNRAY_DURATION
	sunRay.damage = 0
	sunRay.alive = false
	sunRay.speed = 100
	sunRay.effectFlag = false
	sunRay.maxChargeTime = SUNRAY_MAX_CHARGETIME
	sunRay.chargedTime = 0
	sunRay.Charge = BaseCharge
	sunRay.ChargeCast = BaseChargeCast	
	sunRay.particles = createFireballParticles()
	sunRay.owner = {}
	sunRay.moveImpairment = 0.5
	sunRay.cameraSlow = 2.0
	sunRay.castSFX = {}
	sunRay.castSFX[1] = "Effects/CK_Blaster_Shot-226.wav"
	sunRay.castSFX[2] = "Effects/CK_Force_Field_Loop-32.wav"
	sunRay.hitSFX = "Effects/burn_ice_001.wav"
	sunRay.soundID = {}

	local model = Assets.LoadModel( "Models/sunRay.model" )
	Gear.AddStaticInstance(model, sunRay.type.transformID)

	function sunRay:Update(dt)
		hits = self.type:Update(dt)
		self.particles.update(self.type.position.x, self.type.position.y, self.type.position.z)
		tempDir = Transform.GetRotation(player.tranformID)
		Transform.SetRotation(self.type.transformID, Transform.GetRotation(player.tranformID))
		Transform.SetLookAt(self.type.transformID, Transform.GetLookAt(player.tranformID))
		for index = 1, #hits do
			if hits[index].Hurt then	
				if self.effectFlag then
					table.insert(hits[index].effects, self.effect())
				end
				hits[index]:Hurt(self.damage)
				Sound.Play(self.hitSFX, 1, hits[index].position)
			end
		end
		self.type:Shoot(Transform.GetPosition(player.transformID), Camera.GetDirection(), 0)
		self.lifeTime = self.lifeTime - dt
		if self.lifeTime < 0 then 
			self.particles.die(self.type.position) 
			self:Kill() 
		end
	end
	
	function sunRay:Cast(chargetime, effects)
		self.particles.cast()
		Erebus.CameraSensitivity(self.cameraSlow)
		chargetime = math.min(chargetime, SUNRAY_MAX_CHARGETIME)
		player.moveSpeed = player.moveSpeed * self.moveImpairment 
		self.type:Shoot(Transform.GetPosition(player.transformID), Camera.GetDirection(), 0)
		self.alive = true
		self.lifeTime = SUNRAY_DURATION 
		self.effectFlag = effects
		self.damage = (chargetime/SUNRAY_MAX_CHARGETIME) * SUNRAY_DAMAGE
		self.chargedTime = 0
		for index = 1, #self.castSFX do
			self.soundID[index] = Sound.Play(self.castSFX[index], 13, self.type.position)
			Sound.SetVolume(self.soundID[index], 0.8)
		end
	end

	function sunRay:Kill()
		self.alive = false
		Sound.Pause(self.soundID[2])
		Erebus.CameraSensitivity(1 / self.cameraSlow)
		player.moveSpeed = player.moveSpeed * (1 / self.moveImpairment) 
		self.type:Kill()
	end
	return sunRay
end