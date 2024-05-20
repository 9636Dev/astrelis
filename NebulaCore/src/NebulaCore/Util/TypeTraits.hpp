#pragma once

#include <type_traits>

namespace Nebula
{
    template<typename T>
    struct TypeProperties
    {
        constexpr static bool DefaultConstructible = std::is_default_constructible_v<T>;
        constexpr static bool CopyConstructible = std::is_copy_constructible_v<T>;
        constexpr static bool MoveConstructible = std::is_move_constructible_v<T>;
        constexpr static bool NoThrowMoveConstructible = std::is_nothrow_move_constructible_v<T>;
        constexpr static bool NoThrowCopyConstructible = std::is_nothrow_copy_constructible_v<T>;
        constexpr static bool NoThrowMoveAssignable = std::is_nothrow_move_assignable_v<T>;
        constexpr static bool NoThrowCopyAssignable = std::is_nothrow_copy_assignable_v<T>;
        constexpr static bool Destructible = std::is_destructible_v<T>;
        constexpr static bool TriviallyCopyable = std::is_trivially_copyable_v<T>;
        constexpr static bool TriviallyCopyAssignable = std::is_trivially_copy_assignable_v<T>;
        constexpr static bool TriviallyMoveAssignable = std::is_trivially_move_assignable_v<T>;
        constexpr static bool TriviallyDestructible = std::is_trivially_destructible_v<T>;
        constexpr static bool TriviallyDefaultConstructible = std::is_trivially_default_constructible_v<T>;
        constexpr static bool TriviallyMoveConstructible = std::is_trivially_move_constructible_v<T>;
        constexpr static bool NoThrowDestructible = std::is_nothrow_destructible_v<T>;
        constexpr static bool NoThrowDefaultConstructible = std::is_nothrow_default_constructible_v<T>;
        constexpr static bool Polymorphic = std::is_polymorphic_v<T>;
        constexpr static bool Abstract = std::is_abstract_v<T>;
        constexpr static bool Final = std::is_final_v<T>;
        constexpr static bool Aggregate = std::is_aggregate_v<T>;
        constexpr static bool IsIntegral = std::is_integral_v<T>;
        constexpr static bool IsFloatingPoint = std::is_floating_point_v<T>;
        constexpr static bool IsTrvial = std::is_trivial_v<T>;
        constexpr static bool IsEnum = std::is_enum_v<T>;
        constexpr static bool IsUnion = std::is_union_v<T>;
        constexpr static bool IsClass = std::is_class_v<T>;
        constexpr static bool IsFunction = std::is_function_v<T>;
        constexpr static bool IsPointer = std::is_pointer_v<T>;
        constexpr static bool IsReference = std::is_reference_v<T>;
        constexpr static bool IsLValueReference = std::is_lvalue_reference_v<T>;
        constexpr static bool IsRValueReference = std::is_rvalue_reference_v<T>;
        constexpr static bool IsMemberObjectPointer = std::is_member_object_pointer_v<T>;
        constexpr static bool IsMemberFunctionPointer = std::is_member_function_pointer_v<T>;
        constexpr static bool IsArithmetic = std::is_arithmetic_v<T>;
        constexpr static bool IsFundamental = std::is_fundamental_v<T>;
    };
}  // namespace Nebula
