nrOfActors = 50
nrOfEnemies = 49
player = {}
enemy = {}
enemySpeeds = {}
Transform.InitStuff(nrOfActors)
player.trans = Transform.Bind()
player.moveSpeed = 40

for i = 1, nrOfEnemies do
		enemy[i] = Transform.Bind()
		enemySpeeds[i] = math.random(5, 20)
end

function ChangePlayer()
	player.trans = (player.trans + 1) % nrOfActors
	Transform.Switch(player.trans)	
end

buttons = {}
function Controls()
	forward, left, up = 0, 0, 0
	for i = 1, #buttons do
		if buttons[i] == 0 then	forward = player.moveSpeed  end
		if buttons[i] == 1 then	forward = -player.moveSpeed  end
		if buttons[i] == 2 then	left = player.moveSpeed  end
		if buttons[i] == 3 then left = -player.moveSpeed  end
		if buttons[i] == 4 then up = player.moveSpeed  end
		if buttons[i] == 5 then up = -player.moveSpeed  end
		if buttons[i] == 6 then ChangePlayer() end
	end	
	Transform.Move(player.trans, forward, up, left)
	buttons = {}	
end

function doDaHustle()
	for i = 1, nrOfEnemies do
		Transform.Follow(player.trans, enemy[i], enemySpeeds[i])
	end
end

