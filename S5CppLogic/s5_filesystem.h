#pragma once
#include "enumflags.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace BB {
	class IStream {
	public:
		enum class Flags : int {
			None = 0,

			// you need one of these
			CreateAlways = 1, // overrides previous file
			CreateNew = 2,
			OpenExisting = 3,
			OpenAlways = 4,

			// and one of these
			GenericRead = 0x10,
			GenericWrite = 0x20,

			// and one of these
			ShareRead = 0x100,
			ShareWrite = 0x200,

			// optional
			WriteThrough = 0x1000,
			RandomAccess = 0x2000,
			SequentialScan = 0x4000,

			// premade
			DefaultRead = 0x113,
			DefaultWrite = 0x121,
		};
		enum class SeekMode : int {
			Begin = 0,
			Current = 1,
			End = 2,
		};


		virtual ~IStream() = default;
	private:
		virtual bool __stdcall rettrue();
		virtual bool __stdcall rettrue1();
		virtual bool __stdcall rettrue2();
	public:
		virtual const char* __stdcall GetFileName();
		virtual int64_t __stdcall GetLastWriteTime(); // 5 returns in eax and edx, but that should be fine, 0 on memorystream
		virtual size_t __stdcall GetSize();
		virtual void __stdcall SetFileSize(long size); // moves file pointer to eof
		virtual long __stdcall GetFilePointer();
		virtual void __stdcall SetFilePointer(long fp);
		virtual long __stdcall Read(void* buff, long numBytesToRead); // 10 returns num bytes read
		virtual void __stdcall Seek(long seek, SeekMode mode);
		virtual void __stdcall Write(const void* buff, long numBytesToWrite);
	};
	template<>
	class ::enum_is_flags<IStream::Flags> : public std::true_type {};

	class CFileStream : public IStream { // used to read files directly
		HANDLE Handle = 0;
	public:
		char* Filename = nullptr;
		static constexpr int vtp = 0x761C98;

		CFileStream();
		bool OpenFile(const char* name, Flags mode);
	};
	class CMemoryStream : public IStream { // read from archives
		void* Data = nullptr; // 1
		long Capactity = 0;
		long Size = 0;
		long FilePointer = 0;
		PADDINGI(1); // 5, flags, 1 readonly?
	public:
		static constexpr int vtp = 0x77F7CC;

		CMemoryStream();
		void CopyToStream(IStream& to) const;
		void CopyFromStream(IStream& from);
	};




	class IFileSystem {
	public:
		struct FileInfo {
			int DateTimeLow = 0;
			int DateTimeHigh = 0;
			int Size = 0;
			bool Found = false;
			bool IsDirectory = false;
		};
		enum class SearchOptions : int {
			None = 0,
			SkipDirectories = 1,
			SkipFiles = 2,
		};


		virtual ~IFileSystem() = default;
		virtual void Destroy() = 0;
		virtual void FillFilesInDirectory(shok::Set<shok::String>* files, const char* directoryName, SearchOptions opt) = 0;
		virtual void GetFileInfo(FileInfo* out, const char* file, int zero = 0, char* absPath = nullptr) = 0;
	protected:
		virtual IStream* OpenFileStream(const char* path, int mode) = 0;
		virtual bool OpenFileHandle(const char* path, int* pHandle, size_t* psize) = 0; // 5

		static inline constexpr int vtp = 0x77F778;
	};
	template<>
	class ::enum_is_flags<IFileSystem::SearchOptions> : public std::true_type {};

	class CDirectoryFileSystemSpecialOpenFileOnly : public IFileSystem {
	public:
		static inline constexpr int vtp = 0x780398;
	};
	class CDirectoryFileSystem : public CDirectoryFileSystemSpecialOpenFileOnly {
	public:
		char* Path;
		size_t PathLen;
		struct FilterData {
			struct {
				size_t Length;
				char Data[28];
			} Filters[5]; // not sure what these are good for

			bool IsAllowedCaseInsensitive(const char* path);
			bool IsAllowed(const char* path);
		} Filters;

		static inline constexpr int vtp = 0x7803B4;
	};
	static_assert(sizeof(CDirectoryFileSystem) == 43 * 4);

	class IArchiveFile : public IFileSystem {
	public:
		virtual void OpenArchive(const char* filename) = 0; // 6
		virtual void CloseArchive() = 0;
	};
	class CBBArchiveFile : public IArchiveFile { // uses CBBArchiveFileStream for compressed files, CMemoryStream for compressed files
	public:
		BB::CFileStream ArchiveFile;


		static inline constexpr int vtp = 0x77FABC;

		static inline CBBArchiveFile* (__stdcall* const Create)() = reinterpret_cast<CBBArchiveFile * (__stdcall*)()>(0x551701);
	};

	class IFileSystemMgr : public IFileSystem {
	protected:
		virtual void AddArchiveI(const char* path, bool onTop) = 0;
		virtual void SetOverrideArchive(const char* file) = 0; // better not use it, not sure how well it works
		virtual void AddFolderI(const char* path, bool readonly, shok::Set<shok::String>* filters) = 0; // 8
		virtual void Clear() = 0;
	public:
		virtual void RemoveTopArchive() = 0; // 10
		virtual void SetRemoveData() = 0;
		virtual void MakeAbsolute(char* abs, const char* rel) = 0; // 12 seems only to work with external files, not with files in bba archives
	};
	class CFileSystemMgr : public IFileSystemMgr {
	public:
		shok::Vector<BB::IFileSystem*> LoadOrder;
		BB::IFileSystem* Override; // 5
		bool RemoveData;
		
		static inline constexpr int vtp = 0x77F794;

		void AddFolder(const char* path);
		void AddArchive(const char* path);
		// handle + size get set, use BB::CFileSystemMgr::CloseHandle to close the file after you dont need it any more.
		// to read/write a file more easily, use BB::CFileStreamEx.
		// remove data/ before usage, this func does not do that by itself.
		bool OpenFileAsHandle(const char* path, int& handle, size_t& size);
		static bool CloseHandle(int handle);

		static inline BB::CFileSystemMgr** const GlobalObj = reinterpret_cast<BB::CFileSystemMgr**>(0x88F088);
		static inline const char* (__cdecl* const PathGetExtension)(const char* path) = reinterpret_cast<const char* (__cdecl*)(const char*)>(0x40BAB3);

		static BB::IFileSystem* LoadorderTop;

		static const char* ReadFileToString(const char* name, size_t* size);
		static bool DoesFileExist(const char* name);
	};
	//constexpr int i = offsetof(CFileSystemMgr, Override) / 4;


	// looks up file in BB::CFileSystemMgr::GlobalObj and wraps around it
	class CFileStreamEx : public IStream {
		IStream* RealStream = nullptr;

	public:
		static constexpr int vtp = 0x761C60;

		CFileStreamEx();
		bool OpenFile(const char* filename, Flags mode);
		void Close();
	};
	class CBBArchiveFileStream : public IStream {
		// 1 istream*, 3 size, 4 filename, 6+7 qword writetime, 6 filepointer
	public:
		static constexpr int vtp = 0x77FA80;
	};
}
