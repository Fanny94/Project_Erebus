local ARC_LIFETIME = 10
local ARC_GRAVITY = 49.1
local arc = dofile( "Scripts/basespell.lua" )
print(arc.BaseUpdate)
arc.speed = 50
arc.upSpeed = 50
arc.currentUpSpeed = 0
arc.lifeTime = ARC_LIFETIME

local arcModel = Assets.LoadModel( "Models/Sten.model" )
Gear.AddModelInstance(arcModel, arc.transformID)

--[[

function arc:UpdatePosition(dt)
	self.position.x = self.position.x + ((self.direction.x * self.speed)*dt)
	self.position.z = self.position.z + ((self.direction.z * self.speed)*dt)
	self.direction.y = self.direction.y - (ARC_GRAVITY*dt)
	self.position.y = self.position.y + (self.direction.y * dt)
end

]]--


function arc:Cast()
	self.position = Transform.GetPosition(player.transformID)
	self.direction = Transform.GetLookAt(player.transformID)
	self.direction.y = self.upSpeed
	self.alive = true
	self.lifeTime = ARC_LIFETIME
	self.currentUpSpeed = self.upSpeed
	Transform.SetPosition(self.transformID, self.position)
	
end


function arc:Update(dt)
	
	--self:UpdatePosition(dt)

	local height = heightmap:GetHeight(self.position.x, self.position.z)
	if self.position.y <= height then
		self:Kill()
	end

	self.lifeTime = self.lifeTime - dt
	if self.lifeTime<=0 then
		self:Kill()
	end

end



return arc