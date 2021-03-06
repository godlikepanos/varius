#include "anki/util/Memory.h"
#include "anki/util/Exception.h"
#include "anki/util/Functions.h"
#include "anki/util/Assert.h"
#include "anki/util/NonCopyable.h"
#include "anki/util/Thread.h"
#include "anki/util/Vector.h"
#include <cstdlib>
#include <cstring>

namespace anki {

//==============================================================================
// Other                                                                       =
//==============================================================================

//==============================================================================
void* mallocAligned(PtrSize size, PtrSize alignmentBytes)
{
#if ANKI_POSIX 
#	if ANKI_OS != ANKI_OS_ANDROID
	void* out;
	int err = posix_memalign(
		&out, getAlignedRoundUp(alignmentBytes, sizeof(void*)), size);

	if(!err)
	{
		// Make sure it's aligned
		ANKI_ASSERT(isAligned(alignmentBytes, out));
		return out;
	}
	else
	{
		throw ANKI_EXCEPTION("mallocAligned() failed");
		return nullptr;
	}
#	else
	void* out = memalign(
		getAlignedRoundUp(alignmentBytes, sizeof(void*)), size);

	if(out)
	{
		// Make sure it's aligned
		ANKI_ASSERT(isAligned(alignmentBytes, out));
		return out;
	}
	else
	{
		throw ANKI_EXCEPTION("mallocAligned() failed");
		return nullptr;
	}
#	endif
#else
#	error "Unimplemented"
#endif
}

//==============================================================================
void freeAligned(void* ptr)
{
#if ANKI_POSIX
	::free(ptr);
#else
#	error "Unimplemented"
#endif
}

//==============================================================================
// StackMemoryPool                                                             =
//==============================================================================

//==============================================================================
class StackMemoryPool::Implementation: public NonCopyable
{
public:
	/// The header of each allocation
	struct MemoryBlockHeader
	{
		U32 size;
	};

	/// Alignment of allocations
	U32 alignmentBytes;

	/// Pre-allocated memory chunk
	U8* memory = nullptr;

	/// Size of the pre-allocated memory chunk
	PtrSize memsize = 0;

	/// Points to the memory and more specifically to the top of the stack
	std::atomic<U8*> top = {nullptr};

	// Construct
	Implementation(PtrSize size, U32 alignmentBytes_)
		:	alignmentBytes(alignmentBytes_), 
			memsize(getAlignedRoundUp(alignmentBytes, size))
	{
		ANKI_ASSERT(memsize > 0);
		memory = (U8*)mallocAligned(memsize, alignmentBytes);

		if(memory != nullptr)
		{
			// Align allocated memory
			top = memory;
		}
		else
		{
			throw ANKI_EXCEPTION("Failed to allocate memory");
		}
	}

	// Destroy
	~Implementation()
	{
		if(memory != nullptr)
		{
			freeAligned(memory);
		}
	}

	PtrSize getTotalSize() const
	{
		return memsize;
	}

	PtrSize getAllocatedSize() const
	{
		ANKI_ASSERT(memory != nullptr);
		return top.load() - memory;
	}

	const void* getBaseAddress() const
	{
		ANKI_ASSERT(memory != nullptr);
		return memory;
	}

	/// Allocate
	void* allocate(PtrSize size_) throw()
	{
		// memory is nullptr if moved
		ANKI_ASSERT(memory != nullptr);

		PtrSize headerSize = 
			getAlignedRoundUp(alignmentBytes, sizeof(MemoryBlockHeader));
		PtrSize size = 
			getAlignedRoundUp(alignmentBytes, size_ + headerSize);

		ANKI_ASSERT(size < MAX_U32 && "Too big allocation");

		U8* out = top.fetch_add(size);

		if(out + size <= memory + memsize)
		{
			// Write the block header
			((MemoryBlockHeader*)out)->size = size;

			// Set the correct output
			out += headerSize;

			// Check alignment
			ANKI_ASSERT(isAligned(alignmentBytes, out));
		}
		else
		{
			// Error
			out = nullptr;
		}

		return out;
	}

	/// Free
	Bool free(void* ptr) throw()
	{
		// ptr shouldn't be null or not aligned. If not aligned it was not 
		// allocated by this class
		ANKI_ASSERT(ptr != nullptr && isAligned(alignmentBytes, ptr));

		// memory is nullptr if moved
		ANKI_ASSERT(memory != nullptr);

		// Correct the p
		PtrSize headerSize = 
			getAlignedRoundUp(alignmentBytes, sizeof(MemoryBlockHeader));
		U8* realptr = (U8*)ptr - headerSize;

		// realptr should be inside the pool's preallocated memory
		ANKI_ASSERT(realptr >= memory);

		// Get block size
		U32 size = ((MemoryBlockHeader*)realptr)->size;

		// Check if the size is ok
		ANKI_ASSERT(realptr + size <= memory + memsize);

		// Atomic stuff
		U8* expected = realptr + size;
		U8* desired = realptr;

		// if(top == expected) {
		//     top = desired;
		//     exchange = true;
		// } else {
		//     expected = top;
		//     exchange = false;
		// }
		Bool exchange = top.compare_exchange_strong(expected, desired);

		return exchange;
	}

	/// Reset
	void reset()
	{
		// memory is nullptr if moved
		ANKI_ASSERT(memory != nullptr);

#if ANKI_DEBUG
		// Invalidate the memory
		memset(memory, 0xCC, memsize);
#endif

		top = memory;
	}
};

//==============================================================================
StackMemoryPool::StackMemoryPool(PtrSize size, U32 alignmentBytes)
{
	impl.reset(new Implementation(size, alignmentBytes));
}

//==============================================================================
StackMemoryPool::~StackMemoryPool()
{}

//==============================================================================
PtrSize StackMemoryPool::getTotalSize() const
{
	ANKI_ASSERT(impl.get() != nullptr);
	return impl->getTotalSize();
}

//==============================================================================
PtrSize StackMemoryPool::getAllocatedSize() const
{
	ANKI_ASSERT(impl.get() != nullptr);
	return impl->getAllocatedSize();
}

//==============================================================================
void* StackMemoryPool::allocate(PtrSize size) throw()
{
	ANKI_ASSERT(impl.get() != nullptr);
	return impl->allocate(size);
}

//==============================================================================
Bool StackMemoryPool::free(void* ptr) throw()
{
	ANKI_ASSERT(impl.get() != nullptr);
	return impl->free(ptr);
}

//==============================================================================
void StackMemoryPool::reset()
{
	ANKI_ASSERT(impl.get() != nullptr);
	impl->reset();
}

//==============================================================================
// ChainMemoryPool                                                             =
//==============================================================================

//==============================================================================
class ChainMemoryPool::Implementation: public NonCopyable
{
public:
	/// A chunk of memory
	class Chunk
	{
	public:
		StackMemoryPool::Implementation pool;
		U32 allocationsCount;

		Chunk(PtrSize size, U32 alignmentBytes)
			: pool(size, alignmentBytes), allocationsCount(0)
		{}
	};

	/// Alignment of allocations
	U32 alignmentBytes;

	/// A list of chunks
	Vector<Chunk*> chunks;

	/// Current chunk to allocate from
	Chunk* crntChunk = nullptr;

	/// Fast thread locking
	SpinLock lock;

	/// Chunk first chunk size
	U32 initSize;

	/// Chunk max size
	U32 maxSize;

	/// Chunk allocation method value
	U32 step;

	/// Chunk allocation method
	U8 method;

	/// Construct
	Implementation(
		U32 initialChunkSize,
		U32 maxChunkSize,
		ChunkAllocationStepMethod chunkAllocStepMethod, 
		U32 chunkAllocStep, 
		U32 alignmentBytes_)
		:	alignmentBytes(alignmentBytes_), 
			initSize(initialChunkSize),
			maxSize(maxChunkSize),
			step(chunkAllocStep),
			method(chunkAllocStepMethod)
	{
		// Initial size should be > 0
		ANKI_ASSERT(initSize > 0);

		// On fixed step should be 0
		ANKI_ASSERT(!(method == FIXED && step != 0));

		// On fixed initial size is the same as the max
		ANKI_ASSERT(!(method == FIXED && initSize != maxSize));

		// On add and mul the max size should be greater than initial
		ANKI_ASSERT(!(
			(method == ADD || method == MULTIPLY) && initSize < maxSize));
	}

	/// Destroy
	~Implementation()
	{
		for(Chunk* ch : chunks)
		{
			ANKI_ASSERT(ch);
			delete ch;
		}
	}

	/// Create a new chunk
	Chunk* createNewChunk(PtrSize size) throw()
	{
		ANKI_ASSERT(size <= maxSize && "To big chunk");
		//
		// Calculate preferred size
		//
		
		// Get the size of the next chunk
		PtrSize crntMaxSize;
		if(method == FIXED)
		{
			crntMaxSize = initSize;
		}
		else
		{
			// Get the size of the previous max chunk
			if(chunks.size() > 0)
			{
				// Get the size of previous
				crntMaxSize = chunks.back()->pool.getTotalSize();

				// Increase it
				if(method == MULTIPLY)
				{
					crntMaxSize *= step;
				}
				else
				{
					ANKI_ASSERT(method == ADD);
					crntMaxSize += step;
				}
			}
			else
			{
				// No chunks. Choose initial size

				ANKI_ASSERT(crntChunk == nullptr);
				crntMaxSize = initSize;
			}

			ANKI_ASSERT(crntMaxSize > 0);

			// Fix the size
			crntMaxSize = std::min(crntMaxSize, (PtrSize)maxSize);
		}

		size = crntMaxSize;

		//
		// Create the chunk
		//
		Chunk* chunk = new Chunk(size, alignmentBytes);

		if(chunk)
		{
			chunks.push_back(chunk);
		}
		else
		{
			throw ANKI_EXCEPTION("Chunk creation failed");
		}
		
		return chunk;
	}

	/// Allocate from chunk
	void* allocateFromChunk(Chunk* ch, PtrSize size) throw()
	{
		ANKI_ASSERT(ch);
		ANKI_ASSERT(size <= maxSize);
		void* mem = ch->pool.allocate(size);

		if(mem)
		{
			++ch->allocationsCount;
		}

		return mem;
	}

	/// Allocate memory
	void* allocate(PtrSize size) throw()
	{
		Chunk* ch;
		void* mem = nullptr;

		lock.lock();

		// Get chunk
		ch = crntChunk;

		// Create new chunk if needed
		if(ch == nullptr || (mem = allocateFromChunk(ch, size)) == nullptr)
		{
			// Create new chunk
			crntChunk = createNewChunk(size);
			ch = crntChunk;

			// Chunk creation failed
			if(ch == nullptr)
			{
				lock.unlock();
				return mem;
			}
		}

		if(mem == nullptr)
		{
			mem = allocateFromChunk(ch, size);
			ANKI_ASSERT(mem != nullptr && "The chunk should have space");
		}

		lock.unlock();
		return mem;
	}

	/// Free memory
	Bool free(void* ptr) throw()
	{
		lock.lock();

		// Get the chunk that ptr belongs to
		Vector<Chunk*>::iterator it;
		for(it = chunks.begin(); it != chunks.end(); it++)
		{
			Chunk* ch = *it;
			ANKI_ASSERT(ch);

			const U8* from = (const U8*)ch->pool.getBaseAddress();
			const U8* to = from + ch->pool.getTotalSize();
			const U8* cptr = (const U8*)ptr;
			if(cptr >= from && cptr < to)
			{
				break;
			}
		}

		ANKI_ASSERT(it != chunks.end() 
			&& "Not initialized or ptr is incorrect");

		// Decrease the deallocation refcount and if it's zero delete the chunk
		ANKI_ASSERT((*it)->allocationsCount > 0);
		if(--(*it)->allocationsCount == 0)
		{
			// Chunk is empty. Delete it

			Chunk* ch = *it;
			ANKI_ASSERT(ch);
			
			delete ch;
			chunks.erase(it);

			// Change the crntChunk
			ANKI_ASSERT(crntChunk);
			if(ch == crntChunk)
			{
				// If there are chunks chose the last. It's probably full but
				// let the allocate() create a new one
				if(chunks.size() > 0)
				{
					crntChunk = chunks.back();
					ANKI_ASSERT(crntChunk);
				}
				else
				{
					crntChunk = nullptr;
				}
			}
		}

		lock.unlock();

		return true;
	}
};

//==============================================================================
ChainMemoryPool::ChainMemoryPool(
	U32 initialChunkSize,
	U32 maxChunkSize,
	ChunkAllocationStepMethod chunkAllocStepMethod, 
	U32 chunkAllocStep, 
	U32 alignmentBytes)
{
	impl.reset(new Implementation(
		initialChunkSize, maxChunkSize, chunkAllocStepMethod, chunkAllocStep,
		alignmentBytes));
}

//==============================================================================
ChainMemoryPool::~ChainMemoryPool()
{}

//==============================================================================
void* ChainMemoryPool::allocate(PtrSize size) throw()
{
	return impl->allocate(size);
}

//==============================================================================
Bool ChainMemoryPool::free(void* ptr) throw()
{
	return impl->free(ptr);
}

//==============================================================================
PtrSize ChainMemoryPool::getChunksCount() const
{
	return impl->chunks.size();
}

} // end namespace anki
