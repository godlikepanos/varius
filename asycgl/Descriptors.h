#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include <cstdint>
#include <array>

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
private:
	enum
	{
		CLEAR_BIT = 1 << 0,
		VIEWPORT_BIT = 1 << 1,
		FBO_BIT = 1 << 2
	};

	uint8_t setMask = 0;
	uint32_t clearMask;
	uint32_t viewport[4];
	uint32_t texAttachments[4];
	uint32_t fbo;
};

/// Depth descriptor
class GlDepthDescriptor: public GlDescriptor
{
public:
	/// The depth function enum
	enum Function
	{
		NEVER,
		LESS,
		EQUAL,
		LEQUAL,
		GREATER,
		NOTEQUAL,
		GEQUAL,
		ALWAYS
	};

	GlDepthDescriptor()
	{}

	/// Set depth clear value
	void setClearValue(float value)
	{
		clearValue = value;
		setMask |= CLEAR_VALUE_BIT;
	}

	/// Set the depth test function
	void setDepthFunction(Function function)
	{
		func = function;
		setMask |= FUNC_BIT;
	}

	/// Enable or not the depth test
	void enableTest(bool enable)
	{
		test = enable;
		setMask |= TEST_BIT;
	}

	/// Enable or not depth writing
	void enableWriting(bool enable)
	{
		write = enable;
		setMask |= WRITE_BIT;
	}

private:
	enum
	{
		CLEAR_VALUE_BIT = 1 << 0,
		FUNC_BIT = 1 << 1,
		TEST_BIT = 1 << 2,
		WRITE_BIT = 1 << 3
	};

	uint8_t setMask = 0;
	float clearValue; ///< The depth clear color
	uint8_t func; ///< The depth function
	uint8_t test; ///< Enable or not depth testing
	uint8_t write; ///< Enable or not depth writing
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

		setMask |= PROGRAM_PIPELINE_BIT;
	}

	/// Set a buffer binding
	void setBuffer(uint32_t buffer, uint32_t index, size_t offset, size_t size)
	{
		BufferBinding& binding = bindings[bindingsCount++];

		binding.buff = buffer;
		binding.index = index;
		binding.offset = offset;
		binding.size = size;

		setMask |= BUFFER_BINDINGS_BIT;
	}

	/// Set a buffer binding
	void setBuffer(uint32_t buffer, uint32_t index)
	{
		BufferBinding& binding = bindings[bindingsCount++];

		binding.buff = buffer;
		binding.index = index;
		binding.offset = 0;
		binding.size = 0xFF; // TODO buffer size

		setMask |= BUFFER_BINDINGS_BIT;
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
