# C++ Software Design book notes

Book written by Klaus Iglberger

https://learning-oreilly-com.ezproxy.bpl.org/library/view/c-software-design/9781098113155/

Code examples are on [GitHub](https://github.com/igl42/cpp_software_design)

## Software design

Overall guidelines

1. Understand the importance of software design
2. Design for change
3. Separate interfaces to avoid artificial coupling
4. Design for testability
5. Design for extension

### Guideline 1: The importance of software design

The structure of a project that impacts its ability to last long-term. Using the newest and best features doesn't really matter.

Change is one of the biggest constants in development. Having chains of poor dependencies can cause issues to arise where seemingly simple tasks require huge changes to multiple parts of the code.

Some dependencies are necessary. However, many others are artificial, unintentional, and harmful. Managing those dependencies is essential to keep a project nimble over many years.

Software development can be thought of as having 3 levels

* Architecture - Architecture patterns (client-server, microservices), large-scale structure, etc.
* Design - Design patterns (visitor, strategy, observer), function design, extensibility, maintainability, testability, etc.
* Implementation details - Implementation patterns, language-specific constructs

Architecture tends to involve the big decisions, the ones that are hard to change and that you want to get right early on. You address how to ensure components are independently changeable and how they communicate.

#### Guideline 1 Takeaways

* Treat design as essential
* Focus less on language details and more on design
* Avoid unnecessary coupling and dependencies for more agility
* Think of design as the art of managing dependencies and abstractions

### Guideline 2: Design for Change

The ability to change easily is essential to good software and differentiates **soft**ware from **hard**ware.

Separating concerns into small, well-named, tested, comprehensible pieces is a tried-and-true method for simplifying dependencies. It is also known as cohesion, orthogonality, and single responsibility principle (SRP in SOLID).

Try to separate things which don't strictly belong together so changes can be isolated.

The book presents an example of a document class with 2 pure virtual methods `exportToJSON(), serialize(ByteStream&, ...)` as an example of where hidden dependencies creep in.

* Changes in the JSON library can cause all derived classed to reimplement their JSON export and may be reflected in the `exportToJSON()` method signature
* Similar for `serialize()` wrt the implementation and `ByteStream`
* A shared enum enumerating all document types requires a centralized change

These dependencies can then bubble up to clients of `Document`.

To rework this design, we can look at **variation points**, i.e. places where we expect changes, and separate those out into isolated, wrapped components to protect dependencies on these from leaking out.

In this case we have core components `JSONLibrary, Document, ByteStream` and malleable components `JSONSerializer, User (Document client), Serializer`

Highly related to SRP is DRY (don't repeat yourself).

Avoid premature separation of concerns until you know what kind of change to expect. If you don't know how things might change, wait and then separate. Apply the "You aren't gonna need it" (YAGNI) principle.

#### Guideline 2 Takeaways

* Expect change
* Design for easy change
* Separate unrelated things to avoid coupling
* Coupling increases the scope of changes
* Apply SRP and DRY
* Avoid premature abstraction if upcoming change is unclear

### Guideline 3: Separate interfaces to avoid unnecessary coupling

The **interface segregation principle** (ISP), the I in SOLID, says clients shouldn't be forced to depend on things they don't use. That would argue for separating 2 interfaces from `Document`, say `JSONExportable` and `Serializable`.

Limiting requirements on template parameters also helps with ISP.

#### Guideline 3 takeaways

* Coupling also impacts interfaces
* Use the interface segregation principle to separate concerns in interfaces
* ISP is a special case of the SRP which applies to inheritance hierarchies and template parameters

### Guideline 4: Design for testability

Enough said, tests are essential.

The book gives the example of a class with a private method we'd like to test. Ideas

* Test via other public method. Introduces dependency that can be broken.
* Make the test a friend. Introduces extra coupling as now production code knows about tests.
* Make the method protected. Inheritance is rarely the answer and this case abuses it just for visibility.

The book suggests separating the method into a free function or a separate class with a public method. That class is then held privately in our original structure. Application of SRP.

Such a separation also improves encapsulation by means of restricting what the method can touch in the original containing class.

#### Guideline 4 takeaways

* Tests are your protection against breaking things
* Tests and testability are essential
* Separate concerns for testability
* `private` methods needing testing are misplaced. Prefer nonmember non-friend functions to member functions

### Guideline 5: Design for extension

Extensibility should be a primary goal of designs. In the document example

```cpp
class Document {
  // All documents must implement a serialize
  virtual serialize(ByteStream&) = 0;
};

enum class DocumentType {
  pdf,
  word,
  xml, // Newly adding this
};
```

we see issues where adding a new type, say `xml`, causes all other document types to recompile in the best case. In the worst case, they may need to change their implementations. We ideally should be able to add a new type without touching any other document types.

This violates the open-closed principle as the `serialize` implementations are artificially coupled. Separating into 3 tiers

* `Document`
* `PDF, Word, XML`
* `JSON, Serialization`

where lower levels can depend on higher levels, we avoid these issues and can add new document types without changing other document types or the base document.

SRP can lead to improved extensibility.

Compile-time customization in C++ is done in a few primary ways

* Using ADL like `std::swap`
* Using template parameters and concepts like `std::find`
* Using template specialization like `std::hash`

Apply the YAGNI principle to extension as well.

#### Guideline 5 takeaways

* Favor designs which allow for easy extension and the open-closed principle
* Use base classes, templates, function overloading, template specialization, and ADL to allow for extension
* Apply "you ain't gonna need it"
* Identify customization and extension points and ensure they're easy to use

## The art of building abstractions

Abstractions help to deal with complexity.

### Guideline 6: Adhere to the expected behavior of abstractions

Maintain the Liskov substitution principle

> Let `p(x)` be a property provable about objects `x` of type `T`. Then `p(y)` should be true for objects `y` of type `S` where `S` is a subtype of `T`.

This formalizes the is-a relationship that we usually desire for inheritance.

Follow-up implications

* Preconditions can't be strengthened in a subtype. Namely, subtypes can't expect more than what the super type expresses
* Post conditions can't be weakened in a subtype
* Function return types in a subtype must be **covariant**. Member functions of the subtype can return subtypes of the return type of the super type method. However, the subtype cannot return a super type of the type returned by the method in the super type.
* Function parameters in a subtype must be **contravariant**. Subtype methods can accept a super type of the function parameter in the corresponding member function of the super type. This does not have direct C++ support.
* Invariants of the super type must be preserved in a subtype. Any expectations on the state of the super type must be valid before and after calls to any member function.

#### Guideline 6 takeaways

* Abstractions represent a set of requirements and expectations
* Follow the Liskov Substitution Principle (LSP) to ensure expected behavior of abstractions
* Ensure that derived classes adhere to the expected base class behavior
* Communication abstraction expectations
* Apply these to static polymorphism and template parameters too

### Guideline 7: Understand the similarities between base classes and concepts

The LSP also applies to templates, compile-time polymorphism, and concepts.

### Guideline 8: Understand the semantic requirements of overload sets

Overloads and overload sets are another form of compile-time abstraction. You can add a free function overload for any type without having to modify the types. Using unqualified calls via `using std::begin; begin(thing);` and ADL lets your code choose the right overload based on the input type.

Free functions enable separation of concerns and DRY very well.

Overloaded operations if and only if they're roughly equivalent. Using overloading for things which aren't very related can cause much confusion.

#### Guideline 8 takeaways

* Function overloading is compile-time abstraction
* Functions in an overload set all have expectations
* Pay attention to existing names and conventions

### Guideline 9: Pay attention to the ownership of abstractions

The dependency inversion principle (DIP) says that source dependencies should rely on abstractions, not concrete details, types, or implementations.

High-abstraction modules should not depend on concrete modules. Both should depend on abstractions. Abstractions should not depend on details. Details should depend on abstractions.

The book gives a very unsatisfying example that relies on a technicality of "changing where the high-abstraction layer is" to recategorize things. It does defend this by saying that you could move headers from the low-level to the high level and thereby change ownership.

The book makes the point that not only must we invert dependencies by introducing an abstraction (e.g. an interface or a concept, but we must also ensure that the abstraction is owned by the high-abstraction (high-level, stable) part of the code. For example

* Core component - high-level, stable
* Abstraction used by core (e.g. a plugin interface) - should be high-level, stable, owned by high-abstraction layer
* Implementation of abstraction (e.g. a specific plugin) - should be low-level (high-detail), not in the core layer but in the detail layer

For inheritance and interfaces, this ownership must be manually managed. For templates, ownership is handled by default as the template definition provides and thereby owns the abstractions.

#### Guideline 9 takeaways

* Implementation details should depend on high-level abstractions
* Use the dependency inversion principle and ensure that abstractions are owned by the high level architecture components

### Guideline 10: Create architectural documents

Architectural documents are good and ensure that everyone has a shared understanding of the system design. Such a document provides a global vision of the codebase.

Such documents can also contain ideas, visions, thought processes, and decisions. Only the high-level items and interactions should be documented. Low-level implementation details which change frequently don't belong in such a document.

Just get started making one.

## Chapter 3. The purpose of design patterns

### Guideline 11: Understand the purpose of design patterns

Design patterns have a name, carry intents, introduce abstractions, and are proven.

Having names allows us to communicate efficiently without having to describe ideas in excruciating detail.

The name of the pattern expresses the intent concisely. The intent describes the proposed structure, how dependencies are managed, and how we expect the system to evolve.

Abstractions introduced by patters allow for decoupling and also introduce customization points.

### Guideline 12: Beware of design pattern misconceptions

Design patterns are **not** the goal. They are tools used to achieve goals and should reduce complexity rather than introduce it.

Design patterns are not implementation details. They provide intent and concepts which can be implemented in any number of ways. The book shows an inheritance-based strategy and a template-based strategy.

Design patterns are not limited to OOP or runtime polymorphism. For example, `std::accumulate` accepts an optional reducer function that allows modifying the algorithm externally.

### Guideline 13: Design patterns are everywhere

### Guideline 14: Use design pattern's name to communicate intent

## Chapter 4: The visitor design pattern


### Guideline 15: Design for the additions of types and operations

## Other references mentioned

* *Large Scale C++ Software Design* by John Lakos
* Gang of Four book

## Design and architecture patterns mentioned

* Non-virtual interface (NVI)
* Pimpl
* Template method design pattern
* Bridge design pattern

## Next

https://learning-oreilly-com.ezproxy.bpl.org/library/view/c-software-design/9781098113155/ch02.html
