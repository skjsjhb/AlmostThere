MapVersion = 1
Duration = 15
-- Song = "youraudio.flac"
Background = "test.png"
use("at")
use("atm")
-----
make_ { CAMERA, id = "cam" }
---
out(POS, { 0, 0, 50 })
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, -1 })
usecam("cam")

----
make_ { TAPU, 0, 2, player = 1, len = 1 }
---
pos = {0, 10, 0}
move_ {pos, {0, -5, 0}}
out(POS, pos)
out(UP, {0, 1, 0})
out(NORM, { 0, 0, 1 })

----
make_ { EUREKA, 0, 15 }
---
out(POS, {0, -5, 0})
out(UP, {0, 1, 0})
out(NORM, {0, 0, 1})
