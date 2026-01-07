#include "../exercise.h"

// READ: 静态字段 <https://zh.cppreference.com/w/cpp/language/static>
// READ: 虚析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>
// 如果你计划通过基类指针删除派生类对象，基类必须有虚析构函数。
struct A {
    // TODO: 正确初始化静态字段
    static inline int num_a = 0;

    A() {
        ++num_a;
    }
    virtual ~A() {
        --num_a;
    }

    virtual char name() const {
        return 'A';
    }
};
struct B final : public A {
    // TODO: 正确初始化静态字段
    static inline int num_b = 0;

    B() {
        ++num_b;
    }
    ~B() override {
        --num_b;
    }

    char name() const final {
        return 'B';
    }
};

// 当执行 A *ab = new B; 时，实际上是在创建一个 B 类的对象。在 C++ 中，派生类（子类）对象的创建过程遵循特定的构造顺序：

// 内存分配：首先在堆上分配足够容纳 B 对象（包括其继承自 A 的部分）的内存。
// 基类构造：C++ 规定，在构造派生类对象之前，必须先构造其基类部分。这是为了确保子类在使用继承来的成员之前，这部分成员已经被正确初始化。所以 A() 首先被调用。
// 此时 A::num_a 增加。
// 派生类构造：基类构造完成后，接着执行派生类自己的构造函数 B()。
// 此时 B::num_b 增加。

int main(int argc, char **argv) {
    auto a = new A;
    auto b = new B;
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()");
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()");

    delete a;
    delete b;
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    A *ab = new B;// 派生类指针可以随意转换为基类指针
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()");

    // TODO: 基类指针无法随意转换为派生类指针，补全正确的转换语句
    B &bb = dynamic_cast<B &>(*ab);
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()");

    // TODO: ---- 以下代码不要修改，通过改正类定义解决编译问题 ----
    delete ab;// 通过指针可以删除指向的对象，即使是多态对象
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    return 0;
}
