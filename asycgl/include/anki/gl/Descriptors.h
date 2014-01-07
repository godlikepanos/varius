#ifndef ANKI_GL_DESCRIPTORS_H
#define ANKI_GL_DESCRIPTORS_H

#include "anki/util/StdTypes.h"
#include "anki/util/Array.h"
#include "anki/util/Assert.h"
#include "anki/gl/Common.h"

namespace anki {

// Macros to avoid typos
#define ANKI_ENABLE_BITS(bit_) (this->mask |= (bit_))
#define ANKI_CHECK_IMMUTABLE() ANKI_ASSERT((this->mask | IMMUTABLE_BIT) == 1)

/// The base of all descriptors
class GlDescriptor
{
public:
	virtual ~GlDescriptor()
	{}

protected:
	U8 mask = 0; ///< A common generic mask

	enum
	{
		IMMUTABLE_BIT = 1 << 0
	};
};

/// Framebuffer descriptor
class GlFramebufferDescriptor: public GlDescriptor
{
public:
	/// Clear the color buffers with the specified values
	void clearColor(F32 r, F32 g, F32 b, F32 a)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(COLOR_CLEAR_SET_BIT);

		colorClear[0] = r;
		colorClear[1] = g;
		colorClear[2] = b;
		colorClear[3] = a;
	}

	/// Clear the depth buffer with the specified value
	void clearDepth(F32 value)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(DEPTH_CLEAR_SET_BIT);

		depthClear = value;
	}

	/// Clear the stencil buffer with the specified value
	void clearStencil(U32 value)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(STENCIL_CLEAR_SET_BIT);

		stencilClear = value;
	}

	/// Set the viewport
	void setViewport(U16 minx, U16 miny, U16 maxx, U16 maxy)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(VIEWPORT_SET_BIT);

		viewport[0] = minx;
		viewport[1] = miny;
		viewport[2] = maxx;
		viewport[3] = maxy;
	}

	/// Invalidate framebuffer right after binding it
	void invalidate()
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(IVALIDATE_FBO_SET_BIT);
	}

private:
	enum
	{
		// State mask bits
		COLOR_CLEAR_SET_BIT = 1 << 1,
		DEPTH_CLEAR_SET_BIT = 1 << 2,
		STENCIL_CLEAR_SET_BIT = 1 << 3,
		VIEWPORT_SET_BIT = 1 << 4,
		FBO_SET_BIT = 1 << 5,
		IVALIDATE_FBO_SET_BIT = 1 << 6
	};

	Array<F32, 4> colorClear; ///< For glClearColor
	F32 depthClear; ///< For glClearDepth
	U32 stencilClear; ///< For glStencilClear
	Array<U16, 4> viewport; ///< For glViewport
	//uint32_t texAttachments[4];
	GLuint fbo;
};

/// Color descriptor
class GlColorDescriptor: public GlDescriptor
{
public:
	/// Enable or not the color writing
	void setWriteMask(Bool r, Bool g, Bool b, Bool a)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(COLOR_WRITE_SET_BIT);

		if(r)
		{
			ANKI_ENABLE_BITS(COLOR_WRITE_ENABLED_R_BIT);
		}

		if(g)
		{
			ANKI_ENABLE_BITS(COLOR_WRITE_ENABLED_G_BIT);
		}

		if(b)
		{
			ANKI_ENABLE_BITS(COLOR_WRITE_ENABLED_B_BIT);
		}

		if(a)
		{
			ANKI_ENABLE_BITS(COLOR_WRITE_ENABLED_A_BIT);
		}
	}

private:
	enum
	{
		// State mask bits
		COLOR_WRITE_SET_BIT = 1 << 1,
		// Other bits
		COLOR_WRITE_ENABLED_R_BIT = 1 << 2, ///< For glColorMask
		COLOR_WRITE_ENABLED_G_BIT = 1 << 3, ///< For glColorMask
		COLOR_WRITE_ENABLED_B_BIT = 1 << 4, ///< For glColorMask
		COLOR_WRITE_ENABLED_A_BIT = 1 << 5 ///< For glColorMask
	};
};

/// Depth descriptor
class GlDepthDescriptor: public GlDescriptor
{
public:
	/// The depth function enum
	enum Function
	{
		NEVER_FUNCTION,
		LESS_FUNCTION,
		EQUAL_FUNCTION,
		LESS_EQUAL_FUNCTION,
		GREATER_FUNCTION,
		NOT_EQUAL_FUNCTION,
		GREATER_EQUAL_FUNCTION,
		ALWAYS_FUNCTION
	};

	GlDepthDescriptor()
	{}

	/// Set the depth test function
	void setTestFunction(Function function)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(FUNC_SET_BIT);
		func = function;
	}

	/// Enable or not the depth test
	void enableTest(Bool enable)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(TEST_SET_BIT);
		ANKI_ENABLE_BITS(enable ? TEST_ENABLED_BIT : 0);
	}

	/// Enable or not depth writing
	void enableWriting(Bool enable)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(WRITE_SET_BIT);
		ANKI_ENABLE_BITS(enable ? WRITE_ENABLED_BIT : 0);
	}

private:
	enum
	{
		// State mask bits
		FUNC_SET_BIT = 1 << 1,
		TEST_SET_BIT = 1 << 2,
		WRITE_SET_BIT = 1 << 3,
		// Other bits
		TEST_ENABLED_BIT = 1 << 4, ///< Enable or not depth testing
		WRITE_ENABLED_BIT = 1 << 5 ///< Enable or not depth writing
	};

	U8 func; ///< The depth test function. For glDepthFunc
};

/// Stencil descriptor
class GlStencilDescriptor: public GlDescriptor
{
public:
	/// The stencil function enum
	enum Function
	{
		NEVER_FUNCTION,
		LESS_FUNCTION,
		EQUAL_FUNCTION,
		LESS_EQUAL_FUNCTION,
		GREATER_FUNCTION,
		NOT_EQUAL_FUNCTION,
		GREATER_EQUAL_FUNCTION,
		ALWAYS_FUNCTION
	};

	/// The stencil operation
	enum Operation
	{
		KEEP_OPERATION,
		ZERO_OPERATION,
		REPLACE_OPERATION,
		INCREASE_OPERATION,
		INCREASE_WRAP_OPERATION,
		DECREASE_OPERATION,
		DECREASE_WRAP_OPERATION,
		INVERT_OPERATION
	};

	/// Enable or note stencil test
	void enableTest(Bool enable)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(TEST_SET_BIT);
		ANKI_ENABLE_BITS(enable ? TEST_ENABLED_BIT : 0);
	}

	/// Set the function. Equivalent to glStencilFunc
	void setFunction(Function function, U32 reference, U32 mask)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(FUNC_SET_BIT);
		func = function;
		ref = reference;
		funcMask = mask;
	}

	/// Set the stencil mask. Equivalent to glStencilMask
	void setPlaneMask(U32 mask)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(MASK_SET_BIT);
		planeMask = mask;
	}

	/// Set the operations of stencil fail, depth fail, depth pass. Equivalent
	/// to glStencilOp
	void setOperations(Operation stencFail, Operation depthFail, 
		Operation depthPass)
	{
		ANKI_CHECK_IMMUTABLE();
		ANKI_ENABLE_BITS(OP_SET_BIT);
		sfail = stencFail;
		dpfail = depthFail;
		dppass = depthPass;
	}

private:
	enum 
	{
		// State mask bits
		TEST_SET_BIT = 1 << 1,
		FUNC_SET_BIT = 1 << 2,
		MASK_SET_BIT = 1 << 3,
		OP_SET_BIT = 1 << 4,
		// Other bits
		TEST_ENABLED_BIT = 1 << 5
	};

	U8 sfail; ///< For glStencilOp 
	U8 dpfail; ///< For glStencilOp 
	U8 dppass; ///< For glStencilOp
	U32 ref; ///< Param for glStencilFunc
	U32 funcMask; ///< Param for glStencilFunc
	U32 planeMask; ///< Param for glStencilMask
	U8 func; ///< Param for glStencilFunc 
};

/// Blending decriptor
class GlBlendDescriptor: public GlDescriptor
{
};

#if 0

/// Program descriptor
class ProgramDescriptor
{
public:
	/// The max number of buffer to attach to the descriptor.
	static const uint32_t MAX_BUFFERS = 8;

	/// Set the shader programs for each stage. 
	///
	/// It is not advised to call it more than once.
	void setPrograms(
		uint32_t* vert, 
		uint32_t* tc,
		uint32_t* te,
		uint32_t* geom,
		uint32_t* frag,
		uint32_t* comp)
	{
		progs[0] = vert ? *vert : 0;
		progs[4] = frag ? *frag : 0;
		ANKI_ENABLE_BITS(PROGRAM_PIPELINE_BIT);
	}

	/// Set a buffer binding
	void setBuffer(uint32_t buffer, uint32_t index, size_t offset, size_t size)
	{
		BufferBinding& binding = bindings[bindingsCount++];

		binding.buff = buffer;
		binding.index = index;
		binding.offset = offset;
		binding.size = size;

		ANKI_ENABLE_BITS(BUFFER_BINDINGS_BIT);
	}

	/// Set a buffer binding
	void setBuffer(uint32_t buffer, uint32_t index)
	{
		BufferBinding& binding = bindings[bindingsCount++];

		binding.buff = buffer;
		binding.index = index;
		binding.offset = 0;
		binding.size = 0xFF; // TODO buffer size

		ANKI_ENABLE_BITS(BUFFER_BINDINGS_BIT);
	}

private:
	enum
	{
		PROGRAM_PIPELINE_BIT,
		BUFFER_BINDINGS_BIT
	};

	/// Info on the attached buffer
	struct BufferBinding
	{
		uint32_t buff;
		uint32_t index;
		size_t offset = 0;
		size_t size = 0;
	};

	uint8_t setMask = 0;
	uint32_t ppline = 0;
	uint32_t progs[6];
	std::array<BufferBinding, MAX_BUFFERS> bindings;
	uint8_t bindingsCount = 0;
};

/// Vertex descriptor
class GlVertexDescriptor: public GlDescriptor
{
public:
	static const size_t MAX_ATTRIBUTES = 16;

private:
	
};

#endif

// Undef the macro
#undef ANKI_ENABLE_BITS

} // end namespace anki

#endif
