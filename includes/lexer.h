#include <string>
#include <fstream>
#include <vector>

#pragma once

enum TokenFlag
{
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_num = -5
};

// union Token
// {
//     TokenFlag tokFlag;
//     int unidentified_tok;
// };

typedef int Token;

class Lexer_Checks
{
    static const char ONELINE_COMMENT = '#';
    static const std::vector<std::string> commands;

    friend class Lexer;

    static inline bool is_space(char c);
    static inline bool is_alpha(char c);
    static inline bool is_num(char c);
    static inline bool is_alphanum(char c);

    static bool is_command(const std::string &tok);
};

class Lexer
{
    std::ifstream *file;

    double NumVal = 0;
    std::string IdentifierString = "";
    char lastChar = ' ';

public:
    Token get_tok();
    Lexer(std::ifstream *file) : file(file){};

    std::string get_IdentifierString() { return IdentifierString; }
    double get_NumVal() { return NumVal; }
};
