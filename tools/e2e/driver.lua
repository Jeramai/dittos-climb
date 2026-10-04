-- Plays PLAN one frame at a time. A step is { frames = n, keys = { ... } } or { shot = "name" }.
local KEY = { A = 0, B = 1, SELECT = 2, START = 3, RIGHT = 4, LEFT = 5, UP = 6, DOWN = 7, R = 8, L = 9 }
local OUT = os.getenv("OUT")
local RECORD = os.getenv("RECORD") == "1"
local MAX_FRAMES = 5000

-- Title screen, then skip the story.
local BOOT = { { frames = 60 }, { frames = 4, keys = { "START" } }, { frames = 30 }, { frames = 4, keys = { "START" } } }

local steps = {}
for _, step in ipairs(BOOT) do steps[#steps + 1] = step end
for _, step in ipairs(PLAN) do steps[#steps + 1] = step end

local function mask(keys)
    local result = 0
    for _, key in ipairs(keys or {}) do result = result | (1 << KEY[key]) end
    return result
end

local index, left, frame = 1, nil, 0

callbacks:add("frame", function()
    frame = frame + 1

    if frame > MAX_FRAMES then
        os.exit(1)
    end

    if RECORD and frame % 4 == 0 then
        emu:screenshot(string.format("%s/rec_%05d.png", OUT, frame // 4))
    end

    while true do
        local step = steps[index]

        if not step then
            os.exit(0)
        end

        if step.shot then
            emu:screenshot(OUT .. "/" .. step.shot .. ".png")
            index = index + 1
        else
            if left == nil then
                left = step.frames
                emu:setKeys(mask(step.keys))
            end

            left = left - 1

            if left <= 0 then
                left = nil
                index = index + 1
            end

            return
        end
    end
end)
