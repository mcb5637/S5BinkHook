#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_RWEngine.h"

namespace RWE::Anim {
	/**
	 * \ingroup rtanim
	 * \ref RtAnimKeyFrameApplyCallBack
	 * defines a callback function for converting
	 * an interpolated animation keyframe into the required result.
	 *
	 * \param result       Void pointer to store the output of the conversion.
	 * \param voidIFrame   Void pointer to the keyframe and should be cast
	 *                     to the interpolated keyframe type this callback
	 *                     is for.
	 */
	typedef void (*RtAnimKeyFrameApplyCallBack) (void* result, void* voidIFrame);

	/**
	 * \ingroup rtanim
	 * \ref RtAnimKeyFrameBlendCallBack
	 * defines a callback function for blending between two
	 * interpolated keyframes by the given blend factor. This is used for
	 * blending the states of two different \ref RtAnimInterpolator objects.
	 * The \ref RtAnimKeyFrameInterpolateCallBack is used for interpolating
	 * actual animation keyframes into an interpolated frame.
	 *
	 * \param voidOut      Void pointer to store the output of the blend.
	 * \param voidIn1      Void pointer containing the first input
	 *                      interpolated keyframe.
	 * \param voidIn2      Void pointer containing the second input
	 *                      interpolated keyframe.
	 * \param alpha        \ref RwReal containing the blend factor.
	 */
	typedef void (*RtAnimKeyFrameBlendCallBack) (void* voidOut, void* voidIn1, void* voidIn2, float alpha);

	/**
	 * \ingroup rtanim
	 * \ref RtAnimKeyFrameInterpolateCallBack
	 * defines a callback function for interpolating between two
	 * animation keyframes according to the given time. The output is
	 * an interpolated frame object residing in an \ref RtAnimInterpolator
	 * and will usually have the same structure as the keyframe apart from
	 * the header data (\ref RtAnimInterpFrameHeader).
	 *
	 * \param voidOut      Void pointer for the output of the interpolation.
	 * \param voidIn1      Void pointer containing the first input keyframe.
	 * \param voidIn2      Void pointer containing the second input keyframe.
	 * \param time         \ref RwReal containing the time at which to interpolate.
	 * \param customData   Void pointer to the custom data associated with this
	 *                     animation
	 */
	typedef void (*RtAnimKeyFrameInterpolateCallBack) (void* voidOut, void* voidIn1, void* voidIn2, float time, void* customData);

	/**
	 * \ingroup rtanim
	 * \ref RtAnimKeyFrameAddCallBack
	 * defines a callback function for adding two interpolated
	 * keyframes together. This is used when adding the states of two
	 * \ref RtAnimInterpolator objects, such as when adding a delta animation
	 * to a base animation.
	 *
	 * \param voidOut      Void pointer for the output interpolated frame.
	 * \param voidIn1      Void pointer containing the first input
	 *                      interpoalted keyframe.
	 * \param voidIn2      Void pointer containing the second input
	 *                      interpolated keyframe.
	 */
	typedef void (*RtAnimKeyFrameAddCallBack) (void* voidOut, void* voidIn1, void* voidIn2);

	/**
	 * \ingroup rtanim
	 * \ref RtAnimKeyFrameMulRecipCallBack
	 * defines a callback function for multiplying a keyframe
	 * by the inverse of another keyframe. This is used for creating delta
	 * animations.
	 *
	 * \param voidFrame Void pointer for the keyframe to be modified.
	 * \param voidStart Void pointer for the start keyframe to take the reciprocal of.
	 */
	typedef void (*RtAnimKeyFrameMulRecipCallBack) (void* voidFrame, void* voidStart);

	/**
	 * \ingroup rtanim
	 * \ref RtAnimKeyFrameStreamReadCallBack
	 * defines a callback function for reading in keyframes
	 * from an \ref RwStream for the given animation.
	 *
	 * \param stream      Pointer to the \ref RwStream to read the keyframes from.
	 * \param animation   Pointer to the \ref RtAnimAnimation to read the keyframes into.
	 *
	 * \return Pointer to the \ref RtAnimAnimation.
	 */
	typedef RtAnimAnimation* (*RtAnimKeyFrameStreamReadCallBack) (RwStream* stream, RtAnimAnimation* animation);

	/**
	 * \ingroup rtanim
	 * \ref RtAnimKeyFrameStreamWriteCallBack
	 * defines a callback function for writing keyframes from the
	 * given animation to an \ref RwStream.
	 *
	 * \param animation   Pointer to the \ref RtAnimAnimation to write out from.
	 * \param stream      Pointer to the \ref RwStream to write the keyframes to.
	 *
	 * \return \ref RwBool, TRUE if successful.
	 */
	typedef int(*RtAnimKeyFrameStreamWriteCallBack) (const RtAnimAnimation* animation, RwStream* stream);

	/**
	 * \ingroup rtanim
	 * \ref RtAnimKeyFrameStreamGetSizeCallBack
	 * defines a callback function for calculating the binary stream
	 * size of keyframe data within an animation.
	 *
	 * \param animation  Pointer to the \ref RtAnimAnimation to calculate sizes from.
	 *
	 * \return \ref RwInt32 containing the size, in bytes, of the keyframe data.
	 */
	typedef int(*RtAnimKeyFrameStreamGetSizeCallBack) (const RtAnimAnimation* animation);

	/**
	 * \ingroup rtanim
	 * \ref RtAnimCallBack
	 * defines a callback function for use with the
	 * \ref RtAnimInterpolatorSetAnimCallBack and
	 * \ref RtAnimInterpolatorSetAnimLoopCallBack functions.
	 *
	 * \param  animInstance
	 * A pointer to the \ref RtAnimInterpolator structure.
	 *
	 * \param  data   Void pointer for user-defined data.
	 * You can use this to pass your own data
	 * structure(s) to the callback function.
	 *
	 * \see RtAnimInterpolatorSetAnimCallBack
	 * \see RtAnimInterpolatorSetAnimLoopCallBack
	 *
	 */
	typedef RtAnimInterpolator* (*RtAnimCallBack) (RtAnimInterpolator* animInstance, void* data);

	/**
	 * \ingroup rtanim
	 * \struct RtAnimInterpolatorInfo
	 * This structure is used to hold information for a keyframe interpolation scheme.
	 *
	 * \see RtAnimRegisterInterpolationScheme
	 * \see RtAnimGetInterpolatorInfo
	 */
	struct RtAnimInterpolatorInfo
	{
		int typeID;
		/**< The ID of the interpolation scheme. */
		int interpKeyFrameSize;
		/**< Size, in bytes, of the interpolated keyframe structure. */
		int animKeyFrameSize;
		/**< Size, in bytes, of the animation keyframe structure. */
		RtAnimKeyFrameApplyCallBack         keyFrameApplyCB;
		/**< Pointer to a function that converts a keyframe to the needed
		 * format. This function is never called from the \ref rtanim
		 * toolkit (as the toolit doesn't know the data layout of the
		 * results) but is stored to ease the creation of overloaded
		 * interpolators. */
		RtAnimKeyFrameBlendCallBack         keyFrameBlendCB;
		/**< Pointer to a function that blends between two
		 *   interpolated keyframes, for the purpose of blending
		 *   between the states of two \ref RtAnimInterpolator objects. */
		RtAnimKeyFrameInterpolateCallBack   keyFrameInterpolateCB;
		/**< Pointer to a function that interpolates between two keyframes
		 * for a given time in between. */
		RtAnimKeyFrameAddCallBack           keyFrameAddCB;
		/**< Pointer to a function that adds two interpolated keyframes
		 *   for the purpose of adding the states of two
		 *   \ref RtAnimInterpolator objects. */
		RtAnimKeyFrameMulRecipCallBack      keyFrameMulRecipCB;
		/**< Pointer to a function that multiplies a keyframe by the
		 * reciprocal of another. */
		RtAnimKeyFrameStreamReadCallBack    keyFrameStreamReadCB;
		/**< Pointer to a function that reads the keyframes from a stream
		 * for a given animation. */
		RtAnimKeyFrameStreamWriteCallBack   keyFrameStreamWriteCB;
		/**< Pointer to a function that writes the keyframes to a stream for
		 *  a given animation. */
		RtAnimKeyFrameStreamGetSizeCallBack keyFrameStreamGetSizeCB;
		/**< Pointer to a function that returns the binary stream size of
		 * the keyframes for a given animation. */
		int customDataSize;
		/**< Amount of custom data required per animation. */
	};
}

/**
 * \ingroup rtanim
 * \struct RtAnimAnimation
 * A keyframed animation consists of an array of keyframe structures,
 * along with some flags and a duration.
 *
 * The keyframes should be presented in the order they are needed
 * to animate forwards through time. That is, the next keyframe
 * at some point in the sequence should correspond to the node whose
 * previous two keyframes are next to expire as an interpolation
 * pair.
 *
 * Put another way, the correct ordering can be achieved by sorting
 * an unordered keyframe array using the following primary and secondary
 * sort keys:
 *
 * - time of previous keyframe for node
 * - node index
 *
 * An example is shown in the following diagram, where each vertical bar
 * indicates a keyframe point in time. The position of the keyframe
 * within the animation sequence is shown by an index to the left of
 * each bar.
 *
 * \verbatim

				t=0                               t=duration
  node 0    kf0..|  kf3.......|  kf8....|  kf10....|
  node 1    kf1..|  kf4...|  kf6........|  kf11....|
  node 2    kf2..|  kf5.....|  kf7..|  kf9.........|

   \endverbatim
 *
 * Each node MUST have an initial keyframe at time = 0.0, and a terminal
 * keyframe at time = duration of the animation.
 *
 * Pointers link all of the keyframes for a particular node backwards
 * through time in a list.
 *
 * \see RtAnimAnimationCreate
 */
struct RtAnimAnimation
{
	RWE::Anim::RtAnimInterpolatorInfo* interpInfo;
	/**< Pointer to interpolation scheme information */
	int numFrames;
	/**< Number of keyframes in the animation  */
	int flags;
	/**< Specifies details about animation,
	 * relative translation modes etc. */
	float duration;
	/**< Duration of animation in seconds */
	void* pFrames;
	/**< Pointer to the animation keyframes  */
	void* customData;
	/**< Pointer to custom data for this animation */


	static RtAnimAnimation* Read(const char* name);
	void Destroy();
};


namespace RWE::Anim {
	/**
	 * \ingroup rtanim
	 * \struct RtAnimInterpolator
	 * holds the current state for a particular instance of an animation,
	 * corresponding to a specific point in time.
	 *
	 * The current interpolated keyframes are stored in an array after the
	 * end of this structure. For advanced use, these may be accessed
	 * using the macro rtANIMGETINTERPFRAME(interpolator, nodeIndex)
	 * which takes a pointer to the interpolator and the node index
	 * for the interpolated keyframe required.
	 *
	 * \see \ref RtAnimInterpolatorCreate
	 * \see \ref RtAnimInterpolatorDestroy
	 * \see \ref RtAnimInterpolatorSetCurrentAnim
	 * \see \ref RtAnimInterpolatorGetCurrentAnim
	 * \see \ref RtAnimInterpolatorSetKeyFrameCallBacks
	 * \see \ref RtAnimInterpolatorSetAnimLoopCallBack
	 * \see \ref RtAnimInterpolatorSetAnimCallBack
	 * \see \ref RtAnimInterpolatorCopy
	 * \see \ref RtAnimInterpolatorSubAnimTime
	 * \see \ref RtAnimInterpolatorAddAnimTime
	 * \see \ref RtAnimInterpolatorSetCurrentTime
	 * \see \ref RtAnimCallBack
	 */
	struct RtAnimInterpolator
	{
		RtAnimAnimation* pCurrentAnim;
		/**< Current \ref RtAnimAnimation applied */
		float currentTime;
		/**< Current animation time  */
		void* pNextFrame;
		/**< Next animation keyframe to be played  */
		RtAnimCallBack pAnimCallBack;
		/**< Animation callback function pointer  */
		void* pAnimCallBackData;
		/**< Animation callback function user data  */
		float animCallBackTime;
		/**< Trigger time for callback function  */
		RtAnimCallBack pAnimLoopCallBack;
		/**< Animation loop callback function pointer */
		void* pAnimLoopCallBackData;
		/**< Animation loop callback function data  */
		int maxInterpKeyFrameSize;
		/**< Maximum size of interpolated keyframes
		 * usable on this animation (set at creation
		 * time) */
		int currentInterpKeyFrameSize;
		/**< Size of interpolated keyframes in the current
		 * animation scheme */
		int currentAnimKeyFrameSize;
		/**< Size of animation keyframes in the current
		 * animation scheme */
		int numNodes;
		/**< Number of nodes driven by the animation */
		int isSubInterpolator;
		/**< Internal use */
		int offsetInParent;
		/**< Internal use */
		RtAnimInterpolator* parentAnimation;
		/**< Internal use */

		RtAnimKeyFrameApplyCallBack       keyFrameApplyCB;
		/**< Internal use */
		RtAnimKeyFrameBlendCallBack       keyFrameBlendCB;
		/**< Internal use */
		RtAnimKeyFrameInterpolateCallBack keyFrameInterpolateCB;
		/**< Internal use */
		RtAnimKeyFrameAddCallBack         keyFrameAddCB;
		/**< Internal use */

		void Destroy();
		void SetAnimation(RWE::RtAnimAnimation* a);
		void AddTime(float t);
		void SubTime(float t);
		void SetCurrentTime(float t);
		void Blend(RtAnimInterpolator* in1, RtAnimInterpolator* in2, float alpha);

		// 6EADA0 RtAnimInterpolatorCreate
	};

	/**
	 * \ingroup rphanim
	 * \struct RpHAnimNodeInfo
	 *
	 * Used to describe a hierarchy toplogy.
	 * It holds flags representing its position in the
	 * hierarchy as well as a pointer to the matching \ref RwFrame if the
	 * hierarchy has been attached to a \ref RwFrame hierarchy.
	 *
	 */
	struct RpHAnimNodeInfo
	{
		enum class Flags : int
		{
			Deformable = 0,
			NubBone = 1,
			Rigid = 3,
		};

		int nodeID;     /**< User defined ID for this node  */
		int nodeIndex;  /**< Array index of node  */
		Flags flags;      /**< Matrix push/pop flags  */
		RwFrame* pFrame;     /**< Pointer to an attached RwFrame (see \ref RpHAnimHierarchyAttach) */
	};

	/**
	 * \ingroup rphanim
	 * \struct RpHAnimHierarchy
	 * An RpHAnimHierarchy is used to "play back" an animation.
	 *
	 * The structure of a hierarchy is defined by an array
	 * of \ref RpHAnimNodeInfo structures.
	 *
	 * The hierarchy is defined by running through the node array in order,
	 * pushing the parent-node's matrix whenever a child is reached that has
	 * more than one sibling, and popping the parent matrix when a "leaf"
	 * node is encountered.
	 *
	 */
	struct RpHAnimHierarchy
	{
		/**
		 * \ingroup rphanim
		 * \ref RpHAnimHierarchyFlag defines type and update modes in HAnimHierarchies
		 */
		enum class RpHAnimHierarchyFlag : int
		{
			None = 0,
			/* creation flags */
			SubHierarchy = 0x01, /**< This hierarchy is a sub-hierarchy */
			NoMatrices = 0x02, /**< This hierarchy has no local matrices */

			/* update flags */
			UpdateModellingMatrices = 0x1000, /**< This hierarchy updates modeling matrices */
			UpdateLTMs = 0x2000, /**< This hierarchy updates LTMs */
			LocalSpaceMatrices = 0x4000, /**< This hierarchy calculates matrices in a space
																   relative to its root */
		};

		RpHAnimHierarchyFlag flags;          /**< Flags for the hierarchy  */
		int numNodes;      /**< Number of nodes in the hierarchy  */

		RwMatrix* pMatrixArray;   /**< Pointer to node matrices*/
		void* pMatrixArrayUnaligned; /**< Pointer to memory used for node matrices
													* from which the aligned pMatrixArray is allocated */
		RpHAnimNodeInfo* pNodeInfo;     /**< 4 Array of node information (push/pop flags etc) */
		RwFrame* parentFrame;    /**< Pointer to the Root RwFrame of the hierarchy this
											   * RpHAnimHierarchy represents */
		RpHAnimHierarchy* parentHierarchy;                       /**< Internal use */
		int rootParentOffset;                      /**< Internal use */

		RtAnimInterpolator* currentAnim;                    /**< Internal use */

		static inline RpHAnimHierarchy* (__cdecl* const Create)(int numNodes, unsigned int* nodeFlags, int nodeIds, RpHAnimHierarchyFlag flags, int maxInterpKeyFrameSize) = reinterpret_cast<RpHAnimHierarchy * (__cdecl*)(int, unsigned int*, int, RpHAnimHierarchyFlag, int)>(0x6EB890);
		RpHAnimHierarchy* Clone();
		void Destroy();
		void UpdateMatrices();
		int IDGetIndex(int id) const;
		RwFrame* GetFromIndex(int idx);

		void SetupForModel(RWE::RpClump* c);
	};
	template<>
	class ::enum_is_flags<RpHAnimHierarchy::RpHAnimHierarchyFlag> : public std::true_type {};
	//constexpr int i = offsetof(RpHAnimHierarchy, pNodeInfo) / 4;

	// 6EC770 RpHAnimPluginAttach
}
