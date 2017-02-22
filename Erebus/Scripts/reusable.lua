MAX_FIRE_EFFECT_PARTICLES = 50
function BaseCombine(self, effect,damage)
	if #self.effects < 2 then
		table.insert(self.effects, effect)
		self.damage = self.damage + damage
	end
end

function BaseCharge(self, dt)
	if self == nil then
		return
	end
	if self.chargedTime < self.maxChargeTime then 
		self.chargedTime = self.chargedTime + dt
	end
	if self.owner == player then
		ZoomInCamera()
	end
end

function BaseChargeCast(self, entity)
	if self.owner == player then
		ZoomOutCamera()
	end
	self:Cast(entity, self.chargedTime)
end

function BaseChange(self)
	self.isActiveSpell = not self.isActiveSpell
end

function BaseGetEffect(self)
	return self.effects[1]
end

function GetHeightmap(position)
	local result = nil
	for _,v in pairs(heightmaps) do
		if v.asset:Inside(position) then
			result = v
		end
	end
	return result
end


fireeffectparticles = {particles = {}, nextIndex = 0}
function InitFireEffectParticles()
	for i = 1, MAX_FIRE_EFFECT_PARTICLES do
		fireeffectparticles.particles[i] = CreateFireEffectParticles()
	end
end

function GetNextFireEffectParticle()
	fireeffectparticles.nextIndex = (fireeffectparticles.nextIndex%MAX_FIRE_EFFECT_PARTICLES)+1
	return fireeffectparticles.particles[fireeffectparticles.nextIndex]
end

InitFireEffectParticles()

function PrintInfo() 
	if player.printInfo then
		local scale = 0.8
		local color = {0.4, 1, 0.4, 1}
		local info = "Player"
		Gear.Print(info, 60, 570, scale, color)

		local position = Transform.GetPosition(player.transformID)
		info = "Position\nx:"..Round(player.position.x, 1).."\ny:"..Round(player.position.y, 1).."\nz:"..Round(player.position.z, 1)
		Gear.Print(info, 0, 600, scale, color)

		local direction = Transform.GetLookAt(player.transformID)
		info = "LookAt\nx:"..Round(direction.x, 3).."\ny:"..Round(direction.y, 3).."\nz:"..Round(direction.z, 3)
		Gear.Print(info, 120, 600, scale, color)

		info = "Camera"
		Gear.Print(info, 60, 400, scale, color)

		position = Camera.GetPos()
		info = "Position\nx:"..Round(player.position.x, 1).."\ny:"..Round(player.position.y, 1).."\nz:"..Round(player.position.z, 1)
		Gear.Print(info, 0, 430, scale, color)

		direction = Camera.GetDirection()
		info = "Direction\nx:"..Round(direction.x, 3).."\ny:"..Round(direction.y, 3).."\nz:"..Round(direction.z, 3)
		Gear.Print(info, 120, 430, scale, color)
	end
end