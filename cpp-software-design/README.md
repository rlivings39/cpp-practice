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



## Other references mentioned

* *Large Scale C++ Software Design* by John Lakos
* Gang of Four book

## Design and architecture patterns mentioned

* Non-virtual interface (NVI)
* Pimpl
* Template method design pattern
* Bridge design pattern
