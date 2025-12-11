#pragma once
#include <memory>
#include <string_view>
#include <utility/enumflags.h>
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>

namespace BB {
	class IStream {
		friend class BB::CFileStreamEx;
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

			NoThrow = 0x10000,

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
		virtual bool __stdcall rettrue() = 0;
		virtual bool __stdcall rettrue1() = 0;
		virtual bool __stdcall rettrue2() = 0;
	public:
		virtual const char* __stdcall GetFileName() = 0;
		virtual int64_t __stdcall GetLastWriteTime() = 0; // 5 returns in eax and edx, but that should be fine, 0 on memorystream
		virtual size_t __stdcall GetSize() = 0;
		virtual void __stdcall SetFileSize(long size) = 0; // moves file pointer to eof
		virtual long __stdcall GetFilePointer() = 0;
		virtual void __stdcall SetFilePointer(long fp) = 0;
		virtual long __stdcall Read(void* buff, long numBytesToRead) = 0; // 10 returns num bytes read
		virtual int __stdcall Seek(long seek, SeekMode mode) = 0;
		virtual void __stdcall Write(const void* buff, long numBytesToWrite) = 0;

		static constexpr int vtp = 0x761C28;

		inline void Write(std::string_view data) {
			Write(data.data(), data.length());
		}
		inline void Indent(size_t indent, char c = '\t') {
			for (size_t i = 0; i < indent; ++i) {
				Write(&c, 1);
			}
		}
	};

	class CFileStream : public IStream { // used to read files directly
		HANDLE Handle = 0;
	public:
		char* Filename = nullptr;
		static constexpr int vtp = 0x761C98;


		virtual ~CFileStream() override;
	private:
		virtual bool __stdcall rettrue() override;
		virtual bool __stdcall rettrue1() override;
		virtual bool __stdcall rettrue2() override;
	public:
		virtual const char* __stdcall GetFileName() override;
		virtual int64_t __stdcall GetLastWriteTime() override;
		virtual size_t __stdcall GetSize() override;
		virtual void __stdcall SetFileSize(long size) override;
		virtual long __stdcall GetFilePointer() override;
		virtual void __stdcall SetFilePointer(long fp) override;
		virtual long __stdcall Read(void* buff, long numBytesToRead) override;
		virtual int __stdcall Seek(long seek, SeekMode mode) override;
		virtual void __stdcall Write(const void* buff, long numBytesToWrite) override;

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


		virtual ~CMemoryStream() override;
	private:
		virtual bool __stdcall rettrue() override;
		virtual bool __stdcall rettrue1() override;
		virtual bool __stdcall rettrue2() override;
	public:
		virtual const char* __stdcall GetFileName() override;
		virtual int64_t __stdcall GetLastWriteTime() override;
		virtual size_t __stdcall GetSize() override;
		virtual void __stdcall SetFileSize(long size) override;
		virtual long __stdcall GetFilePointer() override;
		virtual void __stdcall SetFilePointer(long fp) override;
		virtual long __stdcall Read(void* buff, long numBytesToRead) override;
		virtual int __stdcall Seek(long seek, SeekMode mode) override;
		virtual void __stdcall Write(const void* buff, long numBytesToWrite) override;

		CMemoryStream();
		void CopyToStream(IStream& to) const;
		void CopyFromStream(IStream& from);
		inline std::string_view GetData() {
			return std::string_view{ static_cast<char*>(Data), static_cast<size_t>(Size) };
		}
	};
	static_assert(sizeof(CMemoryStream) == 0x18);





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
		virtual FileInfo* GetFileInfo(FileInfo* out, const char* file, int zero = 0, char* absPath = nullptr) = 0;
		virtual IStream* OpenFileStream(const char* path, BB::IStream::Flags mode) = 0;
		virtual bool OpenFileHandle(const char* path, int* pHandle, size_t* psize) = 0; // 5

		std::unique_ptr<IStream> OpenFileStreamUnique(const char* path, BB::IStream::Flags mode);

		static inline constexpr int vtp = 0x77F778;
	};

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
	class CBBArchiveFile : public IArchiveFile { // uses CBBArchiveFileStream for uncompressed files, CMemoryStream for compressed files
	public:
		enum class FileType : int {
			FileUncompressed = 0,
			Directory = 1,
			FileCompressed = 2,
		};
		struct DirectoryEntry {
			FileType FType;
			size_t OwnOffset;
			size_t Size;
			uint16_t FilenameSize;
			uint16_t DirectoryPartSize;
			size_t FirstChildOffset;
			size_t NextSiblingOffset;
			uint64_t Timestamp;
			char Name[1];
		};
		struct HashTableEntry {
			uint32_t Hash;
			size_t DirOffset;
		};

		BB::CFileStream ArchiveFile;
		void* DirectoryData; // size, then DirectoryEntrys
		struct {
			size_t TableSize;
			HashTableEntry Entries[1];
		}* HashTable;

		static inline constexpr int vtp = 0x77FABC;

		DirectoryEntry* SearchByHash(const char* filename);
		DirectoryEntry* GetByOffset(size_t offset);

		static inline CBBArchiveFile* (__stdcall* const Create)() = reinterpret_cast<CBBArchiveFile * (__stdcall*)()>(0x551701);
		static std::unique_ptr<CBBArchiveFile, CppLogic::DestroyCaller<CBBArchiveFile>> CreateUnique();

		// len -1 for infinite (stops at \0)
		static inline uint32_t(__cdecl* const HashString)(const char* data, int len) = reinterpret_cast<uint32_t(__cdecl*)(const char*, int)>(0x547D90);

		static void HookFixDoubleFree();
	};
	static_assert(sizeof(CBBArchiveFile) == 6 * 4);

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
		std::pair<std::string_view, std::unique_ptr<BB::IStream>> OpenFileStreamWithSource(const char* path, BB::IStream::Flags f);

		static inline BB::CFileSystemMgr** const GlobalObj = reinterpret_cast<BB::CFileSystemMgr**>(0x88F088);
		static inline const char* (__cdecl* const PathGetExtension)(const char* path) = reinterpret_cast<const char* (__cdecl*)(const char*)>(0x40BAB3);
		static inline bool(__cdecl* const PathIsAbsolute)(const char* path) = reinterpret_cast<bool(__cdecl*)(const char*)>(0x54DE1C);

		static BB::IFileSystem* LoadorderTop;

		static std::string ReadFileToString(const char* name);
		static bool DoesFileExist(const char* name);

		// init filesystem 5466E1(const char* lang, int extraNum, 0, bool noBba)
	};
	//constexpr int i = offsetof(CFileSystemMgr, Override) / 4;


	// looks up file in BB::CFileSystemMgr::GlobalObj and wraps around it
	class CFileStreamEx : public IStream {
		IStream* RealStream = nullptr;

	public:
		static constexpr int vtp = 0x761C60;

		virtual ~CFileStreamEx() override;
	protected:
		virtual bool __stdcall rettrue() override;
		virtual bool __stdcall rettrue1() override;
		virtual bool __stdcall rettrue2() override;
	public:
		virtual const char* __stdcall GetFileName() override;
		virtual int64_t __stdcall GetLastWriteTime() override;
		virtual size_t __stdcall GetSize() override;
		virtual void __stdcall SetFileSize(long size) override;
		virtual long __stdcall GetFilePointer() override;
		virtual void __stdcall SetFilePointer(long fp) override;
		virtual long __stdcall Read(void* buff, long numBytesToRead) override;
		virtual int __stdcall Seek(long seek, SeekMode mode) override;
		virtual void __stdcall Write(const void* buff, long numBytesToWrite) override;

		CFileStreamEx();
		bool OpenFile(const char* filename, Flags mode);
		void Close();
	};
	class CBBArchiveFileStream : public IStream {
		// 1 istream*, 3 size, 4 filename, 6+7 qword writetime, 6 filepointer
	public:
		static constexpr int vtp = 0x77FA80;
	};

	class IOSFileSystem : public IFileSystem {

	};

	class COSFileSystem : public IOSFileSystem {
	public:
		static inline constexpr int vtp = 0x77FA50;
		static inline BB::COSFileSystem** const GlobalObj = reinterpret_cast<BB::COSFileSystem**>(0x8937DC);
	};
	static_assert(sizeof(COSFileSystem) == 4);
}

namespace CppLogic::IO {
	class StringViewReadStream : public BB::IStream {
	private:
		virtual bool __stdcall rettrue() override;
		virtual bool __stdcall rettrue1() override;
		virtual bool __stdcall rettrue2() override;
	public:
		virtual const char* __stdcall GetFileName() override;
		virtual int64_t __stdcall GetLastWriteTime() override;
		virtual size_t __stdcall GetSize() override;
		virtual void __stdcall SetFileSize(long size) override;
		virtual long __stdcall GetFilePointer() override;
		virtual void __stdcall SetFilePointer(long fp) override;
		virtual long __stdcall Read(void* buff, long numBytesToRead) override;
		virtual int __stdcall Seek(long seek, SeekMode mode) override;
		virtual void __stdcall Write(const void* buff, long numBytesToWrite) override;

		std::string_view Buffer;
		size_t CurrentPos;

		explicit StringViewReadStream(std::string_view data, size_t current = 0);
	};
}


template<>
class enum_is_flags<BB::IStream::Flags> : public std::true_type {};
template<>
class enum_is_flags<BB::IFileSystem::SearchOptions> : public std::true_type {};
