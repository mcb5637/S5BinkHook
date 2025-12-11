cmake_minimum_required(VERSION 4.0)
project(S5CppLogic)

enable_language(CXX ASM)

set(CMAKE_CXX_STANDARD 20)

find_package(magic_enum REQUIRED)

option(DEBUG_FUNCS OFF)

add_compile_options(-Wno-invalid-offsetof -Wno-microsoft-cast -Wno-microsoft-string-literal-from-predefined)

if(DEBUG_FUNCS)
    add_compile_options(-DDEBUG_FUNCS)
endif()

add_library(LuaDebugger SHARED
        S5CppLogic/dllmain.cpp
        S5CppLogic/luaext.cpp

        S5CppLogic/luafuncs/l_api.cpp
        S5CppLogic/luafuncs/l_combat.cpp
        S5CppLogic/luafuncs/l_effect.cpp
        S5CppLogic/luafuncs/l_entity.cpp
        S5CppLogic/luafuncs/l_entitytype.cpp
        S5CppLogic/luafuncs/l_logic.cpp
        S5CppLogic/luafuncs/l_mem.cpp
        S5CppLogic/luafuncs/l_tech.cpp
        S5CppLogic/luafuncs/l_ua.cpp
        S5CppLogic/luafuncs/l_ui.cpp

        S5CppLogic/Luapp/luapp50.cpp

        S5CppLogic/shok/s5_baseDefs.cpp
        S5CppLogic/shok/s5_datastructures.cpp
        S5CppLogic/shok/s5_scriptsystem.cpp

        S5CppLogic/shok/effect/s5_effects.cpp
        S5CppLogic/shok/effect/s5_effects_manual.asm
        S5CppLogic/shok/effect/s5_effecttype.cpp

        S5CppLogic/shok/engine/s5_BBRw.cpp
        S5CppLogic/shok/engine/s5_RWE_2d.cpp
        S5CppLogic/shok/engine/s5_RWE_anim.cpp
        S5CppLogic/shok/engine/s5_RWE_particles.cpp
        S5CppLogic/shok/engine/s5_RWEngine.cpp

        S5CppLogic/shok/entity/s5_animset.cpp
        S5CppLogic/shok/entity/s5_baseprops.cpp
        S5CppLogic/shok/entity/s5_behaviors.cpp
        S5CppLogic/shok/entity/s5_entity.cpp
        S5CppLogic/shok/entity/s5_entity_manual.asm
        S5CppLogic/shok/entity/s5_entitydisplay.cpp
        S5CppLogic/shok/entity/s5_tasklist.cpp

        S5CppLogic/shok/entitytype/s5_behaviorProps.cpp
        S5CppLogic/shok/entitytype/s5_behaviorProps_manual.asm
        S5CppLogic/shok/entitytype/s5_entitytype.cpp

        S5CppLogic/shok/events/s5_events.cpp
        S5CppLogic/shok/events/s5_netevents.cpp

        S5CppLogic/shok/globals/s5_classfactory.cpp
        S5CppLogic/shok/globals/s5_config.cpp
        S5CppLogic/shok/globals/s5_filesystem.cpp
        S5CppLogic/shok/globals/s5_filesystem_maual.asm
        S5CppLogic/shok/globals/s5_framework.cpp
        S5CppLogic/shok/globals/s5_glue.cpp
        S5CppLogic/shok/globals/s5_idmanager.cpp
        S5CppLogic/shok/globals/s5_mapdisplay.cpp
        S5CppLogic/shok/globals/s5_maplogic.cpp
        S5CppLogic/shok/globals/s5_staticlist.cpp

        S5CppLogic/shok/player/s5_ai.cpp
        S5CppLogic/shok/player/s5_player.cpp

        S5CppLogic/shok/sound/s5_sound.cpp

        S5CppLogic/shok/tech/s5_tech.cpp

        S5CppLogic/shok/ui/s5_guistates.cpp
        S5CppLogic/shok/ui/s5_ui.cpp
        S5CppLogic/shok/ui/s5_widget.cpp

        S5CppLogic/utility/EntityAddonData.cpp
        S5CppLogic/utility/entityiterator.cpp
        S5CppLogic/utility/hooks.cpp
        S5CppLogic/utility/luaserializer.cpp
        S5CppLogic/utility/ModBehavior.cpp
        S5CppLogic/utility/ModConfig.cpp
        S5CppLogic/utility/ModEffect.cpp
        S5CppLogic/utility/ModFilesystem.cpp
        S5CppLogic/utility/modloader.cpp
        S5CppLogic/utility/ModUI.cpp
        S5CppLogic/utility/savegame_extra.cpp
        S5CppLogic/utility/SchemaGenerator.cpp
        S5CppLogic/utility/StringUtil.cpp
        S5CppLogic/utility/WinAPIUtil.cpp

        S5CppLogic/Resource.rc
)

target_include_directories(LuaDebugger PRIVATE S5CppLogic)

target_link_libraries(LuaDebugger PRIVATE magic_enum::magic_enum "${CMAKE_CURRENT_SOURCE_DIR}/S5CppLogic/lua50/S5Lua5.lib")
