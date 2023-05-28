local amath = {}

-- Fast sin/cos/t


amath.qsin = __native_qsin
amath.qcos = __native_qcos
amath.qtan = __native_qtan

__native_qsin = nil
__native_qcos = nil
__native_qtan = nil

amath.rad = function(deg)
    return deg / 180 * math.pi
end

amath.deg = function(rad)
    return rad / math.pi * 180
end

amath.cic = function(count)
    return count * 2 * math.pi
end

-- Vector operation

amath.vadd = function(a, b)
    local o = {}
    for i = 1, 3 do
        o[i] = a[i] + b[i]
    end
    return o;
end

amath.vaddh = function(a, b)
    for i = 1, 3 do
        a[i] = a[i] + b[i]
    end
end

amath.vsub = function(a, b)
    local o = {}
    for i = 1, 3 do
        o[i] = a[i] - b[i]
    end
    return o;
end

amath.vsubh = function(a, b)
    for i = 1, 3 do
        a[i] = a[i] - b[i]
    end
end

amath.vdot = function(a, b)
    local sum = 0
    for i = 1, 3 do
        sum = sum + a[i] * b[i]
    end
end

amath.vcross = function(a, b)
    local o = {}
    o[1] = a[2] * b[3] - a[3] * b[2]
    o[2] = a[3] * b[1] - a[1] * b[3]
    o[3] = a[1] * b[2] - a[2] * b[1]
    return o
end

amath.vscale = function(a, s)
    local o = {}
    for i = 1, 3 do
        o[i] = a[i] * s
    end
    return o
end

amath.vscaleh = function(a, s)
    for i = 1, 3 do
        a[i] = a[i] * s
    end
end

amath.vlen2 = function(a)
    return a[1] ^ 2 + a[2] ^ 2 + a[3] ^ 2
end

amath.vlen = function(a)
    return math.sqrt(amath.vlen2(a))
end

amath.vnorm = function(a)
    local o = {}
    local len = amath.vlen(a)
    if len == 0 then return a end
    for i = 1, 3 do
        o[i] = a[i] / len
    end
    return o
end

amath.vnormh = function(a)
    local len = amath.vlen(a)
    if len == 0 then return a end
    for i = 1, 3 do
        a[i] = a[i] / len
    end
end

-- Implemented in C
amath.vrotate = __native_raw_rotate
__native_raw_rotate = nil

amath.vrotateh = function(a, axis, rad)
    local o = amath.vrotate(a, axis, rad)
    for i = 1, 3 do
        a[i] = o[i]
    end
end


return amath
