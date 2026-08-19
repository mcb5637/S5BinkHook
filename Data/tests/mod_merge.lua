---@type Manifest
local a = {
    EntityTypes = {
        "a"
    },
    SettlerUpgradeCategory = {
        a = "a",
        c = "a",
    },
    SoundGroups = {
        {"a", "aa"},
        {"c", "aa"},
    },
    EntityTypeMerges = {
        {
            AppliesTo = {"aa", "a"},
            MergeFunc = function() end,
        },
        {
            AppliesTo = {"b"},
            MergeFunc = function() end,
        },
    },
    ConfigFileMerges = {
        {
            AppliesTo = {"a"},
            MergeFunc = function() end,
        },
    },
}
---@type Manifest
local b = {
    EntityTypes = {
        "b"
    },
    SettlerUpgradeCategory = {
        b = "b",
        c = "b",
    },
    SoundGroups = {
        {"b", "bb"},
        {"c", "bb"},
    },
    EntityTypeMerges = {
        {
            AppliesTo = {"bb"},
            MergeFunc = function() end,
        },
    },
    ConfigFileMerges = {
        {
            AppliesTo = {"a"},
            MergeFunc = function() end,
        },
        {
            AppliesTo = {"ab"},
            MergeFunc = function() end,
        },
    },
}

ModLoader.MergeManifest(a, b)

LuaDebugger.Log(a)

assert(a.EntityTypes[1] == "a")
assert(a.EntityTypes[2] == "b")
assert(a.SettlerUpgradeCategory.a == "a")
assert(a.SettlerUpgradeCategory.b == "b")
assert(a.SettlerUpgradeCategory.c == "b")
assert(a.SoundGroups[1][1] == "a")
assert(a.SoundGroups[3][1] == "b")
assert(a.SoundGroups[2][1] == "c")
assert(a.SoundGroups[2][2] == "bb")
assert(a.EntityTypeMerges[1].AppliesTo[1] == "aa")
assert(a.EntityTypeMerges[1].AppliesTo[2] == "a")
assert(a.EntityTypeMerges[2].AppliesTo[1] == "bb")
assert(a.EntityTypeMerges[3] == nil)
assert(a.ConfigFileMerges[1].AppliesTo[1] == "a")
assert(a.ConfigFileMerges[2].AppliesTo[1] == "a")
assert(a.ConfigFileMerges[3].AppliesTo[1] == "ab")
