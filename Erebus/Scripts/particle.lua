function createFireballParticles()
	--Args = Antal partiklar, livstid, hastighet, utskjut/sekund, antal/utskjut, koncentration p� spruuut
	local fireball = {}
	fireball.fly = Particle.Bind("particle.dp", 10)
	fireball.exploda = Particle.Bind("particle.dp", 10)

	function fireball.cast()
		Particle.SetAlive(fireball.fly)
	end

	function fireball.die(pos)
		Particle.SetDead(fireball.fly)
		Particle.SetPosition(fireball.exploda, pos.x, pos.y, pos.z)
		Particle.SetDirection(fireball.exploda, 0, 0, 0)
		Particle.Explode(fireball.exploda)	
	end

	function fireball.update(x, y, z, mx, my, mz)
		Particle.SetPosition(fireball.fly, x, y, z)
		--Particle.SetDirection(fireball.fly, 0, 10, 0)
	end

	return fireball
end
