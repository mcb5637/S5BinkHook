#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace BB {
	class IFileSystem {
	public:
		struct FileInfo {
			int DateTimeLow;
			int DateTimeHigh;
			int Size;
			bool Found;
			bool IsDirectory;
		};


		virtual ~IFileSystem() = default;
	private:
		virtual void unknownfunc0() = 0;
		virtual void unknownfunc1() = 0;
	public:
		virtual void GetFileInfo(FileInfo* out, const char* file, int zero=0, char* absPath=nullptr) = 0;
	protected:
		virtual void* OpenFileStream(const char* path, int mode) = 0;
		virtual bool OpenFileHandle(const char* path, int* pHandle, size_t* psize) = 0;
	};
	class CDirectoryFileSystemSpecialOpenFileOnly : public IFileSystem {

	};
	class CDirectoryFileSystem : public CDirectoryFileSystemSpecialOpenFileOnly {
	public:
		char* Path;

		static inline constexpr int vtp = 0x7803B4;
	};

	class IArchiveFile : public IFileSystem {
		// 2 more methods
	};
	class CBBArchiveFile : public IArchiveFile {
		PADDINGI(2); // BB::CFileStream 761C98, then probably file handle
		char* Path;

		static inline constexpr int vtp = 0x77FABC;
	};

	class IFileSystemMgr : public IFileSystem {
	protected:
		virtual void AddArchiveI(const char* path, bool onTop) = 0;
	private:
		virtual void unknownfunc3() = 0;
	protected:
		virtual void AddFolderI(const char* path, bool onTop, int unknown) = 0; // 8
	private:
		virtual void unknownfunc4() = 0;
	protected:
		virtual void RemoveTop() = 0; // 10
	private:
		virtual void unknownfunc5() = 0;
	public:
		virtual void MakeAbsolute(char* abs, const char* rel) = 0; // 12 seems only to work with external files, not with files in bba archives
	};
	class CFileSystemMgr : public IFileSystemMgr {
	public:
		shok::Vector<BB::IFileSystem*> LoadOrder;
		
		static inline constexpr int vtp = 0x77F794;

		void AddFolder(const char* path);
		void AddArchive(const char* path);
		void RemoveTopArchive();
		// handle + size get set, use BB::CFileSystemMgr::CloseHandle to close the file after you dont need it any more.
		// to read/write a file more easily, use BB::CFileStreamEx.
		// remove data/ before usage, this func does not do that by itself.
		bool OpenFileAsHandle(const char* path, int& handle, size_t& size);
		static bool CloseHandle(int handle);

		static inline BB::CFileSystemMgr** const GlobalObj = reinterpret_cast<BB::CFileSystemMgr**>(0x88F088);
		static inline const char* (__cdecl* const PathGetExtension)(const char* path) = reinterpret_cast<const char* (__cdecl*)(const char*)>(0x40BAB3);


		static const char* ReadFileToString(const char* name, size_t* size);
		static bool DoesFileExist(const char* name);
	};

	class IStream {
	public:
		virtual ~IStream() = default;
		// 12 more methods
	};
	class CFileStreamEx : public IStream { // todo research about modes?, better read and exists files?
		int x = 0;

	public:
		static constexpr int vtp = 0x761C60;

		CFileStreamEx();
		bool OpenFile(const char* filename, int u);
		size_t GetSize();
		int ReadToBuffer(void* buff, size_t s);
		void Close();
	};
}
