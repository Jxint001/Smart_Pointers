// // #include "weak_ptr.hpp"
// // #include <iostream>
// // #include <vector>
// // using std::cout, std::endl;
// // bool TEST() {
// //     // Test weak_ptr assignment and lock
// //     SharedPtr<std::string> sp1{new std::string{"Hello"}};
// //     WeakPtr<std::string> wp1(sp1);

// //     // cout << "wp1 " << wp1._wp<< " sp1 " << sp1._p << endl;
// //     // cout << "wp1 " << wp1.ctrl.countsp<< " sp1 " << sp1.ctrl.countsp << endl;
// //     // cout << "wp1 " << wp1.ctrl.countwp<< " sp1 " << sp1.ctrl.countwp << endl;
// //     // cout << "wp1 " << wp1.ctrl.expd<< " sp1 " << sp1.ctrl.expd << endl;
// //     // cout << endl;

// //     WeakPtr<std::string> wp2 = wp1;

// //     // cout << "wp2 " << wp2._wp<< " sp1 " << sp1._p << endl;
// //     // cout << "wp2 " << wp2.ctrl.countsp<< " sp1 " << sp1.ctrl.countsp << endl;
// //     // cout << "wp2 " << wp2.ctrl.countwp<< " sp1 " << sp1.ctrl.countwp << endl;
// //     // cout << "wp2 " << wp2.ctrl.expd<< " sp1 " << sp1.ctrl.expd << endl;
// //     // cout << endl;


// //     SharedPtr<std::string> sp2 = wp1.lock();
// //     cout << *sp1 << " " << *sp2 << endl;

// //     // cout << "sp2 " << sp2._p<< " sp1 " << sp1._p << endl;
// //     // cout << "sp2 " << sp2.ctrl.countsp<< " sp1 " << sp1.ctrl.countsp << endl;
// //     // cout << "sp2 " << sp2.ctrl.countwp<< " sp1 " << sp1.ctrl.countwp << endl;
// //     // cout << "sp2 " << sp2.ctrl.expd<< " sp1 " << sp1.ctrl.expd << endl;
// //     // cout << endl;

// //     // cout << "wp1 " << wp1._wp<< " sp1 " << sp1._p << endl;
// //     // cout << "wp1 " << wp1.ctrl.countsp<< " sp1 " << sp1.ctrl.countsp << endl;
// //     // cout << "wp1 " << wp1.ctrl.countwp<< " sp1 " << sp1.ctrl.countwp << endl;
// //     // cout << "wp1 " << wp1.ctrl.expd<< " sp1 " << sp1.ctrl.expd << endl;
// //     // cout << endl;

// //     if (!sp2 || *sp2 != "Hello" || wp1.use_count() != 2) {
// //         cout << "hono" << endl;
// //         return false;
// //     }

// //     sp1.reset();
// //     //cout << "sp1 reset finished" << endl;
// //     sp2.reset();
// //     //cout << "sp2 reset finished" << endl;
// //     //cout << "result: wp1: " << wp1.expired() << endl;
// //     //cout << "result: wp2: " << wp2.expired() << endl;
// //     //cout << "finish all" << endl;
// //     if (!wp1.expired() || !wp2.expired()) {
// //         cout << "hello" << endl;
// //         return false;
// //     }
// //     return true;
// // }
// // int main() {
// //     //TEST();
// //     cout << TEST() << endl;

// //     SharedPtr<int> sp1{new int{42}};
// //     //sp1.reset();
// //     WeakPtr<int> wp1;
// //     WeakPtr<int> wp2(sp1);
// //     bool result = true;
// //     //cout << wp1.expired() << " " << wp2.expired() << endl;
// //     //pppppppppppp
// //     if (wp1.expired() != true || wp2.expired() != false) {
// //         result = false;
// //     }
// //     cout << result << endl;

// //     cout << "test" << endl;
// //     SharedPtr<int> p1{new int{999}};
// //     p1.reset(new int{20});
// //     cout << *p1 << endl;
// //     wp2 = p1;
// //     SharedPtr<int> locked = wp2.lock();
// //     // cout << *locked << endl;
// //     // cout << locked.use_count() << endl;
// //     cout << !locked << endl;

// //     std::vector<SharedPtr<int>> a;
// //     a.push_back(p1);
// // }
// #include "sp.hpp"
// #include <iostream>
// using std::cout, std::endl;
// bool TEST() {
//     bool result = 1;
//     SharedPtr<int> ptr1{make_shared<int>(10)};
//     SharedPtr<int> ptr2{ptr1};
//     if(*ptr1 != 10 || *ptr2 != 10 || ptr1.get() != ptr2.get()
//     || ptr1.use_count() != 2 || ptr2.use_count() != 2)
//     {
//         result = 0;
//     }
//     return result;
// }
// int main() {
//     cout << TEST() << endl;

//     SharedPtr<int> sp1{make_shared<int>(12)};
//     sp1.reset(new int(20));
//     cout << *sp1 << endl;
// }
#include "shared_ptr.hpp"
#include <iostream>
int main() {
    const SharedPtr<int> a = make_shared<int>(2);
    std::cout << a.use_count()<<std::endl;
    return 0;
}