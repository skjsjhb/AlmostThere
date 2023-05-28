MapVersion = 1
Duration = 15
-- Song = "youraudio.flac"
Background = "test.png"
use("at")
use("atm")
-----
make_ { CAMERA, id = "cam" }
---
out(POS, { 0, 0, 5 })
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, -1 })
usecam("cam")

----
make_ { TAPU, 0, 10, player = 1, len = 1 }
---
out(POS, { 0, 0, 0 })
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })
