
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

#ifndef __yas__detail__io__json_streams_hpp
#define __yas__detail__io__json_streams_hpp

#include <yas/detail/config/config.hpp>

#include <yas/detail/io/io_exceptions.hpp>
#include <yas/detail/io/serialization_exceptions.hpp>
#include <yas/detail/type_traits/type_traits.hpp>
#include <yas/detail/tools/cast.hpp>
#include <yas/detail/tools/json_tools.hpp>

#include <stack>
#include <cassert>
#include <limits>

namespace yas {
namespace detail {

/***************************************************************************/

template<typename IS, std::size_t F, typename Trait>
struct json_istream {
	json_istream(IS &is)
		:is(is)
	{}

	template<typename T = std::size_t>
	T read_seq_size() {
		T size{};
		read(size);

		return size;
	}

    char peekch() const { return is.peekch(); }
    char getch() { return is.getch(); }
    void ungetch(char ch) { is.ungetch(ch); }

	// for arrays
	std::size_t read(void *ptr, std::size_t size) {
		__YAS_THROW_READ_ERROR(size != is.read(ptr, size));

        return size;
	}

    // for char and signed char
    template<typename T>
    void read(T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, char, signed char)) {
        std::int16_t t;
        read(t);

        v = __YAS_SCAST(T, t);
    }
    // for unsigned char
    template<typename T>
    void read(T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, unsigned char)) {
        std::uint16_t t;
        read(t);

        v = __YAS_SCAST(T, t);
    }

	// for bools only
	template<typename T>
	void read(T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, bool)) {
        static const char ltrue[] = "true";
        static const char lfalse[] = "false";

        char buf[sizeof(lfalse)];
		__YAS_THROW_READ_ERROR(1 != is.read(buf, 1));

        v = (buf[0] == 't');

        const std::size_t to_read = (v ? sizeof(ltrue)-1-1 : sizeof(lfalse)-1-1);
        __YAS_THROW_READ_ERROR(to_read != is.read(buf, to_read));
	}

	// for signed 16/32/64 bits
	template<typename T>
	void read(T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, std::int16_t, std::int32_t, std::int64_t)) {
		char buf[sizeof(T)*4];
		const std::size_t n = json_read_num(is, buf, sizeof(buf));
		v = Trait::template atoi<T>(buf, n);
	}

	// for unsigned 16/32/64 bits
	template<typename T>
	void read(T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, std::uint16_t, std::uint32_t, std::uint64_t)) {
        char buf[sizeof(T)*4];
        const std::size_t n = json_read_num(is, buf, sizeof(buf));

        v = Trait::template atou<T>(buf, n);
	}

	// for floats
	template<typename T>
	void read(T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, float)) {
		char buf[std::numeric_limits<T>::max_exponent10+20];
		const std::size_t n = json_read_double(is, buf, sizeof(buf));

		v = Trait::template atof<T>(buf, n);
	}

	// for doubles
	template<typename T>
	void read(T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, double)) {
        char buf[std::numeric_limits<T>::max_exponent10+20];
        const std::size_t n = json_read_double(is, buf, sizeof(buf));

		v = Trait::template atod<T>(buf, n);
	}

private:
	IS &is;
};

/***************************************************************************/

template<typename OS, std::size_t F, typename Trait>
struct json_ostream {
	json_ostream(OS &os)
		:os(os)
	{}

	template<typename T>
	void write_seq_size(T v) {
		write(__YAS_SCAST(std::uint64_t, v));
	}

	// for arrays
	template<typename T>
	void write(const T *ptr, std::size_t size) {
		__YAS_THROW_WRITE_ERROR(size != os.write(ptr, size));
	}

    // for char and signed char
    template<typename T>
    void write(const T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, char, signed char)) {
        write(__YAS_SCAST(std::int16_t, v));
    }

    // for unsigned char
    template<typename T>
    void write(const T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, unsigned char)) {
        write(__YAS_SCAST(std::uint16_t, v));
    }

	// for bools only
	template<typename T>
	void write(const T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, bool)) {
        if ( v ) {
            static const char ltrue[] = "true";
            __YAS_THROW_WRITE_ERROR(sizeof(ltrue)-1 != os.write(ltrue, sizeof(ltrue)-1));
        } else {
            static const char lfalse[] = "false";
            __YAS_THROW_WRITE_ERROR(sizeof(lfalse)-1 != os.write(lfalse, sizeof(lfalse)-1));
        }
	}

	// for signed 16/32/64 bits
	template<typename T>
	void write(const T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, std::int16_t, std::int32_t, std::int64_t)) {
		char buf[sizeof(v)*4];
		std::size_t len = Trait::itoa(buf, sizeof(buf), v);

		__YAS_THROW_WRITE_ERROR(len != os.write(buf, len));
	}

	// for unsigned 16/32/64 bits
	template<typename T>
	void write(const T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, std::uint16_t, std::uint32_t, std::uint64_t)) {
		char buf[sizeof(v)*4];
		std::size_t len = Trait::utoa(buf, sizeof(buf), v);

		__YAS_THROW_WRITE_ERROR(len != os.write(buf, len));
	}

	// for floats
	template<typename T>
	void write(const T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, float)) {
		char buf[std::numeric_limits<T>::max_exponent10 + 20];
		std::size_t len = Trait::ftoa(buf, sizeof(buf), v);

		__YAS_THROW_WRITE_ERROR(len != os.write(buf, len));
	}

	// for doubles
	template<typename T>
	void write(const T &v, __YAS_ENABLE_IF_IS_ANY_OF(T, double)) {
		char buf[std::numeric_limits<T>::max_exponent10 + 20];
		std::size_t len = Trait::dtoa(buf, sizeof(buf), v);

		__YAS_THROW_WRITE_ERROR(len != os.write(buf, len));
	}

private:
	OS &os;
};

/***************************************************************************/

} // ns detail
} // ns yas

#endif // __yas__detail__io__json_streams_hpp
