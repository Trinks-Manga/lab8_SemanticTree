#pragma once
#define MaxLen 10000 //максимальная длина текста ИМ
#define LexSize 20 //максимальная длина лексемы
#define LexBuff 10000

#define TYPE_MAIN 1
#define TYPE_WHILE 2
#define TYPE_SHORT 3
#define TYPE_LONG 4
#define TYPE_INT 5
#define TYPE_RETURN 6
#define TYPE_CLASS 7

#define TYPE_IDENTIFIER 20
#define TYPE_CONST_NUMERIC 31


#define TYPE_DOT 39
#define TYPE_COMMA 40
#define TYPE_SEMICOLON 41
#define TYPE_OPEN_PARENTHESES 42
#define TYPE_CLOSED_PARENTHESES 43
#define TYPE_OPEN_BRACES 44
#define TYPE_CLOSED_BRACES 45
#define TYPE_OPEN_SQ_BRACKETS 46
#define TYPE_CLOSED_SQ_BRACKETS 47

#define TYPE_OR 80
#define TYPE_AND 81
#define TYPE_LESS 50
#define TYPE_LESS_EQUAL 51
#define TYPE_MORE 52
#define TYPE_MORE_EQUAL 53
#define TYPE_EQUAL 54
#define TYPE_NOT_EQUAL 55
#define TYPE_PLUS 56
#define TYPE_MINUS 57
#define TYPE_MULTIPLICATION 58
#define TYPE_DIVISION 59
#define TYPE_MOD 60
#define TYPE_SAVE 61
#define TYPE_INCREMENT 62
#define TYPE_DECREMENT 63

#define TYPE_END 100

#define TYPE_ERROR 200

#define LowL ( (text[it]>= 'a') && (text[it]<='z') || text[it] == '_' )
#define HighL ( (text[it]>='A') && (text[it]<='Z') )
#define Num ( (text[it]>='0') && (text[it]<='9') )
#define math ((type >= TYPE_LESS && type <= TYPE_DECREMENT) || (type >= TYPE_OR && type <= TYPE_AND))



typedef char Text[MaxLen];
typedef char LEX[LexBuff];


