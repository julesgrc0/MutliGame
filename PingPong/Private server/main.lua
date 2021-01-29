local socket = require("socket")
local config = require "configuration"


server = assert(socket.udp())
server:setsockname('*', 101)
server:settimeout(0)


local gameStats = {}
gameStats.hit = 0
gameStats.maxFPS = 0
gameStats.minFPS = 500
gameStats.invertU = 0
gameStats.invertR = 0
local clientIP = nil
local clientPORT = nil
local SCREEN_SIZE = 500
function love.load()
  love.window.setTitle("PingPong")
  love.window.setMode(500,500,{
        fullscreen = false,
        resizable = false,
        centered = config.WINDOW_CENTERED
  })
  player1 = getPlayer(10)
  player2 = getPlayer(480)
  ball = createBall()
end

function love.update(dt)
  FPSStats(love.timer.getFPS())
  if config.SHOW_FPS then
    love.window.setTitle("PingPong - FPS:" .. tostring(love.timer.getFPS()))
  end
  if love.keyboard.isDown("left") or love.keyboard.isDown("up") then
    if not (player1.y - player1.speed * dt < 0) then
      player1.y = player1.y - player1.speed*dt
    end
  elseif love.keyboard.isDown("right") or love.keyboard.isDown("down") then
    if not (player1.y + player1.speed*dt > SCREEN_SIZE) then
      player1.y = player1.y + player1.speed*dt
    end
  end
  
  
  if clientIP then
    server:sendto(tostring(player1.y),clientIP,clientPORT)
  end
  
	data,ip,port = server:receivefrom()
	if data then
    clientData(data,ip,port)
	end
  moveBall(ball,dt)
  sendData("ballpos:"..ball.x.."-"..ball.y)
  sendData("balldir:"..tostring(ball.isDireactionR).."-"..tostring(ball.isDireactionU))
end


function love.draw()
  FPSStats(love.timer.getFPS())
  if config.SHOW_FPS then
    love.window.setTitle("PingPong - FPS:" .. tostring(love.timer.getFPS()))
  end
  love.graphics.setColor(255,255,255,255)
  love.graphics.rectangle("fill", player1.x, player1.y, player1.width, player1.height)
  if not config.SOLO then
    love.graphics.rectangle("fill", player2.x, player2.y, player2.width, player2.height)
  end
  love.graphics.rectangle("fill", ball.x, ball.y, ball.width, ball.height)
  if config.DEBUG then
    love.graphics.print("debug enabled\nhit: "..gameStats.hit.."\nFPS max: "..gameStats.maxFPS.."\nFPS min: "..gameStats.minFPS.."\nUp inversion: "..gameStats.invertU.."\nLeft inversion: "..gameStats.invertR,0,0)
  end
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

function sendData(data)
  if clientIP then
    server:sendto(data,clientIP,clientPORT)
  end
end

function Split(s, delimiter)
    result = {};
    for match in (s..delimiter):gmatch("(.-)"..delimiter) do
        table.insert(result, match);
    end
    return result;
end

function clientData(data,ip,port)
  if #Split(data,':') == 2 then
    
    res = Split(data,':')
    key = res[1]
    value = res[2]
    clientIP = ip
    clientPORT = port
    if key == "balldir" then
      sendData("balldir:"..tostring(ball.isDireactionR).."-"..tostring(ball.isDireactionU))
    elseif key == "ballpos" then
      sendData("ballpos:"..ball.x.."-"..ball.y)
    elseif key == "player" then
      player2.y = tonumber(value)
      sendData("playerinfo:"..player1.y)
    elseif key == "playerinfo" then
      sendData("playerinfo:"..player1.y)
    end
  end
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
  
  if not PlayerBallCollision() then
      hitBall(x,y)
  else
    ball.x = x
    ball.y = y
  end
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

function PlayerBallCollision()
  local ps1 = player1.x + player1.width
  local ps2 = player2.x + player2.width
  local bx = ball.x
  local by = ball.y
  
  if bx >= player1.x and bx <= ps1 then
    if by >= player1.y and by <= player1.y+player1.height then
      InvertU()
      return true
    end 
  end
  if bx >= player2.x and bx <= ps2 then
    if by >= player2.y and by <= player2.y+player2.height then
      InvertU()
      return true
    end
  end 
  return false
end

function InvertU()
  gameStats.hit = gameStats.hit+1
  gameStats.invertU = gameStats.invertU +1
  if ball.isDireactionU then
      ball.isDireactionU= false
  else 
      ball.isDireactionU = true
  end
end

function InvertR()
  gameStats.hit = gameStats.hit+1
  gameStats.invertR = gameStats.invertR +1
  if ball.isDireactionR then
      ball.isDireactionR = false
  else
      ball.isDireactionR = true
  end
end

function FPSStats(fps)
  gameStats.maxFPS = math.max(fps,gameStats.maxFPS)
  gameStats.minFPS = math.min(fps,gameStats.minFPS)
end