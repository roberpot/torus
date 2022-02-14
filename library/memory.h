/*
 * This file is part of Torus.
 * Torus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Torus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with Torus. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TORUS_MEMORY_H
#define __TORUS_MEMORY_H

#include <iostream>
#include <string>
#include <type_traits>

#define PRINT_INFO(X)  { \
std::cout << __FILE__ << "::" << __PRETTY_FUNCTION__ << "(" << __LINE__ << "): "; \
ttl::memory::print_info(X); \
}

namespace ttl {

    namespace memory {
        template<typename T, typename Allocator=std::allocator<T>>
                T* memmove(T* dest, T* origin, std::size_t count);

        template<typename T, typename Allocator=std::allocator<T>>
                T* memcpy(T* dest, T* origin, std::size_t count);

        template<typename T>
        void print_info(const T& o);

        template<>
        void print_info(const std::string& o);

        namespace internal {

            template<typename T, typename Allocator=std::allocator<T>, bool is_fundamental=std::is_fundamental<T>::value>
                    class Memory {
                    public:
                        static T* memmove(T* dest, T* origin, std::size_t count);
                        static T* memcpy(T* dest, T* origin, std::size_t count);
                    };

            template<typename T, typename Allocator>
            class Memory<T, Allocator, true> {
            public:
                static T* memmove(T* dest, T* origin, std::size_t count);
                static T* memcpy(T* dest, T* origin, std::size_t count);
            };

            template<typename T, typename Allocator>
            class Memory<T, Allocator, false> {
            public:
                static T* memmove(T* dest, T* origin, std::size_t count);
                static T* memcpy(T* dest, T* origin, std::size_t count);
            };
        }
    }
}

/**
 * Implementations
 */

namespace ttl {
    namespace memory {
        template<typename T, typename Allocator>
        T* memmove(T* dest, T* origin, std::size_t count) {
            return ttl::memory::internal::Memory<T, Allocator>::memmove(dest, origin, count);
        }

        template<typename T, typename Allocator>
        T* memcpy(T* dest, T* origin, std::size_t count, Allocator allocator) {
            return ttl::memory::internal::Memory<T, Allocator>::memcpy(dest, origin, count);
        }

        template<typename T>
        void print_info(const T& o) {
            std::cout << std::hex << &o << std::endl;
        }

        namespace internal {

            template<typename T, typename Allocator>
            T* Memory<T, Allocator, true>::memmove(T* dest, T* origin, std::size_t count) {
                return static_cast<T*>(::memmove(dest, origin, sizeof(T) * count));
            }

            template<typename T, typename Allocator>
            T* Memory<T, Allocator, false>::memmove(T* dest, T* origin, std::size_t count) {
                Allocator allocator;
                for (std::size_t i = 0; i < count; ++i) {
                    std::size_t index = count - i - 1;
                    allocator.construct(&dest[index], std::move(origin[index]));
                }
                return dest;
            }

            template<typename T, typename Allocator>
            T* Memory<T, Allocator, true>::memcpy(T* dest, T* origin, std::size_t count) {
                return static_cast<T*>(::memcpy(dest, origin, sizeof(T) * count));
            }

            template<typename T, typename Allocator>
            T* Memory<T, Allocator, false>::memcpy(T* dest, T* origin, std::size_t count) {
                Allocator allocator;
                for (std::size_t i = count ; i < count; ++i) {
                    std::size_t index = count - i - 1;
                    allocator.construct(&dest[index], origin[index]);
                }
                return dest;
            }
        }
    }

}

#endif //TORUS_MEMORY_H
