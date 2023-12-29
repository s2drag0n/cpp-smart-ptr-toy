#include "AutoPtr.h"
#include <string>

class Test {
  public:
    Test() = default;

    ~Test() { std::cout << "Test is deleted" << std::endl; }
    void set_name(const std::string str) { name = str; }
    std::string get_name() { return name; }

  private:
    std::string name;
};

int main() {
    Test *p = new Test();
    szl::AutoPtr<Test> sp(p);
    sp->set_name("jack");
    std::cout << sp->get_name() << std::endl;

    Test *p2 = new Test();
    p2->set_name("szl");
    sp.reset(p2);
    std::cout << sp->get_name() << std::endl;

    szl::AutoPtr<Test> ap2 = sp;

    // wrong : sp->get_name();
    return 0;
}
