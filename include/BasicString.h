#ifndef __STRING_H
#define __STRING_H
#include <iostream>
#include "CharTraits.h"
#include "Allocator.h"

enum class StringCategory {
    SC_Large,
    SC_Small
};

template<class Char, class Traits = CharTraits<Char>, class Alloc = Allocator<Char>>
class BasicString {
public:
    typedef StringCategory          category_type;
    typedef Traits                  traits_type;
    typedef Alloc                   allocator_type;

    typedef Char                    value_type; 
    typedef size_t                  size_type;
    typedef Char*                   pointer;
    typedef const Char*             const_pointer;
    typedef Char&                   reference;
    typedef const Char&             const_reference;

private:
    //
    pointer         Ptr() noexcept                                          {   return IsLarge() ? m_string.ptr : m_string.buffer;      }
    const_pointer   Ptr() const noexcept                                    {   return IsLarge() ? m_string.ptr : m_string.buffer;      }
    bool            IsLarge() const noexcept                                {   return m_category == StringCategory::SC_Large;          }
    
    //
    void            Move( BasicString& rhs );
    size_t          CalculateCapacity( const size_t size );
    template<class Fn, class... Args>
    BasicString&    Assign( const size_type count, Fn fn, Args... args );
    template<class Fn, class... Args>
    BasicString&    Append( const size_type count, Fn fn, Args... args );

public:
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    /// ctors/dtor
    BasicString() noexcept                                                  {   traits_type::Assign(*Ptr(), value_type());                          }
    BasicString( const_pointer ptr )                                        {   assign( ptr, traits_type::Length(ptr) );                            }
    BasicString( const_pointer ptr, const size_type size )                  {   assign( ptr, size );                                                }
    BasicString( const size_type count, const value_type ch )               {   assign( count, ch );                                                }
    BasicString( const BasicString& rhs )                                   {   assign( rhs.data(), rhs.size() );                                   }
    BasicString( BasicString&& rhs ) noexcept                               {   Move( rhs );                                                        }
    ~BasicString()                                                          {   if( IsLarge() ) allocator_type::Deallocate( Ptr(), capacity() );    }

    BasicString&    operator=( const BasicString& rhs );
    BasicString&    operator=( BasicString&& rhs ) noexcept;
    BasicString&    operator=(const_pointer src)                            {   return assign( src );                                               }
    BasicString&    operator=( const Char ch )                              {   return assign( 1, ch );                                             }

    // Assign
    BasicString&    assign( const BasicString& rhs )                        {   return *this = rhs;                                                 }
    BasicString&    assign( const_pointer src )                             {   return assign( src, traits_type::Length(src) );                     }
    BasicString&    assign( const_pointer src, const size_type count )      {   return Assign( count, traits_type::Copy, src, count );              }
    BasicString&    assign( const size_type count, const value_type ch )    {   return Assign( count, traits_type::Assign_n_copies, count, ch );    }
    
    // Append
    BasicString&    append( const_pointer src )                             {   return append( src, traits_type::Length(src) );                     }
    BasicString&    append( const BasicString& rhs )                        {   return append( rhs.data(), rhs.size() );                            }
    BasicString&    append( const_pointer src, const size_type count )      {   return Append( count, traits_type::Copy, src, count );              }
    BasicString&    append(const size_type count, const value_type ch)      {   return Append( count, traits_type::Assign_n_copies, count, ch );    }
    
    // Arithmetics
    BasicString&    operator+=( const_pointer src )                         {   return append( src );                                               }
    BasicString&    operator+=( const value_type ch )                       {   return append( 1, ch );                                             }
    BasicString&    operator+=( const BasicString& rhs )                    {   return append( rhs.data(), rhs.size() );                            }
      
    // Info
    size_type       size() const noexcept                                   {   return m_size;                                                      }
    bool            empty() const noexcept                                  {   return size() == 0;                                                 }
    size_type       length() const noexcept                                 {   return m_size;                                                      }
    size_type       capacity() const noexcept                               {   return m_capacity;                                                  }
    category_type   category() const noexcept                               {   return m_category;                                                  }
    size_type       max_size() const noexcept                               {   return allocator_type::MaxSize();                                   }

    // Access
    const_pointer   data() const noexcept                                   {   return Ptr();                                                       }
    const_pointer   c_str() const noexcept                                  {   return Ptr();                                                       }
    reference       operator[]( const size_type pos ) noexcept              {   return *( Ptr() + pos );                                            }
    const_reference operator[]( const size_type pos ) const noexcept        {   return this->operator[]( pos );                                     }

private:
    static constexpr size_t BUF_SIZE = 16;
    // SSO - small string optimisation
    union SSO
    {
        pointer			ptr = nullptr;
        value_type      buffer[BUF_SIZE];
    };
    
    //
    SSO                 m_string;
    size_type		    m_size = 0;
    size_type           m_capacity = BUF_SIZE - 1;
    category_type       m_category = category_type::SC_Small;
};

template<class Char, class Traits, class Alloc>
size_t BasicString<Char, Traits, Alloc>::CalculateCapacity( const size_t size )
{
    size_type maxSize = allocator_type::MaxSize();
    if ( size > maxSize )
        return maxSize;

    size_t newCapacity = capacity();
    while ( size > newCapacity )
        newCapacity += BUF_SIZE;
    return newCapacity;
}

template<class Char, class Traits, class Alloc>
void BasicString<Char, Traits, Alloc>::Move( BasicString& rhs )
{
    if ( rhs.IsLarge() )
    {
        m_string.ptr = rhs.m_string.ptr;
        rhs.m_string.ptr = nullptr;
        m_category = category_type::SC_Large;
    }
    else
    {
        traits_type::Copy( Ptr(), rhs.Ptr(), rhs.m_size + 1 );
        m_category = category_type::SC_Small;
    }

    m_size = rhs.m_size;
    m_capacity = rhs.m_capacity;

    rhs.m_size = 0;
    rhs.m_capacity = BUF_SIZE - 1;
    rhs.m_category = category_type::SC_Small;
    traits_type::Assign( *rhs.Ptr(), value_type() );
}

template<class Char, class Traits, class Alloc>
template<class Fn, class ...Args>
BasicString<Char, Traits, Alloc>& BasicString<Char, Traits, Alloc>::Assign( const size_type count, Fn fn, Args ...args )
{
    if ( IsLarge() )
        allocator_type::Deallocate( Ptr(), capacity() );

    if ( count < BUF_SIZE )
    {
        m_category = category_type::SC_Small;
    }
    else
    {
        const size_type newCapacity = CalculateCapacity( count );
        m_capacity = newCapacity;
        m_string.ptr = allocator_type::Allocate( newCapacity );
        m_category = StringCategory::SC_Large;
    }

    fn( Ptr(), args... );
    traits_type::Assign( *(Ptr() + count), value_type() );
    m_size = count;
    return *this;
}

template<class Char, class Traits, class Alloc>
template<class Fn, class ...Args>
BasicString<Char, Traits, Alloc>& BasicString<Char, Traits, Alloc>::Append( const size_type count, Fn fn, Args ...args )
{
    const size_type oldSize = size();
    if ( count <= ( capacity() - oldSize ) )
    {
        fn( Ptr() + oldSize, args... );
    }
    else
    {
        const size_type newCapacity = CalculateCapacity( oldSize + count );
        const size_type oldCapacity = m_capacity;
        pointer oldPtr = Ptr();
        pointer newPtr = allocator_type::Allocate( newCapacity );

        traits_type::Copy( newPtr, oldPtr, oldSize );
        fn( newPtr + oldSize, args... );
        m_capacity = newCapacity;
        m_string.ptr = newPtr;

        if (IsLarge())
            allocator_type::Deallocate( oldPtr, oldCapacity );
        else
            m_category = category_type::SC_Large;
    }

    m_size += count;
    traits_type::Assign( *(Ptr() + m_size), value_type() );
    return *this;
}

template<class Char, class Traits, class Alloc>
BasicString<Char, Traits, Alloc>& BasicString<Char, Traits, Alloc>::operator=( const BasicString<Char, Traits, Alloc>& rhs )
{
     if ( this != &rhs )
         assign( rhs.data(), rhs.size() );
     return *this;
}

template<class Char, class Traits, class Alloc>
BasicString<Char, Traits, Alloc>& BasicString<Char, Traits, Alloc>::operator=( BasicString<Char, Traits, Alloc>&& rhs ) noexcept
 {
     if ( this != &rhs )
     {
         if ( IsLarge() )
              allocator_type::Deallocate( Ptr(), capacity() );
         Move( rhs );
     }
     return *this;
 }

template<class Char, class Traits, class Alloc>
BasicString<Char, Traits, Alloc> operator+( const BasicString<Char, Traits, Alloc>& lhs, const BasicString<Char, Traits, Alloc>& rhs )
{
    BasicString<Char, Traits> temp( lhs );
    return temp += rhs;
}

template<class Char, class Traits, class Alloc>
BasicString<Char, Traits, Alloc> operator+( const BasicString<Char, Traits, Alloc>& lhs, const Char* rhs )
{
    BasicString<Char, Traits> temp( lhs );
    return temp += rhs;
}

template<class Char, class Traits, class Alloc>
BasicString<Char, Traits, Alloc> operator+( const BasicString<Char, Traits, Alloc>& lhs, const Char rhs )
{
    BasicString<Char, Traits> temp( lhs );
    return temp += rhs;
}

template<class Char, class Traits, class Alloc>
std::ostream& operator<<( std::ostream& out, const BasicString<Char, Traits, Alloc>& basic_string )
{
    out << basic_string.data();
    return out;
}

typedef BasicString<char, CharTraits<char>, Allocator<char>> String;

#endif // !__STRING_H
