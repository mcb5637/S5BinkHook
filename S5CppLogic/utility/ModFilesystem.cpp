#include "pch.h"
#include "ModFilesystem.h"
#include <filesystem>

void CppLogic::Mod::FileSystem::RedirectFileSystem::Destroy()
{
	delete this;
}

void CppLogic::Mod::FileSystem::RedirectFileSystem::FillFilesInDirectory(shok::Set<shok::String>* files, const char* directoryName, SearchOptions opt)
{
	if ((opt & SearchOptions::SkipFiles) == SearchOptions::SkipFiles)
		return;
	std::string_view dir{ directoryName };
	for (auto& [n, r] : Redirects) {
		if (std::string_view(n).substr(0, dir.length()) == dir) { // fast check
			std::filesystem::path p{ n };
			std::filesystem::path name = p.filename();
			p.remove_filename();
			std::filesystem::path dirp(dir);
			dirp.append(""); // fix trailing // after remove_filename
			if (p == dirp) { // proper check
				auto fn = name.string();
				files->emplace(std::string_view(fn));
			}
		}
	}
}

BB::IFileSystem::FileInfo* CppLogic::Mod::FileSystem::RedirectFileSystem::GetFileInfo(FileInfo* out, const char* file, int zero, char* absPath)
{
	auto f = Redirects.find(std::string_view(file));
	if (f == Redirects.end()) {
		*out = FileInfo{};
		return out;
	}
	return (*BB::CFileSystemMgr::GlobalObj)->GetFileInfo(out, f->second.c_str(), zero, absPath);
}

BB::IStream* CppLogic::Mod::FileSystem::RedirectFileSystem::OpenFileStream(const char* path, BB::IStream::Flags mode)
{
	auto f = Redirects.find(std::string_view(path));
	if (f == Redirects.end()) {
		return nullptr;
	}
	return (*BB::CFileSystemMgr::GlobalObj)->OpenFileStream(f->second.c_str(), mode);
}

bool CppLogic::Mod::FileSystem::RedirectFileSystem::OpenFileHandle(const char* path, int* pHandle, size_t* psize)
{
	auto f = Redirects.find(std::string_view(path));
	if (f == Redirects.end()) {
		return false;
	}
	return (*BB::CFileSystemMgr::GlobalObj)->OpenFileHandle(f->second.c_str(), pHandle, psize);
}

CppLogic::Mod::FileSystem::RedirectFileSystem* CppLogic::Mod::FileSystem::RedirectFileSystem::CreateRedirectLayer(std::string_view s)
{
	auto* mng = (*BB::CFileSystemMgr::GlobalObj);
	auto sv = mng->LoadOrder.SaveVector();
	auto* r = new CppLogic::Mod::FileSystem::RedirectFileSystem(s);
	sv.Vector.push_back(r);
	for (size_t i = sv.Vector.size() - 1; i > 0; i--) {
		sv.Vector[i] = sv.Vector[i - 1];
	}
	sv.Vector[0] = r;
	return r;
}
