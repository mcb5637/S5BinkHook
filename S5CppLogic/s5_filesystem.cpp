#include "pch.h"
#include "s5_filesystem.h"

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

bool __stdcall BB::IStream::rettrue()
{
	return true;
}
bool __stdcall BB::IStream::rettrue1()
{
	return true;
}
bool __stdcall BB::IStream::rettrue2()
{
	return true;
}
const char* __stdcall BB::IStream::GetFileName()
{
	return (*reinterpret_cast<const char* (__stdcall***)(BB::IStream*)>(this))[4](this);
}
int64_t __stdcall BB::IStream::GetLastWriteTime()
{
	return (*reinterpret_cast<int64_t(__stdcall***)(BB::IStream*)>(this))[5](this);
}
size_t __stdcall BB::IStream::GetSize()
{
	return (*reinterpret_cast<size_t(__stdcall***)(BB::IStream*)>(this))[6](this);
}
void __stdcall BB::IStream::SetFileSize(long size)
{
	return (*reinterpret_cast<void(__stdcall***)(BB::IStream*, long)>(this))[7](this, size);
}
long __stdcall BB::IStream::GetFilePointer()
{
	return (*reinterpret_cast<long(__stdcall***)(BB::IStream*)>(this))[8](this);
}
void __stdcall BB::IStream::SetFilePointer(long fp)
{
	return (*reinterpret_cast<void(__stdcall***)(BB::IStream*, long)>(this))[9](this, fp);
}
long __stdcall BB::IStream::Read(void* buff, long numBytesToRead)
{
	return (*reinterpret_cast<long(__stdcall***)(BB::IStream*, void*, long)>(this))[10](this, buff, numBytesToRead);
}
void __stdcall BB::IStream::Seek(long seek, SeekMode mode)
{
	return (*reinterpret_cast<void(__stdcall***)(BB::IStream*, long, SeekMode)>(this))[11](this, seek, mode);
}
void __stdcall BB::IStream::Write(const void* buff, long numBytesToWrite)
{
	return (*reinterpret_cast<void(__stdcall***)(BB::IStream*, const void*, long)>(this))[12](this, buff, numBytesToWrite);
}

BB::CFileStream::CFileStream()
{
	*reinterpret_cast<int*>(this) = vtp;
}
inline void(__thiscall* const filestream_dtor)(BB::CFileStream* f) = reinterpret_cast<void(__thiscall*)(BB::CFileStream*)>(0x548DBB);
BB::CFileStream::~CFileStream()
{
	filestream_dtor(this);
}
static inline bool(__thiscall* const filestream_open)(BB::CFileStream* th, const char* n, BB::IStream::Flags f) = reinterpret_cast<bool(__thiscall*)(BB::CFileStream*, const char*, BB::IStream::Flags)>(0x548DEE);
bool BB::CFileStream::OpenFile(const char* name, Flags mode)
{
	return filestream_open(this, name, mode);
}

inline void(__thiscall* const memstream_dtor)(BB::CMemoryStream* th) = reinterpret_cast<void(__thiscall*)(BB::CMemoryStream*)>(0x54EEB0);
BB::CMemoryStream::~CMemoryStream()
{
	memstream_dtor(this);
}
BB::CMemoryStream::CMemoryStream()
{
	*reinterpret_cast<int*>(this) = vtp;
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

static inline bool(__thiscall* const shok_BB_CFileStreamEx_OpenFile)(BB::CFileStreamEx* th, const char* name, BB::CFileStreamEx::Flags m) = reinterpret_cast<bool(__thiscall*)(BB::CFileStreamEx*, const char*, BB::CFileStreamEx::Flags)>(0x54924D);
static inline void(__thiscall* const shok_BB_CFileStreamEx_Close)(BB::CFileStreamEx* th) = reinterpret_cast<void(__thiscall*)(BB::CFileStreamEx*)>(0x54920A);
BB::CFileStreamEx::CFileStreamEx()
{
	*reinterpret_cast<int*>(this) = BB::CFileStreamEx::vtp;
}
inline void(__thiscall* const filestreamex_dtor)(BB::CFileStreamEx* f) = reinterpret_cast<void(__thiscall*)(BB::CFileStreamEx*)>(0x549215);
BB::CFileStreamEx::~CFileStreamEx()
{
	filestreamex_dtor(this);
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

const char* BB::CFileSystemMgr::ReadFileToString(const char* name, size_t* size)
{
	char* buff = nullptr;
	try
	{
		BB::CFileStreamEx filestr{};
		if (filestr.OpenFile(name, BB::CFileStreamEx::Flags::DefaultRead)) {
			size_t s = filestr.GetSize();
			if (size)
				*size = s;
			if (s > 0) {
				buff = new char[s + 1];
				memset(buff, 0, s + 1);
				filestr.Read(buff, s);
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
	FileInfo i{};
	(*GlobalObj)->GetFileInfo(&i, name, 0);
	return i.Found && !i.IsDirectory;
}
