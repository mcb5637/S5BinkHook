#include "ModPlayers.h"

#include <filesystem>
#include <shok/s5_exception.h>
#include <shok/s5_scriptsystem.h>
#include <shok/events/s5_events.h>
#include <shok/events/s5_netevents.h>
#include <shok/globals/s5_classfactory.h>
#include <shok/ui/s5_ui.h>
#include <shok/ui/s5_widget.h>

const BB::SerializationData CppLogic::Mod::Player::ExtraPlayerManager::ExtraPlayer::SerializationData[4] = {
    AutoMemberSerialization(ExtraPlayer, EPlayer),
    BB::SerializationData::ObjectPointerData("GPlayer", offsetof(ExtraPlayer, GPlayer), sizeof(GPlayer)),
    AutoMemberSerialization(ExtraPlayer, PlayerColorMapping),
    BB::SerializationData::GuardData(),
};

const BB::SerializationData CppLogic::Mod::Player::ExtraPlayerManager::PlayerRelationKey::SerializationData[3] = {
    AutoMemberSerialization(PlayerRelationKey, From),
    AutoMemberSerialization(PlayerRelationKey, To),
    BB::SerializationData::GuardData(),
};

const BB::SerializationData CppLogic::Mod::Player::ExtraPlayerManager::PlayerRelation::SerializationData[3] = {
    AutoMemberSerialization(PlayerRelation, Diplomacy),
    AutoMemberSerialization(PlayerRelation, ShareExploration),
    BB::SerializationData::GuardData(),
};

CreateSerializationListFor(CppLogic::Mod::Player::ExtraPlayerManager, ExtraPlayers);
CreateSerializationListFor(CppLogic::Mod::Player::ExtraPlayerManager, Diplomacy);
const BB::SerializationData CppLogic::Mod::Player::ExtraPlayerManager::SerializationData[3] = {
    AutoMemberSerialization(ExtraPlayerManager, ExtraPlayers),
    AutoMemberSerialization(ExtraPlayerManager, Diplomacy),
    BB::SerializationData::GuardData(),
};

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
        ExtraPlayers[i].PlayerColorMapping = static_cast<int>(i) + 8 + 1;
    }
}

shok::DiploState CppLogic::Mod::Player::ExtraPlayerManager::GetDiplomacy(shok::PlayerId p, shok::PlayerId p2) const {
    auto i = Diplomacy.find(PlayerRelationKey(p, p2));
    if (i == Diplomacy.end())
        return shok::DiploState::Neutral;
    return i->second.Diplomacy;
}

bool CppLogic::Mod::Player::ExtraPlayerManager::SetDiplomacy(shok::PlayerId p1, shok::PlayerId p2, shok::DiploState d) {
    if (GetDiplomacy(p1, p2) == d)
        return false;
    Diplomacy[PlayerRelationKey(p1, p2)].Diplomacy = d;
    GGL::CEventDiplomacyChanged e{shok::EventIDs::LogicEvent_DiplomacyChanged, p1, p2, d};
    (*EScr::CScriptTriggerSystem::GlobalObj)->PostEvent(&e);
    return true;
}

bool CppLogic::Mod::Player::ExtraPlayerManager::GetSharedExploration(shok::PlayerId p1, shok::PlayerId p2) const {
    if (p1 == p2)
        return true;
    auto i = Diplomacy.find(PlayerRelationKey(p1, p2));
    if (i == Diplomacy.end())
        return false;
    return i->second.ShareExploration;
}

void CppLogic::Mod::Player::ExtraPlayerManager::SetSharedExploration(shok::PlayerId p1, shok::PlayerId p2, bool b) {
    Diplomacy[PlayerRelationKey(p1, p2)].ShareExploration = b;
}

shok::PlayerId CppLogic::Mod::Player::ExtraPlayerManager::GetMaxPlayer() const {
    int id = static_cast<int>(ExtraPlayers.size());
    return static_cast<shok::PlayerId>(id + static_cast<size_t>(shok::PlayerId::P8));
}

void CppLogic::Mod::Player::ExtraPlayerManager::Serialize(const char *path, const char *savename) {
    SavegameName = savename;
    std::string p{ path };
    p.append(SaveGameFile);
    auto s = BB::CBinarySerializer::CreateUnique();
    try {
        BB::CFileStreamEx f{};
        if (f.OpenFile(p.c_str(), BB::IStream::Flags::DefaultWrite)) {
            s->SerializeByData(&f, this, SerializationData);
        }
        f.Close();
    }
    catch (const BB::CFileException& e) {
        char buff[201]{};
        e.CopyMessage(buff, 200);
        shok::LogString(__FUNCSIG__" error: %s\n", buff);
    }
}

void CppLogic::Mod::Player::ExtraPlayerManager::Deserialize(const char *path, const char *savename) {
    Clear();
    std::string p{ path };
    p.append(SaveGameFile);
    auto s = BB::CBinarySerializer::CreateUnique();
    try {
        BB::CFileStreamEx f{};
        if (f.OpenFile(p.c_str(), BB::IStream::Flags::DefaultRead)) {
            s->DeserializeByData(&f, this, SerializationData);
        }
        f.Close();
    }
    catch (const BB::CFileException& e) {
        char buff[201]{};
        e.CopyMessage(buff, 200);
        shok::LogString(__FUNCSIG__" error: %s\n", buff);
    }
    if (SavegameName != savename)
    {
        shok::LogString(__FUNCSIG__" save name does not match, is %s, should be %s\n", SavegameName.c_str(), savename);
        Clear();
        return;
    }
    auto max = GetMaxPlayer();
    auto* l = *GGL::CGLGameLogic::GlobalObj;
    for (auto pid = static_cast<shok::PlayerId>(9); pid < max; ++pid) {
        l->SetTechResearchedCallback(pid);
    }
}

void CppLogic::Mod::Player::ExtraPlayerManager::Hook(lua::CFunction getnumberofplayers) {
    EGL::PlayerManager::HookExtraPlayers();
    GGL::PlayerManager::HookExtraPlayers(getnumberofplayers);
    ED::CPlayerColors::HookExtraPlayers();
    GGUI::CManager::HookExtraPlayers();
    GGUI::CStatisticsRendererCustomWidget::HookFillPlayersToDisplay();
}
