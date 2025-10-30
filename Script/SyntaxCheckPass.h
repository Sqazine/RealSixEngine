#pragma once
#include <cstdint>
#include "AstOptimizePass.h"

namespace RealSix::Script
{
    class REALSIX_API SyntaxCheckPass : public AstOptimizePass
    {
    public:
         SyntaxCheckPass() noexcept;
         virtual ~SyntaxCheckPass() noexcept override = default;

    protected:
        virtual Stmt *ExecuteAstStmts(AstStmts *stmt) override;

        virtual Stmt *ExecuteVarDecl(VarDecl * decl) override;
        virtual Stmt *ExecuteFunctionDecl(FunctionDecl * decl) override;
        virtual Stmt *ExecuteModuleDecl(ModuleDecl * decl) override;
        virtual Stmt *ExecuteEnumDecl(EnumDecl * decl) override;
        virtual Stmt *ExecuteClassDecl(ClassDecl * decl) override;
        
        virtual Stmt *ExecuteExprStmt(ExprStmt *stmt) override;
        virtual Stmt *ExecuteReturnStmt(ReturnStmt *stmt) override;
        virtual Stmt *ExecuteIfStmt(IfStmt *stmt) override;
        virtual Stmt *ExecuteScopeStmt(ScopeStmt *stmt) override;
        virtual Stmt *ExecuteWhileStmt(WhileStmt *stmt) override;
        virtual Stmt *ExecuteBreakStmt(BreakStmt *stmt) override;
        virtual Stmt *ExecuteContinueStmt(ContinueStmt *stmt) override;

        virtual Expr *ExecuteLiteralExpr(LiteralExpr *expr) override;
        virtual Expr *ExecuteInfixExpr(InfixExpr *expr) override;
        virtual Expr *ExecutePrefixExpr(PrefixExpr *expr) override;
        virtual Expr *ExecutePostfixExpr(PostfixExpr *expr) override;
        virtual Expr *ExecuteConditionExpr(ConditionExpr *expr) override;
        virtual Expr *ExecuteGroupExpr(GroupExpr *expr) override;
        virtual Expr *ExecuteArrayExpr(ArrayExpr *expr) override;
        virtual Expr *ExecuteAppregateExpr(AppregateExpr *expr) override;
        virtual Expr *ExecuteDictExpr(DictExpr *expr) override;
        virtual Expr *ExecuteIndexExpr(IndexExpr *expr) override;
        virtual Expr *ExecuteNewExpr(NewExpr *expr) override;
        virtual Expr *ExecuteThisExpr(ThisExpr *expr) override;
        virtual Expr *ExecuteBaseExpr(BaseExpr *expr) override;
        virtual Expr *ExecuteIdentifierExpr(IdentifierExpr *expr) override;
        virtual Expr *ExecuteLambdaExpr(LambdaExpr *expr) override;
        virtual Expr *ExecuteCompoundExpr(CompoundExpr *expr) override;
        virtual Expr *ExecuteCallExpr(CallExpr *expr) override;
        virtual Expr *ExecuteDotExpr(DotExpr *expr) override;
        virtual Expr *ExecuteRefExpr(RefExpr *expr) override;
        virtual Expr *ExecuteStructExpr(StructExpr *expr) override;
        virtual Expr *ExecuteVarArgExpr(VarArgExpr *expr) override;
        virtual Expr *ExecuteFactorialExpr(FactorialExpr *expr) override;
        virtual Expr *ExecuteVarDescExpr(VarDescExpr *expr) override;

        bool IsConstantLiteral(Expr *expr);

        uint32_t mLoopDepth;
        uint32_t mVarArgScopeDepth;
        uint32_t mClassScopeDepth;
    };
}