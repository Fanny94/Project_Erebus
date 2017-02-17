STATE_ZOOMING_IN, STATE_ZOOMING_OUT, STATE_INTERUPT = 0, 1, 2
camera = {distance = 4, angle = 0, xOffset = 0, yOffset = 1.4, fov = (3.14/180) *50, state = STATE_ZOOMING_OUT}

interuptedState = 0

timeSinceShot = 0
DelayZoomOut = 0.5

--distance was 4
ZoomedOut = {distance = 6, angle = 0, time = 1, timeSpent = 0, xOffset = 0, yOffset = 1.4, fov = (3.14/180) *50}	--fov �r i radianer, strange things happen with higher values. 90 grader ar standard i fps spel
--distance was 3.6
ZoomedIn = {distance = 5.6, angle = 0, time = 1, timeSpent = 0, xOffset = 0.6, yOffset = 1.4, fov = (3.14/180)*50}		--fov �r i radianer, be careful when changing

StartState = {distance = 0, angle = 0, xOffset = 0, yOffset = 0, fov = (3.14/180)*50}

function interpolate(a, b, factor) 
	return a + factor*(b-a)
end

function cross(a, b)
	return {x = a.y*b.z - a.z*b.y,
			y = a.z*b.x - a.x*b.z,
			z = a.x*b.y - a.y*b.x}
end

function scalarvec3mult(a, b)
	return {x = a*b.x, 
			y = a*b.y, 
			z = a*b.z}
end

function vec3add(a, b)
	return {x = a.x+b.x,
			y = a.y+b.y,
			z = a.z+b.z}
end

function vec3sub(a, b)
	return {x = a.x-b.x,
			y = a.y-b.y,
			z = a.z-b.z}
end

function vec3length(a)
	return math.sqrt(a.x * a.x + a.y * a.y + a.z * a.z)
end

function vec3equal(a, b)
	return a.x == b.x and a.y == b.y and a.z == b.z
end

function vec3print(a)
	print("x: " .. a.x .. "\ty:" .. a.y .. "\tz:" ..a.z)
end

function vec3gief(a, b)
	a.x, a.y, a.z = b.x, b.y, b.z
end

function ZoomInCamera()
	if camera.state ~= STATE_ZOOMING_IN then
		camera.state = STATE_ZOOMING_IN
		StartState.distance = camera.distance
		StartState.angle = camera.angle
		StartState.xOffset = camera.xOffset
		StartState.yOffset = camera.yOffset
		StartState.fov = camera.fov		
		ZoomedIn.timeSpent = 0
	end
end

function ZoomOutCamera()
	if camera.state ~= STATE_ZOOMING_OUT then
		camera.state = STATE_ZOOMING_OUT
		StartState.distance = camera.distance
		StartState.angle = camera.angle
		StartState.xOffset = camera.xOffset
		StartState.yOffset = camera.yOffset
		StartState.fov = camera.fov
		ZoomedOut.timeSpent = 0
	end
end

function UpdateCamera(dt)	
	if camera.state == STATE_ZOOMING_OUT then
		ZoomedOut.timeSpent = ZoomedOut.timeSpent + dt

		--i vilket l�ge �verg�ngen �r i
		local factor = math.sin((math.min(ZoomedOut.timeSpent, ZoomedOut.time)/ZoomedOut.time)*3.14 - 3.14*0.5)*0.5 + 0.5	--cirukl�r (mjukare, men dyrare)
		--local factor = math.min(ZoomedOut.timeSpent, ZoomedOut.time)/ZoomedOut.time										--linj�r

		--Linj�r interpolation mellan vart kameran var n�r �verg�ngen b�rjade och vart den ska vara n�r �verg�ngen �r klar
		camera.distance = interpolate(	StartState.distance,	ZoomedOut.distance, factor	)
		camera.angle = interpolate(		StartState.angle,		ZoomedOut.angle,	factor	)
		camera.xOffset = interpolate(	StartState.xOffset,		ZoomedOut.xOffset,	factor	)
		camera.yOffset = interpolate(	StartState.yOffset,		ZoomedOut.yOffset,	factor	) 
		camera.fov = interpolate(		StartState.fov,			ZoomedOut.fov,		factor	) 

	elseif camera.state == STATE_ZOOMING_IN then
		ZoomedIn.timeSpent =  ZoomedIn.timeSpent + dt 
		
		--i vilket l�ge �verg�ngen �r i
		local factor = math.sin((math.min(ZoomedIn.timeSpent, ZoomedIn.time)/ZoomedIn.time)*3.14 - 3.14/2)/2 + 0.5	--cirkul�r (mjukare, men dyrare)
		--local factor = math.min(ZoomedIn.timeSpent, ZoomedIn.time)/ZoomedIn.time										--linj�r

		--interpolation mellan vart kameran var n�r �verg�ngen b�rjade och vart den ska vara n�r �verg�ngen �r klar, factor kan vara linj�r eller cirkul�r :)
		camera.distance = interpolate(	StartState.distance,	ZoomedIn.distance,	factor	)
		camera.angle = interpolate(		StartState.angle,		ZoomedIn.angle,		factor	)
		camera.xOffset = interpolate(	StartState.xOffset,		ZoomedIn.xOffset,	factor	)
		camera.yOffset = interpolate(	StartState.yOffset,		ZoomedIn.yOffset,	factor	)
		camera.fov = interpolate(		StartState.fov,			ZoomedIn.fov,		factor	) 

	end

	Camera.Follow(camera.fov, player.transformID, camera.yOffset, camera.xOffset, camera.distance, camera.angle)
	local temppos = Camera.GetPos()
	local distance = camera.distance
	local dir = Camera.GetDirection()
	local height = 0
	local hm = GetHeightmap(temppos)
	if hm then
		height = hm.asset:GetHeight(temppos.x, temppos.z)
	end
	local incrementfactor = (0.03) --absolute length of increment is 0.03 units
	local interupted = false

	if camera.state ~= STATE_INTERUPT then --save the state of the camera or something
		interuptedState = camera.state
	end
	while distance > 0.5 do
		--height = heightmaps[1].asset:GetHeight(temppos.x, temppos.z
		local hm = GetHeightmap(temppos)
		if hm then
			height = hm.asset:GetHeight(temppos.x, temppos.z)
			if height > temppos.y then
				distance = distance - 0.03
				temppos.x = temppos.x + dir.x * incrementfactor
				temppos.y = temppos.y + dir.y * incrementfactor
				temppos.z = temppos.z + dir.z * incrementfactor
				if camera.state ~= STATE_INTERUPT then
					interuptedState = camera.state
					camera.state = STATE_INTERUPT
					if camera.state == STATE_ZOOMING_IN then
						ZoomedIn.timeSpent = 0
					else
						ZoomedOut.timeSpent = 0
					end
				end
				StartState.distance = distance
				ZoomedIn.timeSpent = 0
				interupted = true
				--Camera.SetHeight(height + 0.5) 
			else
				break
			end
		else
			break
			--distance = distance - 0.03
		end
	end
	if not interupted then -- restore the state of the camera or somehting
		
		if interuptedState == STATE_ZOOMING_IN then
			ZoomInCamera()
		else
			ZoomOutCamera()
		end
	end
	camera.distance = distance
	Camera.Follow(camera.fov, player.transformID, camera.yOffset, camera.xOffset, camera.distance, camera.angle)
end

return { Update = UpdateCamera }