-- flags: -DDITTO_TEST_FORM=26 -DDITTO_TEST_START_KIND=1 -DDITTO_TEST_NO_ENEMIES -DDITTO_TEST_REWARD=3 -DDITTO_TEST_ITEM=11
-- about: A SELECT hold drops the form and keeps the Rare Candy; a SELECT tap uses it.
PLAN = {
    { frames = 200 },
    { frames = 70, keys = { "UP" } }, { frames = 30 }, { shot = "1_picked_up" },
    { frames = 4, keys = { "START" } }, { frames = 20 }, { shot = "2_map_bag" },
    { frames = 4, keys = { "START" } }, { frames = 20 },
    { frames = 40, keys = { "SELECT" } }, { frames = 6 }, { shot = "3_hold_select" },
    { frames = 4, keys = { "START" } }, { frames = 20 }, { shot = "4_map_bag_after_hold" },
    { frames = 4, keys = { "START" } }, { frames = 20 },
    { frames = 4, keys = { "SELECT" } }, { frames = 6 }, { shot = "5_tap_select" },
    { frames = 4, keys = { "START" } }, { frames = 20 }, { shot = "6_map_bag_after_tap" },
}
dofile(os.getenv("E2E") .. "/driver.lua")
