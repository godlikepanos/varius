#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <new>

template<typename T>
struct RemovePointer;

template<typename T>
struct RemovePointer<T*>
{
	typedef T Type;
};

template<typename T>
struct RemoveReference;

template<typename T>
struct RemoveReference<T&>
{
	typedef T Type;
};

template<typename T>
struct RemoveReference<const T&>
{
	typedef T Type;
};

#define ANKI_ASSERT assert
#define ANKI_EXCEPTION std::runtime_error

#include <cstddef> // For ptrdiff_t
#include <cstring> // For memset
#include <atomic>

#if !defined(ANKI_DEBUG_ALLOCATORS)
#	define ANKI_DEBUG_ALLOCATORS 1
#endif
#define ANKI_PRINT_ALLOCATOR_MESSAGES 1

/// For debugging of the #Allocator class
struct AllocatorDebug
{
	static size_t allocatedSize;

	/// Print a few debugging messages
	static void dump()
	{
#if ANKI_DEBUG_ALLOCATORS
		if(allocatedSize > 0)
		{
			std::cout << "You have memory leak of " << allocatedSize 
				<< " bytes" << std::endl;
		}
#endif
	}
};

/// The default allocator. It uses malloc and free for 
/// allocations/deallocations. It's STL compatible
template<typename T>
class Allocator
{
public:
	// Typedefs
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	/// Default constructor
	Allocator() throw()
	{}
	/// Copy constructor
	Allocator(const Allocator&) throw()
	{}
	/// Copy constructor with another type
	template<typename U>
	Allocator(const Allocator<U>&) throw()
	{}

	/// Destructor
	~Allocator()
	{}

	/// Copy
	Allocator<T>& operator=(const Allocator&)
	{
		return *this;
	}
	/// Copy with another type
	template<typename U>
	Allocator& operator=(const Allocator<U>&) 
	{
		return *this;
	}

	pointer address(reference x) const 
	{
		return &x; 
	}
	const_pointer address(const_reference x) const 
	{
		return &x;
	}

	/// Allocate memory
	pointer allocate(size_type n, const void* = 0)
	{
		size_type size = n * sizeof(value_type);
		pointer out = (pointer)(::malloc(size));

		if(out != nullptr)
		{
			// Zero the buffer
			memset(out, 0, size);
#if ANKI_DEBUG_ALLOCATORS
			AllocatorDebug::allocatedSize += size;
#endif
		}
		else
		{
			throw ANKI_EXCEPTION("malloc() failed");
		}
		
		return out;
	}

	/// Deallocate memory
	void deallocate(void* p, size_type n)
	{
		if(p)
		{
			::free(p);
#if ANKI_DEBUG_ALLOCATORS
			AllocatorDebug::allocatedSize -= n * sizeof(T);
#else
			(void)n; // Make static analyzer happy
#endif
		}
	}

	/// Call constructor
	void construct(pointer p, const T& val)
	{
		// Placement new
		new ((T*)p) T(val); 
	}
	/// Call constructor with more arguments
	template<typename U, typename... Args>
	void construct(U* p, Args&&... args)
	{
		// Placement new
		::new((void *)p) U(std::forward<Args>(args)...);
	}

	/// Call the destructor of p
	void destroy(pointer p) 
	{
		p->~T();
	}
	/// Call the destructor of p of type U
	template<typename U>
	void destroy(U* p)
	{
		p->~U();
	}

	/// Get the max allocation size
	size_type max_size() const 
	{
		return size_t(-1); 
	}

	/// A struct to rebind the allocator to another allocator of type U
	template<typename U>
	struct rebind
	{ 
		typedef Allocator<U> other; 
	};
};

/// Another allocator of the same type can deallocate from this one
template<typename T1, typename T2>
inline bool operator==(const Allocator<T1>&, const Allocator<T2>&)
{
	return true;
}

/// Another allocator of the another type cannot deallocate from this one
template<typename T1, typename AnotherAllocator>
inline bool operator==(const Allocator<T1>&, const AnotherAllocator&)
{
	return false;
}

namespace detail {

/// Thread safe memory pool
struct MemoryPool
{
	/// Allocated memory
	void* memory = nullptr;
	/// Size of the allocated memory
	size_t size = 0;
	/// Points to the memory and more specifically to the address of the next
	/// allocation
	std::atomic<void*> ptr = {nullptr};
	/// Reference counter
	std::atomic<int> refCounter = {1};
};

} // end namespace detail

/// Stack based allocator
///
/// @tparam T The type
/// @tparam deallocationFlag If true then the allocator will try to deallocate
///                          the memory. This is extremely important to 
///                          understand when it should be true. See notes
/// @tparam alignmentBits Set the alighment in bits
///
/// @note The deallocationFlag can brake the allocator when the deallocations 
///       are not in the correct order. For example when deallocationFlag==true 
///       and the allocator is used in vector it is likely to fail
///
/// @note Don't ever EVER remove the double copy constructor and the double
///       operator=. The compiler will create defaults
template<typename T, bool deallocationFlag = false, size_t alignmentBits = 16>
class StackAllocator
{
	template<typename U, bool deallocationFlag_, size_t alignmentBits_>
	friend class StackAllocator;

public:
	// Typedefs
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	/// Default constructor deleted
	StackAllocator() = delete;
	/// Copy constructor
	StackAllocator(const StackAllocator& b) throw()
	{
		*this = b;
	}
	/// Copy constructor
	template<typename U>
	StackAllocator(const StackAllocator<U, deallocationFlag, 
		alignmentBits>& b) throw()
	{
		*this = b;
	}
	/// Constuctor with size
	StackAllocator(size_type size) throw()
	{
		init(size);
	}

	/// Destructor
	~StackAllocator()
	{
		deinit();
	}

	/// Copy
	StackAllocator& operator=(const StackAllocator& b)
	{
		mpool = b.mpool;
		// Retain the mpool
		++mpool->refCounter;
		return *this;
	}
	/// Copy
	template<typename U>
	StackAllocator& operator=(
		const StackAllocator<U, deallocationFlag, alignmentBits>& b)
	{
		mpool = b.mpool;
		// Retain the mpool
		++mpool->refCounter;
		return *this;
	}

	pointer address(reference x) const 
	{
		return &x; 
	}
	const_pointer address(const_reference x) const 
	{
		return &x;
	}

	/// Allocate memory
	pointer allocate(size_type n, const void* hint = 0)
	{
		(void)hint;
		size_type size = n * sizeof(value_type);
		size_type alignedSize = calcAlignSize(size);
		
		void* out = mpool->ptr.fetch_add(alignedSize);

#if ANKI_PRINT_ALLOCATOR_MESSAGES
			std::cout << "Allocating: size: " << size 
				<< ", size after alignment: " << alignedSize 
				<< ", returned address: " << out 
				<< ", hint: " << hint <<std::endl;
#endif

		if((char*)out + alignedSize <= (char*)mpool->memory + mpool->size)
		{
			// Everything ok
		}
		else
		{
			throw ANKI_EXCEPTION("Allocation failed. There is not enough room");
		}

		return (pointer)out;
	}

	/// Deallocate memory
	void deallocate(void* p, size_type n)
	{
		(void)p;
		(void)n;

		if(deallocationFlag)
		{
			size_type alignedSize = calcAlignSize(n * sizeof(value_type));
#if ANKI_PRINT_ALLOCATOR_MESSAGES
			std::cout << "Deallocating: size: " << (n * sizeof(value_type))
				<< " alignedSize: " << alignedSize 
				<< " pointer: " << p << std::endl;
#endif
			void* headPtr = mpool->ptr.fetch_sub(alignedSize);

			if((char*)headPtr - alignedSize != p)
			{
				throw ANKI_EXCEPTION("Freeing wrong pointer. "
					"The deallocations on StackAllocator should be in order");
			}

			ANKI_ASSERT(((char*)headPtr - alignedSize) >= mpool->memory);
		}
	}

	/// Call constructor
	void construct(pointer p, const T& val)
	{
		// Placement new
		new ((T*)p) T(val); 
	}
	/// Call constructor
	template<typename U, typename... Args>
	void construct(U* p, Args&&... args)
	{
		// Placement new
		::new((void *)p) U(std::forward<Args>(args)...);
	}

	/// Call destructor
	void destroy(pointer p) 
	{
		p->~T();
	}
	/// Call destructor
	template<typename U>
	void destroy(U* p)
	{
		p->~U();
	}

	/// Get the max allocation size
	size_type max_size() const 
	{
		return mpool->size; 
	}

	template<typename U>
	struct rebind
	{ 
		typedef StackAllocator<U, deallocationFlag, alignmentBits> other; 
	};

	/// Reset the allocator
	void rewind()
	{
		mpool->ptr = mpool->memory;
	}

private:
	/// The memory pool
	detail::MemoryPool* mpool = nullptr;

	/// Called by a constructor
	void init(size_type size)
	{
		mpool = new detail::MemoryPool;

		if(mpool != nullptr)
		{
			mpool->memory = ::malloc(size);

			if(mpool->memory != nullptr)
			{
				mpool->size = size;
				mpool->ptr = mpool->memory;
				// Memory pool's refcounter is 1
#if ANKI_PRINT_ALLOCATOR_MESSAGES
			std::cout << "New MemoryPool created: Address: " << mpool 
				<< ", size: " << size 
				<< ", pool address: " << mpool->memory << std::endl;
#endif
				return;
			}
		}

		// Errors happened

		if(mpool->memory)
		{
			::free(mpool->memory);
		}

		if(mpool)
		{
			delete mpool;
		}

		std::cerr << "Stack allocator constuctor failed. I will cannot " 
			"throw but I have to exit" << std::endl;
		exit(0);
	}

	/// Called by the destructor
	void deinit()
	{
		int refCounter = mpool->refCounter.fetch_sub(1);

		if(mpool && (refCounter - 1) == 0)
		{
#if ANKI_PRINT_ALLOCATOR_MESSAGES
			std::cout << "Deleting MemoryPool " << mpool << std::endl;
#endif
			// It is safe to delete the memory pool
			ANKI_ASSERT(mpool->refCounter == 0);

			::free(mpool->memory);
			delete mpool;
		}
	}

	/// Calculate tha align size
	size_type calcAlignSize(size_type size)
	{
		return size + (size % (alignmentBits / 8));
	}
};

/// Another allocator of the same type can deallocate from this one
template<typename T1, typename T2, bool deallocationFlag, size_t alignmentBits>
inline bool operator==(
	const StackAllocator<T1, deallocationFlag, alignmentBits>&, 
	const StackAllocator<T2, deallocationFlag, alignmentBits>&)
{
	return true;
}

/// Another allocator of the another type cannot deallocate from this one
template<typename T1, typename AnotherAllocator, bool deallocationFlag, 
	size_t alignmentBits>
inline bool operator==(
	const StackAllocator<T1, deallocationFlag, alignmentBits>&, 
	const AnotherAllocator&)
{
	return false;
}

/// Function that imitates the new operator. The function allocates memory for 
/// a number of elements and calls their constructor. The interesting thing is 
/// that if the elements size is >1 then it allocates size bigger than the 
/// required. The extra chunk is a number that will be used in 
/// deleteObjectArray to identify the number of elements that were allocated
template<typename T, typename Alloc = Allocator<T>>
struct New
{
	size_t n;
	Alloc alloc;

	New(size_t n_ = 1, const Alloc& alloc_ = Alloc())
		: n(n_), alloc(alloc_)
	{}

	template<typename... Args>
	T* operator()(Args&&... args)
	{
		T* out = nullptr;

		// If the number of elements is then do a simple allocaton
		if(n == 1)
		{
			out = alloc.allocate(n);
		}
		else
		{
			// Allocate a memory block that includes the array size
			typedef typename Alloc::template rebind<char>::other CharAlloc;
			CharAlloc charAlloc(alloc);
			char* mem = charAlloc.allocate(sizeof(size_t) + n * sizeof(T));

			// Set the size of the block
			*(size_t*)mem = n;

			// Set the output address
			out = (T*)(mem + sizeof(size_t));
		}

		// Call the constuctors
		for(size_t i = 0; i < n; i++)
		{
			alloc.construct(&out[i], std::forward<Args>(args)...);
		}

		// Return result
		return out;
	}
};

/// Function that imitates the delete operator
template<typename T, typename Alloc = Allocator<T>>
struct Delete
{
	Alloc alloc;

	Delete(const Alloc& alloc_ = Alloc())
		: alloc(alloc_)
	{}

	void operator()(void* ptr)
	{
		// Make sure the type is defined
		typedef char TypeMustBeComplete[sizeof(T) ? 1 : -1];
		(void) sizeof(TypeMustBeComplete);

		T* p = (T*)ptr;

		// Call the destructor
		alloc.destroy(p);

		// Deallocate
		alloc.deallocate(p, 1);
	}
};

/// Function that imitates the delete[] operator
template<typename T, typename Alloc>
void deleteObjectArray(const Alloc& allocator, T* p)
{
	ANKI_ASSERT(p);

	// Make sure the type is defined
	typedef char TypeMustBeComplete[sizeof(T) ? 1 : -1];
	(void) sizeof(TypeMustBeComplete);

	// Rebind allocator
	typename Alloc::template rebind<T>::other alloc(allocator);

	// Get the allocated block
	char* block = (char*)(p) - sizeof(size_t);

	// Get size of elements
	size_t size = *(size_t*)block;

	// Call the destructors
	for(size_t i = 0; i < size; i++)
	{
		alloc.destroy(&p[i]);
	}

	// Deallocate the block
	typename Alloc::template rebind<char>::other allocc(allocator);
	allocc.deallocate(block, size * sizeof(T) + sizeof(size_t));
}

// Don't wrap it around ANKI_DEBUG_ALLOCATORS because we want this to link 
// under all circumstances
size_t AllocatorDebug::allocatedSize = 0;

struct Foo
{
	int x = -1;

	Foo()
	{
		std::cout << "Foo()" << std::endl;
	}
	Foo(const Foo& f)
		: x(f.x)
	{
		std::cout << "Foo(const Foo&) " << x << std::endl;
	}
	Foo(Foo&& f)
	{
		x = f.x;
		f.x = 0;
		std::cout << "Foo(Foo&&)" << std::endl;
	}
	Foo(int x_)
		: x(x_)
	{
		std::cout << "Foo(int) " << x << std::endl;
	}
	Foo(Allocator<Foo>&)
	{
		std::cout << "Foo(Allocator<Foo>&) " << x << std::endl;
	}

	~Foo()
	{
		std::cout << "~Foo() " << x << std::endl;
	}
};

int main(int, char**) 
{
	std::cout << "main() starts" << std::endl;

	//Allocator<Foo> alloc;
	//std::vector<Foo, Allocator<Foo>> vec(alloc);
	//std::vector<Foo> vec;
	/*vec.resize(1);
	vec.shrink_to_fit();
	vec.resize(256);*/

#if 0
	/*Foo* foo = Allocator<Foo>().allocate(1);
	Allocator<Foo>().construct(foo, 10);*/
	Allocator<Foo> alloc;
	//Foo* foo = ANKI_NEW_ARRAY(Foo, alloc, 2);
	Foo* foo = New<Foo>(2, alloc)();
	//ANKI_DELETE(foo, Allocator);

	std::cout << foo->x << std::endl;
#endif

	//Foo* foo = new Foo[2];

	/*{
		typedef StackAllocator<int, true, 16> Alloc;
		Alloc salloc(16 * sizeof(int));
		std::vector<int, Alloc> vec(salloc);
		//std::vector<int> vec;

		vec.resize(10, 1);
		std::cout << &vec[0] << std::endl;

		for(int i : vec)
		{
			std::cout << i << std::endl;
		}
	}*/

	typedef StackAllocator<char, false> SceneStackAllocator;
	SceneStackAllocator stackAllocator(32);

	Foo f(666);

	Foo* pf = New<Foo, StackAllocator<Foo>>(2, stackAllocator)(f);

	AllocatorDebug::dump();
	return 0;
}
