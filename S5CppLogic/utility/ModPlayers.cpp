#include "ModPlayers.h"

#include <shok/s5_scriptsystem.h>
#include <shok/events/s5_events.h>
#include <shok/events/s5_netevents.h>


CppLogic::Mod::Player::ExtraPlayerManager& CppLogic::Mod::Player::ExtraPlayerManager::GlobalObj() {
    static ExtraPlayerManager m{};
    return m;
}

CppLogic::Mod::Player::ExtraPlayerManager::ExtraPlayer& CppLogic::Mod::Player::ExtraPlayerManager::GetExtra(shok::PlayerId p) {
    return ExtraPlayers.at(static_cast<size_t>(p) - static_cast<size_t>(shok::PlayerId::P8) - 1);
}

CppLogic::Mod::Player::ExtraPlayerManager::ExtraPlayer* CppLogic::Mod::Player::ExtraPlayerManager::TryGetExtra(shok::PlayerId p) {
    const auto id = static_cast<size_t>(p) - static_cast<size_t>(shok::PlayerId::P8) - 1;
    if (id > ExtraPlayers.size())
        return nullptr;
    return &ExtraPlayers[id];
}

EGL::PlayerManager::Player & CppLogic::Mod::Player::ExtraPlayerManager::GetEGL(shok::PlayerId p) {
    if (p <= shok::PlayerId::P8)
        return (*EGL::CGLEGameLogic::GlobalObj)->PlayerMng->Players.Data[static_cast<size_t>(p)];
    auto&[e, g, c] = GetExtra(p);
    return e;
}

EGL::PlayerManager::Player * CppLogic::Mod::Player::ExtraPlayerManager::TryGetEGL(shok::PlayerId p) {
    if (p <= shok::PlayerId::P8)
        return &(*EGL::CGLEGameLogic::GlobalObj)->PlayerMng->Players.Data[static_cast<size_t>(p)];
    auto* ex = TryGetExtra(p);
    if (ex == nullptr)
        return nullptr;
    auto&[e, g, c] = *ex;
    return &e;
}

GGL::CPlayerStatus & CppLogic::Mod::Player::ExtraPlayerManager::GetGGL(shok::PlayerId p) {
    if (p <= shok::PlayerId::P8)
        return
            *(*GGL::CGLGameLogic::GlobalObj)->Players->GetPlayerRaw(p);
    auto&[e, g, c] = GetExtra(p);
    return *g;
}

GGL::CPlayerStatus * CppLogic::Mod::Player::ExtraPlayerManager::TryGetGGL(shok::PlayerId p) {
    if (p <= shok::PlayerId::P8)
        return (*GGL::CGLGameLogic::GlobalObj)->Players->GetPlayerRaw(p);
    auto* ex = TryGetExtra(p);
    if (ex == nullptr)
        return nullptr;
    auto&[e, g, c] = *ex;
    return g.get();
}

int & CppLogic::Mod::Player::ExtraPlayerManager::ColorMapping(shok::PlayerId p) {
    return ExtraPlayers.at(static_cast<size_t>(p) - static_cast<size_t>(shok::PlayerId::P8) - 1).PlayerColorMapping;
}

void CppLogic::Mod::Player::ExtraPlayerManager::Clear() {
    ExtraPlayers.clear();
}

void CppLogic::Mod::Player::ExtraPlayerManager::SetMaxPlayer(shok::PlayerId p) {
    ExtraPlayers.resize(static_cast<size_t>(p) - static_cast<size_t>(shok::PlayerId::P8));
    for (size_t i = 0; i < ExtraPlayers.size(); ++i) {
        ExtraPlayers[i].PlayerColorMapping = static_cast<int>(i) + 8;
    }
}

shok::DiploState CppLogic::Mod::Player::ExtraPlayerManager::GetDiplomacy(shok::PlayerId p, shok::PlayerId p2) {
    auto i = Diplomacy.find(std::pair(p, p2));
    if (i == Diplomacy.end())
        return shok::DiploState::Neutral;
    return i->second;
}

bool CppLogic::Mod::Player::ExtraPlayerManager::SetDiplomacy(shok::PlayerId p1, shok::PlayerId p2, shok::DiploState d) {
    if (GetDiplomacy(p1, p2) == d)
        return false;
    Diplomacy[std::pair(p1, p2)] = d;
    GGL::CEventDiplomacyChanged e{shok::EventIDs::LogicEvent_DiplomacyChanged, p1, p2, d};
    (*EScr::CScriptTriggerSystem::GlobalObj)->PostEvent(&e);
    return true;
}

shok::PlayerId CppLogic::Mod::Player::ExtraPlayerManager::GetMaxPlayer() const {
    int id = static_cast<int>(ExtraPlayers.size());
    return static_cast<shok::PlayerId>(id + static_cast<size_t>(shok::PlayerId::P8));
}

void CppLogic::Mod::Player::ExtraPlayerManager::Hook() {
    EGL::PlayerManager::HookExtraPlayers();
    GGL::PlayerManager::HookExtraPlayers();
    ED::CPlayerColors::HookExtraPlayers();
}
