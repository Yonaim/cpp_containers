# Overview

ft_containers is a reimplementation of key components of the C++ Standard Template Library (STL), a project from the École 42 curriculum.
The project replicates the behavior and interface of standard containers such as `vector`, `map`, and `stack`, along with supporting utilities like `pair`, `enable_if`, and custom iterators.  

All functionality is implemented from scratch using C++98, emphasizing template programming, allocator usage, and iterator design.  
Through this project, developers gain a deeper understanding of STL internals, template metaprogramming, and container performance characteristics.

# Structure

| 디렉토리        | 역할                          |
| ----------- | --------------------------- |
| algorithm   | 절차/알고리즘 (equal, lex cmp)    |
| functional  | 비교 functor, policy 객체 (런타임 호출)  |
| type_traits | 컴파일타임 타입 판정                 |
| iterator    | 반복자 구조 및 traits             |
| tree        | RB-tree 자료구조 구현             |
| allocator   | 메모리 관리 객체                   |
| utility     | pair 등 유틸 타입                |
| container   | vector/map/stack 같은 실제 컨테이너 |

- container는 iterator / tree / functional / type_traits / allocator / utility에 의존함
- tree는 functional(Select1st, less)와 allocator에 의존함
- algorithm은 iterator_traits만 의존하고 다른 디렉토리에 종속되지 않음
- functional과 type_traits는 어떤 컨테이너에도 종속되지 않음 (독립적)

# Schedule

### 10/03 (1회차) — 환경 세팅 / Utils Part 1
- [x] Git repo & 디렉토리 구조 생성  
- [x] Makefile 작성 (all, clean, fclean, re)  
- [x] ft::pair, ft::make_pair  
- [x] ft::equal, ft::lexicographical_compare  

---

### 10/26 ~ 11/01 (2회차) — Utils Part 2
- [x] ft::enable_if  
- [x] ft::is_integral  
- [x] ft::iterator_traits  
- [x] ft::reverse_iterator  

---

### 11/02 ~ 11/08 (3회차) — Vector Part 1
- [x] 내부 동적 배열 + allocator  
- [x] 생성자 / 소멸자 / 대입 연산자  
- [x] size, capacity, max_size, empty  
- [x] operator[], at, front, back  

---

### 11/09 ~ 11/15 (4회차) — Vector Part 2
- [x] push_back, pop_back  
- [x] insert, erase, clear  
- [x] resize, reserve  
- [x] 반복자 (begin, end, rbegin, rend)  
- [x] 비교 연산자 (==,!=,<,<=,>,>=,<=>)  

---

### 11/16 ~ 11/22 (5회차) — Map Part 1
- [ ] RBT Node 구조체 정의  
- [ ] 반복자 설계 (iterator, const_iterator)  
- [ ] insert (balancing 전)  
- [ ] find, operator[]  

---

### 11/23 ~ 11/29 (6회차) — Map Part 2
- [ ] RBT 삽입 balancing  
- [ ] erase + 삭제 balancing  
- [ ] clear, swap  
- [ ] count  

---

### 11/30 ~ 12/06 (7회차) — Map Part 3
- [ ] lower_bound, upper_bound, equal_range  
- [ ] key_compare, value_compare  
- [ ] 비교 연산자 구현  

---

### 12/21 ~ 12/27 (8회차) — Stack + 마무리
- [ ] 내부 컨테이너 ft::vector 연결  
- [ ] push, pop, top, empty, size  
- [ ] 비교 연산자  

# References

- CppReference.com (C++11 이후 정리된 interface를 보여줌)
- https://github.com/gcc-mirror/gcc/tree/releases/gcc-3.0/libstdc%2B%2B-v3/include