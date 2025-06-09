#include "pch.h"
#include "s5_filesystem.h"
#include "s5_exception.h"

#include "hooks.h"

static inline bool(__thiscall* const dirfilesys_filter_allowedI)(BB::CDirectoryFileSystem::FilterData* th, const char* f) = reinterpret_cast<bool(__thiscall*)(BB::CDirectoryFileSystem::FilterData*, const char*)>(0x5532C4);
bool BB::CDirectoryFileSystem::FilterData::IsAllowedCaseInsensitive(const char* filename)
{
	return dirfilesys_filter_allowedI(this, filename);
}
static inline bool(__thiscall* const dirfilesys_filter_allowed)(BB::CDirectoryFileSystem::FilterData* th, const char* f) = reinterpret_cast<bool(__thiscall*)(BB::CDirectoryFileSystem::FilterData*, const char*)>(0x5532F8);
bool BB::CDirectoryFileSystem::FilterData::IsAllowed(const char* filename)
{
	return dirfilesys_filter_allowed(this, filename);
}

inline BB::CBBArchiveFile::DirectoryEntry* (__thiscall* const archivefile_searchbyhash)(BB::CBBArchiveFile* th, const char* fn) = reinterpret_cast<BB::CBBArchiveFile::DirectoryEntry* (__thiscall*)(BB::CBBArchiveFile*, const char*)>(0x5512E9);
BB::CBBArchiveFile::DirectoryEntry* BB::CBBArchiveFile::SearchByHash(const char* filename)
{
	return archivefile_searchbyhash(this, filename);
}
BB::CBBArchiveFile::DirectoryEntry* BB::CBBArchiveFile::GetByOffset(size_t offset)
{
	DirectoryEntry* r = nullptr;
	void* d = DirectoryData;
	size_t o = offset;
	__asm {
		mov eax, d;
		add eax, o;
		add eax, 4;
		mov r, eax;
	};
	return r;
}

void __declspec(naked) archivefile_doublefreeasm() {
	__asm {
		push[esi + 0x10];
		mov eax, 0x5C2E2D;
		call eax;
		mov eax, 0;
		mov[esi + 0x10], eax;

		push[esi + 0x14];
		mov eax, 0x5C2E2D;
		call eax;
		mov eax, 0;
		mov[esi + 0x14], eax;

		push 0x55116C;
		ret;
	}
}
bool HookFixDoubleFree_Hooked = false;
void BB::CBBArchiveFile::HookFixDoubleFree()
{
	if (HookFixDoubleFree_Hooked)
		return;
	HookFixDoubleFree_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x55115C), 0x55116C - 0x55115C };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x55115C), &archivefile_doublefreeasm, reinterpret_cast<void*>(0x55116C));
}

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

std::unique_ptr<BB::CBBArchiveFile, CppLogic::DestroyCaller<BB::CBBArchiveFile>> BB::CBBArchiveFile::CreateUnique()
{
	return std::unique_ptr<CBBArchiveFile, CppLogic::DestroyCaller<CBBArchiveFile>>(Create());
}

void BB::CFileSystemMgr::AddArchive(const char* path)
{
	AddArchiveI(path, true);
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

std::pair<std::string_view, std::unique_ptr<BB::IStream>> BB::CFileSystemMgr::OpenFileStreamWithSource(const char* path, BB::IStream::Flags f)
{
	if (Override == nullptr && !PathIsAbsolute(path)) { // override seems unused by shok
		BB::IStream::Flags fnoex = f | BB::IStream::Flags::NoThrow;
		std::string_view p{ path };
		if (RemoveData && (p.starts_with("Data") || p.starts_with("data")) && (p[4] == '\\' || p[4] == '/')) {
			p = p.substr(5);
		}
		for (auto* fs : LoadOrder) {
			try {
				auto s = fs->OpenFileStreamUnique(p.data(), fnoex);
				if (s != nullptr) {
					std::string_view an = "";
					if (auto* as = dynamic_cast<BB::CBBArchiveFile*>(fs)) {
						an = as->ArchiveFile.Filename;
					}
					return { an, std::move(s) };
				}
			}
			catch (const BB::CFileException&) { // directory filesystem does still throw even with nothrow
			}
		}
	}
	return { "", OpenFileStreamUnique(path, f) };
}

bool __stdcall BB::CFileStream::rettrue()
{
	return true;
}
bool __stdcall BB::CFileStream::rettrue1()
{
	return true;
}
bool __stdcall BB::CFileStream::rettrue2()
{
	return true;
}
const char* __stdcall BB::CFileStream::GetFileName()
{
	return Filename;
}
int64_t __stdcall BB::CFileStream::GetLastWriteTime()
{
	return reinterpret_cast<int64_t(__stdcall*)(BB::CFileStream*)>(0x548C6F)(this);
}
size_t __stdcall BB::CFileStream::GetSize()
{
	return reinterpret_cast<size_t(__stdcall*)(BB::CFileStream*)>(0x548EE9)(this);
}
void __stdcall BB::CFileStream::SetFileSize(long size)
{
	reinterpret_cast<void(__stdcall*)(BB::CFileStream*, long)>(0x548F17)(this, size);
}
long __stdcall BB::CFileStream::GetFilePointer()
{
	return reinterpret_cast<long(__stdcall*)(BB::CFileStream*)>(0x548F57)(this);
}
void __stdcall BB::CFileStream::SetFilePointer(long fp)
{
	reinterpret_cast<void(__stdcall*)(BB::CFileStream*, long)>(0x548F89)(this, fp);
}
long __stdcall BB::CFileStream::Read(void* buff, long numBytesToRead)
{
	return reinterpret_cast<long(__stdcall*)(BB::CFileStream*, void*, long)>(0x548FBC)(this, buff, numBytesToRead);
}
int __stdcall BB::CFileStream::Seek(long seek, SeekMode mode)
{
	return reinterpret_cast<int(__stdcall*)(BB::CFileStream*, long, SeekMode)>(0x54900A)(this, seek, mode);
}
void __stdcall BB::CFileStream::Write(const void* buff, long numBytesToWrite)
{
	reinterpret_cast<void(__stdcall*)(BB::CFileStream*, const void*, long)>(0x54905A)(this, buff, numBytesToWrite);
}

BB::CFileStream::CFileStream()
{
	*reinterpret_cast<int*>(this) = vtp;
}

static inline bool(__thiscall* const filestream_open)(BB::CFileStream* th, const char* n, BB::IStream::Flags f) = reinterpret_cast<bool(__thiscall*)(BB::CFileStream*, const char*, BB::IStream::Flags)>(0x548DEE);
bool BB::CFileStream::OpenFile(const char* name, Flags mode)
{
	return filestream_open(this, name, mode);
}

BB::CMemoryStream::CMemoryStream()
{
	*reinterpret_cast<int*>(this) = vtp;
}

bool __stdcall BB::CMemoryStream::rettrue()
{
	return true;
}
bool __stdcall BB::CMemoryStream::rettrue1()
{
	return true;
}
bool __stdcall BB::CMemoryStream::rettrue2()
{
	return reinterpret_cast<bool(__stdcall*)(BB::CMemoryStream*)>(0x54EBF9)(this);
}
const char* __stdcall BB::CMemoryStream::GetFileName()
{
	return nullptr;
}
int64_t __stdcall BB::CMemoryStream::GetLastWriteTime()
{
	return 0;
}
size_t __stdcall BB::CMemoryStream::GetSize()
{
	return Size;
}
void __stdcall BB::CMemoryStream::SetFileSize(long size)
{
	reinterpret_cast<void(__stdcall*)(BB::CMemoryStream*, long)>(0x54EEE3)(this, size);
}
long __stdcall BB::CMemoryStream::GetFilePointer()
{
	return FilePointer;
}
void __stdcall BB::CMemoryStream::SetFilePointer(long fp)
{
	reinterpret_cast<void(__stdcall*)(BB::CMemoryStream*, long)>(0x54EC23)(this, fp);
}
long __stdcall BB::CMemoryStream::Read(void* buff, long numBytesToRead)
{
	return reinterpret_cast<long(__stdcall*)(BB::CMemoryStream*, void*, long)>(0x54EC3A)(this, buff, numBytesToRead);
}
int __stdcall BB::CMemoryStream::Seek(long seek, SeekMode mode)
{
	return reinterpret_cast<int(__stdcall*)(BB::CMemoryStream*, long, SeekMode)>(0x54EC86)(this, seek, mode);
}
void __stdcall BB::CMemoryStream::Write(const void* buff, long numBytesToWrite)
{
	reinterpret_cast<void(__stdcall*)(BB::CMemoryStream*, const void*, long)>(0x54EDF3)(this, buff, numBytesToWrite);
}

inline void(__thiscall* const memstream_copyto)(const BB::CMemoryStream* th, BB::IStream* to) = reinterpret_cast<void(__thiscall*)(const BB::CMemoryStream*, BB::IStream*)>(0x5A19D1);
void BB::CMemoryStream::CopyToStream(IStream& to) const
{
	memstream_copyto(this, &to);
}
inline void(__thiscall* const memstream_copyfrom)(BB::CMemoryStream* th, BB::IStream* fr) = reinterpret_cast<void(__thiscall*)(BB::CMemoryStream*, BB::IStream*)>(0x5A1B04);
void BB::CMemoryStream::CopyFromStream(IStream& from)
{
	memstream_copyfrom(this, &from);
}

std::unique_ptr<BB::IStream> BB::IFileSystem::OpenFileStreamUnique(const char* path, BB::IStream::Flags mode)
{
	return std::unique_ptr<IStream>(OpenFileStream(path, mode));
}



bool __stdcall BB::CFileStreamEx::rettrue()
{
	if (RealStream)
		return RealStream->rettrue();
	else
		return false;
}
bool __stdcall BB::CFileStreamEx::rettrue1()
{
	if (RealStream)
		return RealStream->rettrue1();
	else
		return false;
}
bool __stdcall BB::CFileStreamEx::rettrue2()
{
	if (RealStream)
		return RealStream->rettrue1();
	else
		return false;
}
const char* __stdcall BB::CFileStreamEx::GetFileName()
{
	if (RealStream)
		return RealStream->GetFileName();
	else
		return nullptr;
}
int64_t __stdcall BB::CFileStreamEx::GetLastWriteTime()
{
	if (RealStream)
		return RealStream->GetLastWriteTime();
	else
		return 0;
}
size_t __stdcall BB::CFileStreamEx::GetSize()
{
	if (RealStream)
		return RealStream->GetSize();
	else
		return 0;
}
void __stdcall BB::CFileStreamEx::SetFileSize(long size)
{
	if (RealStream)
		return RealStream->SetFileSize(size);
}
long __stdcall BB::CFileStreamEx::GetFilePointer()
{
	if (RealStream)
		return RealStream->GetFilePointer();
	else
		return 0;
}
void __stdcall BB::CFileStreamEx::SetFilePointer(long fp)
{
	if (RealStream)
		return RealStream->SetFilePointer(fp);
}
long __stdcall BB::CFileStreamEx::Read(void* buff, long numBytesToRead)
{
	if (RealStream)
		return RealStream->Read(buff, numBytesToRead);
	else
		return 0;
}
int __stdcall BB::CFileStreamEx::Seek(long seek, SeekMode mode)
{
	if (RealStream)
		return RealStream->Seek(seek, mode);
	else
		return 0;
}
void __stdcall BB::CFileStreamEx::Write(const void* buff, long numBytesToWrite)
{
	if (RealStream)
		return RealStream->Write(buff, numBytesToWrite);
}

static inline bool(__thiscall* const shok_BB_CFileStreamEx_OpenFile)(BB::CFileStreamEx* th, const char* name, BB::CFileStreamEx::Flags m) = reinterpret_cast<bool(__thiscall*)(BB::CFileStreamEx*, const char*, BB::CFileStreamEx::Flags)>(0x54924D);
static inline void(__thiscall* const shok_BB_CFileStreamEx_Close)(BB::CFileStreamEx* th) = reinterpret_cast<void(__thiscall*)(BB::CFileStreamEx*)>(0x54920A);
BB::CFileStreamEx::CFileStreamEx()
{
	*reinterpret_cast<int*>(this) = BB::CFileStreamEx::vtp;
}
bool BB::CFileStreamEx::OpenFile(const char* filename, Flags mode)
{
	return shok_BB_CFileStreamEx_OpenFile(this, filename, mode);
}
void BB::CFileStreamEx::Close()
{
	shok_BB_CFileStreamEx_Close(this);
}

BB::IFileSystem* BB::CFileSystemMgr::LoadorderTop = nullptr;

std::string BB::CFileSystemMgr::ReadFileToString(const char* name)
{
	std::string r{};
	BB::CFileStreamEx filestr{};
	if (filestr.OpenFile(name, BB::CFileStreamEx::Flags::DefaultRead)) {
		size_t s = filestr.GetSize();
		if (s > 0) {
			r.resize(s);
			filestr.Read(r.data(), s);
		}
		filestr.Close();
	}
	return r;
}

bool BB::CFileSystemMgr::DoesFileExist(const char* name)
{
	FileInfo i{};
	(*GlobalObj)->GetFileInfo(&i, name, 0);
	return i.Found && !i.IsDirectory;
}
