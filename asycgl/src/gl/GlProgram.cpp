#include "anki/gl/GlProgram.h"
#include "anki/util/StringList.h"
#include <sstream>

#define ANKI_DUMP_SHADERS ANKI_DEBUG

#if ANKI_DUMP_SHADERS
#	include "anki/core/App.h"
#	include "anki/util/File.h"
#endif

namespace anki {

//==============================================================================
// GlProgramVariable                                                           =
//==============================================================================

//==============================================================================
namespace {

// Template functions that return the GL type using an AnKi type
template<typename T>
static Bool checkType(GLenum glDataType);

template<>
Bool checkType<F32>(GLenum glDataType)
{
	return glDataType == GL_FLOAT;
}

template<>
Bool checkType<Vec2>(GLenum glDataType)
{
	return glDataType == GL_FLOAT_VEC2;
}

template<>
Bool checkType<Vec3>(GLenum glDataType)
{
	return glDataType == GL_FLOAT_VEC3;
}

template<>
Bool checkType<Vec4>(GLenum glDataType)
{
	return glDataType == GL_FLOAT_VEC4;
}

template<>
Bool checkType<Mat3>(GLenum glDataType)
{
	return glDataType == GL_FLOAT_MAT3;
}

template<>
Bool checkType<Mat4>(GLenum glDataType)
{
	return glDataType == GL_FLOAT_MAT4;
}

} // end anonymous namespace

//==============================================================================
template<typename T>
void GlProgramVariable::writeClientMemorySanityChecks(
	void* buffBase, U32 buffSize,
	const F32 arr[], U32 size) const
{
	// Check pointers
	ANKI_ASSERT(buffBase != nullptr && arr != nullptr);

	// Check T
	ANKI_ASSERT(checkType<T>(dataType));
	
	// Check array size
	ANKI_ASSERT(size <= arrSize && size > 0);
	
	// Check if var in block
	ANKI_ASSERT(block != nullptr);
	ANKI_ASSERT(offset != -1 && arrStride != -1);

	// Check if there is space
	ANKI_ASSERT(block->getSize() <= buffSize);

	// arrStride should not be zero if array
	ANKI_ASSERT(!(size > 1 && arrStride == 0));
}

//==============================================================================
template<typename T>
void GlProgramVariable::writeClientMemoryInternal(
	void* buffBase, U32 buffSize, const T arr[], U32 size) const
{
	writeClientMemorySanityChecks<T>(buffBase, buffSize, buffSize, size);
	
	U8* buff = (U8*)buffBase + offset;
	for(U32 i = 0; i < size; i++)
	{
		ANKI_ASSERT((U8*)buff + sizeof(T) <= (U8*)buffBase + buffSize);

		T* ptr = (T*)buff;
		*ptr = arr[i];
		buff += arrStride;
	}
}

//==============================================================================
template<typename T, typename Vec>
void GlProgramVariable::writeClientMemoryInternalMatrix(
	void* buffBase, U32 buffSize, const T arr[], U32 size) const
{
	writeClientMemorySanityChecks<T>(buffBase, buffSize, buffSize, size);
	ANKI_ASSERT(matrixStride != -1 && matrixStride >= (I32)sizeof(Vec));

	U8* buff = (U8*)buffBase + offset;
	for(U32 i = 0; i < size; i++)
	{
		U8* subbuff = buff;
		T matrix = arr[i];
		matrix.transpose();
		for(U j = 0; j < sizeof(T) / sizeof(Vec); j++)
		{
			ANKI_ASSERT(subbuff + sizeof(Vec) <= (U8*)buffBase + buffSize);

			Vec* ptr = (Vec*)subbuff;
			*ptr = matrix.getRow(j);
			subbuff += matrixStride;
		}
		buff += arrayStride;
	}
}

//==============================================================================
void GlProgramVariable::writeClientMemory(void* buff, U32 buffSize,
	const F32 arr[], U32 size) const
{
	writeClientMemoryInternal(buff, buffSize, arr, size);
}

//==============================================================================
void GlProgramVariable::writeClientMemory(void* buff, U32 buffSize,
	const Vec2 arr[], U32 size) const
{
	writeClientMemoryInternal(buff, buffSize, arr, size);
}

//==============================================================================
void GlProgramVariable::writeClientMemory(void* buff, U32 buffSize,
	const Vec3 arr[], U32 size) const
{
	writeClientMemoryInternal(buff, buffSize, arr, size);
}

//==============================================================================
void GlProgramVariable::writeClientMemory(void* buff, U32 buffSize,
	const Vec4 arr[], U32 size) const
{
	writeClientMemoryInternal(buff, buffSize, arr, size);
}

//==============================================================================
void GlProgramVariable::writeClientMemory(void* buff, U32 buffSize,
	const Mat3 arr[], U32 size) const
{
	writeClientMemoryInternalMatrix<Mat3, Vec3>(buff, buffSize, arr, size);
}

//==============================================================================
void GlProgramVariable::writeClientMemory(void* buff, U32 buffSize,
	const Mat4 arr[], U32 size) const
{
	writeClientMemoryInternalMatrix<Mat4, Vec4>(buff, buffSize, arr, size);
}

//==============================================================================
// GlProgram                                                                   =
//==============================================================================

//==============================================================================
GlProgram::GlProgram(GLenum shaderType, const char* source)
	: type(shaderType)
{
	ANKI_ASSERT(source);

	// 1) Append some things in the source string
	std::string fullSrc;
#if ANKI_GL == ANKI_GL_DESKTOP
	fullSrc = "#version " + std::to_string(version) + " core\n" 
		+ std::string(source);
#else
	fullSrc = "#version " + std::to_string(version) + " es\n"
		+ std::string(source);
#endif

#if ANKI_DUMP_SHADERS
	{
		const char* ext = nullptr;
		switch(type)
		{
		case GL_VERTEX_SHADER:
			ext = ".vert";
			break;
		case GL_FRAGMENT_SHADER:
			ext = ".frag";
			break;
#	if ANKI_GL == ANKI_GL_DESKTOP
		case GL_TESS_EVALUATION_SHADER:
			ext = ".tese";
			break;
		case GL_TESS_CONTROL_SHADER:
			ext = ".tesc";
			break;
		case GL_GEOMETRY_SHADER:
			ext = ".geom";
			break;
		case GL_COMPUTE_SHADER:
			ext = ".comp";
			break;
#	endif
		default:
			ANKI_ASSERT(0);
		}

		std::stringstream fname;
		fname << AppSingleton::get().getCachePath() << "/" 
			<< std::setfill('0') << std::setw(4) << (U32)shader << ext;

		File file(fname.str().c_str(), File::OF_WRITE);

		file.writeText("%s", fullSrc.c_str());
	}
#endif

	// 2) Gen name, create, compile and link
	const char* sourceStrs[1] = {nullptr};
	sourceStrs[0] = &fullSrc[0];
	glName = glCreateShaderProgramv(type, 1, sourceStrs);
	if(glName == 0)
	{
		throw ANKI_EXCEPTION("glCreateShaderProgramv() failed");
	}
	
	GLint status = GL_FALSE;
	glGetProgramiv(glName, GL_LINK_STATUS, &status);

	if(status == GL_FALSE)
	{
		GLint infoLen = 0;
		GLint charsWritten = 0;
		std::string infoLogTxt;

		static const char* padding = 
			"======================================="
			"=======================================";

		glGetProgramiv(glName, GL_INFO_LOG_LENGTH, &infoLen);

		infoLogTxt.resize(infoLen + 1);
		glGetProgramInfoLog(prog, infoLen, &charsWritten, &infoLogTxt[0]);
		
		std::stringstream err;
		err << "Shader compile failed (0x" << std::hex << type << std::dec
			<< "):\n" << padding << "\n" << &infoLog[0]
			<< "\n" << padding << "\nSource:\n" << padding << "\n";

		// Prettyfy source
		StringList lines = StringList::splitString(fullSrc.c_str(), '\n', true);
		int lineno = 0;
		for(const std::string& line : lines)
		{
			err << std::setw(4) << std::setfill('0') << ++lineno << ": "
				<< line << std::endl;
		}

		err << padding;
	}

	// 3) Populate with vars and blocks
	initBlocksOfType(GL_UNIFORM_BLOCK);
	initBlocksOfType(GL_SHADER_STORAGE_BLOCK);

	initVariablesOfType(GL_UNIFORM);
	initVariablesOfType(GL_PROGRAM_INPUT);
	initVariablesOfType(GL_BUFFER_VARIABLE);
}

//==============================================================================
GlProgram::initVariablesOfType(GLenum interface)
{
	GLint activeCount;
	glGetProgramInterfaceiv(glName, interface, GL_ACTIVE_RESOURCES, 
		&activeCount);

	for(U i = 0; i < activeCount; i++)
	{
		// Get name
		char name[1024];
		GLint len;
		glGetProgramResourceName(prog, interface, i, sizeof(name), &len, name);
		name[len] = '\0';

		// Get the properties
		Array<GLenum, 7> prop = {{GL_LOCATION, GL_TYPE, GL_ARRAY_SIZE, 
			GL_ARRAY_STRIDE, GL_OFFSET, GL_MATRIX_STRIDE, GL_BLOCK_INDEX}};
		Array<GLint, prog.getSize()> out = {{-1, GL_NONE, -1, -1, -1, -1, -1}};

		U fromIdx, toIdx;
		GlProgramVariable::Type akType;
		switch(interface)
		{
		case GL_UNIFORM:
			fromIdx = 0;
			toIdx = prog.getSize() - 1;
			akType = GlProgramVariable::UNIFORM_VARIABLE;
			break;
		case GL_BUFFER_VARIABLE:
			fromIdx = 1;
			toIdx = prog.getSize() - 1;
			akType = GlProgramVariable::BUFFER_VARIABLE;
			break;
		case GL_PROGRAM_INPUT:
			fromIdx = 0;
			toIdx = 2;
			akType = GlProgramVariable::INPUT_VARIABLE;
		default:
			ANKI_ASSERT(0);
		};

		GLsizei outCount = 0;
		GLsizei count = toIdx - fromIdx + 1;
		glGetProgramResourceiv(glName, interfase, i, 
			count, &prop[fromIdx], 
			count, &outCount, &out[fromIdx]);

		if(count != outCount)
		{
			throw ANKI_EXCEPTION("glGetProgramResourceiv() didn't got all "
				"the params");
		}

		// Create and populate the variable
		variables.push_back(GlProgramVariable());
		GlProgramVariable& var = variables.back();

		var->type = akType;
		var->name = name;
		var->prog = this;
		var->dataType = out[1];
		ANKI_ASSERT(var->dataType != GL_NONE);

		var->arrSize = out[2];

		if(interface == GL_UNIFORM || interface == GL_PROGRAM_INPUT)
		{
			var->loc = out[0];
		}

		if(interface == GL_UNIFORM || interface == GL_BUFFER_VARIABLE)
		{
			var->arrStride = out[3];
			var->offset = out[4];
			var->matrixStride = out[5];
		}

		// Add to dict
		variablesDict[var.name.c_str()] = &var;

		// Do the block variable connection
	}
}

//==============================================================================
void GlProgram::initBlocksOfType(GLenum interface)
{
	GLint activeCount;
	glGetProgramInterfaceiv(glName, interface, GL_ACTIVE_RESOURCES, 
		&activeCount);

	for(U i = 0; i < activeCount; i++)
	{
		// Get name
		char name[1024];
		GLint len;
		glGetProgramResourceName(prog, interface, i, sizeof(name), &len, name);
		name[len] = '\0';

		// Get the properties
		Array<GLenum, 2> prop = {{GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE}};
		Array<GLint, prog.getSize()> out = {{-1, -1}};

		GLsizei outCount = 0;
		glGetProgramResourceiv(glName, interface, i, 
			prop.getSize(), &prop[0], 
			prop.getSize(), &outCount, &out[0]);

		if(prop.getSize() != outCount)
		{
			throw ANKI_EXCEPTION("glGetProgramResourceiv() didn't got all "
				"the params");
		}

		GlProgramBlock::Type akType;
		switch(interface)
		{
		case GL_UNIFORM_BLOCK;
			akType = GlProgramBlock::UNIFORM_BLOCK;
			break;
		case GL_SHADER_STORAGE_BLOCK:
			akType = GlProgramBlock::SHADER_STORAGE_BLOCK;
			break;
		default:
			ANKI_ASSERT(0);
		}

		// Create and populate the block
		blocks.push_back(GlProgramBlock());
		GlProgramBlock& block = blocks.back();

		block.type = akType;
		block.name = name;
		block.size = out[1];
		ANKI_ASSERT(out[1] > 0);
		block.bindingPoint = out[0];
		ANKI_ASSERT(out[0] >= 0);

		// Add to dict
		blocksDict[block.name.c_str()] = &block;
	}
}

} // end namespace anki
