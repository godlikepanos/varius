print("-- Script starting")

--[[Foo_mt = {}
function Foo_mt.__add(a, b)
	print("adding " .. tostring(a) .. " + " .. tostring(b))
end
function Foo_mt.__tostring(this)
	return "lalala"
end

Foo_methods = {}
function Foo_methods.method(self)
	print("Method for " .. tostring(self))
end

Foo_mt.__index = Foo_methods

Foo = {}
function Foo.new()
	local this = {}
	print("Foo.new")
	setmetatable(this, Foo_mt)
	return this
end
function Foo.method(self)
	print("Method for " .. tostring(self))
end

s1 = Foo.new()
print(s1)

s1:method()

c = s1 + 2

--[[ud = Foo_methods.new(123, 456)
out = Foo_methods.method(ud, 123)
print(out)
--setmetatable(ud, Foo_mt)]]--

--[[ud = Foo.new(123, 456)
ud:methodii(666)
print("-- " .. ud:methodstr("vraki"))

ud1 = Foo.new(1, 2)
ud1:methodargfoo(ud)

print("-- Copy")
a = ud1:copy(ud)
a:methodii(777)]]--

v = Vec2.new(1, 2)

v:print()
v:setX(v:getX() + 1)
v:print()


--print(v:getX())

print("-- end")
