#pragma once

#include <type_traits>
#include <tuple>
#include <array>
#include <algorithm>
// #include <execution>
#include <iostream>

namespace type
{
    template <typename Type>
    using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<Type>>;

    template <typename Type, Type Value>
    using indexer_type = std::integral_constant<Type, Value>;

    template <auto Value>
    using indexer_t = indexer_type<remove_cvref_t<decltype(Value)>, Value>;

    template<auto Value>
    constexpr auto extract_value(indexer_t<Value>)
    {
        return Value;
    }

    template <auto... Indices>
    using sequence_t = std::integer_sequence<std::common_type_t<std::remove_cvref_t<decltype(Indices)>...>, Indices...>;

    template <auto Size>
    using make_sequence_t = std::make_integer_sequence<remove_cvref_t<decltype(Size)>, (std::size_t)Size>;

    template<typename TupleType>
    constexpr int tuple_count_v = static_cast<int>(std::tuple_size_v<std::remove_cvref_t<TupleType>>);

    template <int Index>
    struct ItemIndex
    {
        constexpr static auto value = Index;
    };

    namespace hidden
    {
        template<int... Indices>
        void compile_loop(auto&& work, std::integer_sequence<int, Indices ...>)
        {
            (..., (void)work(ItemIndex<Indices>{}));
        }
    }

    template<int END>
    void compile_loop(auto&& work)
    {
        hidden::compile_loop(work, std::make_integer_sequence<int, END>{});
    }

    // template<typename... ArgTypes>
    // void process_arguments(ArgTypes&&...) { }

    // template<typename WorkhorseType, auto... Indices>
    // void drive_compile_loop(WorkhorseType&& workhorse, type_extention::sequence_t<Indices...>)
    // {

    //     #if defined(__clang__)
    //         process_arguments(workhorse(type_extention::indexer_t<Indices>{})...);
    //     #else
    //         constexpr auto Size = sizeof...(Indices);
    //         process_arguments(workhorse(type_extention::indexer_t<Size - Indices - 1>{})...);
    //     #endif
    // }

}
