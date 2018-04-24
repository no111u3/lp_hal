/* Copyright 2018 Boris Vinogradov <no111u3@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Lepestrum C++ HAL
 * Io register represetation layer
 * @file io_register.hh
 * @author Boris Vinogradov
 */

#include <lp/types.hh>
#include <lp/bit_field_traits.hh>

#ifndef LP_HAL_IO_REGISTER_HH
#define LP_HAL_IO_REGISTER_HH

namespace hal {
    /// Io register base operations
    template <typename Type, lp::addr_t Address>
    struct io_register_base {
        using type = Type;

        static constexpr const type get() noexcept {
            return *reinterpret_cast<volatile type *>(Address);
        }

        static constexpr void set(const type value) noexcept {
            *reinterpret_cast<volatile type *>(Address) = value;
        }
    };

    /// Io register operations implementation
    template <typename Base_register>
    struct io_register_ops {
        using base_register = Base_register;
        using type = typename Base_register::type;

        template <typename ...Bits, typename ...Bits_value>
        static constexpr void set(Bits_value ...bits_value) noexcept {
            base_register::set(lp::const_value_of<type, Bits...>::value
                | lp::variable_value_of<type, Bits_value...>::
                    value_of(0, bits_value...));
        }

        template <lp::word_t Shift, typename ...Bits, typename ...Bits_value>
        static constexpr void set(Bits_value ...bits_value) noexcept {
            base_register::set((lp::const_value_of<type, Bits...>::value
                | lp::variable_value_of<type, Bits_value...>::
                    value_of(0, bits_value...)) << Shift);
        }

        static constexpr void set_value(const type value) noexcept {
            base_register::set(value);
        }

        template <typename ...Bits, typename ...Bits_value>
        static constexpr void modify(Bits_value ...bits_value) noexcept {
            base_register::set(
                lp::variable_value_of<type, Bits_value...>::
                    value_of(
                        lp::const_value_of<type, Bits...>::
                            value_of(base_register::get()),
                        bits_value...));
        }

        template <typename ...Bits, typename ...Values>
        static constexpr void get(Values &...outs) noexcept {
            lp::get_value_of<type, Bits...>::
                get_value(base_register::get(), outs...);
        }

        template <typename ...Bits, typename ...Bits_value>
        static constexpr const type get_and(Bits_value ...bits_value) noexcept {
            return base_register::get() &
                (lp::const_value_of<type, Bits...>::value
                    | lp::variable_value_of<type, Bits_value...>::
                        value_of(0, bits_value...));
        }

        static constexpr type get_value() noexcept {
            return base_register::get();
        }
    };

    // Io register implementation
    template <typename Type, lp::addr_t Address>
    using io_register = io_register_ops<io_register_base<Type, Address>>;
} // namespace hal

#endif // LP_HAL_IO_REGISTER_HH
