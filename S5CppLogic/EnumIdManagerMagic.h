#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_idmanager.h"
#include "luaext.h"

#include <magic_enum.hpp>

namespace CppLogic::MagicEnum {
	template<class En>
	requires std::is_enum_v<En>
	class EnumIdManager {
	static constexpr const auto entries = magic_enum::enum_entries<En>();
	public:
		// does not add
		En GetIdByName(const char* name) const {
			auto v = magic_enum::enum_cast<En>(name);
			if (!v.has_value())
				return En::Invalid;
			return *v;
		}
		const char* GetNameByID(En id) const {
			return magic_enum::enum_name(id).data();
		}

		auto begin() const {
			return entries.begin();
		}
		auto end() const {
			return entries.end();
		}

		void PushToState(lua::State L) const {
			L.NewTable();
			for (const auto& [id, name] : *this) {
				L.Push(name);
				L.Push(static_cast<int>(id));
				L.SetTableRaw(-3);
			}
		}
	};
}
