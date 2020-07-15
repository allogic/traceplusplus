#pragma once

#include "Core.h"
#include "Utils.h"

using TTransformationShader = TConstexprStringView<decltype(R"compute(
void main()
{

}
)compute"_cexpr)>;