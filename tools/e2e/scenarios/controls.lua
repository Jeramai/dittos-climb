-- flags: -DDITTO_TEST_FORM=26 -DDITTO_TEST_START_KIND=1 -DDITTO_TEST_NO_ENEMIES -DDITTO_TEST_REWARD=3 -DDITTO_TEST_ITEM=11
-- about: START opens the floor map, B opens the controls screen, B goes back, START resumes.
PLAN = {
    { frames = 200 },
    { frames = 4, keys = { "START" } }, { frames = 30 }, { shot = "1_floor_map" },
    { frames = 4, keys = { "B" } }, { frames = 30 }, { shot = "2_controls" },
    { frames = 4, keys = { "B" } }, { frames = 30 }, { shot = "3_back_to_map" },
    { frames = 4, keys = { "START" } }, { frames = 30 }, { shot = "4_resumed" },
}
dofile(os.getenv("E2E") .. "/driver.lua")
