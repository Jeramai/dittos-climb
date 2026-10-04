-- flags: -DDITTO_TEST_FORM=26 -DDITTO_TEST_START_KIND=1 -DDITTO_TEST_NO_ENEMIES -DDITTO_TEST_REWARD=3 -DDITTO_TEST_ITEM=11
-- about: Holding R shows the aim arrow in all 8 directions; it stays locked while moving and hides on release.
local directions = {
    { "right", { "RIGHT" } }, { "up_right", { "UP", "RIGHT" } }, { "up", { "UP" } }, { "up_left", { "UP", "LEFT" } },
    { "left", { "LEFT" } }, { "down_left", { "DOWN", "LEFT" } }, { "down", { "DOWN" } }, { "down_right", { "DOWN", "RIGHT" } },
}
PLAN = { { frames = 200 }, { shot = "00_no_r" } }
for index, direction in ipairs(directions) do
    PLAN[#PLAN + 1] = { frames = 4, keys = direction[2] }
    PLAN[#PLAN + 1] = { frames = 6, keys = { "R" } }
    PLAN[#PLAN + 1] = { shot = string.format("%02d_r_%s", index, direction[1]) }
    PLAN[#PLAN + 1] = { frames = 4 }
end
PLAN[#PLAN + 1] = { frames = 4, keys = { "UP" } }
PLAN[#PLAN + 1] = { frames = 30, keys = { "R", "RIGHT" } }
PLAN[#PLAN + 1] = { shot = "09_locked_up_moving_right" }
PLAN[#PLAN + 1] = { frames = 10 }
PLAN[#PLAN + 1] = { shot = "10_r_released" }
dofile(os.getenv("E2E") .. "/driver.lua")
