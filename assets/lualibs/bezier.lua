local generatedCache = {}

local function bn(n, i, x)
    if i > n then
        return 0
    elseif i == 0 then
        return (1 - x) ^ n
    elseif i == 1 then
        return n * x * ((1 - x) ^ (n - 1))
    else
        return bn(n - 1, i, x) * (1 - x) + bn(n - 1, i - 1, x) * x
    end
end

local function genVal(points, t)
    local x = 0
    local y = 0
    for i = 1, 4 do
        x = x + (bn(3, i - 1, t) * points[i][1])
        y = y + (bn(3, i - 1, t) * points[i][2])
    end
    return x, y
end

local function genCoord(points)
    local tbl = {}
    for i = 1, 101 do
        local x, y = genVal(points, (i - 1) / 100)
        tbl[i] = {x, y}
    end
    return tbl
end

local function isBetween(t, a, b) return math.abs(b - a) >= math.abs(b - t) end

local function bezier(x1, y1, x2, y2)
    local id = x1 .. y1 .. x2 .. y2
    local f = generatedCache[id]
    if f ~= nil then return f end
    local points = {{0, 0}, {x1, y1}, {x2, y2}, {1, 1}}
    local vtb = genCoord(points)
    local nf = function(t)
        if t == 0 then
            return 0
        elseif t == 1 then
            return 1
        end
        for i = 1, 99 do
            if isBetween(t, vtb[i][1], vtb[i + 1][1]) then
                local cx1 = vtb[i][1]
                local cy1 = vtb[i][2]
                local cx2 = vtb[i + 1][1]
                local cy2 = vtb[i + 1][2]
                return (cy2 - cy1) * ((t - cx1) / (cx2 - cx1)) + cy1
            end
        end
        return 0
    end
    generatedCache[id] = nf
    return nf
end

return bezier
