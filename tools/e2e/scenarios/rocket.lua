-- flags: -DDITTO_TEST_FLOOR=9 -DDITTO_TEST_START_KIND=2 -DDITTO_TEST_BOSS_HP=2 -DDITTO_TEST_FORM=24
-- about: Arbok and Weezing have 1 HP each; one faints alone and leaves, the other fights on, then both outlines appear.
PLAN = {
    { frames = 200 },
    { frames = 24, keys = { "UP" } }, { frames = 300 }, { shot = "1_fight" },
}
local aims = { { "UP" }, { "UP", "RIGHT" }, { "RIGHT" }, { "DOWN", "RIGHT" }, { "DOWN" }, { "DOWN", "LEFT" }, { "LEFT" }, { "UP", "LEFT" } }
for round = 1, 2 do
    for index, aim in ipairs(aims) do
        PLAN[#PLAN + 1] = { frames = 2, keys = aim }
        PLAN[#PLAN + 1] = { frames = 24, keys = { "A" } }
        PLAN[#PLAN + 1] = { shot = string.format("%d_%d_shot", round + 1, index) }
    end
end
PLAN[#PLAN + 1] = { frames = 120 }
PLAN[#PLAN + 1] = { shot = "4_end" }
dofile(os.getenv("E2E") .. "/driver.lua")
