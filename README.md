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
```
AnimalEnum animal = Animal{};
AnimalEnum mammal = Animal::Mammal{};
AnimalEnum dog = Animal::Mammal::Dog{};
AnimalEnum cat = Animal::Mammal::Cat{};
AnimalEnum bird = Animal::Bird{};
AnimalEnum crow = Animal::Bird::Crow{};
AnimalEnum parrot = Animal::Bird::Parrot{};
```
## Behaviour
The `==` operator behaves as a (commutative) 'is-a' relation. For example:
```
mammal == animal;   // true
animal == mammal;   // true
mammal == mammal;   // true
dog    == animal;   // true
dog    == cat;      // false
dog    == bird;     // false
bird   == crow;     // true
```
