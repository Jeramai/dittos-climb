-- flags: -DDITTO_TEST_FLOOR=2 -DDITTO_TEST_START_KIND=2 -DDITTO_TEST_BOSS_HP=1 -DDITTO_TEST_FORM=24
-- about: Venusaur faints on the stairs; DITTO drops its form, takes the outline on the stairs, and goes up only on A.
PLAN = {
    { frames = 200 },
    { frames = 24, keys = { "UP" } }, { frames = 60 }, { shot = "1_boss_card" },
    { frames = 300 }, { shot = "2_fight" },
    { frames = 2, keys = { "UP" } }, { frames = 60, keys = { "A" } }, { frames = 30 }, { shot = "3_defeated" },
    { frames = 40, keys = { "SELECT" } }, { frames = 10 }, { shot = "4_dropped_form" },
    { frames = 18, keys = { "UP" } }, { frames = 10 }, { shot = "5_on_stairs_with_outline" },
    { frames = 4, keys = { "B" } }, { frames = 45 }, { shot = "6_transformed_on_stairs" },
    { frames = 60 }, { shot = "7_still_on_stairs" },
    { frames = 4, keys = { "A" } }, { frames = 150 }, { shot = "8_next_floor" },
}
dofile(os.getenv("E2E") .. "/driver.lua")
