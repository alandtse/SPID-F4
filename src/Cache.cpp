#include "Cache.h"

namespace Cache
{
	EditorID* EditorID::GetSingleton()
	{
		static EditorID singleton;
		return std::addressof(singleton);
	}

	void EditorID::FillMap()
	{
		const auto& [map, lock] = RE::TESForm::GetAllFormsByEditorID();
		const RE::BSAutoReadLock locker{ lock };
		if (map) {
			for (auto& [id, form] : *map) {
				_editorIDToFormIDMap.emplace(id.c_str(), form->GetFormID());
				_formIDToEditorIDMap.emplace(form->GetFormID(), id.c_str());
			}
		}
	}

	RE::FormID EditorID::GetFormID(const std::string& a_editorID)
	{
		Locker locker(_lock);
		auto it = _editorIDToFormIDMap.find(a_editorID);
		return it != _editorIDToFormIDMap.end() ? it->second : 0;
	}

	std::string EditorID::GetEditorID(RE::FormID a_formID)
	{
		Locker locker(_lock);
		auto it = _formIDToEditorIDMap.find(a_formID);
		return it != _formIDToEditorIDMap.end() ? it->second : std::string();
	}

	std::string FormType::GetWhitelistFormString(const RE::FormType a_type)
	{
		auto it = whitelistMap.find(a_type);
		return it != whitelistMap.end() ? std::string(it->second) : std::string();
	}

	std::string FormType::GetBlacklistFormString(const RE::FormType a_type)
	{
		auto it = blacklistMap.find(a_type);
		return it != blacklistMap.end() ? std::string(it->second) : std::string();
	}
}
