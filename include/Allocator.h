#ifndef __ALLOCATOR_H_
#define __ALLOCATOR_H_

template<typename T>
class Allocator {
public:
    /// aliases
    typedef T               value_type;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef std::size_t     size_type;

    /// ctor
    constexpr       Allocator() noexcept                                    = default;
    constexpr       Allocator( const Allocator& rhs ) noexcept              = default;

    /// return address an of object
    static  pointer         Address( reference value ) noexcept             {   return &value;                                                              }
    static  const_pointer   Address( const_reference value ) noexcept       {   return &value;                                                              }

    /// allocates storage
    static  pointer         Allocate( size_type count )                     {   return static_cast<pointer>(::operator new(count * sizeof(value_type)));    }

    /// deallocates storage
    static  void            Deallocate( pointer ptr, size_type count )      {   ::operator delete(ptr, count);                                              }

    /// return largest allocation size
    static  size_type       MaxSize() noexcept                              {   return static_cast<size_type>(-1) / sizeof(value_type);                     }
};

#endif //!__ALLOCATOR_H_