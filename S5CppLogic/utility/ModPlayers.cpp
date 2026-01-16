#include "ModPlayers.h"


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

std::pair<EGL::PlayerManager::Player&, GGL::CPlayerStatus&> CppLogic::Mod::Player::ExtraPlayerManager::Get(shok::PlayerId p) {
    if (p <= shok::PlayerId::P8)
        return {
            (*EGL::CGLEGameLogic::GlobalObj)->PlayerMng->Players.Data[static_cast<size_t>(p)],
            *(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(p),
        };
    auto&[e, g] = GetExtra(p);
    return {e, *g};
}

std::pair<EGL::PlayerManager::Player*, GGL::CPlayerStatus*> CppLogic::Mod::Player::ExtraPlayerManager::TryGet(shok::PlayerId p) {
    if (p <= shok::PlayerId::P8)
        return {
            &(*EGL::CGLEGameLogic::GlobalObj)->PlayerMng->Players.Data[static_cast<size_t>(p)],
            (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(p),
        };
    auto* ex = TryGetExtra(p);
    if (ex == nullptr)
        return {nullptr, nullptr};
    auto&[e, g] = *ex;
    return {&e, g.get()};
}

void CppLogic::Mod::Player::ExtraPlayerManager::Clear() {
    ExtraPlayers.clear();
}

void CppLogic::Mod::Player::ExtraPlayerManager::SetMaxPlayer(shok::PlayerId p) {
    ExtraPlayers.resize(static_cast<size_t>(p) - static_cast<size_t>(shok::PlayerId::P8));
}
