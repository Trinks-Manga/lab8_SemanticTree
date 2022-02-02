#include "Diagram.h"

int Diagram::lookforward(int k) {
    LEX lex;
    int savedUk = _sc->getUK();
    int savedLine = _sc->getLine();
    int savedPos = _sc->getPos();

    int nextType = 0;
    for (int i = 0; i < k; i++)
        nextType = _sc->scanner(lex);
    _sc->setLine(savedLine);
    _sc->setUK(savedUk);
    _sc->setPos(savedPos);
    return nextType;
}


Diagram::Diagram(Scan* sc)
{
    _sc = sc;
    st = new SemanticTree;

}

void Diagram::setTree(SemanticTree* s)
{
    st = s;
}

bool Diagram::Process() {
    do {
        type = lookforward(1);
        if (type == TYPE_CLASS) {
            Class();
        }
        else if (type == TYPE_INT && lookforward(2) == TYPE_MAIN) {
            Main();
        }
        else if ((type == TYPE_INT || type == TYPE_SHORT || type == TYPE_LONG) && (lookforward(2) == TYPE_IDENTIFIER) && (lookforward(3) == TYPE_OPEN_PARENTHESES)) Func();
        else if (type == TYPE_INT || type == TYPE_SHORT || type == TYPE_LONG) {
            Data();
        }
        else if (type == TYPE_CLOSED_BRACES) {
            return false;
        }
        else if (type == TYPE_SEMICOLON) type = _sc->scanner(lex);
        else if (type == TYPE_END) return true;
        else {
            std::cout << "Неожиданная ошибка типа" << std::endl;
            return true;
        }
    } while (type != TYPE_END);
    return true;
}

void Diagram::Class() {
    type = _sc->scanner(lex);

    if (type != TYPE_CLASS) {
        std::cout << "Непредвиденная ошибка !" << std::endl;
        return;
    }

    type = _sc->scanner(lex);

    if (type != TYPE_IDENTIFIER) {
        std::cout << "Ошибка объявления класса ожидался идентификатор." << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
        return;
    }

    /*!!!Semantica*/ // заносим определение класса в дерево
    SemanticTree* v = st->SemInclude(lex, ObjTypeClass);
    v->getData()->dataIdent.typeVar = st->lastUsersType++; // увеличиваем пользовательские типыы

    type = _sc->scanner(lex);

    if (type == TYPE_OPEN_BRACES) {
        bool f = Process();
        if (!f) {
            type = _sc->scanner(lex);
                return;
            }
            else
                std::cout << "Ошибка. Ожидалось \"}\"" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
        }
        else if (type == TYPE_SEMICOLON) {
            return;
        }
        else {
            std::cout << "Ошибка объявления класса!" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            return;
        }

}


void Diagram::While()
{
    type = _sc->scanner(lex);
    SemanticTree* ret = st->SetMakeNewArea(); // запоминаем состояние
    if (type == TYPE_OPEN_PARENTHESES) {
        Expression();
        type = _sc->scanner(lex);
        if (type == TYPE_CLOSED_PARENTHESES) {

            type = _sc->scanner(lex);
            if (type == TYPE_OPEN_BRACES) {
                ComplexOper();
            }
            else
                SimpleOper();
        }
        else {
            st->SetCur(ret);
            std::cout << "Ошибка. Ожидалось \")\". " << std::endl << "Line: " << _sc->getLine() << _sc->getPos() << " Pos: " << std::endl;
            return;
        }
    }
    else {
        std::cout << "Ошибка объявления цикла while. Ожидалось \"(\"." << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;

        st->SetCur(ret);
        return;
    }

    st->SetCur(ret);

}

void Diagram::Equ()
{
    type = _sc->scanner(lex);
    if (type == TYPE_SAVE) {
        Expression();

    }
    else {
        std::cout << "Equ ошибка, смотри внимательно вызов." << std::endl;
    }

}


void Diagram::SimpleOper()
{
    if (type == TYPE_RETURN) {
        Expression();
    }
    else if (type == TYPE_WHILE) {
        While();
    }
    else if (type == TYPE_IDENTIFIER) {
        type = _sc->scanner(lex);
        int temp = lookforward(1);

        SemanticTree* f = Field();
        /*
        while (type = TYPE_DOT && temp == TYPE_IDENTIFIER) {
            type = _sc->scanner(lex);
            temp = lookforward(1);
        }
        */
        if (type == TYPE_SAVE)
        {
            Equ();
        }
        else
            if (type == TYPE_OPEN_PARENTHESES)
            {
                type = _sc->scanner(lex);
                if (type == TYPE_CLOSED_PARENTHESES) {
                    return;
                }
                else std::cout << "Ошибка. Ожидалось \")\". Обнаружено: " << lex << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            }
            else {
                std::cout << "Ошибка в простом операторе!" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            }

        if (type != TYPE_SEMICOLON)
        {
            std::cout << "Ошибка. Ожидалось \";\". Обнаружено: " << lex << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
        }
    }
    else if (type == TYPE_SEMICOLON);
    else {
        std::cout << "Ошибка в простом операторе! " << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
    }
}

void Diagram::ComplexOper()
{
    while (true) {
        int temp = lookforward(1);

        if (temp == TYPE_INT || temp == TYPE_SHORT || temp == TYPE_LONG) {
            Data();
        }
        else if (temp == TYPE_IDENTIFIER) {
            type = lookforward(2);
            if (type == TYPE_IDENTIFIER) {
                Data(); continue;
            }
            type = lookforward(2);
            if (type == TYPE_SAVE) {
                type = _sc->scanner(lex);
                Equ();
            }
            else if (type == TYPE_SEMICOLON)
                type = _sc->scanner(lex);
            else if (type == TYPE_OPEN_SQ_BRACKETS) {
                type = _sc->scanner(lex);
                type = _sc->scanner(lex);
                temp = lookforward(1);
                if (temp == TYPE_CONST_NUMERIC) {
                    type = _sc->scanner(lex);
                    type = _sc->scanner(lex);
                    if (type = TYPE_SAVE) Equ();
                }
                else
                Mass();
            }
            else if (type == TYPE_OPEN_PARENTHESES) {
                type = _sc->scanner(lex);
                type = _sc->scanner(lex);
                type = _sc->scanner(lex);
                if (type == TYPE_CLOSED_PARENTHESES) {

                }
                else {
                    std::cout << "Ошибка. Ожидалось \")\"." << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
                }
            }
            else if (type == TYPE_COMMA) {
                type = _sc->scanner(lex);
                Datas();
            }
            else if (type == TYPE_DOT) {
                type = _sc->scanner(lex);
                SemanticTree* f = Field();
                type = lookforward(1);
                if (type == TYPE_SAVE) Equ();
                else if (type == TYPE_SEMICOLON);
            }
            else
            {
                Expression(); type = _sc->scanner(lex);
            }
        }
        else if (temp == TYPE_WHILE) {
            type = _sc->scanner(lex);
            While();
        }
        else if (temp == TYPE_SEMICOLON) type = _sc->scanner(lex);
        else if (temp == TYPE_CLOSED_BRACES) {
            type = _sc->scanner(lex);  return;
        }
        else if (temp == TYPE_RETURN) {
            type = _sc->scanner(lex);
            type = lookforward(1);
            Expression();
            /*
            if (type == TYPE_CONST_NUMERIC || type == TYPE_IDENTIFIER) Expression();
            else std::cout << "Непредвиденная ошибка в составном операторе!" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            */
            type = _sc->scanner(lex);
            if (type == TYPE_SEMICOLON) {

            }
            else std::cout << "Непредвиденная ошибка в составном операторе!" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
        }
        else {
            type = _sc->scanner(lex);
            std::cout << "Непредвиденная ошибка в составном операторе!" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            return;
        }
    }
}


void Diagram::Mass()
{
    type = _sc->scanner(lex);
    if (type == TYPE_CONST_NUMERIC) {
        type = _sc->scanner(lex);
        if (type == TYPE_CLOSED_SQ_BRACKETS) {
            return;
        }
        else {
            std::cout << "Ошибка. Ожидалось \"]\". Обнаружено: " << lex << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            return;
        }
    }
    else if (type == TYPE_CLOSED_SQ_BRACKETS) {
        type = _sc->scanner(lex); 
        if (type == TYPE_SAVE) {
            type = _sc->scanner(lex);
            if (type == TYPE_OPEN_BRACES) {
                type = _sc->scanner(lex);
                while (type == TYPE_CONST_NUMERIC) {
                    type = _sc->scanner(lex);
                    if (type == TYPE_CLOSED_BRACES)
                        break;
                    else if (type != TYPE_COMMA) {
                        std::cout << "Ошибка. Ожидалось \",\". Обнаружено " << lex << std::endl << "Line :" << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
                        return;
                    }
                    else type = _sc->scanner(lex);
                }
                return;
            }
            else {
                std::cout << "Ошибка. Ожидалось агрегатное определение. " << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            }
            
        }
        else {
            std::cout << "Неинициализированная локальная переменная!" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            type = _sc->scanner(lex);
        }
    }
    else {
        std::cout << "Ошибка. Ожидалась константа. Обнаружено: " << lex << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
    }


}

SemanticTree* Diagram::Field() // возвращает ссылку на поле или переменную
{
    
    if (type != TYPE_IDENTIFIER)
    {
        std::cout << "Не является идентификатором" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
    }

    /*!!!Semantica*/ // проверяем, что в классе есть поле
    //
    SemanticTree* object = st->SemGetObject(lex);
    SemanticTree* field = nullptr;
    while (lookforward(1) == TYPE_DOT)
    {
        _sc->scanner(lex);
        type = _sc->scanner(lex);
        if (type != TYPE_IDENTIFIER)
        {
            std::cout << "Не является полем" << std::endl << "Line: " << _sc->getLine()<< " Pos: " << _sc->getPos() << std::endl;
        }

        field = object->getData()->dataIdent.typeClass->FindRightLeft(lex);
        if (field == NULL)
        {

            std::cout << " В классе не найден элемент " << lex << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;//////////////////////////////////
            field = st->AddField(object, lex); // Устраняем ошибку отсутствия поля
        }
        object = field;
    }
    return field;
}

void Diagram::Expression()
{
    type = _sc->scanner(lex);
    if (type != TYPE_CONST_NUMERIC && type != TYPE_IDENTIFIER) {
        std::cout << "Непредвиденная ошибка в выражении!" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
    }
    while (true) {
        int temp = type;
        type = lookforward(1);
       
        if (type == TYPE_OPEN_PARENTHESES) {

            SemanticTree* foo = st->SemGetFunct(lex);
            type = _sc->scanner(lex);
            type = _sc->scanner(lex);
            if (type == TYPE_CLOSED_PARENTHESES) {
                type = lookforward(1);
                if math
                    continue;
                else return;
            }
            else {
                std::cout << "Ошибка в выражении!" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
                return;
            }
        }
        else if math{
            _sc->scanner(lex);
            int temp = lookforward(1);
            if ((type == TYPE_DECREMENT || type == TYPE_INCREMENT) && temp == math) continue;
            else if ((type == TYPE_DECREMENT || type == TYPE_INCREMENT) && temp == TYPE_SEMICOLON) return;
            type = _sc->scanner(lex);
            if (type == TYPE_CONST_NUMERIC || type == TYPE_IDENTIFIER) continue;
            else if (type == TYPE_OPEN_PARENTHESES) {
                Expression();
                if (type == TYPE_CLOSED_PARENTHESES) type = _sc->scanner(lex);
                continue;
            }
            else {
                std::cout << "Ошибка в выражении!" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
                return;
            }
        }
        else if (type == TYPE_SEMICOLON) return;
        else if (type == TYPE_CLOSED_PARENTHESES) {
            return;
        }
        else if (type == TYPE_COMMA) return;
        else if (type == TYPE_OPEN_SQ_BRACKETS && temp == TYPE_IDENTIFIER) {
            type = _sc->scanner(lex);
            Expression();
            type = _sc->scanner(lex);
        }
        else if (type == TYPE_DOT) {
            type = temp;
            if (lookforward(3) == TYPE_OPEN_PARENTHESES) {
                SemanticTree* temp = st->GetCur();
                st = st->SemGetClass(lex);
                type = _sc->scanner(lex);
                type = _sc->scanner(lex);

                SemanticTree* foo = st->SemGetFunct(lex);

                st->SetCur(temp);
                type = _sc->scanner(lex);
                if (type == TYPE_OPEN_PARENTHESES && lookforward(1)) {
                    type = _sc->scanner(lex);
                    //if (lookforward(1) == TYPE_SEMICOLON) return;
                    continue;
                }
                else std::cout << "Ошибка в вызове функции, ожидалось \"()\", обнаружено: " << lex << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            }
            else
            SemanticTree* f = Field();
        }
        else {
            return;
        }
    }

}

void Diagram::Datas()
{
    while (true) {
        if (type == TYPE_COMMA) {
            type = _sc->scanner(lex);
            if (type == TYPE_IDENTIFIER) {

                type = lookforward(1);
                if (type == TYPE_OPEN_SQ_BRACKETS) {
                    Mass();
                }
                else if (type == TYPE_SAVE) {
                    Equ();
                    type = lookforward(1);
                    if (type == TYPE_SEMICOLON) return;
                }
                else if (lookforward(1) == TYPE_COMMA) continue;
                else if (type == TYPE_SEMICOLON) return;
                else std::cout << "Ошибка в объявлении переменных!" << std::endl << "Line :" << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            }
        }
        else if (type == TYPE_IDENTIFIER) {
            if (lookforward(1) == TYPE_SAVE) {
                Equ();
                type = lookforward(1);
                if (type == TYPE_SEMICOLON) return;
            }
            else if (type == TYPE_OPEN_SQ_BRACKETS) {
                Mass();
            }
            else if (type == TYPE_SEMICOLON) return;
            else if (lookforward(1) == TYPE_COMMA) continue;
            else std::cout << "Ошибка в объявлении переменных!" << std::endl << "Line :" << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
        }
        else if (type == TYPE_SEMICOLON) return;
        else {
            return;
        }
        

    }
}

void Diagram::Func()
{

    type = _sc->scanner(lex);
    type = _sc->scanner(lex);
    /*!!!Semantica*/ // заносим функцию в дерево
    SemanticTree* func = st->SemInclude(lex, ObjFunct);
    type = _sc->scanner(lex);
    type = _sc->scanner(lex);
    if (type == TYPE_CLOSED_PARENTHESES) {
        type = _sc->scanner(lex);
        if (type == TYPE_OPEN_BRACES) {
            ComplexOper();
        }
        else if (type == TYPE_SEMICOLON) return;
        else std::cout << "Ошибка. Ожидался составной оператор." << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
    }
    else std::cout << "Ошибка. Ожидался составной оператор." << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;

}


void Diagram::Main()
{
    if (lookforward(1) == TYPE_INT) {
        type = _sc->scanner(lex);
        type = _sc->scanner(lex);
    }
    else if (lookforward(1) == TYPE_MAIN) {

        type = _sc->scanner(lex);
    }
    int temp = lookforward(1);
    if (temp == TYPE_OPEN_BRACES) {
        ComplexOper();
    }
    else if (temp == TYPE_OPEN_PARENTHESES) {
        temp = _sc->scanner(lex);
        temp = _sc->scanner(lex);
        if (temp == TYPE_CLOSED_PARENTHESES) {
            type = _sc->scanner(lex);
            if (type == TYPE_OPEN_BRACES) ComplexOper();
            else if (type == TYPE_SEMICOLON) return;
        }
        else std::cout << "Ошибка в объявлении функции main, ожидалось \")\". " << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
    }
    else if (temp == TYPE_SEMICOLON) return;
    else {
        type = _sc->scanner(lex);
        std::cout << "Ошибка в объявлении функции main, ожидалось \"{\" или \";\". " << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
    }
}

void Diagram::Data() {
    type = _sc->scanner(lex);

    if (type != TYPE_INT && type != TYPE_SHORT && type != TYPE_LONG && type != TYPE_IDENTIFIER) {
        std::cout << " Непредвиденная ошибка типа в объявлении данных. " << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
        return;
    }


    int typeVar = st->convertLexicalTypeToVarType(type);
    SemanticTree* classType = nullptr;
    TypeObject typeObject = ObjVar;
    if (typeVar == TypeUndef)
    {
        // добавляем экземпляр класса
        classType = st->SemGetClass(lex);
        typeObject = ObjClass;
        typeVar = classType->getVarType();
    }

    type = _sc->scanner(lex);

    if (type == TYPE_IDENTIFIER) {
        /*!!!Semantica*/ // заносим в дерево переменные
        SemanticTree* v = st->SemInclude(lex, typeObject);
        st->SemSetTypeVar(v, typeVar);
        if (typeObject == ObjClass)
        {
            v->getData()->dataIdent.typeClass = classType; // указать какой класс
        }
        int temp_type = lookforward(1);

        if (temp_type == TYPE_OPEN_SQ_BRACKETS) {
            type = _sc->scanner(lex);
            Mass();
        }
        else if (temp_type == TYPE_COMMA) {
            Datas();
        }
        else if (temp_type == TYPE_SEMICOLON) {
            return;
        }
        else if (temp_type == TYPE_SAVE) {
            Equ();

            temp_type = lookforward(1);
            if (temp_type == TYPE_COMMA)
                Datas();
            else if (temp_type == TYPE_SEMICOLON) {
                return;
            }
            else {
                type = _sc->scanner(lex);
                std::cout << "Непредвиденная ошибка при объявлении данных! " << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            }
        }
        else {
            temp_type = _sc->scanner(lex);
            std::cout << "Непредвиденная ошибка объявления данных!" << std::endl << "Line: " << _sc->getLine() << " Pos: " << _sc->getPos() << std::endl;
            return;
        }

    }

}

