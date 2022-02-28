#include "pch.h"
#include "s5data.h"


static inline void(__cdecl* const filesystem_addfolder)(const char* p, char* d) = reinterpret_cast<void(__cdecl*)(const char*, char*)>(0x546514);
void BB::CFileSystemMgr::AddFolder(const char* path)
{
	filesystem_addfolder(path, nullptr);
	auto v = LoadOrder.SaveVector();
	size_t l = v.Vector.size();
	BB::IFileSystem* last = v.Vector[l - 1];
	for (int i = l - 1; i > 0; i--) {
		v.Vector[i] = v.Vector[i - 1];
	}
	v.Vector[0] = last;
}

void BB::CFileSystemMgr::AddArchive(const char* path)
{
	AddArchiveI(path, true);
}

void BB::CFileSystemMgr::RemoveTopArchive()
{
	RemoveTop();
}

bool BB::CFileSystemMgr::OpenFileAsHandle(const char* path, int& handle, size_t& size)
{
	return OpenFileHandle(path, &handle, &size);
}

static inline bool(__cdecl* const file_closehandle)(int h) = reinterpret_cast<bool(__cdecl*)(int)>(0x54E9EC);
bool BB::CFileSystemMgr::CloseHandle(int handle)
{
	return file_closehandle(handle);
}

static inline bool(__thiscall* const shok_BB_CFileStreamEx_OpenFile)(BB::CFileStreamEx* th, const char* name, int unk) = reinterpret_cast<bool(__thiscall*)(BB::CFileStreamEx*, const char*, int)>(0x54924D);
static inline size_t(__stdcall* const shok_BB_CFileStreamEx_GetSize)(BB::CFileStreamEx* th) = reinterpret_cast<size_t(__stdcall*)(BB::CFileStreamEx*)>(0x549140);
static inline int(__stdcall* const shok_BB_CFileStreamEx_ReadToBuffer)(BB::CFileStreamEx* th, void* buff, size_t buffsiz) = reinterpret_cast<int(__stdcall*)(BB::CFileStreamEx*, void*, size_t)>(0x5491A8);
static inline void(__thiscall* const shok_BB_CFileStreamEx_Close)(BB::CFileStreamEx* th) = reinterpret_cast<void(__thiscall*)(BB::CFileStreamEx*)>(0x54920A);
BB::CFileStreamEx::CFileStreamEx()
{
	*reinterpret_cast<int*>(this) = BB::CFileStreamEx::vtp;
}
bool BB::CFileStreamEx::OpenFile(const char* filename, int u)
{
	return shok_BB_CFileStreamEx_OpenFile(this, filename, u);
}
size_t BB::CFileStreamEx::GetSize()
{
	return shok_BB_CFileStreamEx_GetSize(this);
}
int BB::CFileStreamEx::ReadToBuffer(void* buff, size_t s)
{
	return shok_BB_CFileStreamEx_ReadToBuffer(this, buff, s);
}
void BB::CFileStreamEx::Close()
{
	shok_BB_CFileStreamEx_Close(this);
}

const char* BB::CFileSystemMgr::ReadFileToString(const char* name, size_t* size)
{
	char* buff = nullptr;
	try
	{
		BB::CFileStreamEx filestr{};
		if (filestr.OpenFile(name, 0x10113)) {
			size_t s = filestr.GetSize();
			if (size)
				*size = s;
			if (s > 0) {
				buff = new char[s + 1];
				memset(buff, 0, s + 1);
				filestr.ReadToBuffer(buff, s);
			}
			filestr.Close();
		}
	}
	catch (...)
	{
		if (buff)
			delete[] buff;
		return nullptr;
	}
	return buff;
}

bool BB::CFileSystemMgr::DoesFileExist(const char* name)
{
	bool r = false;
	try
	{
		BB::CFileStreamEx filestr{};
		if (filestr.OpenFile(name, 0x10113)) {
			size_t s = filestr.GetSize();
			if (s > 0) {
				r = true;
			}
			filestr.Close();
		}
	}
	catch (...)
	{
		return false;
	}
	return r;
}
