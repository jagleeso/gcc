// Pointer Traits -*- C++ -*-

// Copyright (C) 2011-2015 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/ptr_traits.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{memory}
 */

#ifndef _PTR_TRAITS_H
#define _PTR_TRAITS_H 1

#if __cplusplus >= 201103L

#include <type_traits>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  class __undefined;

  // Given Template<T, ...> return T, otherwise invalid.
  template<typename _Tp>
    struct __get_first_arg
    { using type = __undefined; };

  template<template<typename, typename...> class _Template, typename _Tp,
           typename... _Types>
    struct __get_first_arg<_Template<_Tp, _Types...>>
    { using type = _Tp; };

  template<typename _Tp>
    using __get_first_arg_t = typename __get_first_arg<_Tp>::type;

  // Given Template<T, ...> and U return Template<U, ...>, otherwise invalid.
  template<typename _Tp, typename _Up>
    struct __replace_first_arg
    { using type = __undefined; };

  template<template<typename, typename...> class _Template, typename _Up,
           typename _Tp, typename... _Types>
    struct __replace_first_arg<_Template<_Tp, _Types...>, _Up>
    { using type = _Template<_Up, _Types...>; };

  template<typename _Tp, typename _Up>
    using __replace_first_arg_t = typename __replace_first_arg<_Tp, _Up>::type;

  template<typename _Tp>
    using __make_not_void
      = typename conditional<is_void<_Tp>::value, __undefined, _Tp>::type;

  /**
   * @brief  Uniform interface to all pointer-like types
   * @ingroup pointer_abstractions
  */
  template<typename _Ptr>
    struct pointer_traits
    {
    private:
      template<typename _Tp>
	using __element_type = typename _Tp::element_type;

      template<typename _Tp>
	using __difference_type = typename _Tp::difference_type;

      template<typename _Tp, typename _Up>
	using __rebind = typename _Tp::template rebind<_Up>;

    public:
      /// The pointer type.
      using pointer = _Ptr;

      /// The type pointed to.
      using element_type
	= __detected_or_t_<__get_first_arg_t, __element_type, _Ptr>;

      /// The type used to represent the difference between two pointers.
      using difference_type
	= __detected_or_t<ptrdiff_t, __difference_type, _Ptr>;

      /// A pointer to a different type.
      template<typename _Up>
        using rebind
	  = __detected_or_t_<__replace_first_arg_t, __rebind, _Ptr, _Up>;

      static _Ptr
      pointer_to(__make_not_void<element_type>& __e)
      { return _Ptr::pointer_to(__e); }

      static_assert(!is_same<element_type, __undefined>::value,
	  "pointer type defines element_type or is like SomePointer<T, Args>");
      static_assert(!is_same<rebind<element_type>, __undefined>::value,
	  "pointer type defines rebind<U> or is like SomePointer<T, Args>");
    };

  /**
   * @brief  Partial specialization for built-in pointers.
   * @ingroup pointer_abstractions
  */
  template<typename _Tp>
    struct pointer_traits<_Tp*>
    {
      /// The pointer type
      typedef _Tp* pointer;
      /// The type pointed to
      typedef _Tp  element_type;
      /// Type used to represent the difference between two pointers
      typedef ptrdiff_t difference_type;

      template<typename _Up>
        using rebind = _Up*;

      /**
       *  @brief  Obtain a pointer to an object
       *  @param  __r  A reference to an object of type @c element_type
       *  @return @c addressof(__r)
      */
      static pointer
      pointer_to(__make_not_void<element_type>& __r) noexcept
      { return std::addressof(__r); }
    };

  /// Convenience alias for rebinding pointers.
  template<typename _Ptr, typename _Tp>
    using __ptr_rebind = typename pointer_traits<_Ptr>::template rebind<_Tp>;

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

#endif

#endif
