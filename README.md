# Overview

cpp_containers is a reimplementation of core components of the C++ Standard Template Library (STL)

This project reproduces the interface and behavior of standard containers such as vector, map, and stack, along with essential utilities including pair, enable_if, iterators, and comparison functors.

All components are implemented from scratch in C++98, with a strong focus on:

- Template metaprogramming
- Iterator categories and traits
- Red–Black Tree–based associative containers
- Performance and memory layout considerations

Through this project, developers gain a deep, practical understanding of STL internals, container abstraction, and the design principles behind generic programming in C++.

# Structure

| Directory   | Description                                                 |
| ----------- | ----------------------------------------------------------- |
| algorithm   | Core algorithms (e.g. `equal`, `lexicographical_compare`)   |
| functional  | Comparison functors and policy objects (runtime invocation) |
| type_traits | Compile-time type inspection and traits                     |
| iterator    | Iterator implementations and iterator traits                |
| tree        | Red–Black Tree data structure implementation                |
| allocator   | Memory management and custom allocator logic                |
| utility     | Utility types such as `pair`                                |
| container   | STL-style containers (`vector`, `map`, `stack`, etc.)       |

## Dependencies

- container는 iterator / tree / functional / type_traits / allocator / utility에 의존함
- tree는 functional 및 allocator에 의존함
- algorithm은 iterator_traits만 의존하고 다른 디렉토리에 종속되지 않음
- functional과 type_traits는 어떤 컨테이너에도 종속되지 않음 (독립적)

# References

- CppReference.com (C++11 이후 정리된 interface를 보여줌)
- https://github.com/gcc-mirror/gcc/tree/releases/gcc-3.0/libstdc%2B%2B-v3/include
