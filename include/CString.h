#ifndef __CSTRING_H
#define __CSTRING_H

template<class Char>
constexpr int StrCmp( const Char* lhs, const Char* rhs, size_t length )
{
    for ( ; 0 < length; --length, ++lhs, ++rhs )
    {
        if ( *lhs != *rhs )
            return *lhs < *rhs ? -1 : +1;
    }
    return 0;
}

template<class Char>
constexpr size_t StrLen( const Char* src )
{
    size_t length = 0;
    while ( *src++ != Char() )
    {
        ++length;
    }
    return length;
}

template<class Char>
constexpr Char* StrCpy( Char* dest, const Char* src, size_t count )
{
    auto Dest = dest;
    auto Src = src;
    for ( ; 0 < count; --count )
    {
        *Dest++ = *Src++;
    }
    return dest;
}

template<class Char>
constexpr Char* StrChr( const Char* src, size_t length, const Char ch )
{
    for ( ; 0 < length; --length, ++src )
    {
        if ( *src == ch )
            return src;
    }
    return nullptr;
}

template<class Char>
constexpr Char* StrCat(Char* dest, const Char* src, size_t count) noexcept
{
    if ( dest < src )
    {
        while ( count-- )
            *dest++ = *src++;
    }
    else
    {
        auto Src = const_cast<char*>( src + count - 1 );
        auto Dest = dest + count - 1;
        while ( count-- )
            *Dest-- = *Src--;
    }

    return dest;
}

template<class Char>
constexpr Char* StrSet(Char* dest, size_t count, const Char ch) noexcept
{
    Char* Dest = dest;
    for ( ; 0 < count; --count, ++Dest )
    {
        *Dest = ch;
    }
    return dest;
}

#endif //!__CSTRING_H