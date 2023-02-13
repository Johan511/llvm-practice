#include "../includes/lexer.h"
const std::vector<std::string> Lexer_Checks::commands = {"def", "extern"};

using namespace std;

inline bool Lexer_Checks::is_space(char c)
{
    if (c == '\t' || c == '\n' || c == ' ')
        return true;
    return false;
}

inline bool Lexer_Checks::is_alpha(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    return false;
}

inline bool Lexer_Checks::is_num(char c)
{
    if ((c >= '0' && c <= '9'))
        return true;
    return false;
}

inline bool Lexer_Checks::is_alphanum(char c)
{
    if (is_num(c) || is_alpha(c))
        return true;
    return false;
}

bool Lexer_Checks::is_command(const std::string &tok)
{
    for (std::string const &com : Lexer_Checks::commands)
    {
        if (com == tok)
            return true;
    }
    return false;
}

Token Lexer::get_tok()
{
    this->IdentifierString = "";
    while (Lexer_Checks::is_space(lastChar) == true)
    {
        lastChar = file->get();
    }

    if (lastChar == EOF)
        return tok_eof;

    if (Lexer_Checks::is_alpha(lastChar) == true)
    {
        do
        {
            IdentifierString += lastChar;
            lastChar = file->get();
        } while (Lexer_Checks::is_alphanum(lastChar) == true);

        if (IdentifierString == "def")
            return tok_def;
        if (IdentifierString == "extern")
            return tok_extern;
        return tok_identifier;
    }

    if (Lexer_Checks::is_num(lastChar) == true)
    {
        do
        {
            IdentifierString += lastChar;
            lastChar = file->get();
        } while (Lexer_Checks::is_num(lastChar) == true);

        NumVal = strtod(IdentifierString.c_str(), 0);
        return tok_num;
    }

    if (lastChar == EOF)
        return tok_eof;

    // unidentified char
    // 2+3 => we need to go to 3 after encountering +
    // else we will constantly get +
    int thisChar = lastChar;
    lastChar = file->get();
    return thisChar;
}