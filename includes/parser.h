#include "./ast.h"
#include "./lexer.h"
#include <map>

class Parser
{
public:
    Parser(std::ifstream *file);
    ~Parser();
    void main_loop();

private:
    static std::map<char, int> binOpPrecedence;

    Lexer *lex;
    int curTok;

    int get_next_tok();

    std::unique_ptr<ExprAST> log_error_expr(const char *errorStr);
    std::unique_ptr<PrototypeAST> log_error_proto(const char *errorStr);

    std::unique_ptr<ExprAST> parse_num_expr();
    std::unique_ptr<ExprAST> parse_parenthesis_expr();
    std::unique_ptr<ExprAST> parse_identifier_expr();

    int get_tok_precedence();

    std::unique_ptr<ExprAST> parse_binop_rhs(int exprPrecedence, std::unique_ptr<ExprAST> LHS);

    std::unique_ptr<ExprAST> parse_primary();
    std::unique_ptr<ExprAST> parse_expression();
    std::unique_ptr<PrototypeAST> parse_prototype();
    std::unique_ptr<FunctionAST> parse_definition();
    std::unique_ptr<PrototypeAST> parse_extern();
    std::unique_ptr<FunctionAST> parse_top_level();


    void handle_definition();
    void handle_extern();
    void handle_top_level();
};
