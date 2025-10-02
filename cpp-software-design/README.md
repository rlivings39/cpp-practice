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

### The importance of software design

The structure of a project that impacts its ability to last long-term. Using the newest and best features doesn't really matter.

Change is one of the biggest constants in development. Having chains of poor dependencies can cause issues to arise where seemingly simple tasks require huge changes to multiple parts of the code.

Some dependencies are necessary. However, many others are artificial, unintentional, and harmful. Managing those dependencies is essential to keep a project nimble over many years.

## Other references mentioned

* *Large Scale C++ Software Design* by John Lakos
* Gang of Four book
