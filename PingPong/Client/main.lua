local socket = require("socket")
local address, port = "localhost", 101
client = socket.udp()
client:setpeername(address, port)
client:settimeout(0)

local gI = ""
local SCREEN_SIZE = 500

function love.load()
  love.window.setTitle("PingPong")
  love.window.setMode(500,500,{
        fullscreen = false,
        resizable = false,
        centered = true
  })
  player1 = getPlayer(10)
  player2 = getPlayer(480)
  client:send("ballinfo:nil")
  data = client:receive()
  if data then
    serverData(data)
  end
  ball = createBall()
end

function love.update(dt)
  love.window.setTitle("PingPong - FPS:" .. tostring(love.timer.getFPS()))
  
  if love.keyboard.isDown("left") or love.keyboard.isDown("up") then
    if not (player1.y - player1.speed * dt < 0) then
      player1.y = player1.y - player1.speed*dt
    end
  elseif love.keyboard.isDown("right") or love.keyboard.isDown("down") then
    if not (player1.y + player1.speed*dt > SCREEN_SIZE) then
      player1.y = player1.y + player1.speed*dt
    end
  end
    client:send("player:"..tostring(player1.x))
	
	data = client:receive()
	if data then
		serverData(data)
	end
	
	client:send("ballpos:nil")
	data1 = client:receive()
	if data1 then
		serverData(data1)
	end
	
	client:send("balldir:nil")
	data2 = client:receive()
	if data2 then
		serverData(data2)
	end
	
  moveBall(ball,dt)
end


function love.draw()
  love.window.setTitle("PingPong - FPS:" .. tostring(love.timer.getFPS()))
  love.graphics.setColor(255,255,255,255)
  love.graphics.rectangle("fill", player1.x, player1.y, player1.width, player1.height)
  love.graphics.rectangle("fill", player2.x, player2.y, player2.width, player2.height)
  love.graphics.rectangle("fill", ball.x, ball.y, ball.width, ball.height)
  love.graphics.print(gI,10,10)
end


function getPlayer(x)
  local player = {}
  player.x = x
  player.y = 150
  player.width = 10
  player.height = 200
  player.speed = 100
  return player
end

function createBall()
  local up = math.random(1,2)
  local right = math.random(1,2)
  local ball = {}
  ball.x = 245
  ball.y = 245
  ball.height = 10
  ball.width = 10
  ball.speed = 100
  ball.isDireactionR = false
  ball.isDireactionU = true
  
  if right == 1 then 
    ball.isDireactionR = true
  end
  if up == 1 then 
    ball.isDireactionR = false
  end
  
  return ball
end

function moveBall(ball,dt)
  local x = ball.x
  local y = ball.y
  PlayerBallCollision()
  
  if ball.isDireactionR then
    x = ball.x + ball.speed*dt
  else
    x = ball.x - ball.speed*dt
  end
  
  if ball.isDireactionU then
    y = ball.y + ball.speed*dt
  else
    y = ball.y - ball.speed*dt
  end
  
  hitBall(x,y)
end

function hitBall(x,y)
  local hitX = false
  local hitY = false
  
  if x < 0 then
    hitX = true 
  end
  if x > SCREEN_SIZE - ball.width then
    hitX = true
  end
  
  if y < 0 then
    hitY = true 
  end 
  if  y > SCREEN_SIZE - ball.height then
    hitY = true 
  end
  
  if hitX and hitY then
    InvertU()
    InvertR()
  elseif hitX and not hitY then
    InvertR()
    ball.y = y
  elseif hitY and not hitX then
    InvertU()
    ball.x = x
  else
    ball.x = x
    ball.y = y
  end
  
end

function sendData(data)
  client:send(data)
end

function serverData(data)
	
	if #Split(data,':') == 2 then
		res = Split(data,':')
		key = res[1]
		value = res[2]
	  gI = tostring(#res)
	  if key == "balldir" then
		ball.isDireactionR = toboolean(Split(value,'-')[1])
		ball.isDireactionU = toboolean(Split(value,'-')[2])
	  elseif key == "ballpos" then
		ball.x = tonumber(Split(value,'-')[1])
		ball.y = tonumber(Split(value,'-')[2])	
	  elseif key == "playerinfo" then
		player2.y = tonumber(value)
	  end
	end
end

function toboolean(value)
	if value == "true" then
		return true
	else
		return false
	end
end

function PlayerBallCollision()
  local ps1 = player1.x + player1.width
  local ps2 = player2.x + player2.width
  local bx = ball.x
  local by = ball.y
  
  if bx >= player1.x and bx <= ps1 then
    if by >= player1.y and by <= player1.y+player1.height then
      InvertU()
    end 
  end
  if bx >= player2.x and bx <= ps2 then
    if by >= player2.y and by <= player2.y+player2.height then
      InvertU()
    end
  end 
end

function Split(s, delimiter)
    result = {};
	if s or delimiter then
		for match in (s..delimiter):gmatch("(.-)"..delimiter) do
			table.insert(result, match);
		end
	end
    return result;
end

function InvertU()
  if ball.isDireactionU then
      ball.isDireactionU= false
  else 
      ball.isDireactionU = true
  end
end

function InvertR()
  if ball.isDireactionR then
      ball.isDireactionR = false
  else
      ball.isDireactionR = true
  end
end