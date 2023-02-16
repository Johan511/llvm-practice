#include "../includes/parser.h"
#include <iostream>
const char *FILENAME = "/home/hhn/cs/comp-des/src/.toy";

int main()
{
    std::ifstream file;
    file.open(FILENAME,  std::ios::out);


    Parser parser(&file);
    parser.main_loop();
}