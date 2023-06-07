local bez = require("bezier")
local atlib = {}

-- In/out constants

local innerInc = __native_inc
__native_inc = nil

atlib.inc = function(what)
    return innerInc(what)
end

local innerOut = __native_out
__native_out = nil

atlib.out = function(what, value)
    innerOut(what, value)
end

-- Timing function

local t = {}

t.SQRT_X = function(i)
    return math.sqrt(i)
end

t.X = function(i)
    return i
end

t.X_2 = function(i)
    return i ^ 2
end

local valEm1 = math.exp(1) - 1
t.EXP_X = function(i)
    return (math.exp(i) - 1) / valEm1
end

t.X_3 = function(i)
    return i ^ 3
end

t.CBRT_X = function(i)
    return i ^ (1 / 3)
end

t.X_3_SYM = function(i)
    local x = i * 2 - 1
    return (x ^ 3 + 1) / 2
end

t.CBRT_X_SYM = function(i)
    local x = i * 2 - 1
    if x < 0 then
        return (-(-x) ^ (1 / 3) + 1) / 2
    else
        return (x ^ (1 / 3) + 1) / 2
    end
end

-- Pre-generated ease functions
t.EASE = bez(0.25, 0.1, 0.25, 1)

t.EASE_IN = bez(0.42, 0, 1, 1)

t.EASE_OUT = bez(0, 0, 0.58, 1)

t.EASE_IO = bez(0.42, 0, 0.58, 1)

t.genBez = bez

t.revert = function(f)
    return function(r)
        return f(1 - r)
    end
end

atlib.T = t;

atlib.move = function(tp, to, curve, start, fin, p)
    for i = 1, 3 do
        tp[i] = atlib.vary(tp[i], to[i], curve, start, fin, p)
    end
end

atlib.move_ = function(ina)
    atlib.move(ina.t or ina[1], ina.to or ina[2], ina.curve or ina[3] or t.X, ina.start or ina[4] or 0,
            ina.fin or ina[5] or 1, ina.p or ina[6] or innerInc(atlib.PCT))
end

-- Rotate functions

local lcRotate = __native_raw_rotate
__native_raw_rotate = nil

atlib.rotate = function(tp, axis, rad, curve, start, fin, p)
    local pr = atlib.vary(0, rad, curve, start, fin, p)
    lcRotate(tp, axis, pr)
end

atlib.rotate_ = function(ina)
    atlib.rotate(ina.t or ina[1], ina.axis or ina[2], ina.rad or ina[3], ina.curve or ina[4] or t.X,
            ina.start or ina[5] or 0, ina.fin or ina[6] or 1, ina.p or ina[7] or innerInc(atlib.PCT))
end

-- Timing control

atlib.cont = function(timings, f, values)
    local sec = #timings - 1
    if (#timings == #values) then
        for i = 1, sec do
            f(timings[i], timings[i + 1], values[i], values[i + 1])
        end
    else
        for i = 1, sec do
            f(timings[i], timings[i + 1], values[i])
        end
    end
end

atlib.cont_ = function(ina)
    atlib.cont(ina.t or ina[1], ina.f or ina[2], ina.v or ina[3] or {})
end

-- Varying

atlib.vary = function(v1, v2, curve, start, fin, p)
    if p > fin then
        return v2
    end
    if p < start then
        return v1
    end
    local fac = curve((p - start) / (fin - start))
    return v1 * (1 - fac) + v2 * fac;
end

atlib.vary_ = function(ina)
    return atlib.vary(ina.v1 or ina[1], ina.v2 or ina[2], ina.curve or ina[3] or t.X, ina.start or ina[4] or 0,
            ina.fin or ina[5] or 1, ina.p or ina[6] or innerInc(atlib.PCT))
end

-- Memo

local memoTbl = {}

atlib.memo = function(generator, deps)
    local reuse = true
    local id = atlib.inc(atlib.OBJID) or ""
    if memoTbl[id] ~= nil then
        for i = 1, #memoTbl[id][2] do
            if deps[i] ~= memoTbl[id][2][i] then
                reuse = false
                break
            end
        end
    else
        memoTbl[id] = {}
        reuse = false
    end

    if not reuse then
        memoTbl[id][1] = generator()
        memoTbl[id][2] = { table.unpack(deps) }
    end
    return memoTbl[id][1]
end

atlib.memo_ = function(ina)
    return atlib.memo(ina.gen or ina[1], ina.deps or ina[2] or {})
end


-- Constants
atlib.NONE = -2
atlib.TAPU = 0
atlib.PURESU = 1
atlib.SHIZUKU = 2
atlib.RANKU = 3
atlib.KYOZETSU = 4
atlib.HOSHI = 5
atlib.HASHI = 6
atlib.SUKI = 7

atlib.CIRCLE = 8;
atlib.EUREKA = 9;
atlib.LINE = 10;
atlib.CAMERA = -1

atlib.POS = "POS"
atlib.NORM = "NORMAL"
atlib.MODE = "RELMODE"
atlib.UP = "UP"
atlib.ALPHA = "ALPHA"
atlib.LEN = "LEN"
atlib.TIME = "TIME"
atlib.ATIME = "ATIME"
atlib.PCT = "PCT"
atlib.OBJID = "OBJID"

atlib.ABS = 0
atlib.RPJ = 1
atlib.PSP = 2
atlib.OFS = 3


-- Object generator

local objReg = __native_objreg;
__native_objreg = nil;

atlib.make = function(tp, gen, hit, id, player, rel, len, fak)
    objReg(tp, id, player, gen, hit, rel, len, fak)
end

atlib.make_ = function(ina)
    atlib.make(ina.tp or ina[1], ina.gen or ina[2], ina.hit or ina[3], ina.id or ina[4] or "", ina.player or ina[5] or 0,
            ina.rel or ina[6] or "", ina.len or ina[7] or 0, ina.fake or ina[8] or false)
end

local lcUseCam = __native_use_cam
__native_use_cam = nil

atlib.usecam = function(id)
    lcUseCam(id)
end

return atlib
