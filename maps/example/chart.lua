MapVersion = 1
Duration = 12.5
SongName = "Test"
Artist = "Me"
Mapper = "You"
ID = "example"
DiffColor = "233,191,211,255"
DiffLevel = "Example Lv.10"
Audio = "test.mp3"
Background = "test.png"
Banner = "banner.png"
use("at")
use("atm")
-----
make_ { CAMERA, 0, 12.5, id = "cam" }
---
out(MODE, { ABS, ABS, ABS })
out(POS, { 0, -15, 25 })
out(UP, { 0, 0.866, 0.5 })
out(NORM, { 0, 0.5, -0.866 })
usecam("cam")
----
make_ { CAMERA, 0, 12.5, id = "base" }
---
local t = inc(ATIME) / 3;
out(MODE, { ABS, ABS, ABS })
out(POS, { 0, 0, 0 })
out(UP, { math.cos(t), math.sin(t), 0 })
out(NORM, { 0, 0, 1 })
----
make_ { LINE, 0, 12.5, id = "1", rel = "base" }
---
out(MODE, { PSP, RPJ, ABS })
out(POS, { 0, 6, 0 })
out(UP, { 0, -1, 0 })
out(NORM, { 0, 0, 1 })
----
make_ { LINE, 0, 12.5, id = "2", rel = "base" }
---
out(MODE, { PSP, RPJ, ABS })
out(POS, { -6, 0, 0 })
out(UP, { 1, 0, 0 })
out(NORM, { 0, 0, 1 })
----
make_ { LINE, 0, 12.5, id = "3", rel = "base" }
---
out(MODE, { PSP, RPJ, ABS })
out(POS, { 0, -6, 0 })
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })
----
make_ { LINE, 0, 12.5, id = "4", rel = "base" }
---
out(MODE, { PSP, RPJ, ABS })
out(POS, { 6, 0, 0 })
out(UP, { -1, 0, 0 })
out(NORM, { 0, 0, 1 })

----
make_ { TAPU, 0.636, 1.636, player = 1, rel = "1" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
make_ { TAPU, 1.988, 2.988, player = 1, rel = "2" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
make_ { TAPU, 3.341, 4.341, player = 1, rel = "3" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
make_ { TAPU, 4.693, 5.693, player = 1, rel = "4" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.045
make_ { RANKU, a - 1, a, player = 1, rel = "2" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.215
make_ { RANKU, a - 1, a, player = 1, rel = "2" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.384
make_ { RANKU, a - 1, a, player = 1, rel = "2" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.553
make_ { RANKU, a - 1, a, player = 1, rel = "2" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.722
make_ { RANKU, a - 1, a, player = 1, rel = "2" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.891
make_ { RANKU, a - 1, a, player = 1, rel = "2" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 8.060
make_ { RANKU, a - 1, a, player = 1, rel = "2" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 8.229
b = 1.353
make_ { RANKU, a - 1, a, player = 1, rel = "2" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.045 + b
make_ { RANKU, a - 1, a, player = 1, rel = "3" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.215 + b
make_ { RANKU, a - 1, a, player = 1, rel = "3" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.384 + b
make_ { RANKU, a - 1, a, player = 1, rel = "3" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.553 + b
make_ { RANKU, a - 1, a, player = 1, rel = "3" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.722 + b
make_ { RANKU, a - 1, a, player = 1, rel = "3" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 7.891 + b
make_ { RANKU, a - 1, a, player = 1, rel = "3" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 8.060 + b
make_ { RANKU, a - 1, a, player = 1, rel = "3" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 8.229 + b
b = nil
make_ { RANKU, a - 1, a, player = 1, rel = "3" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 9.750
make_ { HOSHI, a - 1, a, player = 1, rel = "2" }
---
pos = { 0, 0, 30 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })

----
local a = 10.257
make_ { HOSHI, a - 1, a, player = 1, rel = "2" }
---
pos = { 0, 0, 30 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })
----
local a = 10.765
make_ { HOSHI, a - 1, a, player = 1, rel = "2" }
---
pos = { 0, 0, 30 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })
----
make_ { PURESU, 10.103, 11.103, len = 0.5, player = 1, rel = "3" }
---
pos = { 0, 30, 0 }
move_ { pos, { 0, 0, 0 } }
out(MODE, { PSP, RPJ, ABS });
out(POS, pos)
out(UP, { 0, 1, 0 })
out(NORM, { 0, 0, 1 })




