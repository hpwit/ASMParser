#include <regex.h>
#include <stdio.h>
#include <list>
#include <string>
#include "memory.h"
#include <vector>

#pragma once
using namespace std;
// #include "asm_parser.h"
#include "string_function.h"
#include "asm_struct_enum.h"

#define EOF_TEXT 0

#ifndef ARDUINO
/*
template <typename... Args>
std::string string_format(const std::string &format, Args... args)

{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size_s <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
*/
#endif

enum varTypeEnum
{
    __none__,
    __uint8_t__,
    __uint16_t__,
    __uint32_t__,
    __int__,
    __float__,
    __void__,
    __CRGB__,
    __CRGBW__,
    __char__,
    __userDefined__
};

struct varType
{
    varTypeEnum _varType;
    string varName;
    uint8_t _varSize;
    asmInstruction load[20];
    asmInstruction store[20];
    string membersNames[10];
    uint8_t starts[20];

    // asmInstruction add;
    // asmInstruction mul;
    // asmInstruction div;
    // string reg_name;
    uint8_t sizes[20];
    uint8_t memberSize[20];
    // string vnames[20];
    varTypeEnum types[20];
    uint8_t size;
    uint8_t total_size;
};
varType usded;

vector<varType> _userDefinedTypes;
varType __v;
string varTypeEnumNames[] = {
#ifdef __TEST_DEBUG
    "__none__",
    "__unit8_t__",
    "__unit16_t__",
    "__unit32_t__",
    "__int__",
    "__float__",
    "__void__",
    "__CRGB__",
    "__CRGBW__",
    "__char__",
    "__userDefined__",
#endif

};

vector<string> userDefinedVarTypeNames;

varType _varTypes[] = {
    {
        ._varType = __none__,
    },
    {._varType = __uint8_t__,
     ._varSize = 1,
     // .load={l8ui},
     // .store = {s8i},
     // .reg_name = "a",
     .sizes = {1},
     .size = 1,
     .total_size = 1},
    {
        ._varType = __uint16_t__,
        ._varSize = 2,
        .load = {l16ui},
        .store = {s16i},
        // .reg_name = "a",
        .sizes = {2},
        .size = 1,
        .total_size = 2,
    },
    {
        ._varType = __uint32_t__,
        ._varSize = 4,
        .load = {l32i},
        .store = {s32i},
        // .reg_name = "a",
        .sizes = {4},
        .size = 1,
        .total_size = 4,
    },
    {
        ._varType = __int__,
        ._varSize = 2,
        .load = {l16si},
        .store = {s16i},
        // .reg_name = "a",
        .sizes = {2},
        .size = 1,
        .total_size = 2,
    },
    {
        ._varType = __float__,
        ._varSize = 4,
        .load = {lsi},
        .store = {ssi},
        // .reg_name = "f",
        .sizes = {4},
        .size = 1,
        .total_size = 4,
    },
    {
        ._varType = __void__,
        ._varSize = 0,
        .load = {},
        .store = {},
        // .reg_name = "a",
        .sizes = {0},
        .size = 0,
        .total_size = 0,
    },
    {
        ._varType = __CRGB__,
        ._varSize = 3,
        .load = {l8ui, l8ui, l8ui},
        .store = {s8i, s8i, s8i},
        // .reg_name = "a",
        .sizes = {1, 1, 1},
        .size = 3,
        .total_size = 3,
    },
    {
        ._varType = __CRGBW__,
        ._varSize = 4,
        .load = {l8ui, l8ui, l8ui, l8ui},
        .store = {s8i, s8i, s8i, s8i},
        // .reg_name = "a",
        .sizes = {1, 1, 1, 1},
        .size = 4,
        .total_size = 4,
    },
    {
        ._varType = __char__,
        ._varSize = 1,
        .load = {l8ui},
        .store = {s8i},
        // .reg_name = "a",
        .sizes = {1},
        .size = 1,
        .total_size = 1,
    }

};

string keywordTypeNames[] = {
#ifdef __TEST_DEBUG
    "KeywordVarType",
    "KeywordVarType",
    "KeywordVarType",
    "KeywordVarType",
    "KeywordVarType",
    "KeywordVarType",
    "KeywordVarType",
    "KeywordVarType",
    "KeywordVarType",
    "KeywordExternalVar",
    "KeywordFor",
    "KeywordIf",
    "KeywordThen",
    "KeywordElse",
    "KeywordWhile",
    "KeywordReturn",
    "KeywordImport",
    "KeywordFrom",
    "KeywordASM",
    "KeywordDefine",
    "KeywordSafeMode",
    "KeywordHeader",
    "KeywordContent",
#endif

};

#define nb_keywords 33
#define nb_typeVariables 10
string keyword_array[nb_keywords] =
    {"none", "uint8_t", "uint16_t", "uint32_t", "int", "float", "void", "CRGB",
     "CRGBW", "char", "external", "for", "if", "then", "else", "while", "return",
     "import", "from", "__ASM__",
     "define", "safe_mode", "_header_", "_content_", "and", "or", "continue",
     "break", "fabs", "abs", "save_reg",
     "save_reg_abs", "struct"};

bool __isBlockComment = false;
enum tokenType
{
    TokenNumber,
    TokenAddition,
    TokenStar,
    TokenSubstraction,
    TokenOpenParenthesis,
    TokenCloseParenthesis,
    TokenOpenBracket,
    TokenCloseBracket,
    TokenOpenCurlyBracket,
    TokenCloseCurlyBracket,
    TokenEqual,
    TokenDoubleEqual,
    TokenIdentifier,
    TokenSemicolon,
    TokenUnknown,
    TokenSpace,
    TokenNewline,
    TokenEndOfFile,
    TokenSlash,
    TokenKeyword,
    TokenString,
    TokenExternal,
    TokenComma,
    TokenLessThan,
    TokenPlusPlus,
    TokenMinusMinus,
    TokenModulo,
    TokenLessOrEqualThan,
    TokenMoreThan,
    TokenMoreOrEqualThan,
    TokenNotEqual,
    TokenNot,
    TokenFunction,
    TokenUppersand,
    TokenDiese,
    TokenLineComment,
    TokenStartBlockComment,
    TokenEndBlockComment,
    TokenNegation,
    TokenShiftLeft,
    TokenShiftRight,
    TokenKeywordVarType,
    TokenKeywordExternalVar,
    TokenKeywordFor,
    TokenKeywordIf,
    TokenKeywordThen,
    TokenKeywordElse,
    TokenKeywordWhile,
    TokenKeywordReturn,
    TokenKeywordImport,
    TokenKeywordFrom,
    TokenKeywordASM,
    TokenKeywordDefine,
    TokenKeywordSafeMode,
    TokenKeywordHeader,
    TokenKeywordContent,
    TokenKeywordAnd,
    TokenKeywordOr,
    TokenPower,
    TokenKeywordContinue,
    TokenKeywordBreak,
    TokenKeywordFabs,
    TokenKeywordAbs,
    TokenKeywordSaveReg,
    TokenKeywordSaveRegAbs,
    TokenKeywordStruct,
    TokenUserDefinedName,
    TokenUserDefinedVariable,
    TokenMember,

};

tokenType __keywordTypes[] = {
    TokenKeywordVarType,
    TokenKeywordVarType,
    TokenKeywordVarType,
    TokenKeywordVarType,
    TokenKeywordVarType,
    TokenKeywordVarType,
    TokenKeywordVarType,
    TokenKeywordVarType,
    TokenKeywordVarType,
    TokenKeywordVarType,
    TokenKeywordExternalVar,
    TokenKeywordFor,
    TokenKeywordIf,
    TokenKeywordThen,
    TokenKeywordElse,
    TokenKeywordWhile,
    TokenKeywordReturn,
    TokenKeywordImport,
    TokenKeywordFrom,
    TokenKeywordASM,
    TokenKeywordDefine,
    TokenKeywordSafeMode,
    TokenKeywordHeader,
    TokenKeywordContent,
    TokenKeywordAnd,
    TokenKeywordOr,
    TokenKeywordContinue,
    TokenKeywordBreak,
    TokenKeywordFabs,
    TokenKeywordAbs,
    TokenKeywordSaveReg,
    TokenKeywordSaveRegAbs,
    TokenKeywordStruct,

};

string tokenNames[] = {
#ifdef __TEST_DEBUG
    "TokenNumber",
    "TokenAddition",
    "TokenStar",
    "TokenSubstraction",
    "TokenOpenParenthesis",
    "TokenCloseParenthesis",
    "TokenOpenBracket",
    "TokenCloseBracket",
    "TokenOpenCurlyBracket",
    "TokenCloseCurlyBracket",
    "TokenEqual",
    "TokenDoubleEqual",
    "TokenIdentifier",
    "TokenSemicolon",
    "TokenUnknown",
    "TokenSpace",
    "TokenNewline",
    "TokenEndOfFile",
    "TokenSlash",
    "TokenKeyword",
    "TokenString",
    "TokenExternal",
    "TokenComma",
    "TokenLessThan",
    "TokenPlusPlus",
    "TokenMinusMinus",
    "TokenModulo",
    "TokenLessOrEqualThan",
    "TokenMoreThan",
    "TokenMoreOrEqualThan",
    "TokenNotEqual",
    "TokenNot",
    "TokenFunction",
    "TokenUppersand",
    "TokenDiese",
    "TokenLineComment",
    "TokenStartBlockComment",
    "TokenEndBlockComment",
    "TokenNegation",
    "TokenShiftLeft",
    "TokenShiftRight",
    "TokenKeywordVarType",
    "TokenKeywordExternalVar",
    "TokenKeywordFor",
    "TokenKeywordIf",
    "TokenKeywordThen",
    "TokenKeywordElse",
    "TokenKeywordWhile",
    "TokenKeywordReturn",
    "TokenKeywordImport",
    "TokenKeywordFrom",
    "TokenKeywordASM",
    "TokenKeywordDefine",
    "TokenKeywordSafeMode",
    "TokenKeywordHeader",
    "TokenKeywordContent",
    "TokenKeywordAnd",
    "TokenKeywordOr",
    "TokenPower",
    "TokenKeywordContinue",
    "TokenKeywordBreak",
    "TokenKeywordFabs",
    "TokenKeywordAbs",
    "TokenKeywordSaveReg",
    "TokenKeywordSaveRegAbs",
    "TokenKeywordStruct",
    "TokenUserDefinedName",
    "TokenUserDefinedVariable",
    "TokenMember",

#endif
};

typedef struct
{
    string name;
    string content;
    string hh;
} _define;

vector<_define> define_list;

const char * getDefine(string name)
{
    for (vector<_define>::iterator it = define_list.begin();
         it != define_list.end(); ++it)
    {
        if ((*it).name.compare(name) == 0)
        {
            // printf("one rrent %s\n",(*it).content.c_str());
            return (*it).content.c_str();
        }
    }
    return NULL;
}

void deleteDefine()
{
    for (vector<_define>::iterator it = define_list.begin();
         it != define_list.end(); ++it)
    {
        (*it).content.clear();
        (*it).content.shrink_to_fit();
                (*it).name.clear();
        (*it).name.shrink_to_fit();
       
    }
}

#ifdef __CONSOLE_ESP32
const char *tokenFormat[] = {
    termColor.Yellow,   // TokenNumber,
    termColor.BWhite,   // TokenAddition,
    termColor.BWhite,   // TokenStar,
    termColor.BWhite,   // TokenSubstraction,
    termColor.BWhite,   // TokenOpenParenthesis,
    termColor.BWhite,   //  TokenCloseParenthesis,
    termColor.BWhite,   // TokenOpenBracket,
    termColor.BWhite,   // TokenCloseBracket,
    termColor.LMagenta, //  TokenOpenCurlyBracket,
    termColor.LMagenta, //  TokenCloseCurlyBracket,
    termColor.BWhite,   //  TokenEqual,
    termColor.BWhite,   //  TokenDoubleEqual,
    termColor.Cyan,     //  TokenIdentifier,
    termColor.BWhite,   //  TokenSemicolon,
    termColor.BWhite,   // TokenUnknown,
    termColor.BWhite,   //  TokenSpace,
    termColor.BWhite,   //  TokenNewline,
    termColor.BWhite,   // TokenEndOfFile,
    termColor.BWhite,   // TokenSlash,
    termColor.Magenta,  //   TokenKeyword,
    termColor.Orange,   // TokenString,
    termColor.LMagenta, //   TokenExternal,
    termColor.BWhite,   //  TokenComma,
    termColor.BWhite,   //   TokenLessThan,
    termColor.BWhite,   // TokenPlusPlus,
    termColor.BWhite,   // TokenMinusMinus,
    termColor.BWhite,   // TokenModulo,
    termColor.BWhite,   // TokenLessOrEqualThan,
    termColor.BWhite,   // TokenMoreThan,
    termColor.BWhite,   // TokenMoreOrEqualThan,
    termColor.BWhite,   // TokenNotEqual,
    termColor.BWhite,   // TokenNot,
    termColor.Yellow,   // TokenFunction
    termColor.BWhite,   // TokenUppersand
    termColor.BWhite,   // TokenDiese
    termColor.Grey,     // TokenLineComment
    termColor.Grey,     //  TokenStartBlockComment
    termColor.Grey,     // TokenEndBlockComment
    termColor.BWhite,   // TokenNegation
    termColor.BWhite,   // TokenShiftLeft
    termColor.BWhite,   // TokenShiftRight
    termColor.Green,    // KeywordVarType,
    termColor.Green,    // KeywordExternalVar,
    termColor.Magenta,  // KeywordFor,
    termColor.Magenta,  // KeywordIf,
    termColor.Magenta,  // KeywordThen,
    termColor.Magenta,  // KeywordElse,
    termColor.Magenta,  // KeywordWhile,
    termColor.Magenta,  // KeyWordReturn
    termColor.LMagenta, // KeyWordImport
    termColor.LMagenta, // KeyWordFrom
    termColor.LMagenta, // KeyWordASM
    termColor.LMagenta, // KeyWordefine
    termColor.BCyan,    // KeywordsafeMmode
    termColor.BCyan,    // KeywordHeader
    termColor.BCyan,    // KeywordContent
    termColor.Magenta,  // Keywordand,
    termColor.Magenta,  // Keywordor,
    termColor.Magenta,  // TokenPower
    termColor.Magenta,  // Keywordcontinue,
    termColor.Magenta,  // Tokenbreak
    termColor.BWhite,   // Tokenfabs
    termColor.BWhite,   // Tokenabs
    termColor.BCyan,    // Keywordsave_reg
    termColor.BCyan,    // Keywordsave_regabs
    termColor.LMagenta, // TokenKeywordStruct,
    termColor.Cyan,     // TokenUserDefinedName,
    termColor.Green,    // TokenUserDefinedVariable,
    termColor.BWhite,   // TokenMember
};

/*
const char *KeywordTypeFormat[] =
    {
        termColor.Green,    // KeywordVarType,
        termColor.Green,    // KeywordExternalVar,
        termColor.Magenta,  // KeywordFor,
        termColor.Magenta,  // KeywordIf,
        termColor.Magenta,  // KeywordThen,
        termColor.Magenta,  // KeywordElse,
        termColor.Magenta,  // KeywordWhile,
        termColor.Magenta,  // KeyWordReturn
        termColor.LMagenta, // KeyWordImport
        termColor.LMagenta, // KeyWordFrom
        termColor.LMagenta, // KeyWordASM
        termColor.LMagenta, // KeyWordefine
        termColor.BCyan,    // KeywordsafeMmode
        termColor.BCyan,    // KeywordHeader
        termColor.BCyan,    // KeywordContent
};
*/
#endif
typedef struct
{
    // switch to uin8_t
    tokenType type;
    varType *_vartype;

    // varTypeEnum _varType;

    // needs to find a solution for this maybe a pointer to list string et on ne garde que les identifiers ...

    uint8_t line;
    string text;

    // switch to uin8_t unn ligne ne fera pas plus de 256 caracteres
    // uint8_t pos;

    // switch to uin8_t
    // KeywordType _keyword;

    // possible sizee  1+4+4+1+1+4=15 au lieu de 44 ...

} token;

#define EOF_TEXTARRAY 0xFFFF
#define EOF_VARTYPE 0xFF

vector<char *> texts;

class Script
{
public:
    Script()
    {

        position = -1;
    }
    void init()
    {
        it = script.begin();
        position = -1;
    }
    void addContent(char *str)
    {
        script.push_back(str);
    }
    void clear()
    {
        script.clear();
        script.shrink_to_fit();
        position = -1;
    }

    char nextChar()
    {

        if ((*it)[position + 1] != 0)
        {
            position++;
            return (*it)[position];
        }
        else
        {

            if (next(it) == script.end())
            {

                position++;
                return EOF_TEXT;
            }
            else
            {
                it = next(it);
                position = 0;
                return (*it)[position];
            }
        }
    }

    char currentChar()
    {

        if ((*it)[position] != 0)
        {

            return (*it)[position];
        }
        else
        {

            return EOF_TEXT;
        }
    }

    char previousChar()
    {

        if ((position - 1) >= 0)
        {
            position--;
            return (*it)[position];
        }
        else
        {
            if (it != script.begin())
            {

                it--;
                position = 0;
                while ((*it)[position] != 0)
                {
                    position++;
                }
                position--;
                return (*it)[position];
            }
            else
            {
                position = 0;
                return (*it)[0];
            }
        }
    }

    void insert(char *toInsert)
    {

        char *_cur = &(*it)[position];
        char *_next = &(*it)[position + 1];
        it++;
        if (*_next != 0 and *_cur != 0)
            it = script.insert(it, _next);
        it = script.insert(it, toInsert);
        position = -1;
    }

private:
    // string * script;
    int position;

    vector<char *> script;
    vector<char *>::iterator it;
};
class Text
{
public:
    Text()
    {
        //_texts=t;
        _texts.clear();
        _texts.shrink_to_fit();
    }

    int addText(string str)
    {
        char *m = (char *)malloc(str.size() + 1);
        memcpy(m, str.c_str(), str.size());
        m[str.size()] = 0;
        _texts.push_back(m);
        return _texts.size() - 1;
    }
    void replaceText(int pos, string str)
    {
        if (pos > 0 and pos < size())
        {
            free(_texts[pos]);
            char *m = (char *)malloc(str.size() + 1);
            memcpy(m, str.c_str(), str.size());
            m[str.size()] = 0;
            _texts[pos] = m;
        }
    }
    void clear()
    {

        for (int i = 0; i < _texts.size(); i++)
        {
            free(_texts[i]);
        }
        _texts.clear();
        _texts.shrink_to_fit();
    }
    int size()
    {
        return _texts.size();
    }
    char *getText(int pos)
    {
        if (pos >= 0 and pos < _texts.size())
        {
            return _texts[pos];
        }
        else
        {
            return cc;
        }
    }

    void pop()
    {
        if (size() > 0)
        {
            free(_texts.back());
            _texts.pop_back();
            _texts.shrink_to_fit();
        }
    }

private:
    vector<char *> _texts;
    char cc[1] = {'\0'};
};

Text all_text = Text();
class Token
{

public:
    Token()
    {
        type=(int)TokenUnknown;
    }

    Token(tokenType h)
    {
        type = (int)h;
    }
    Token(tokenType _type, int __vartype, int _line)
    {
        type = (int)_type;
        _vartype = __vartype;
        line = _line;
    }
    Token(tokenType _type, int __vartype)
    {
        type = (int)_type;
        _vartype = __vartype;
    }
    tokenType getType()
    {
        return (tokenType)type;
    }
    void setType(tokenType _type)
    {
        type = (int)_type;
    }
    void clean()
    {
        line = 0;
        type = 0;
        _vartype = EOF_VARTYPE;

        textref = EOF_TEXTARRAY;
    }
    void addText(string t)
    {
        textref = all_text.addText(t);
    }
    char *getText()
    {
        return all_text.getText(textref);
    }
    varType *getVarType()
    {
        if (_vartype != EOF_VARTYPE)
        {
            return &_varTypes[_vartype];
        }
        else
        {
            return NULL;
        }
    }
    uint16_t line = 0;
    uint8_t type = 0;
    uint8_t _vartype = EOF_VARTYPE;

    uint16_t textref = EOF_TEXTARRAY;
};

vector<Token> _list_of_tokens;
#ifdef __FULL_TOKEN
#define __DEPTH 0
#else
#define __DEPTH 5
#endif
int tokenizer(Script *script, bool update, bool increae_line,
               int nbMaxTokenToRead);
class Tokens
{
public:
    void init()
    {
#ifdef __FULL_TOKEN

        position = 0;
#endif
    }
    Tokens()
    {
        // _tokens = &_list_of_tokens;
        clear();
        init();
    }

    void clear()
    {
        _tokens.clear();

        _tokens.shrink_to_fit();

#ifdef __FULL_TOKEN
        position = 0;
#endif
    }
    int size()
    {
        return _tokens.size();
    }
    void tokenize(Script *script, bool update, bool increae_line, int nbToken)
    {
        _script = script;
        clear();
        tokenizer(script, true, increae_line, nbToken);
        // list_of_token.push_back(token());
      // Serial.printf("token read %d\n", tokenizer(script, true, increae_line, nbToken));
    }
    void push(Token t)
    {
        _tokens.push_back(t);
    }
    void pop_back()
    {
        if (_tokens.size() > 0)
        {
            //	Token t = _tokens.back ();

            _tokens.pop_back();

            //	return t;
        }
        //	else
        //	  return end_token;
    }
    Token *getTokenAtPos(int pos)
    {
        if (pos >= 0 and pos < _tokens.size())
        {
            return &(_tokens[pos]);
        }
        else
            return &end_token;
    }
    Token *current()
    {
#ifdef __FULL_TOKEN
        return getTokenAtPos(position);
#else
        return getTokenAtPos(__DEPTH);
#endif
    }
    Token *next()
    {
#ifdef __FULL_TOKEN
        
        //position++;
tokenizer(_script, false, true, 1);
    position++;

    
       
        return getTokenAtPos(position);
#else
        _tokens.erase(_tokens.begin());
        _tokens.shrink_to_fit();

        tokenizer(_script, false, true, 1);
        return getTokenAtPos(__DEPTH);
#endif
    }
    Token *prev()
    {
#ifdef __FULL_TOKEN
        return getTokenAtPos(position - 1);
#else
        _tokens.insert(_tokens.begin(), Token());
        return getTokenAtPos(__DEPTH);
#endif
    }
    Token *peek(int index)
    {

#ifdef __FULL_TOKEN
        if (index + position < _tokens.size() && index + position >= 0)
        {
            return getTokenAtPos(index + position);
        }
        else
        {
            tokenizer(_script, false, true,
                      -_tokens.size() + index + position + 1);

            return getTokenAtPos(index + position);
        }
#else
        if (index + __DEPTH < _tokens.size() && index + __DEPTH >= 0)
        {
            return getTokenAtPos(index + __DEPTH);
        }
        else
        {
            tokenizer(_script, false, true,
                      -_tokens.size() + index + __DEPTH + 1);

            return getTokenAtPos(index + __DEPTH);
        }
#endif
    }
    Token back()
    {
        if(_tokens.size()>0)
        return _tokens.back();
        else
        return Token();
    }

    bool Match(tokenType tt)
    {
        Token *g = current();
        if (g->getType() == tt)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Match(tokenType tt, int index)
    {
        Token *g = peek(index);
        if (g->getType() == tt)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    vector<Token> _tokens;
    Token end_token = Token(TokenEndOfFile);
    Script *_script;
#ifdef __FULL_TOKEN
    int position;
#endif
};

Tokens _tks = Tokens();
void displaytoken(token t)
{
    // printf("%s\t%d\t%f\t%s%d %d\t%s\n", tokenNames[t.type].c_str(), t.int_value, t.float_value, t.text.c_str(), t.line, //t.pos, keywordTypeNames[t._keyword].c_str());
}

Token transNumber(string str)
{
    // t;
    // t.float_value=0;
    // t.int_value=0;
    if (str.find(".") != string::npos)
    {

        Token t = Token(TokenNumber, (int)__float__);
        t.addText(str);
        return t;
    }
    else
    {

        Token t = Token(TokenNumber, (int)__uint32_t__);
        t.addText(str);
        return t;
    }
}

int isKeyword(string str)
{
    for (int i = 0; i < nb_keywords; i++)
    {
        if (keyword_array[i].compare(str) == 0)
        {
            return i;
        }
    }

    return -1;
}

int isUserDefined(string str)
{
    for (int i = 0; i < userDefinedVarTypeNames.size(); i++)
    {
        if (userDefinedVarTypeNames[i].compare(str) == 0)
        {
            return i;
        }
    }

    return -1;
}

bool isIna_zA_Z_(unsigned char c)
{
    if (c >= 97 && c <= 122)
    {
        return true;
    }
    if (c >= 65 && c <= 90)
    {
        return true;
    }
    if (c == '_')
    {
        return true;
    }
    return false;
}

bool isIna_zA_Z_0_9(unsigned char c)
{
    if (c >= 97 && c <= 122)
    {
        return true;
    }
    if (c >= 65 && c <= 90)
    {
        return true;
    }
    if (c == '_')
    {
        return true;
    }
    if (c >= 48 && c <= 57)
    {
        return true;
    }
    return false;
}

bool isIn0_9(unsigned char c)
{
    if (c >= 48 && c <= 57)
    {
        return true;
    }
    return false;
}

bool isIn0_9_x_b(unsigned char c)
{
    if (c >= 48 && c <= 57)
    {
        return true;
    }
    if (c == 'b')
    {
        return true;
    }
    if (c >= 'A' && c <= 'F')
    {
        return true;
    }
    if (c >= 'a' && c <= 'f')
    {
        return true;
    }
    if (c == 'x')
    {
        return true;
    }
    if (c == '.')
    {
        return true;
    }
    return false;
}

bool _for_display = false;

int _token_line;
int _sav_token_line;
list<token>::iterator _index_token;
int tokenizer(Script *script, bool update, bool increae_line,
               int nbMaxTokenToRead)
{

    // list<token> list_of_token;
    // int line = 1;
    
  
    Token t;
    int pos = 0;
    char c;
    char c2;
     _define newdef;
    string v;
    if (update)
    {
        userDefinedVarTypeNames.clear();
        userDefinedVarTypeNames.shrink_to_fit();
        all_text.clear();
        _tks.clear();
        for (int i = 0; i < __DEPTH; i++)
        {
            _tks.push(Token());
        }
        _token_line = 1;

        define_list.clear();

        __isBlockComment = false;
        // _for_display= true;
    }
    else
    {
        script->previousChar();
    }
    int nbReadToken = 0;
    while (script->nextChar() != EOF_TEXT and nbReadToken < nbMaxTokenToRead)
    {
        // printf(" nb read :%d:\n",nbReadToken);
        t.clean();
        v.clear();
        pos++;

        c = script->currentChar();
        // printf("line : %d pos:%d char:%c token size:%d %d\n",line,pos,c,list_of_token.size(),heap_caps_get_free_size(MALLOC_CAP_8BIT));
        if (c == '=')
        {
            c2 = script->nextChar();
            if (c2 == '=')
            {
                t = Token(TokenDoubleEqual, EOF_VARTYPE);
                // t._vartype = NULL;
                // t.type = TokenDoubleEqual;
                if (_for_display)
                    t.addText("==");
                t.line = _token_line;
                // t.pos = pos;
                _tks.push(t);
                nbReadToken++;

                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                //  t._vartype = NULL;
                //  t.type = TokenEqual;
                //  t.line = _token_line;
                t = Token(TokenEqual, EOF_VARTYPE, _token_line);
                // t.pos = pos;
                if (_for_display)
                    t.addText("=");
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }
        if (c == '<')
        {
            c2 = script->nextChar();
            if (c2 == '=')
            {
                // token t;
                // t._vartype = NULL;
                // t.type = TokenLessOrEqualThan;

                t = Token(TokenLessOrEqualThan, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("<=");
                // t.line = _token_line;
                // t.pos = pos;
                _tks.push(t);
                nbReadToken++;
                continue;
            }
            else if (c2 == '<')
            {
                // token t;
                // t._vartype = NULL;
                //  t.type = TokenShiftLeft;
                t = Token(TokenShiftLeft, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("<<");
                // t.line = _token_line;
                //  t.pos = pos;
                //  _tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                // t._vartype = NULL;
                // t.type = TokenLessThan;
                // t.line = _token_line;

                t = Token(TokenShiftLeft, EOF_VARTYPE, _token_line);
                // t.pos = pos;
                if (_for_display)
                    t.addText("<");
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }
        if (c == '>')
        {
            c2 = script->nextChar();
            if (c2 == '=')
            {
                //  token t;
                //  t._vartype = NULL;
                // t.type = TokenMoreOrEqualThan;
                t = Token(TokenMoreOrEqualThan, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText(">=");
                // t.line = _token_line;
                // t.pos = pos;
                //_tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
            else if (c2 == '>')
            {
                // token t;
                // t._vartype = NULL;
                // t.type = TokenShiftRight;
                t = Token(TokenShiftRight, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText(">>");
                // t.line = _token_line;
                // t.pos = pos;
                // _tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                // t._vartype = NULL;
                // t.type = TokenMoreThan;
                // t.line = _token_line;
                t = Token(TokenMoreThan, EOF_VARTYPE, _token_line);
                // t.pos = pos;
                if (_for_display)
                    t.addText(">");
                // _tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }
        if (c == '!')
        {
            c2 = script->nextChar();
            if (c2 == '=')
            {
                // token t;
                // t._vartype = NULL;
                // t.type = TokenNotEqual;
                t = Token(TokenNotEqual, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("!=");
                // t.line = _token_line;
                // t.pos = pos;
                //_tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
            else
            {
                script->previousChar();
                //  token t;
                //  t._vartype = NULL;
                // t.type = TokenNot;
                // t.line = _token_line;
                t = Token(TokenNot, EOF_VARTYPE, _token_line);
                // t.pos = pos;
                if (_for_display)
                    t.addText("!");
                // _tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }
        if (c == '+')
        {
            c2 = script->nextChar();
            if (c2 == '+')
            {
                // token t;
                // t._vartype = NULL;
                // t.type = TokenPlusPlus;
                t = Token(TokenPlusPlus, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("++");
                // t.line = _token_line;
                // t.pos = pos;
                // _tks.push(t);
                // nbReadToken++;
                _tks.push(t);
                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                //  t._vartype = NULL;
                // t.type = TokenAddition;
                t = Token(TokenAddition, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("+");
                // t.line = _token_line;
                // t.pos = pos;
                // _tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }

        if (isIna_zA_Z_(c))
        {
            v;
            int newpos = pos;
            while (isIna_zA_Z_0_9(c))
            {
                v += c;
                newpos++;
                c = script->nextChar();
            }
            script->previousChar(); // on revient un caractere en arriere
            // token t;
            // Token t;
            //  t._vartype=NULL;
            t.line = _token_line;
            // t.pos = pos;
            if (isKeyword(v) > -1)
            {
                // printf("keyword;%s\n",v.c_str());
                // t.type = TokenKeyword;
                t.type = (int)__keywordTypes[isKeyword(v)];
                if (isKeyword(v) < nb_typeVariables)
                    t._vartype = isKeyword(v);
                if (t.getType() == TokenKeywordExternalVar)
                {
                    t.type = (int)TokenExternal;
                }
                if ((t.getType() == TokenKeywordImport or t.getType() ==
                                                              TokenKeywordDefine) &&
                    !_for_display)
                {

                    nbReadToken--;
                }
            }
            else if (isUserDefined(v) > -1)
            {
                t.type = (int)TokenUserDefinedVariable;
                t._vartype = (int)__userDefined__;
            }
            else
            {
                t.type = (int)TokenIdentifier;

                if (_tks.size() >= __DEPTH)
                {

                    Token prev = _tks.back();
                    if (prev.getType() == TokenKeywordImport && !_for_display)
                    {

                        // script->insert(import);

                        _sav_token_line = _token_line;
                        nbReadToken--;
                        if (findLibFunction(v) > -1)
                        {

                            _tks.pop_back();

                            all_text.pop();

                            // list_of_token.pop_back();
                            //  add_on.push_back(findLibFunction(v));
                            script->insert((char *)((*_stdlib[findLibFunction(v)]).c_str()));

                            // script->previousChar ();

                            continue;
                        }
                    }
                    else if (prev.getType() == TokenKeywordDefine && !_for_display)
                    {
                        _tks.pop_back();
                        all_text.pop();
                        // nbReadToken--;
                       
                        newdef.name = v;
                        newdef.content = "";

                        c2 = script->nextChar();
                        // c2 = script->nextChar();
                        while (c2 != '\n' and c2 != EOF_TEXT)
                        {
                            newdef.content = newdef.content + c2;
                            c2 = script->nextChar();
                        }
                        define_list.push_back(newdef);
                        if (increae_line)
                            _token_line++;
                        // script->previousChar();
                        continue;
                    }
                    else if (prev.getType() == TokenKeywordStruct && !_for_display)
                    {
                        userDefinedVarTypeNames.push_back(v);
                        t.type = (int)TokenUserDefinedName;
                        // continue;
                    }
                }
                if (!_for_display) // on ne remplace pas lorsque l'on display
                {
                    if (getDefine(v) != NULL)
                    {
                 
                        script->insert((char *)(getDefine(v)));
                        script->nextChar();
                        // nbReadToken--;
                        continue;
                    }
                }
            }
            pos = newpos - 1;
            if (t.type == (int)TokenIdentifier or _for_display)
                t.addText(v);
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }

        if (isIn0_9(c))
        {
            // //printf("on a %c\n",c);
            v = "";
            int newpos = pos;
            while (isIn0_9_x_b(c))
            {
                v += c;
                c = script->nextChar();
                newpos++;
            }
            script->previousChar(); // on revient un caractere en arriere
            t = transNumber(v);
            //  t._vartype=NULL;
            t.line = _token_line;
            // t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            pos = newpos - 1;
            continue;
        }
        if (c == ';')
        {
            // token t;
            // t.type = TokenSemicolon;
            // t._vartype = NULL;
            t = Token(TokenSemicolon, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText(";");
            // t.line = _token_line;
            // t.pos = pos;
            // _tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '\t')
        {
            // token t;
            // t.type = TokenSpace;
            // t._vartype = NULL;
            t = Token(TokenSpace, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("\t");
            // t.line = _token_line;
            //  t.pos = pos;
            if (_for_display)
            {
                //_tks.push(t);
                // nbReadToken++;
                _tks.push(t);
            }
            continue;
        }
        if (c == '&')
        {
            // token t;
            // t.type = TokenUppersand;
            // t._vartype = NULL;
            t = Token(TokenUppersand, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("&");
            // t.line = _token_line;
            //  t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '#')
        {
            // token t;
            // t.type = TokenDiese;
            // t._vartype = NULL;
            t = Token(TokenDiese, EOF_VARTYPE, _token_line);
            if (_for_display)
            {
                t.addText("#");
                // _tks.push(t);
                _tks.push(t);
                nbReadToken++;
            }
            // t.line = _token_line;
            //  t.pos = pos;

            continue;
        }
        if (c == '(')
        {
            // token t;
            // t.type = TokenOpenParenthesis;
            // t._vartype = NULL;
            t = Token(TokenOpenParenthesis, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("(");
            // t.line = _token_line;
            // t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '%')
        {
            // token t;
            //  t.type = TokenModulo;
            // t._vartype = NULL;
            t = Token(TokenModulo, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("%");
            // t.line = _token_line;
            // t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == ')')
        {
            //  token t;
            // t.type = TokenCloseParenthesis;
            // t._vartype = NULL;
            t = Token(TokenCloseParenthesis, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText(")");
            // t.line = _token_line;
            // t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '{')
        {
            // token t;
            // t.type = TokenOpenCurlyBracket;
            // t._vartype = NULL;
            t = Token(TokenOpenCurlyBracket, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("{");
            // t.line = _token_line;
            //  t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '}')
        {
            // token t;
            // t.type = TokenCloseCurlyBracket;
            // t._vartype = NULL;
            t = Token(TokenCloseCurlyBracket, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("}");
            // t.line = _token_line;
            // t.pos = pos;
            // _tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '[')
        {
            //  token t;
            // t.type = TokenOpenBracket;
            // t._vartype = NULL;
            t = Token(TokenOpenBracket, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("[");
            // t.line = _token_line;
            // t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == ']')
        {
            // token t;
            // t.type = TokenCloseBracket;
            // t._vartype = NULL;
            t = Token(TokenCloseBracket, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("]");
            t.line = _token_line;
            // t.pos = pos;
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '/')
        {
            char c2 = script->nextChar();
            if (c2 == '/')
            {
                // Token t;
                t._vartype = EOF_VARTYPE;
                t.type = (int)TokenLineComment;
                string str = "//";

                c2 = script->nextChar();
                while (c2 != '\n' and c2 != EOF_TEXT)
                {
                    str = string_format("%s%c", t.getText(), c2);
                    c2 = script->nextChar();
                }
                if (_for_display)
                    t.addText(str);
                t.line = _token_line;
                // t.pos = pos;
                if (increae_line)
                    _token_line++;
                if (_for_display)
                {
                    _tks.push(t);
                    nbReadToken++;
                }
                continue;
            }
            else if (c2 == '*')
            {
                // Token t;
                t._vartype = EOF_VARTYPE;
                t.type = (int)TokenLineComment;

                string str = "/*";
                c = script->nextChar();
                c2 = script->nextChar();
                while ((c != '*' or c2 != '/') and c2 != EOF_TEXT and c != EOF_TEXT) // stop when (c=* and c2=/) or c=0 or c2=0
                {
                    if (_for_display)
                        str = string_format("%s%c", t.getText(), c);
                    c = c2;
                    c2 = script->nextChar();
                }
                if (_for_display)
                    t.addText(str);
                t.line = _token_line;
                // t.pos = pos;
                if (_for_display)
                {
                    _tks.push(t);
                    nbReadToken++;
                }
                continue;
            }
            else
            {
                script->previousChar();
                // Token t;
                t.type = (int)TokenSlash;
                t._vartype = EOF_VARTYPE;
                if (_for_display)
                    t.addText("/");
                t.line = _token_line;
                // t.pos = pos;
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }
        if (c == '-')
        {
            // Token t;
            t.type = (int)TokenSubstraction;
            t._vartype = EOF_VARTYPE;
            if (_for_display)
                t.addText("-");
            t.line = _token_line;
            // t.pos = pos;
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == ' ')
        {
            // Token t;
            t.line = _token_line;
            // t.pos = pos;
            string str = "";

            while (c == ' ')
            {
                c = script->nextChar();
                pos++;
                str = str + " ";
            }
            if (_for_display)
                t.addText(str);
            script->previousChar(); // on revient un caractere en arriere
            pos--;
            t.type = TokenSpace;
            // t.addText(" ";
            if (_for_display)
            {
                _tks.push(t);
                nbReadToken++;
            }
            continue;
        }
        if (c == '"')
        {
            v = "";
            // Token t;
            t._vartype = EOF_VARTYPE;
            t.line = _token_line;
            // t.pos = pos;
            v += c;
            c = script->nextChar();
            pos++;
            while (c != '"' && c != EOF_TEXT)
            {
                v += c;
                c = script->nextChar();
                pos++;
            }
            // script->previousChar(); //on revient un caractere en arriere
            // pos--;
            v += c;
            t.type = (int)TokenString;
            t.addText(v);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '\n')
        {
            // Token t;
            t.type = (int)TokenNewline;
            if (_for_display)
                t.addText("\r\n");
            t.line = _token_line;
            // t.pos = pos;
            if (increae_line)
                _token_line++;
            pos = 0;
            if (_for_display)
                _tks.push(t);
            continue;
        }
        if (c == '?')
        {
            // Token t;
            t.type = (int)TokenUnknown;
            if (_for_display)
                t.addText("?");
            t.line = _token_line;
            // t.pos = pos;
            //_token_line++;
            //  pos = 0;
            if (_for_display)
                _tks.push(t);
            continue;
        }
        if (c == '.')
        {
            // Token t;
            t.type = (int)TokenMember;
            if (_for_display)
                t.addText(".");
            t.line = _token_line;
            // t.pos = pos;
            //_token_line++;
            //  pos = 0;
            // if (_for_display)
            _tks.push(t);
            // nbReadToken++;
            continue;
        }
        if (c == '^')
        {
            // Token t;
            t.type = (int)TokenPower;
            if (_for_display)
                t.addText("^");
            t.line = _token_line;
            // t.pos = pos;
            //_token_line++;
            //  pos = 0;
            // if (_for_display)
            _tks.push(t);
            // nbReadToken++;
            continue;
        }
        if (c == '@')
        {
            // Token t;
            t.type = (int)TokenUnknown;
            if (_for_display)
                t.addText("@");
            _token_line = _sav_token_line;
            if (_for_display)
                _tks.push(t);
        }
        if (c == '\'')
        {
            // Token t;
            t.type = (int)TokenUnknown;
            if (_for_display)
                t.addText("\'");
            t.line = _token_line;
            // t.pos = pos;
            //  _token_line++;
            //  pos = 0;
            if (_for_display)
                _tks.push(t);
            continue;
        }
        if (c == ':')
        {
            // Token t;
            t.type = (int)TokenUnknown;
            if (_for_display)
                t.addText(":");
            t.line = _token_line;
            // t.pos = pos;
            //_token_line++;
            //  pos = 0;
            if (_for_display)
                _tks.push(t);
            continue;
        }
        if (c == '*')
        {
            // Token t;
            t._vartype = EOF_VARTYPE;
            t.type = (int)TokenStar;
            if (_for_display)
                t.addText("*");
            t.line = _token_line;
            // t.pos = pos;
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == ',')
        {
            // Token t;
            t._vartype = EOF_VARTYPE;
            t.type = (int)TokenComma;
            if (_for_display)
                t.addText(",");
            t.line = _token_line;
            // t.pos = pos;
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        // printf("Error invalid character |%c|\n", c);
    }
    //  token t;
    //  t.type = TokenEndOfFile;
    //  t.addText("";
    //  t.line = _token_line;
    // t.pos = pos + 1;
    if (script->currentChar() == EOF_TEXT)
    {
        if (_tks.back().getType() != TokenEndOfFile)
        { // printf("oinr termine");
            // token t;
            //  t.type = TokenEndOfFile;
            //  t.addText("";
            //   t.line = _token_line;

            t = Token(TokenEndOfFile, EOF_VARTYPE, _token_line);
            // _tks.push(t);
            _tks.push(t);
        }
    }
    // return list_of_token;
    return nbReadToken-1;
}

/*
class Tokens
{
public:
    Tokens()
    {
        position = 0;
        _tokens = &list_of_token;
        //_script=script;
    }
    int size()
    {
        return _tokens->size();
    }
    void tokenize(Script *script, bool update, bool increae_line, int nbToken)
    {
        _script = script;
        list_of_token.clear();
        // list_of_token.push_back(token());
        tokenizer(script, true, increae_line, nbToken);
    }

    token *getTokenAtPos(int pos)
    {
        int i = 0;
        if (pos >= _tokens->size() || pos < 0)
        {
            return &end_token;
        }
        for (list<token>::iterator it = _tokens->begin(); it != _tokens->end(); ++it)
        {
            if (i == pos)
            {

                return &*it;
            }
            i++;
        }
        return &end_token;
    }
    token *current()
    {
        return getTokenAtPos(__DEPTH);
    }
    token *next()
    {

        _tokens->pop_front();
        tokenizer(_script, false, true, 1);
        return getTokenAtPos(__DEPTH);
    }
    token *prev()
    {

       list_of_token.push_front(token());
        return getTokenAtPos(__DEPTH);
    }
    token *peek(int index)
    {
        if (index + __DEPTH < _tokens->size() && position + __DEPTH >= 0)
        {
            return getTokenAtPos(index + __DEPTH);
        }
        else
        {
              tokenizer(_script, false, true,  _tokens->size()-index-__DEPTH+1);

            return getTokenAtPos(index + __DEPTH);
        }
    }
    void init()
    {
        position = 0;
    }

    bool Match(tokenType tt)
    {
        token *g = current();
        if (g->type == tt)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Match(tokenType tt, int index)
    {
        token *g = peek(index);
        if (g->type == tt)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void clear()
    {
        _tokens->clear();
    }
    int position;

private:
    list<token> *_tokens;
    token end_token = {.type = TokenEndOfFile};
    Script *_script;
};

*/

#ifdef __CONSOLE_ESP32
list<const char *> _parenthesis;
list<const char *> _curlybracket;
list<const char *> _bracket;
int _prevparenthesis;
int _prevcurlybracket;
int _prevbracket;
#define _NB_COLORS 3
const char *_colors[_NB_COLORS] = {

    termColor.Magenta,
    termColor.LBlue,
    termColor.Yellow,

};

void formatInit()
{
    _parenthesis.clear();
    _curlybracket.clear();
    _bracket.clear();
    _prevparenthesis = 0;
    _prevcurlybracket = 0;
    _prevbracket = 0;
}

void formatNewLine()
{
    _prevparenthesis = 0;
    _prevcurlybracket = 0;
    _prevbracket = 0;
}

string
formatLine(string str)
{
    // Serial.printf("streing:%s\r\n",str.c_str());
    // _parent.clear();
    _for_display = true;
    Script s(&str);
    _tks.tokenize(&s, true, true, 1);
    // _tks.init();
    string res = "";
    for (int i = 0; i < _prevparenthesis; i++)
    {
        _parenthesis.pop_back();
    }
    for (int i = 0; i < _prevbracket; i++)
    {
        _bracket.pop_back();
    }
    for (int i = 0; i < _prevcurlybracket; i++)
    {
        _curlybracket.pop_back();
    }
    _prevparenthesis = 0;
    _prevcurlybracket = 0;
    _prevbracket = 0;

    while (_tks.current()->getType() != TokenEndOfFile) // for (int i = 0; i < _tks.size(); i++)
    {
        token tk = *_tks.current();
        //    Serial.printf("token %s\r\n",tk.text.c_str());
        /* if (tk.type == TokenOpenCurlyBracket)
           {

           // char *color= (char *)_colors[_curlybracket.size()%_NB_COLORS];
           res = res + string_format("%s%s", _colors[_curlybracket.size() % _NB_COLORS], tk.text.c_str());
           _curlybracket.push_back(_colors[_curlybracket.size() % _NB_COLORS]);
           _prevcurlybracket++;
           }
           else if (tk.type == TokenCloseCurlyBracket)
           {

           if (_curlybracket.size() == 0)
           {
           res = res + string_format("%s%s", "\u001b[38;5;196m", tk.text.c_str());
           }
           else
           {
           // char * color=_curlybracket.back();
           _prevcurlybracket--;
           res = res + string_format("%s%s", _curlybracket.back(), tk.text.c_str());
           _curlybracket.pop_back();
           }
           } */
        if (tk.getType() == TokenOpenParenthesis)
        {
            _prevparenthesis++;
            res =
                res + string_format("%s%s",
                                    _colors[(_parenthesis.size() +
                                             2) %
                                            _NB_COLORS],
                                    tk.getText());
            _parenthesis.push_back(_colors
                                       [(_parenthesis.size() + 2) % _NB_COLORS]);
        }
        else if (tk.getType() == TokenCloseParenthesis)
        {

            if (_parenthesis.size() == 0)
            {
                res =
                    res + string_format("%s%s", "\u001b[38;5;196m",
                                        tk.getText());
            }
            else
            {
                _prevparenthesis--;
                res =
                    res + string_format("%s%s", _parenthesis.back(),
                                        tk.getText());
                _parenthesis.pop_back();
            }
        }
        else if (tk.getType() == TokenOpenBracket)
        {
            _prevbracket++;
            res =
                res + string_format("%s%s",
                                    _colors[_bracket.size() % _NB_COLORS],
                                    tk.getText());
            _bracket.push_back(_colors[_bracket.size() % _NB_COLORS]);
        }
        else if (tk.getType() == TokenCloseBracket)
        {

            if (_bracket.size() == 0)
            {
                res =
                    res + string_format("%s%s", "\u001b[38;5;196m",
                                        tk.getText());
            }
            else
            {
                _prevbracket--;
                res =
                    res + string_format("%s%s", _bracket.back(), tk.getText());
                _bracket.pop_back();
            }
        }
        /*
           else if (tk.type == TokenKeyword)
           {
           res = res + string_format("%s%s", KeywordTypeFormat[tk._keyword], tk.text.c_str());
           } */
        else
        {
            token tkn = *_tks.peek(1);
            if (tk.getType() == TokenIdentifier && tkn.getType() == TokenOpenParenthesis)
            {
                res =
                    res + string_format("%s%s", tokenFormat[TokenFunction],
                                        tk.getText());
            }
            else
            {

                res =
                    res + string_format("%s%s", tokenFormat[tk.type],
                                        tk.getText());
            }
            // _tks.prev();
        }
        _tks.next();
    }

    _tks.clear();
    //  _parent.clear();

    _for_display = false;
    return res;
}

class __INIT_TOKEN
{
public:
    __INIT_TOKEN()
    {
        LedOS.addHightLightinf("sc", formatLine, formatInit, formatNewLine);
    }
};
__INIT_TOKEN _init_token;
#endif
