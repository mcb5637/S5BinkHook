#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/globals/s5_filesystem.h>
#include <string>
#include <string_view>

namespace CppLogic::Mod::FileSystem {
	class RedirectFileSystem : public BB::IFileSystem {
	public:

		virtual ~RedirectFileSystem() override = default;
		virtual void Destroy() override;
		virtual void FillFilesInDirectory(shok::Set<shok::String>* files, const char* directoryName, SearchOptions opt) override;
		virtual FileInfo* GetFileInfo(FileInfo* out, const char* file, int zero = 0, char* absPath = nullptr) override;
		virtual BB::IStream* OpenFileStream(const char* path, BB::IStream::Flags mode) override;
		virtual bool OpenFileHandle(const char* path, int* pHandle, size_t* psize) override;

		const std::string Name;
		std::map<std::string, std::string, CppLogic::CaselessStringComparator> Redirects;

		inline explicit RedirectFileSystem(std::string_view n) : Name(n) {}

		static RedirectFileSystem* CreateRedirectLayer(std::string_view s);
	};
}
