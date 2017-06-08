life = 100
dead = false
attack = 100.1
hp = 60.0
str = "hello!"
local speed = 5
--pos = {70,80,90}
--pos2 = math.Vector2(70,80, 90)
--gameObject.SetLocalPosition(this,pos2)
--render = gameObject.GetComponent(this, "SpriteRenderer")
spr = gameObject.GetSprite(this)
--sprite.FlipX(spr)
--sprite.FlipY(spr)
--sprite.Tint(spr, {50, 30, 40, 255})
localBounds = sprite.GetLocalBounds(spr)
globalBounds = sprite.GetGlobalBounds(spr)
sprite.FlipX(this)
localPos = gameObject.GetLocalPosition(render)
globalPos = gameObject.GetGlobalPosition(this)
--gameObject.SetActive(this,false)
testbool = gameObject.IsActive(this)
print(testbool)
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
print(jiji)

Instantiate(this)
Instantiate(render)
go = Instantiate(this)
gameObject.SetTag(go,"bla2")
print(gameObject.GetTag(go))

function Start()
    local bla1local = "hello"
    bla2global = 50
end

function Update()
    print("updating...")
end
