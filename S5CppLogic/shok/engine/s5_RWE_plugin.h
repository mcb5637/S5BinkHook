#pragma once
#include <shok/engine/s5_RWEngine.h>

namespace RWE::Plugin {

	/**
	 * \ingroup rwplugin
	 * \ref RwPluginDataChunkWriteCallBack represents the function
	 * registered by \ref RwCameraRegisterPluginStream, etc. as the function that
	 * writes extension data to a binary stream.
	 *
	 * \param  stream   Pointer to the binary stream
	 *
	 * \param  binaryLength   A RwInt32 value equal to the binary
	 * size (in bytes) of the extension data that will be written to the binary
	 * stream.
	 *
	 * \param  object   Pointer to the object containing the extension
	 * data.
	 *
	 * \param  offsetInObject   A RwInt32 value equal to the byte
	 * offset of the extension data in the object.
	 *
	 * \param  sizeInObject   A RwInt32 value equal to the size
	 * (in bytes) of the extension data.
	 *
	 * \return Pointer to the stream
	 */
	typedef RwStream* (*RwPluginDataChunkWriteCallBack)(RwStream* stream, int32_t binaryLength, const void* object, int32_t offsetInObject, int32_t sizeInObject);

	/**
	 * \ingroup rwplugin
	 * \ref RwPluginDataChunkReadCallBack represents the function
	 * registered by \ref RwCameraRegisterPluginStream, etc. as the function that
	 * reads extension data from a binary stream.
	 *
	 * \param  stream   Pointer to the binary stream
	 *
	 * \param  binaryLength   A RwInt32 value equal to the binary
	 * size (in bytes) of the extension data that will be read from a binary
	 * stream.
	 *
	 * \param  object   Pointer to the object containing the extension
	 * data.
	 *
	 * \param  offsetInObject   A RwInt32 value equal to the byte
	 * offset of the extension data in the object.
	 *
	 * \param  sizeInObject   A RwInt32 value equal to the size
	 * (in bytes) of the extension data.
	 *
	 * \return Pointer to the stream
	 */
	typedef RwStream* (*RwPluginDataChunkReadCallBack)(RwStream* stream, int32_t binaryLength, void* object, int32_t offsetInObject, int32_t sizeInObject);

	/**
	 * \ingroup rwplugin
	 * \ref RwPluginDataChunkGetSizeCallBack represents the callback
	 * registered by \ref RwCameraRegisterPluginStream, etc. as the function that
	 * determines the binary size of the extension data.
	 *
	 * \param  object   Pointer to the object containing the extension data.
	 *
	 * \param  offsetInObject   A RwInt32 value equal to the byte
	 * offset of the extension data in the object.
	 *
	 * \param  sizeInObject   A RwInt32 value equal to the size
	 * (in bytes) of the extension data.
	 *
	 * \return A RwInt32 value equal to the size in bytes of the plugin extension data.
	 */
	typedef int32_t(*RwPluginDataChunkGetSizeCallBack)(const void* object, int32_t offsetInObject, int32_t sizeInObject);

	/**
	 * \ingroup rwplugin
	 * \ref RwPluginDataChunkAlwaysCallBack represents the callback
	 * registered by \ref RwCameraSetStreamAlwaysCallBack, etc. as the
	 * function that is called after the reading of plugin stream data is
	 * finished (useful to set up plugin data for plugins that found no
	 * data in the stream, but that cannot set up the data during the
	 * \ref RwPluginObjectConstructor callback).
	 *
	 * \param  object   Pointer to the object containing the extension data.
	 *
	 * \param  offsetInObject   A RwInt32 value equal to the byte
	 * offset of the extension data in the object.
	 *
	 * \param  sizeInObject   A RwInt32 value equal to the size
	 * (in bytes) of the extension data.
	 *
	 * \return Returns TRUE if successful, FALSE otherwise.
	 */
	typedef int(*RwPluginDataChunkAlwaysCallBack)(void* object, int32_t offsetInObject, int32_t sizeInObject);

	/**
	 * \ingroup rwplugin
	 * \ref RwPluginDataChunkRightsCallBack represents the callback
	 * registered by RwCameraSetStreamRightsCallBack, etc. as the
	 * function that is called after the reading of plugin stream data is
	 * finished, and the object finalised, if and only if the object's rights
	 * id was equal to that of the plugin registering the call.
	 * For convience the extension data is passed to the callback.
	 *
	 * \param  object   Pointer to the object containing the extension data.
	 *
	 * \param  offsetInObject   A RwInt32 value equal to the byte
	 * offset of the extension data in the object.
	 *
	 * \param  sizeInObject   A RwInt32 value equal to the size
	 * (in bytes) of the extension data.
	 *
	 * \param  extraData     An RwUInt32 writen with the plugin id.
	 *
	 * \return Returns TRUE if successful, FALSE otherwise.
	 */
	typedef int(*RwPluginDataChunkRightsCallBack)(void* object, int32_t offsetInObject, int32_t sizeInObject, int32_t extraData);

	/**
	 * \ingroup rwplugin
	 * \ref RwPluginObjectConstructor represents the callback
	 * registered by \ref RwEngineRegisterPlugin, \ref RwCameraRegisterPlugin, etc.
	 * as the function that initializes either the global extension data (in the
	 * case of \ref RwEngineRegisterPlugin) or the object extension data (in all
	 * other cases). Registered by \ref RwCameraSetStreamAlwaysCallBack, etc.
	 *
	 * \param  object   Pointer to the object (global or otherwise)
	 * that contains the extension data.
	 *
	 * \param  offsetInObject   A RwInt32 value equal to the
	 * byte offset of the extension data in the object.
	 *
	 * \param  sizeInObject   A RwInt32 value equal to the size
	 * (in bytes) of the extension data.
	 *
	 * \return Pointer to the object
	 */
	typedef void* (*RwPluginObjectConstructor)(void* object, int32_t offsetInObject, int32_t sizeInObject);

	/**
	 * \ingroup rwplugin
	 * \ref RwPluginObjectCopy represents the callback registered by
	 * \ref RwCameraRegisterPlugin, etc. as the function that copies the object
	 * extension data when an object is duplicated.
	 *
	 * \param  dstObject   Pointer to the destination object that will
	 * receive the extension data.
	 *
	 * \param  srcObject   Pointer to the source object containing
	 * extension data.
	 *
	 * \param  offsetInObject   A RwInt32 value equal to the byte offset
	 * of the extension data in the object.
	 *
	 * \param  sizeInObject   A RwInt32 value equal to the size
	 * (in bytes) of the extension data.
	 *
	 * \return Pointer to the object
	 */
	typedef void* (*RwPluginObjectCopy)(void* dstObject, const void* srcObject, int32_t offsetInObject, int32_t sizeInObject);

	/**
	 * \ingroup rwplugin
	 * \ref RwPluginObjectDestructor represents the callback registered
	 * by \ref RwEngineRegisterPlugin, \ref RwCameraRegisterPlugin, etc. as the
	 * function that destroys either the global extension data (in the case of
	 * \ref RwEngineRegisterPlugin) or the object extension data (in all other
	 * cases).
	 *
	 * \param  object   Pointer to the object (global or otherwise)
	 * containing the extension data.
	 *
	 * \param  offsetInObject   A RwInt32 value equal to the byte
	 * offset of the extension data in the object.
	 *
	 * \param  sizeInObject   A RwInt32 value equal to the size
	 * (in bytes) of the extension data.
	 *
	 * \return Pointer to the object.
	 */
	typedef void* (*RwPluginObjectDestructor)(void* object, int32_t offsetInObject, int32_t sizeInObject);

	typedef void* (*RwPluginErrorStrCallBack)(void*);

	struct RwPluginRegEntry;
	struct RwPluginRegistry
	{
		int32_t          sizeOfStruct;
		int32_t          origSizeOfStruct;
		int32_t          maxSizeOfStruct;
		int32_t          staticAlloc;
		RwPluginRegEntry* firstRegEntry;
		RwPluginRegEntry* lastRegEntry;


		RWE::Plugin::RwPluginRegistry* frameTKList = reinterpret_cast<RWE::Plugin::RwPluginRegistry*>(0x8019A0);
		RWE::Plugin::RwPluginRegistry* clumpTKList = reinterpret_cast<RWE::Plugin::RwPluginRegistry*>(0x842528);
		RWE::Plugin::RwPluginRegistry* atomicTKList = reinterpret_cast<RWE::Plugin::RwPluginRegistry*>(0x842510);

		// 4147D0 _rwPluginRegistryAddPlugin
		// 4149E0 _rwPluginRegistryInitObject
		// 414A40 _rwPluginRegistryDeInitObject
		// 414A70 _rwPluginRegistryCopyObject
		// 625DE0 _rwPluginRegistryAddPluginStream
		// 4147A0 _rwPluginRegistryGetPluginOffset
		// 40fce0 RwEngineRegisterPlugin
	};

	struct RwPluginRegEntry
	{
		int32_t         offset;
		int32_t         size;
		uint32_t        pluginID;
		RwPluginDataChunkReadCallBack readCB;
		RwPluginDataChunkWriteCallBack writeCB;
		RwPluginDataChunkGetSizeCallBack getSizeCB;
		RwPluginDataChunkAlwaysCallBack alwaysCB;
		RwPluginDataChunkRightsCallBack rightsCB;
		RwPluginObjectConstructor constructCB;
		RwPluginObjectDestructor destructCB;
		RwPluginObjectCopy copyCB;
		RwPluginErrorStrCallBack errStrCB;
		RwPluginRegEntry* nextRegEntry;
		RwPluginRegEntry* prevRegEntry;
		RwPluginRegistry* parentRegistry;
	};

}
