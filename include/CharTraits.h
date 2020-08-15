#ifndef __CHAR_TRAITS_H
#define __CHAR_TRAITS_H
#include "CString.h"

template <class Char>
struct CharTraits {
    typedef Char            char_type;
    typedef Char*           pointer;
    typedef const Char*     const_pointer;

    static  int             Compare( const_pointer lhs, const_pointer rhs, size_t count ) noexcept      {   return StrCmp( lhs, rhs, count );   }
    static  size_t          Length( const_pointer src ) noexcept                                        {   return StrLen( src );               }
    static  pointer         Copy( pointer dest, const_pointer src, size_t count ) noexcept              {   return StrCpy( dest, src, count );  }
    static  const_pointer   Find( const_pointer src, size_t length, const char_type ch ) noexcept       {   return StrChr( src, length, ch );   }
    static  pointer         Move( pointer dest, const_pointer src, size_t count ) noexcept              {   return StrCat( dest, src, count );  }
    static  pointer         Assign_n_copies( pointer dest, size_t count, const char_type ch ) noexcept  {   return StrSet( dest, count, ch );   }
    static  void            Assign( char_type& lhs, const char_type& rhs ) noexcept                     {   lhs = rhs;                          }
};

#endif // !__STRING_H
