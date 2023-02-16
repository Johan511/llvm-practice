#include <string>
#include <memory>
#include <vector>

class ExprAST
{
public:
    virtual ~ExprAST() = default;
};

class NumExprAST : public ExprAST
{
    double numVal;

public:
    NumExprAST(double numVal) : numVal(numVal){};
};

class VarExprAST : public ExprAST
{
    std::string varName;

public:
    VarExprAST(std::string &varName) : varName(varName){};
};

class BinExprAST : public ExprAST
{
    char op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinExprAST(char op, std::unique_ptr<ExprAST> LHS,
               std::unique_ptr<ExprAST> RHS) : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExprAST : public ExprAST
{
    std::string callee;
    std::vector<std::unique_ptr<ExprAST>> args;

public:
    CallExprAST(std::string &callee,
                std::vector<std::unique_ptr<ExprAST>> args) : callee(callee), args(std::move(args)) {}
};

class PrototypeAST
{
    std::string prototypeName;
    std::vector<std::string> argNames;

public:
    PrototypeAST(std::string &prototypeName, std::vector<std::string> argNames)
        : prototypeName(prototypeName), argNames(std::move(argNames)){};
};

class FunctionAST
{
public:
    std::unique_ptr<PrototypeAST> prototype;
    std::unique_ptr<ExprAST> funcBody;

    FunctionAST(std::unique_ptr<PrototypeAST> prototype,
                std::unique_ptr<ExprAST> funcBody) : prototype(std::move(prototype)), funcBody(std::move(funcBody)){};
};
