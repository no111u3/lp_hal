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
 * Lepestrum C++ Library implementation
 * Io register test
 * @file io_register_test.cc
 * @author Boris Vinogradov
 */

#include <io_register.hh>
#include <lp/bit_field.hh>
#include <lp/operations.hh>

#include <type_traits.hh>

template <int Input_value, int Check_value = 0>
struct test_register {
    using type = int;

    static constexpr type get() noexcept {
        return value;
    }

    static constexpr void set(const type value) noexcept {
        // compile check for non constant values
        constexpr int x[value == Check_value ? 1 : -1] = {0};
    }

    static constexpr type value = Input_value;
};

template<int Input_value, lp::word_t ...options>
constexpr const int get_value() noexcept {
    int value = 0;

    hal::io_register_ops<test_register<Input_value>>::
        template get<lp::bit_field<options...>>(value);

    return value;
}

template<int Input_value, typename ...Types, typename ...Value_types>
constexpr const int get_and_value(Value_types ...values) noexcept {
    int value = 0;

    return hal::io_register_ops<test_register<Input_value>>::
        template get_and<Types...>(values...);
}

template<int Input_value>
constexpr const int get_value_2() noexcept {
    return hal::io_register_ops<test_register<Input_value>>::
        get_value();
}

template<int Input_value, int Check_value,
    typename ...Types, typename ...Value_types>
constexpr bool set_value(Value_types ...values) noexcept {
    using register_impl_type =
        hal::io_register_ops<test_register<Input_value, Check_value>>;

    register_impl_type::template set<Types...>(values...);

    return true;
}

template<int Input_value, int Check_value, int Shift,
    typename ...Types, typename ...Value_types>
constexpr bool set_value(Value_types ...values) noexcept {
    using register_impl_type =
        hal::io_register_ops<test_register<Input_value, Check_value>>;

    register_impl_type::template set<Shift, Types...>(values...);

    return true;
}

template<int Input_value, int Check_value>
constexpr bool set_value_2() noexcept {
    using register_impl_type =
        hal::io_register_ops<test_register<Input_value, Check_value>>;

    register_impl_type::set_value(Check_value);

    return true;
}

template<int Input_value, int Check_value,
    typename ...Types, typename ...Value_types>
constexpr bool modify_value(Value_types ...values) noexcept {
    using register_impl_type =
        hal::io_register_ops<test_register<Input_value, Check_value>>;

    register_impl_type::template modify<Types...>(values...);

    return true;
}

void io_register_test() {
    using namespace hal;

    static_assert(test_register<0>::value == 0, "");

    static_assert(get_value<0b1010, 0>() == 0b0, "");

    static_assert(get_value<0b1010, 1, 2>() == 0b1, "");

    static_assert(get_and_value<0b1010, lp::bit_field<0>>() == 0b0, "");

    static_assert(get_and_value<0b1010,
                  lp::bit_field<0>>(lp::bit_field<2, 2>{}) == 0b1000, "");

    static_assert(get_value_2<0b0101>() == 0b0101, "");

    static_assert(set_value<0b0, 0b1, lp::bit_field<0>>(), "");

    static_assert(set_value<0b0, 0b0111,
                  lp::bit_field<0>, lp::bit_field<1, 2>>(), "");

    static_assert(set_value<0b0, 0b1010,
                  lp::bit_field<3>>(lp::bit_field<1, 2>{0b01}), "");

    static_assert(set_value<0b0, 0b10100, 1,
                  lp::bit_field<3>>(lp::bit_field<1, 2>{0b01}), "");

    static_assert(set_value_2<0b0, 0b1010>(), "");

    static_assert(modify_value<0b1010, 0b1000>
                  (lp::bit_field<1, 2>{0b0}), "");

    static_assert(modify_value<0b1010, 0b1000,
                  lp::toggle<lp::bit_field<1>>>(), "");

    static_assert(modify_value<0b1010, 0b1000>
                  (lp::toggle<lp::bit_field<1>>{}), "");
}
