MapVersion = 1
Duration = 15
-- Song = "youraudio.flac"
use("at")
use("atm")
-----

make_ { EUREKA, 0, 15, "test", 0 }
---
local pos = { -10, -1, 0 }
local up = { 0, 1, 0 }
local norm = { 0, 0, 1 }
rotate_ { up, norm, rad(-180), start = 0, fin = 1 / 3 }
out(POS, pos)
out(UP, up)
out(NORM, norm)

----
make_ { CAMERA, id = "cam" }
---
out(POS, { 0, 0, 5 })
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, -1 })
usecam("cam")

----
make_ { PURESU, 1, 2, rel = "test", player = 1, len = 1 }
---
out(MODE, { PSP, ABS, ABS })
local pos = { 0, 10, 0 }
move_ { pos, { 0, 0, 0 } }
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })
