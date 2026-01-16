#pragma once
#include <memory>
#include <shok/s5_forwardDecls.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/player/s5_player.h>

// modloader allocates additional players

// EGL::CGLEGameLogic::StartMap loads max number of players from mapdata.xml, override
// then activates EGL players

// search for access 4d6a71

// GGL has a callback that initializes GGL player after EGL one

// anything else?

namespace CppLogic::Mod::Player {
    class ExtraPlayerManager {
        struct ExtraPlayer {
            EGL::PlayerManager::Player EPlayer;
            std::unique_ptr<GGL::CPlayerStatus> GPlayer;
        };

        std::vector<ExtraPlayer> ExtraPlayers;

    public:
        static ExtraPlayerManager& GlobalObj();

        ExtraPlayer& GetExtra(shok::PlayerId p);
        ExtraPlayer* TryGetExtra(shok::PlayerId p);
        std::pair<EGL::PlayerManager::Player&, GGL::CPlayerStatus&> Get(shok::PlayerId p);
        std::pair<EGL::PlayerManager::Player*, GGL::CPlayerStatus*> TryGet(shok::PlayerId p);
        void Clear();
        void SetMaxPlayer(shok::PlayerId p);
    };
}