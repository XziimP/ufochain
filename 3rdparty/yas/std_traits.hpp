
// Copyright (c) 2010-2018 niXman (i dot nixman dog gmail dot com). All
// rights reserved.
//
// This file is part of YAS(https://github.com/niXman/yas) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef __yas__std_traits_hpp
#define __yas__std_traits_hpp

#include <yas/detail/config/config.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace yas {
namespace detail {

/***************************************************************************/

struct std_traits {
	template<typename T>
	static void atou(T &v, const char *str, std::size_t)
	{ v = __YAS_SCAST(T, std::strtoull(str, 0, 10)); }
	template<typename T>
	static void atoi(T &v, const char *str, std::size_t)
	{ v = __YAS_SCAST(T, std::strtoll(str, 0, 10)); }

	template<typename T>
	static void utoa(char *buf, const std::size_t bufsize, std::size_t &len, T v)
	{ len = __YAS_SCAST(std::size_t, std::snprintf(buf, bufsize, "%llu", v)); }
	template<typename T>
	static void itoa(char *buf, const std::size_t bufsize, std::size_t &len, T v)
	{ len = __YAS_SCAST(std::size_t, std::snprintf(buf, bufsize, "%lld", v)); }

	template<typename T>
	static void atof(T &v, const char *str, std::size_t)
	{ v = std::strtof(str, 0); }
	template<typename T>
	static void atod(T &v, const char *str, std::size_t)
	{ v = std::strtod(str, 0); }

	template<typename T>
	static void ftoa(char *buf, const std::size_t bufsize, std::size_t &len, T v)
	{ len = __YAS_SCAST(std::size_t, std::snprintf(buf, bufsize, "%f", v)); }
	template<typename T>
	static void dtoa(char *buf, const std::size_t bufsize, std::size_t &len, T v)
	{ len = __YAS_SCAST(std::size_t, std::snprintf(buf, bufsize, "%f", v)); }
}; // struct std_traits

/***************************************************************************/

} // ns detail
} // ns yas

#endif // __yas__std_traits_hpp
