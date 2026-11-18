#include <iostream>

class Person {
public:
    int a = 4;
};

int main() {
    Person p;
    printf("%d", p.a);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
