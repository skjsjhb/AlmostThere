MapVersion = 1
Duration = 15
use("at")
-----
make_ { EUREKA, 0, 15, "test", 0 }
---
local pos = { 0, 0, 0 }
local up = { 0, 1, 0 }
local norm = { 0, 0, 1 }
out(POS, pos)
out(UP, up)
out(NORM, norm)
