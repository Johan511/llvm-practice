#include "../includes/lexer.h"

const char *FILENAME = ".toy";

int main()
{
    std::ifstream file;
    file.open(FILENAME,  std::ios::out);

    Lexer lexy(&file);
    while (lexy.get_tok() != tok_eof)
    {
    }
}