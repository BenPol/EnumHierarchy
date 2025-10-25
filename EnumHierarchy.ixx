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
    
    constexpr bool isSameAs(const EnumHierarchyVariant& other) const
    {
        return std::visit([](auto&& a, auto&& b) -> bool
        {
            using A = std::decay_t<decltype(a)>;
            using B = std::decay_t<decltype(b)>;
            return std::is_same<A, B>::value;
        }, value, other.value);
    }

    constexpr bool isAncestorOf(const EnumHierarchyVariant& other) const
    {
        return std::visit([](auto&& a, auto&& b) -> bool
        {
            using A = std::decay_t<decltype(a)>;
            using B = std::decay_t<decltype(b)>;
            return parentHasChild<A, B>::value;
        }, value, other.value);
    }

    constexpr bool isDescendantOf(const EnumHierarchyVariant& other) const
    {
        return std::visit([](auto&& a, auto&& b) -> bool
        {
            using A = std::decay_t<decltype(a)>;
            using B = std::decay_t<decltype(b)>;
            return parentHasChild<B, A>::value;
        }, value, other.value);
    }

    constexpr bool hasA(const EnumHierarchyVariant& other) const
    {
        return std::visit([](auto&& a, auto&& b) -> bool
        {
            using A = std::decay_t<decltype(a)>;
            using B = std::decay_t<decltype(b)>;
            return parentHasChild<A, B>::value || std::is_same<A, B>::value;
        }, value, other.value);
    }

    constexpr bool isA(const EnumHierarchyVariant& other) const
    {
        return std::visit([](auto&& a, auto&& b) -> bool
        {
            using A = std::decay_t<decltype(a)>;
            using B = std::decay_t<decltype(b)>;
            return parentHasChild<B, A>::value || std::is_same<A, B>::value;
        }, value, other.value);
    }

    constexpr bool operator==(const EnumHierarchyVariant& other) const
    {
        return isSameAs(other);
    }

    constexpr bool operator>(const EnumHierarchyVariant& other) const
    {
        return isAncestorOf(other);
    
    }
    constexpr bool operator>=(const EnumHierarchyVariant& other) const
    {
        return hasA(other);
    }

    constexpr bool operator<(const EnumHierarchyVariant& other) const
    {
        return isDescendantOf(other);
    }

    constexpr bool operator<=(const EnumHierarchyVariant& other) const
    {
        return isA(other);
    }
};