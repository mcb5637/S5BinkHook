#pragma once
#include "s5data.h"

namespace ED {
	class ILandscape {
	public:
		virtual ~ILandscape() = default;
	private:
		virtual void unknown() = 0;
	public:
		virtual float GetTerrainHeightAtPosF(float x, float y) = 0;
		virtual float GetWaterHeightAtPosF(float x, float y) = 0;
		virtual float GetHigherTerrainOrWaterHeightAtPos(float x, float y) = 0;
	protected:
		virtual bool GetLandscapePosFromMousePos(void* cam, float* mousepos, shok::PositionRot* outpos, int flag) = 0;
	};

	class CLandscape : public ILandscape {
	public:
		EGL::CGLETerrainHiRes* TerrainHiRes;
		EGL::CGLETerrainLowRes* TerrainLowRes;
		EGL::LandscapeBlockingData* Blocking;
		int SizeDiv32X, SizeDiv32Y;
		int HiResSizeX, HiResSizeY;
		float WorldSizeX, WorldSizeY;

		static inline constexpr int vtp = 0x76A404;

		bool GetTerrainPosAtScreenCoords(shok::PositionRot& outpos, int x, int y); // r in this case is the terrain height at the position
		float GetTerrainHeightAtPos(const shok::Position& p);
		float GetWaterHeightAtPos(const shok::Position& p);

	private:
		virtual float retzero(int, int) = 0;
	};
	static_assert(sizeof(ED::CLandscape) == 10 * 4);

	class CGlobalsLogic {
	public:
		virtual ~CGlobalsLogic() = default;
	};
	class CGlobalsLogicEx : public CGlobalsLogic {
	public:
		EGL::CGLEGameLogic* GameLogic;
		PADDINGI(4); // p EGL::CGLEEntitiesDisplay, p EGL::CGLEEffectsDisplay, p EGL::CGLETerrainHiRes, p EGL::CGLETerrainLowRes
		EGL::LandscapeBlockingData* Blocking; // 6
		PADDINGI(2); // p EGL::CGLELandscape, p EGL::CTerrainVertexColors
		EGL::CRegionInfo* RegionInfo; // 9
		PADDINGI(1); // p EGL::CPlayerExplorationHandler
		ED::CLandscape* Landscape;
		// p ED::CLandscapeFogOfWar, 
		PADDINGI(7);
		ED::CVisibleEntityManager* VisibleEntityManager; // 19

		static inline constexpr int vtp = 0x769F74;

		void ToTerrainCoord(const shok::Position& p, int* out);
		bool IsCoordValid(int* out);
		EGL::CGLELandscape::BlockingMode GetBlocking(const shok::Position& p);

		static inline ED::CGlobalsLogicEx** const GlobalObj = reinterpret_cast<ED::CGlobalsLogicEx**>(0x8581EC);
	};

}
