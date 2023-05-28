-- Lua script system init

local innerRequire = require

local trustedLibNames = {
    "at", "atm", "atc", "bezier"
}

-- Block untrusted library loading
require = function(modName)
    for k, l in ipairs(trustedLibNames) do
        if l == modName then
            return innerRequire(modName)
        end
    end
    return nil
end

use = function(module, override)
    local mod = require(module)
    ---@diagnostic disable-next-line: unused-local
    for k, v in pairs(mod) do
        if override or (not _G[k]) then
            _G[k] = mod[k]
        end
    end
end
