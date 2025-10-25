# EnumHierarchy
A C++ module to create enum-like structs with hierarchical behaviour.

## Requirements
C++20.

## Installation
1. Install EnumHierarchy.ixx to a location available to your codebase.
2. `import EnumHierarchy`.

## Usage
To create an enum hierarchy begin by defining a hierarchy of structs, with `using parent = Parent` for each struct. For example:
```cpp
struct Animal
{
    struct Mammal
    {
        using parent = Animal;
        struct Dog { using parent = Mammal; };
        struct Cat { using parent = Mammal; };
    };
    struct Bird
    {
        using parent = Animal;
        struct Crow { using parent = Bird; };
        struct Parrot { using parent = Bird; };
    };
};
```
Next define an `EnumHierarchyVariant` using all the structs the hierarchy.
```cpp
using AnimalEnum = EnumHierarchyVariant<Animal, Animal::Mammal, Animal::Mammal::Cat, Animal::Mammal::Dog, Animal::Bird, Animal::Bird::Crow, Animal::Bird::Parrot>;
```
Instances of the enum hierarchy variant can be initialised as:
```cpp
AnimalEnum animal = Animal{};
AnimalEnum mammal = Animal::Mammal{};
AnimalEnum dog = Animal::Mammal::Dog{};
AnimalEnum cat = Animal::Mammal::Cat{};
AnimalEnum bird = Animal::Bird{};
AnimalEnum crow = Animal::Bird::Crow{};
AnimalEnum parrot = Animal::Bird::Parrot{};
```
## Behaviour
`a.isSameAs(b)` checks for strict equality between two enum hierarchy variants. The `==` operator can also be used to represent this relationship. For example:
```cpp
animal.isSameAs(animal);    // true
animal == animal;           // true
animal == mammal;           // false
```
`a.isA(b)` checks if `a` is a descendant or the same type as `b`. The `<=` operator can also be used to represent this relationship. For example:
```cpp
animal.isA(mammal);     // false
mammal.isA(animal);     // true
dog <= animal;          // true
dog <= dog;             // true
dog <= bird;            // false
```
`a.isDescendantOf(b)` checks if `a` is a descendant of `b`. The `<` operator can also be used to represent this relationship. For example:
```cpp
animal.isDescendantOf(mammal);      // false
mammal.isDescendantOf(animal);      // true
dog < animal;                       // true
dog < dog;                          // false
dog < bird;                         // false
```
`a.hasA(b)` checks if `a` is an ancestor or the same type as `b`. The `>=` operator can also be used to represent this relationship. For example:
```cpp
bird.hasA(crow);        // true
bird.hasA(bird);        // true
bird >= cat;            // false
animal >= cat;          // true
```
`a.isAncestorOf(b)` checks if `a` is an ancestor of `b`. The `>` operator can also be used to represent this relationship. For example:
```cpp
bird.isAncestorOf(crow);        // true
bird.isAncestorOf(bird);        // false
bird > cat;                     // false
animal > cat;                   // true
```
It should also be noted that ths system only forms a _partial_ ordering. For example `parrot <= mammal` and `mammal <= parrot` are both false.