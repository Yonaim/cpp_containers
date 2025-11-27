#include <iostream>
#include <cmath>
using namespace std;

/* ============================================================================
   PATTERN A) 비멤버 함수 / 비멤버 연산자 / 템플릿 비멤버 연산자 friend
   ---------------------------------------------------------------------------
   모두 공통점:
   - "비멤버" 함수인데 클래스의 private 멤버에 접근해야 한다.
   - 그래서 friend로 열어줘야 함.

   (distance / operator== / operator<< / 템플릿 비교 연산자 등 가능)
   ============================================================================ */
class Point
{
  private:
    int x, y;

  public:
    Point(int a, int b) : x(a), y(b) {}

    // 1) 일반 비멤버 함수
    friend double distance(const Point &a, const Point &b);

    // 2) 연산자 오버로딩 (비멤버로 두는 것이 좌우항 대칭성에 좋음)
    friend bool operator==(const Point &a, const Point &b);

    // 3) 출력 연산자
    friend ostream &operator<<(ostream &os, const Point &p);

    // 4) 템플릿 비멤버 함수(friend는 템플릿도 가능)
    template <typename U>
    friend bool compare(const Point &a, const Point &b);
};

double distance(const Point &a, const Point &b)
{
    // private x,y 직접 접근 가능
    return hypot(a.x - b.x, a.y - b.y);
}

bool operator==(const Point &a, const Point &b) { return a.x == b.x && a.y == b.y; }

ostream &operator<<(ostream &os, const Point &p) { return os << "(" << p.x << "," << p.y << ")"; }

template <typename U>
bool compare(const Point &a, const Point &b)
{
    return a.x < b.x; // 템플릿 비멤버 함수도 friend라 접근 가능
}

/* ============================================================================
   PATTERN B) friend class
   ---------------------------------------------------------------------------
   - 특정 클래스 전체(private/protected 포함)를 열어주는 패턴
   - 내부적으로 서로 강하게 얽힌 구조에서 사용
   (예: Tree ↔ Iterator, RBT header ↔ rb_tree)
   ============================================================================ */
class Car;

class Engine
{
  public:
    void burn(Car &c); // Car의 private 연료에 접근 필요
};

class Car
{
  private:
    int fuel = 100;

    friend class Engine; // Engine에게 Car의 private 전체 허용
};

void Engine::burn(Car &c)
{
    c.fuel -= 10; // friend class라 접근 가능
}

/* ============================================================================
   PATTERN C) 특정 멤버 함수만 friend
   ---------------------------------------------------------------------------
   - 클래스 전체를 열지 않고 "특정 함수만" 권한 부여
   - 최소 권한 부여 원칙
   ============================================================================ */
class Logger;

class DB
{
  private:
    int secret = 999;

    // Logger 클래스 전체가 아니라
    // Logger::print() 이 함수 하나만 private 접근 허용
    friend void Logger::print(const DB &);
};

class Logger
{
  public:
    void print(const DB &db)
    {
        cout << "[DB secret] " << db.secret << "\n"; // private 접근 허용됨
    }
};

/* ============================================================================
   PATTERN D) 템플릿 클래스 friend (모든 템플릿 인스턴스 상호 접근)
   ---------------------------------------------------------------------------
   - std::map, std::set 등의 구현에서 나옴
   - Wrapper<int> ↔ Wrapper<double> 서로 private 접근 가능
   ============================================================================ */
template <typename T>
class Wrapper
{
  private:
    T data = 1234;

  public:
    // 모든 Wrapper<U>는 Wrapper<T>의 private 접근 가능
    template <typename U>
    friend class Wrapper;
};

/* ============================================================================
   PATTERN E) Nested friend (외부 클래스가 내부 private 접근)
   ---------------------------------------------------------------------------
   - Tree / Node 구조처럼, 내부 클래스의 private를 외부가 봐야 할 때
   - 실제로 STL RBT 구조(Node ↔ tree_base 구조)에서 많이 등장
   ============================================================================ */
class Tree
{
  public:
    class Node;

  private:
    Node *root;

  public:
    void setRoot(int v);
};

class Tree::Node
{
  private:
    int key;

    // Tree 클래스는 Node의 private 멤버 key에 접근할 수 있음
    friend class Tree;

  public:
    Node(int k) : key(k) {}
};

void Tree::setRoot(int v)
{
    root = new Node(v);
    cout << "[Tree root key] " << root->key << "\n";
}

/* ============================================================================
   MAIN — 모든 패턴 실행
   ============================================================================ */
int main()
{
    cout << "=== PATTERN A (비멤버/연산자/템플릿 friend) ===\n";
    Point p1(0, 0), p2(3, 4);
    cout << "distance: " << distance(p1, p2) << "\n";
    cout << "(p1 == p2): " << (p1 == p2) << "\n";
    cout << "p1: " << p1 << "\n";
    cout << "compare<int>: " << compare<int>(p1, p2) << "\n";

    cout << "\n=== PATTERN B (friend class) ===\n";
    Car    car;
    Engine eng;
    eng.burn(car);
    cout << "Car was burned (private fuel modified)\n";

    cout << "\n=== PATTERN C (특정 멤버만 friend) ===\n";
    DB     db;
    Logger log;
    log.print(db);

    cout << "\n=== PATTERN D (템플릿 클래스 friend) ===\n";
    Wrapper<int>    w1;
    Wrapper<double> w2; // 서로 friend라 내부 접근 가능 구조
    cout << "Wrapper instances created.\n";
