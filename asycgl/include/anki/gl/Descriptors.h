#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include <cstdint>
#include <array>

#if 0

// Macros to avoid typos
#define ANKI_ENABLE_BIT(bit_) this->mask |= bit_
#define ANKI_CHECK_IMMUTABLE() ANKI_ASSERT((this->mask | IMMUTABLE_BIT) == 1)

/// The base of all descriptors
class GlDescriptor
{
public:
	virtual ~GlDescriptor()
	{}
};

/// Framebuffer descriptor
class GlFramebufferDescriptor: public GlDescriptor
{
public:
	/// Clear bits
	enum ClearBit
	{
		COLOR_CLEAR_BIT = 1 << 5,
		DEPTH_CLEAR_BIT = 1 << 6,
		STENCIL_CLEAR_BIT = 1 << 7
	};

private:
	enum
	{
		IMMUTABLE_BIT = 1 << 0,
		// State mask bits
		CLEAR_SET_BIT = 1 << 1,
		VIEWPORT_SET_BIT = 1 << 2,
		FBO_SET_BIT = 1 << 3,
		// Other bits
		IVALIDATE_FBO_ON_BIND_BIT = 1 << 4
	};

	uint8_t mask = 0;
	uint16_t viewport[4];
	uint32_t texAttachments[4];
	uint32_t fbo;

	void setImmutable()
	{
		ANKI_ENABLE_BIT(IMMUTABLE_BIT);
	}
};

/// Color descriptor
class GlColorDescriptor: public GlDescriptor
{
private:
	uint8_t setMask = 0;
	uint8_t colorMask; ///< Enables/disables the color channels (glColorMask)
	std::array<float, 4> clearColor;
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

	/// Set depth clear value
	void setClearValue(float value)
	{
		clearValue = value;
		ANKI_ENABLE_BIT(CLEAR_VALUE_BIT);
	}

	/// Set the depth test function
	void setDepthFunction(Function function)
	{
		func = function;
		ANKI_ENABLE_BIT(FUNC_BIT);
	}

	/// Enable or not the depth test
	void enableTest(bool enable)
	{
		test = enable;
		ANKI_ENABLE_BIT(TEST_BIT);
	}

	/// Enable or not depth writing
	void enableWriting(bool enable)
	{
		write = enable;
		ANKI_ENABLE_BIT(WRITE_BIT);
	}

private:
	enum
	{
		// State mask bits
		CLEAR_VALUE_SET_BIT = 1 << 0,
		FUNC_SET_BIT = 1 << 1,
		TEST_SET_BIT = 1 << 2,
		WRITE_SET_BIT = 1 << 3,
		// Other bits
		TEST_ENABLED_BIT = = 1 << 4, ///< Enable or not depth testing
		WRITE_ENABLED_BIT = 1 << 5 ///< Enable or not depth writing
	};

	uint8_t mask = 0;
	float clearValue; ///< The depth clear color
	uint8_t func; ///< The depth function
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
	void enableTest(bool enable)
	{
		enabled = enable;
		ANKI_ENABLE_BIT(ENABLED_BIT);
	}

	/// Set the function. Equivalent to glStencilFunc
	void setFunction(Function function, uint32_t reference, uint32_t mask)
	{
		func = function;
		ref = reference;
		funcMask = mask;
		ANKI_ENABLE_BIT(FUNC_BIT);
	}

	/// Set the stencil mask. Equivalent to glStencilMask
	void setPlaneMask(uint32_t mask)
	{
		planeMask = mask;
		ANKI_ENABLE_BIT(MASK_BIT);
	}

	/// Set the operations of stencil fail, depth fail, depth pass. Equivalent
	/// to glStencilOp
	void setOperations(Operation stencFail, Operation depthFail, 
		Operation depthPass)
	{
		sfail = stencFail;
		dpfail = depthFail;
		dppass = depthPass;
		ANKI_ENABLE_BIT(OP_BIT);
	}

private:
	enum 
	{
		ENABLED_BIT = 1 << 0,
		FUNC_BIT = 1 << 1,
		MASK_BIT = 1 << 2,
		OP_BIT = 1 << 3,
	};

	uint8_t setMask = 0;
	uint8_t enabled; ///< Enable or not stencil testing
	uint8_t func; ///< Param for glStencilFunc
	uint32_t ref; ///< Param for glStencilFunc
	uint32_t funcMask; ///< Param for glStencilFunc
	uint32_t planeMask; ///< Param for glStencilMask
	uint8_t sfail; ///< Param for glStencilOp 
	uint8_t dpfail; ///< Param for glStencilOp 
	uint8_t dppass; ///< Param for glStencilOp 
};

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
		ANKI_ENABLE_BIT(PROGRAM_PIPELINE_BIT);
	}

	/// Set a buffer binding
	void setBuffer(uint32_t buffer, uint32_t index, size_t offset, size_t size)
	{
		BufferBinding& binding = bindings[bindingsCount++];

		binding.buff = buffer;
		binding.index = index;
		binding.offset = offset;
		binding.size = size;

		ANKI_ENABLE_BIT(BUFFER_BINDINGS_BIT);
	}

	/// Set a buffer binding
	void setBuffer(uint32_t buffer, uint32_t index)
	{
		BufferBinding& binding = bindings[bindingsCount++];

		binding.buff = buffer;
		binding.index = index;
		binding.offset = 0;
		binding.size = 0xFF; // TODO buffer size

		ANKI_ENABLE_BIT(BUFFER_BINDINGS_BIT);
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

// Undef the macro
#undef ANKI_ENABLE_BIT

#endif

#endif
