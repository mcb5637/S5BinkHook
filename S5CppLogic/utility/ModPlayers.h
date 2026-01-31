#pragma once
#include <map>
#include <memory>
#include <shok/s5_forwardDecls.h>
#include <shok/globals/s5_mapdisplay.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/player/s5_player.h>

// modloader allocates additional players

// EGL::CGLEGameLogic::StartMap loads max number of players from mapdata.xml, override
// then activates EGL players

// GGL has a callback that initializes GGL player after EGL one

// anything else?

namespace CppLogic::Mod::Player {
    class ExtraPlayerManager {
        struct ExtraPlayer {
            EGL::PlayerManager::Player EPlayer;
            std::unique_ptr<GGL::CPlayerStatus> GPlayer;
            int PlayerColorMapping;
        };

        std::vector<ExtraPlayer> ExtraPlayers;
        std::map<std::pair<shok::PlayerId, shok::PlayerId>, shok::DiploState> Diplomacy;

    public:
        static ExtraPlayerManager& GlobalObj();

        ExtraPlayer& GetExtra(shok::PlayerId p);
        ExtraPlayer* TryGetExtra(shok::PlayerId p);
        EGL::PlayerManager::Player& GetEGL(shok::PlayerId p);
        EGL::PlayerManager::Player* TryGetEGL(shok::PlayerId p);
        GGL::CPlayerStatus& GetGGL(shok::PlayerId p);
        GGL::CPlayerStatus* TryGetGGL(shok::PlayerId p);
        int& ColorMapping(shok::PlayerId p);
        void Clear();
        void SetMaxPlayer(shok::PlayerId p);
        shok::DiploState GetDiplomacy(shok::PlayerId p, shok::PlayerId p2);
        bool SetDiplomacy(shok::PlayerId p1, shok::PlayerId p2, shok::DiploState d);
        [[nodiscard]] shok::PlayerId GetMaxPlayer() const;

        static void Hook();
    };
}