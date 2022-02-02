
#include "Diagram.h"


int main()
{
    setlocale(LC_ALL, "Russian");
    Scan* S = new Scan("input.txt");
    Diagram D(S);
    SemanticTree* st = new SemanticTree();

    st->SetCur(st);
    D.setTree(st);
    if (!D.Process())
        std::cout << "Ошибка. Ожидалось \"{\"." << std::endl;
    return 0;
}