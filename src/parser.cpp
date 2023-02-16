#include "../includes/parser.h"

std::map<char, int>
    Parser::binOpPrecedence = {{'<', 10}, {'+', 20}, {'-', 30}, {'*', 40}};

Parser::Parser(std::ifstream *file)
{
    lex = new Lexer(file);
}

Parser::~Parser()
{
    delete lex;
}

int Parser::get_next_tok()
{
    return curTok = lex->get_tok();
}

std::unique_ptr<ExprAST> Parser::log_error_expr(const char *errorStr)
{
    fprintf(stderr, "Error Expr: %s", errorStr);
    return NULL;
}

std::unique_ptr<PrototypeAST> Parser::log_error_proto(const char *errorStr)
{
    fprintf(stderr, "Error Proto: %s", errorStr);
    return NULL;
}

std::unique_ptr<ExprAST> Parser::parse_num_expr()
{
    std::unique_ptr<NumExprAST> result = std::make_unique<NumExprAST>(lex->NumVal);
    get_next_tok();
    return std::move(result);
}

std::unique_ptr<ExprAST> Parser::parse_parenthesis_expr()
{
    get_next_tok(); // eat the "("

    auto V = parse_expression();
    if (!V)
        return NULL;

    if (curTok != ')')
        return log_error_expr("Expected \")\"");

    get_next_tok(); // eat the ")"

    return V;
}

std::unique_ptr<ExprAST> Parser::parse_identifier_expr()
{
    std::string idName = lex->IdentifierString;

    get_next_tok();

    if (curTok != '(')
        return std::make_unique<VarExprAST>(idName); // simple variable like x

    // function like foo(a, b, c);
    get_next_tok(); // eat the "("
    std::vector<std::unique_ptr<ExprAST>> args;

    if (curTok != ')')
    {
        while (true)
        {
            if (auto arg = parse_expression())
                args.push_back(std::move(arg));
            else
                return NULL;

            if (curTok == ')')
                break;

            if (curTok != ',')
                return log_error_expr("Expected \",\" or \")\"");

            get_next_tok();
        }
    }

    get_next_tok(); // eat the ")"

    return std::make_unique<CallExprAST>(idName, std::move(args));
}

std::unique_ptr<ExprAST> Parser::parse_primary()
{
    switch (curTok)
    {
    case tok_identifier:
        return parse_identifier_expr();
    case tok_num:
        return parse_num_expr();
    case '(':
        return parse_parenthesis_expr();
    default:
        return log_error_expr("Unidentified token");
    }
}

int Parser::get_tok_precedence()
{
    if (!isascii(curTok))
        return -1;

    int tok_precedence = binOpPrecedence[curTok];
    // [] gives error as map is const
    // [] will modify map is key is not preset (set it  =0)
    if (tok_precedence <= 0)
        return -1;
    return tok_precedence;
}

std::unique_ptr<ExprAST> Parser::parse_expression()
{
    auto LHS = parse_primary();
    if (!LHS)
        return NULL;

    return parse_binop_rhs(0, std::move(LHS));
}

std::unique_ptr<ExprAST> Parser::parse_binop_rhs(int exprPrecedence, std::unique_ptr<ExprAST> LHS)
{
    while (true)
    {
        int curTokPrecedence = get_tok_precedence();
        if (curTokPrecedence < exprPrecedence)
            return LHS;

        int binOp = curTok;
        get_next_tok();

        std::unique_ptr<ExprAST> RHS = parse_primary();
        if (!RHS)
            return NULL;

        int nextTokPrecedence = get_tok_precedence();

        if (nextTokPrecedence > curTokPrecedence)
        {
            RHS = parse_binop_rhs(curTokPrecedence + 1, std::move(RHS));
            if (!RHS)
                return NULL;
        }

        LHS = std::make_unique<BinExprAST>(binOp, std::move(LHS), std::move(RHS));
    }
}

std::unique_ptr<PrototypeAST> Parser::parse_prototype()
{
    if (curTok != tok_identifier)
        return log_error_proto("Expected token identifier");

    std::string funcName = lex->IdentifierString;
    get_next_tok();

    if (curTok != '(')
        return log_error_proto("Expected \"(\" in prototype");

    std::vector<std::string> argNames;
    while (get_next_tok() == tok_identifier)
        argNames.push_back(lex->IdentifierString);

    if (curTok != ')')
        return log_error_proto("Expected \")\" in prototype");

    get_next_tok(); // eat the ")"

    return std::make_unique<PrototypeAST>(funcName, std::move(argNames));
}

std::unique_ptr<FunctionAST> Parser::parse_definition()
{
    get_next_tok();
    auto funcPrototype = parse_prototype();
    if (!funcPrototype)
        return NULL;

    if (auto E = parse_expression())
    {
        return std::make_unique<FunctionAST>(std::move(funcPrototype), std::move(E));
    }
    return NULL;
}

std::unique_ptr<PrototypeAST> Parser::parse_extern()
{
    get_next_tok(); // eat the extern
    return parse_prototype();
}

std::unique_ptr<FunctionAST> Parser::parse_top_level()
{
    auto E = parse_expression();
    if (!E)
        return NULL;
    // top level is a function
    std::string topLevelName = "main";
    std::unique_ptr<PrototypeAST> main_prototype = std::make_unique<PrototypeAST>(topLevelName, std::vector<std::string>());
    std::unique_ptr<FunctionAST> main_function = std::make_unique<FunctionAST>(std::move(main_prototype), std::move(E));
    return main_function;
}

void Parser::main_loop()
{
    get_next_tok();
    while (true)
    {
        fprintf(
            stderr, ">");
        switch (curTok)
        {
        case (tok_eof):
            return;
        case (tok_def):
            handle_definition();
            break;
        case (tok_extern):
            handle_extern();
            break;
        case ((int)';'):
            get_next_tok();
            break;
        default:
            handle_top_level();
            break;
        }
    }
};

inline void Parser::handle_definition()
{
    if (parse_definition())
        fprintf(
            stderr, "\t\t\tHandling Defnition\n");
    else
        get_next_tok();
}
inline void Parser::handle_extern()
{
    if (parse_extern())
        fprintf(
            stderr, "\t\t\tHandling Extern\n");
    else
        get_next_tok();
}

inline void Parser::handle_top_level()
{
    if (parse_top_level())
        fprintf(
            stderr, "\t\tHandling Top Level\n");
    else
        get_next_tok();
}
