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
 * Io register traits
 * @file io_register_traits.hh
 * @author Boris Vinogradov
 */

#include <lp/associate_type.hh>
#include <lp/type_list.hh>
#include <lp/type_list_traits.hh>
#include <lp/types.hh>

#include <type_traits.hh>

#ifndef LP_HAL_IO_REGISTER_TRAITS_HH
#define LP_HAL_IO_REGISTER_TRAITS_HH

namespace hal {
    /// Constant apply to type
    template <typename Register, typename Bit_fields>
    struct const_apply {
        static constexpr void apply() noexcept {
            Register::template modify<Bit_fields>();
        }
    };

    /// Reorder plase of operation and association
    template <typename T>
    struct reorder_assoc {
        using type = lp::assoc_type<T, std::none_type>;
    };

    template <typename T>
    struct reorder_assoc<lp::set<T>> {
        using type = lp::assoc_type<
            lp::set<typename T::type>, typename T::assoc_type>;
    };

    template <typename T>
    struct reorder_assoc<lp::clear<T>> {
        using type = lp::assoc_type<
            lp::clear<typename T::type>, typename T::assoc_type>;
    };

    template <typename T>
    struct reorder_assoc<lp::toggle<T>> {
        using type = lp::assoc_type<
            lp::toggle<typename T::type>, typename T::assoc_type>;
    };

    template <typename T, typename A_type>
    struct reorder_assoc<lp::assoc_type<T, A_type>> {
        using type = lp::assoc_type<T, A_type>;
    };

    /// Associate type apply
    template <typename ...Bit_fields>
    struct assoc_apply;

    template <>
    struct assoc_apply<> {
        static constexpr void apply() noexcept {}
    };

    template <typename ...Bit_fields>
    struct assoc_apply {
        using bit_fields = lp::for_each_t<reorder_assoc, Bit_fields...>;
        using registers =
            lp::unique_t<
                lp::for_each_t<lp::extract_assoc_type, bit_fields>
            >;
        using current_register = typename registers::head;
        template <typename T>
        using is_same_assoc_type = std::is_same<
            typename lp::extract_assoc_type<T>::type, current_register>;

        static constexpr void apply() noexcept {
            const_apply<current_register,
                lp::for_each_t<
                    lp::extract_type,
                    typename bit_fields::template filter<is_same_assoc_type>
                    >
                >::apply();

            bit_fields::
                template remove<is_same_assoc_type>::
                template assign<assoc_apply>::apply();
        }
    };
} // namespace hal

#endif // LP_HAL_IO_REGISTER_TRAITS_HH
