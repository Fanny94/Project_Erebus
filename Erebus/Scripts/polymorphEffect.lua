POLYMORPH_EFFECT_DURATION = 4
POLYMORPH_POOL_SIZE = 4
POLYMORPH_POOFTIME = 0.3
polymorphPool = {}
polymorphParticles = {}
currentFree = 1
function CreatePolyEffect(owner, duration)
	local effect = {}
	effect.duration = duration or POLYMORPH_EFFECT_DURATION
	effect.poofTime = POLYMORPH_POOFTIME
	effect.polymorphTransform, effect.particles = GetNextFreeMorph()

	function effect:Apply(entity, duration)
		if entity == boss then
			return
		end
		Transform.SetScale(entity.transformID, 0)
		local pos = Transform.GetPosition(entity.transformID)
		pos.y = pos.y + 1
		Transform.SetPosition(self.polymorphTransform, pos)
		if entity.SetState then
			if entity.type ~= ENEMY_DUMMY and entity.stateName ~= DEAD_STATE then
				entity.SetState(entity, DO_NOTHING_STATE)
			end
		end
		effect.particles:poof(pos)
	end

	function effect:Deapply(entity)
		if entity == boss then 
			self = nil
			return
		end
		Transform.SetScale(entity.transformID, 1)	
		Transform.ActiveControl(self.polymorphTransform, false)
		local pos = Transform.GetPosition(entity.transformID)
		if entity.SetState then
			if entity.type ~= ENEMY_DUMMY and entity.stateName ~= DEAD_STATE then
				entity.SetState(entity,IDLE_STATE)
			end
		end
		self.particles:poof(pos)
		self = nil
		
	end

	function effect:Update(entity, dt) --return false if you want the enemy to remove the effect from its effect list
		if entity == boss then
			return false
		end
		self.duration = self.duration - dt
		self.poofTime = self.poofTime - dt
		if self.poofTime < 0 then Transform.ActiveControl(self.polymorphTransform, true)  self.poofTime = 1000 end
		local pos = Transform.GetPosition(entity.transformID)
		pos.y = pos.y + 1
		Transform.SetPosition(self.polymorphTransform, pos)	
		return self.duration > 0 
	end

	return effect
end

function InitPolymorphs()
	local someModels = {"Models/Polymorph.model", "Models/Polymorph.model", "Models/Polymorph.model", "Models/Polymorph.model"}
	local models = {}
	for i = 1, POLYMORPH_POOL_SIZE do 
		models[i] = Assets.LoadModel(someModels[i])
	end
	for i = 1, POLYMORPH_POOL_SIZE do
		polymorphPool[i] = Gear.BindStaticInstance(models[i])
		polymorphParticles[i] = createCloudParticles()
		Transform.ActiveControl(polymorphPool[i], false)
	end
	currentFree = 0
end

function UnInitPolymorphs()
	for i=1, POLYMORPH_POOL_SIZE do
		Assets.UnloadModel( "Models/Polymorph.model" )
		Gear.UnbindInstance(polymorphPool[i])
		destroyCloudParticles(polymorphParticles[i])
	end
end

function GetNextFreeMorph()
	if currentFree >= POLYMORPH_POOL_SIZE then currentFree = 0 end
	currentFree = currentFree + 1	
	return polymorphPool[currentFree], polymorphParticles[currentFree]
end

--InitPolymorphs()