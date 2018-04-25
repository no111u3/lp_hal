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
 * Io register traits test
 * @file io_register_traits_test.cc
 * @author Boris Vinogradov
 */

#include <lp/bit_field.hh>
#include <lp/operations.hh>
#include <lp/type_list.hh>

#include <type_traits.hh>

#include <io_register.hh>
#include <io_register_traits.hh>

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

constexpr bool test_1() noexcept {
    using test_reg1 = hal::io_register_ops<test_register<0b0, 0b1>>;
    using bit_field1 = lp::assoc_type<lp::bit_field<0>, test_reg1>;

    hal::assoc_apply<bit_field1>::apply();

    return true;
}

constexpr bool test_2() noexcept {
    using test_reg1 = hal::io_register_ops<test_register<0b0, 0b1>>;
    using test_reg2 = hal::io_register_ops<test_register<0b0, 0b10>>;
    using bit_field1 = lp::assoc_type<lp::bit_field<0>, test_reg1>;
    using bit_field2 = lp::assoc_type<lp::bit_field<1, 1>, test_reg2>;

    hal::assoc_apply<bit_field1, bit_field2::with_value<0b1>>::apply();

    return true;
}

constexpr bool test_3() noexcept {
    using test_reg1 = hal::io_register_ops<test_register<0b1010, 0b1000>>;
    using test_reg2 = hal::io_register_ops<test_register<0b1000, 0b1010>>;
    using bit_field1 = lp::assoc_type<lp::bit_field<1>, test_reg1>;
    using bit_field2 = lp::assoc_type<lp::bit_field<1>, test_reg2>;

    hal::assoc_apply<
        lp::clear<bit_field1>,
        lp::toggle<bit_field2>>::apply();

    return true;
}

void io_register_traits_test() {
    static_assert(test_1(), "");
    static_assert(test_2(), "");
    static_assert(test_3(), "");
}
