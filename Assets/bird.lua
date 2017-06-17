life = 100
dead = false
attack = 100.1
hp = 60.0
str = "hello!"
local speed = 5
--pos = {70,80,90}
--pos2 = math.Vector2(70,80, 90)
--gameObject.SetLocalPosition(this,pos2)
--render = gameObject.GetComponent(this2, "SpriteRenderer")
--print(spr)
--print(this)

--sprite.Tint(spr, {50, 30, 40, 255})
localBounds = gameObject.GetSpriteLocalBounds(this)
globalBounds = gameObject.GetSpriteGlobalBounds(this)
gameObject.FlipSpriteX(this)
localPos = gameObject.GetPosition(render)
globalPos = gameObject.GetWorldPosition(this)
gameObject.SetActive(this,false)
testbool = gameObject.IsActive(this)
--print(testbool)
go8 = gameObject.GetParent(this)
go2 = gameObject.GetParent(this, "New GameObject(2)")
go3 = gameObject.GetRoot(this)
gameObject.SetParent(this, go2)
go4 = gameObject.GetChild(this, "New GameObject(4)")
childscount = gameObject.ChildsCount(this)
go5 = gameObject.GetChildByIndex(this, 3)
--gameObject.SetSprite(this, "AttackIcon.png")
--[[print(gameObject.GetComponent("SpriteRender"))
render = gameObject.GetComponent("SpriteRenderer")
if render == nil then
	print("is nil")
else
	print("not nil")
end]]
--efff = gameObject.GetSelf()
--print(efff.GetTag())
gameObject.SetName(this,"bird")
gameObject.SetTag(this,"birdTag")
tag = gameObject.GetTag(this)

Instantiate(this)
Instantiate(render)
go = Instantiate(this)
gameObject.SetTag(go,"bla2")
--print(gameObject.GetTag(go))
gameObject.SetActive(this,true)
--gameObject.PlayAnimation(this, "bird anim2")

function Start()
    local bla1local = "hello"
    bla2global = 50
	local a = 1
	while (a < 10)
	do
		pos = gameObject.GetPosition(this)
		pos[1] = a
		gameObject.SetPosition(this, pos)
		a = a+1
	end
end

function Update()
    --print("updating...")
	if input.IsKeyRepeated("RIGHT_ARROW") then
		pos = gameObject.GetPosition(this)
		pos[1] = pos[1] + 2.5
		gameObject.SetPosition(this, pos)
	end
	if input.IsMouseButtonRepeated(0) then
		pos = gameObject.GetPosition(this)
		pos[2] = pos[2] + 2.5
		gameObject.SetPosition(this, pos)
		gameObject.PlaySound(this, "Dead_1")
		gameObject.PlaySound(this, "Dead_4")
		gameObject.PlayMusic(this, "m_scene")
	end
	
	--[[if gameObject.IsAnimationFinished(this) then
		gameObject.StopAnimation(this)
		print("stopped animation bird anim2")
	end]]
end

function OnEnable()
	print("Enabled!")
end

function OnDisable()
	print("Disabled!")
end
