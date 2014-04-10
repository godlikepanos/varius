#ifndef ANKI_GL_PROGRAM_H
#define ANKI_GL_PROGRAM_H

#include "anki/gl/GlObject.h"
#include "anki/util/Assert.h"
#include "anki/util/Dictionary.h"

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
	void writeClientMemory(void* buffBase, U32 buffSize,
		const F32 arr[], U32 size) const;

	void writeClientMemory(void* buffBase, U32 buffSize,
		const Vec2 arr[], U32 size) const;

	void writeClientMemory(void* buffBase, U32 buffSize,
		const Vec3 arr[], U32 size) const;

	void writeClientMemory(void* buffBase, U32 buffSize,
		const Vec4 arr[], U32 size) const;

	void writeClientMemory(void* buffBase, U32 buffSize,
		const Mat3 arr[], U32 size) const;

	void writeClientMemory(void* buffBase, U32 buffSize,
		const Mat4 arr[], U32 size) const;
	/// @}

private:
	U8 type = MAX_U8; ///< It's type
	std::string name; ///< The name inside the shader program
	const GlProgram* prog = nullptr; ///< Used for sanity checks

	GLenum dataType = GL_NONE; ///< GL_FLOAT or GL_FLOAT_VEC2 etc.
	U8 arrSize = 0; ///< Its 1 if it is a single or >1 if it is an array

	I32 loc = -1; ///< For uniforms and attributes

	/// @name For variables in interface blocks
	/// @{
	GlProgramBlock* block = nullptr; ///< Interface block

	/// Stride between the each array element if the variable is array
	I32 arrStride = -1;

	I32 offset = -1; ///< Offset inside the block

	/// Identifying the stride between columns of a column-major matrix or rows 
	/// of a row-major matrix
	I32 matrixStride = -1;
	/// @}

	/// Do common checks
	template<typename T>
	void writeClientMemorySanityChecks(void* buffBase, U32 buffSize,
		const F32 arr[], U32 size) const;

	/// Do the actual job of setClientMemory
	template<typename T>
	void writeClientMemoryInternal(
		void* buff, U32 buffSize, const T arr[], U32 size) const;

	/// Do the actual job of setClientMemory for matrices
	template<typename Mat, typename Vec>
	void setClientMemoryInternalMatrix(void* buff, U32 buffSize,
		const Mat arr[], U32 size) const;
};

/// Interface shader block
class GlProgramBlock
{
	friend class GlProgram;

public:
	/// Block type
	enum Type
	{
		UNIFORM_BLOCK,
		SHADER_STORAGE_BLOCK
	};

	/// @name Accessors
	/// @{
	PtrSize getSize() const
	{
		return size;
	}

	const std::string& getName() const
	{
		return name;
	}

	U32 getBinding() const
	{
		return bindingPoint;
	}
	/// @}

private:
	U8 type;
	std::string name;
	GlProgram* prog;
	Vector<GlProgramVariable*> variables;
	U32 size; ///< In bytes
	U32 bindingPoint;
};

/// Shader program. It only contains a single shader and it can be combined 
/// with other programs in a program pipiline.
class GlProgram: public GlContainer
{
public:
	/// @name Constructors/Destructor
	/// @{

	/// Create the program
	/// @param shaderType It can be GL_VERTEX_SHADER, GL_FRAGMENT_SHADER etc
	/// @param source The shader's source
	GlProgram(GLenum shaderType, const char* source);

	~GlProgram();
	/// @}

private:
	GLenum type; ///< GL_VERTEX_SHADER, GL_FRAGMENT_SHADER etc

	Vector<GlProgramVariable> variables;
	Dictionary<GlProgramVariable*> variablesDict;

	Vector<GlProgramBlock*> blocks;
	Dictionary<GlProgramBlock*> blocksDict;

	/// Query the program for variables
	initVariablesOfType(GLenum programInterface);

	/// Query the program for blocks
	initBlocksOfType(GLenum programInterface);
};

/// @}
/// @}

} // end namespace anki

#endif
