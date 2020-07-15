#pragma once

#include "Core.h"

template<s8 ... Chars>
using TConstexprString = std::integer_sequence<s8, Chars...>;

template<s8 ... Chars>
constexpr TConstexprString<Chars...> operator ""_cexpr() { return {}; }

template<typename>
struct TConstexprStringView;

template<s8 ... Chars>
struct TConstexprStringView<TConstexprString<Chars...>>
{
  constexpr static s8 Value[sizeof...(Chars) + 1] = { Chars..., '\0' };
};