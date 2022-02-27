#include "Linter/Linter.h"

int main() {
    Linter::Linter l("test.cpp");
    l.printTokens();
    l.detailedReport(std::cout);
    std::cout << l.areVariablesDefined() << std::endl;
}
