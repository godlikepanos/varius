#ifndef ANKI_GL_PROGRAM_H
#define ANKI_GL_PROGRAM_H

#include "anki/gl/GlObject.h"
#include "anki/util/Assert.h"

namespace anki {

// Forward
class GlProgram;
class GlProgramBlock;

/// @addtogroup OpenGL
/// @{
/// @addtogroup Containers
/// @{

/// Shader program variable. The type is attribute or uniform
class GlProgramVariable
{
	friend class GlProgram;

public:
	/// Shader var types
	enum Type
	{
		INPUT_VARIABLE, ///< Attribute
		UNIFORM_VARIABLE,
		BUFFER_VARIABLE
	};

	GlProgramVariable(
		Type t, 
		const char* name_, 
		const GlProgram* program,
		GLenum dataType_,
		U8 arrSize_,
		GLint location,
		GlProgramBlock* block_,
		I32 arrayStride_,
		I32 matrixStride_)
		:	type(t),
			name(name),
			prog(program),
			dataType(dataType_),
			arrSize(arrSize_),
			loc(location),
			block(block_),
			arrayStride(arrayStride_),
			matrixStride(matrixStride_)
	{
		ANKI_ASSERT(prog);
	}

	virtual ~GlProgramVariable()
	{}

	/// @name Accessors
	/// @{
	Type getType() const
	{
		return type;
	}

	const std::string& getName() const
	{
		return name;
	}

	const GlProgram& getProgram() const
	{
		return *prog;
	}

	GLenum getDataType() const
	{
		return glDataType;
	}

	PtrSize getArraySize() const
	{
		return arrSize;
	}

	GLint getLocation() const
	{
		return loc;
	}

	const GlProgramBlock* getBlock() const
	{
		return block;
	}
	/// @}

	/// @name Block setters
	/// Write a client memory that represents the interface block
	/// @{
	void writeClientMemory(void* buff, U32 buffSize,
		const F32 arr[], U32 size) const;

	void writeClientMemory(void* buff, U32 buffSize,
		const Vec2 arr[], U32 size) const;

	void writeClientMemory(void* buff, U32 buffSize,
		const Vec3 arr[], U32 size) const;

	void writeClientMemory(void* buff, U32 buffSize,
		const Vec4 arr[], U32 size) const;

	void writeClientMemory(void* buff, U32 buffSize,
		const Mat3 arr[], U32 size) const;

	void writeClientMemory(void* buff, U32 buffSize,
		const Mat4 arr[], U32 size) const;
	/// @}

private:
	U8 type; ///< It's type
	std::string name; ///< The name inside the shader program
	const GlProgram* prog; ///< Used for sanity checks
	
	GLenum dataType; ///< GL_FLOAT or GL_FLOAT_VEC2 etc.
	U8 arrSize; ///< Its 1 if it is a single or >1 if it is an array
	
	GLint loc; ///< For uniforms and attributes

	GlProgramBlock* block; ///< Interface block

	I32 offset; ///< Offset inside the block

	/// Stride between the each array element if the variable is array
	I32 arrayStride;

	/// Identifying the stride between columns of a column-major matrix or rows 
	/// of a row-major matrix
	I32 matrixStride;
};

/// Interface shader block
class GlProgramBlock
{
	friend class ShaderProgram;

public:
	/// Block type
	enum Type
	{
		UNIFORM_BLOCK,
		SHADER_STORAGE_BLOCK
	};

	GlProgramBlock()
	{}

	~GlProgramBlock()
	{}

	/// @name Accessors
	/// @{
	GLuint getIndex() const
	{
		return index;
	}

	PtrSize getSize() const
	{
		return size;
	}

	const std::string& getName() const
	{
		return name;
	}

	GLuint getBinding() const
	{
		return bindingPoint;
	}
	/// @}

private:
	U8 type;
	std::string name;
	GlProgram* prog;
	Vector<GlProgramVariables*> variables;
	U32 size; ///< In bytes
	GLuint bindingPoint;
};

/// Shader program. It only contains 1 shader and it can be combined with other
/// programs in a program pipiline.
class GlProgram: public GlObject
{
};

/// @}
/// @}

} // end namespace anki

#endif
