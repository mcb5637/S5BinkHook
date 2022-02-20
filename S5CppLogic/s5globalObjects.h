#pragma once
#include "s5data.h"




struct shok_EScr_CScriptTriggerSystem : shok_BB_IObject {
private:
	shok_BB_IPostEvent PostEvent;

public:
	static inline constexpr int vtp = 0x78667C;

	void RunTrigger(BB::CEvent* ev);

	static inline shok_EScr_CScriptTriggerSystem** const GlobalObj = reinterpret_cast<shok_EScr_CScriptTriggerSystem**>(0x895DEC);
};

struct shok_BB_IFileSystem : shok_object {

};
struct shok_BB_CDirectoryFileSystem : shok_BB_IFileSystem {
	char* Path;

	static inline constexpr int vtp = 0x7803B4;
};
struct shok_BB_CBBArchiveFile : shok_BB_IFileSystem {
	PADDINGI(2); // BB::CFileStream 761C98, then probably file handle
	char* Path;

	static inline constexpr int vtp = 0x77FABC;
};
struct shok_BB_CFileSystemMgr : shok_object {
	vector_padding;
	std::vector<shok_BB_IFileSystem*, shok::Allocator<shok_BB_IFileSystem*>> LoadOrder;

	static inline constexpr int vtp = 0x77F794;

	void AddFolder(const char* path);
	void AddArchive(const char* path);
	void RemoveTopArchive();
	// handle + size get set, use shok_BB_CFileSystemMgr::CloseHandle to close the file after you dont need it any more.
	// to read/write a file more easily, use shok_BB_CFileStreamEx.
	// remove data/ before usage, this func does not do that by itself.
	bool OpenFileAsHandle(const char* path, int& handle, size_t& size);
	static bool CloseHandle(int handle);

	static inline shok_BB_CFileSystemMgr** const GlobalObj = reinterpret_cast<shok_BB_CFileSystemMgr**>(0x88F088);
	static inline const char* (__cdecl* const PathGetExtension)(const char* path) = reinterpret_cast<const char* (__cdecl*)(const char*)>(0x40BAB3);
};

struct shok_color {
	byte R = 255, G = 255, B = 255, A = 255;

	shok_color(int r, int g, int b, int a);
};
static_assert(sizeof(shok_color) == 1 * 4);

struct shok_ED_CDisplayProps : shok_object {
	byte ShadowBlur;
	PADDING(3);
	int ShadowRasterSize;
	float ShadowStrength;
	shok_color InvalidPositionColorModulate;
	shok_color InvalidPositionColorEmissive;
	vector_padding;
	std::vector<shok_color, shok::Allocator<shok_color>> PlayerColor;
	vector_padding;
	std::vector<shok_color, shok::Allocator<shok_color>> MiniMapColor;
	float SelectionRadiusScaleForModelsWithDecal;
	int FogOfWarNeverSeenLuminance;
	int FogOfWarSeenLuminance;
	float AuraRadius;
	float AuraHeight;
	shok::String AuraTexture;
	int CommandAcknowledgementModel;
	int CommandAcknowledgementDuration;

	static inline constexpr int vtp = 0x7AE630;
};

struct shok_ED_CModelsProps : shok_object {


	static inline constexpr int vtp = 0x7AE60C;
};

struct shok_ED_CCameraEx : shok_object {
	PADDINGI(1); //1
	void* SomeCameraData;

	static inline constexpr int vtp = 0x769E64;
};

struct shok_ED_CPlayerColors : shok_object {

	PADDINGI(63);
	shok_color Colors[17]; // 8 player

	static inline constexpr int vtp = 0x76964C;

	shok_color GetColorByIndex(int i);
	void SetColorByIndex(int i, shok_color c);
	void RefreshPlayerColors();
};
struct shok_ED_CCommandAcknowledgements : shok_object {

	static inline constexpr int vtp = 0x7695C0;

	void ShowAck(const shok::Position& pos);
};
struct shok_modelinstance {
	enum class TransformOperation : int {
		Set = 0, // sets to identity, then performs the operation
		Multiply = 1, // the new matrix is performed last
		ReverseMultiply = 2, // the new matrix is performed first
	};


	PADDINGI(1);
	struct {
		PADDINGI(4);
		float Matrix[4 * 4];
	}*Transform;

	void Register();
	void Destroy();
	void Rotate(float r, TransformOperation op);
	void Rotate(float r, float* axis, TransformOperation op);
	void Scale(float* s, TransformOperation op); // 3 coordinates (order?)
	void Scale(float s, TransformOperation op);
	void Translate(const shok::Position& p, float height, TransformOperation op);
	void SetColorByPlayerID(int pl);
	void DisableShadow();
	void DisableParticleEffects();
	void DisableTerrainDecal();
	//argb =  r | (g << 8) | (b << 16) | (a << 24)
	void SetColorModulate(shok_color argb);
	void SetColorModulate(int a, int r, int g, int b);
};
struct shok_modeldata {

	shok_modelinstance* Instanciate() const;
};
struct shok_ED_CResourceManager : shok_object {
	PADDINGI(1); // conat char**?
	void* ModelData; // most likely a vector
	PADDINGI(3);

	static inline constexpr int vtp = 0x769824;

	const shok_modeldata* GetModelData(int modelid);
};
struct shok_ED_CWorld : shok_object {
	void* SomeRenderObj;

	static inline constexpr int vtp = 0x769E94;
};
struct shok_ED_CGlobalsBaseEx : shok_object {
	BB::CIDManagerEx* AnimManager;
	BB::CIDManagerEx* ModelManager;
	BB::CIDManagerEx* SpecialEffectManager; // lightning and snow textures?
	PADDINGI(1); // empty BB::CIDManagerEx
	shok_ED_CDisplayProps* DisplayProps; // 5
	BB::CIDManagerEx** EffectManager; // probably an object without vtable
	BB::CIDManagerEx** EntityTypeManager; // probably an object without vtable
	shok_ED_CModelsProps* ModelProps;
	PADDINGI(1); // p to something terrain related?
	PADDINGI(1); // p to something water related? // 10
	BBRw::CEngine* RWEngine; // p to BBRw::CEngine
	PADDINGI(1); // p to ED::CAuras
	shok_ED_CCameraEx* Camera;
	shok_ED_CCommandAcknowledgements* CommandAcks;
	PADDINGI(1); // p to ED::CEntitiesTypeFlags // 15
	PADDINGI(1); // p to ED::CGUIScene
	PADDINGI(1); // unknown
	PADDINGI(1); // p to ED::CLight
	PADDINGI(1); // unknown
	PADDINGI(1); // p to ED::COcclusionEffect
	PADDINGI(1); // p to ED::COrnamentalItems
	shok_ED_CPlayerColors* PlayerColors; // 22
	PADDINGI(3); // 25 p ED::CRenderSettingsEx
	shok_ED_CResourceManager* ResManager; // 26
	PADDINGI(7);
	shok_ED_CWorld* DisplayWorld;

	static inline constexpr int vtp = 0x769478;

	static inline shok_ED_CGlobalsBaseEx** const GlobalObj = reinterpret_cast<shok_ED_CGlobalsBaseEx**>(0x857E8C);
};
//constexpr int i = offsetof(shok_ED_CGlobalsBaseEx, DisplayWorld) / 4;
