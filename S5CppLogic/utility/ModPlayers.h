#pragma once
#include <map>
#include <memory>
#include <shok/s5_forwardDecls.h>
#include <shok/globals/s5_mapdisplay.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/player/s5_player.h>

namespace CppLogic::Mod::Player {
    class ExtraPlayerManager {
        struct ExtraPlayer {
            EGL::PlayerManager::Player EPlayer;
            std::unique_ptr<GGL::CPlayerStatus> GPlayer;
            int PlayerColorMapping;

            static const BB::SerializationData SerializationData[];
        };
        struct PlayerRelationKey {
            shok::PlayerId From, To;

            static const BB::SerializationData SerializationData[];

            constexpr auto operator<=>(const PlayerRelationKey&) const = default;
        };

        std::string SavegameName;
        std::vector<ExtraPlayer> ExtraPlayers;
        std::map<PlayerRelationKey, shok::DiploState> Diplomacy;

        static const BB::SerializationData SerializationData[];

        SerializationListFriend;

        static constexpr std::string_view SaveGameFile = "\\CppLogicExtraPlayers.bin";

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
        void Serialize(const char* path, const char* savename);
        void Deserialize(const char* path, const char* savename);

        static void Hook(lua::CFunction getnumberofplayers);
    };
}