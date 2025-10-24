module;

#include <variant>

export module EnumHierarchy;

template<typename Parent, typename Child, typename = void> struct parentHasChild : std::false_type {};
template<typename Parent, typename Child> struct parentHasChild<Parent, Child, std::void_t<typename Child::parent>>
    : std::bool_constant<std::is_same<typename Child::parent, Parent>::value || parentHasChild<Parent, typename Child::parent>::value> {};

export template<typename... Ts> struct EnumHierarchyVariant
{
    std::variant<Ts...> value;

    constexpr EnumHierarchyVariant() = default;
    template<typename T> requires std::disjunction<std::is_same<T, Ts>...>::value explicit(false) constexpr EnumHierarchyVariant(T v) noexcept
        : value(std::move(v)) {};
    
    constexpr bool operator==(const EnumHierarchyVariant& other) const
    {
        return std::visit([](auto&& a, auto&& b) -> bool
        {
            using A = std::decay_t<decltype(a)>;
            using B = std::decay_t<decltype(b)>;
            return std::is_same<A, B>::value || parentHasChild<A, B>::value || parentHasChild<B, A>::value;
        }, value, other.value);
    }
};