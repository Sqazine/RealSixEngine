#include "Ast.hpp"

namespace RealSix::Script
{
	//----------------------Expressions-----------------------------

	LiteralExpr::LiteralExpr(Token *tagToken)
		: Expr(tagToken, AstKind::LITERAL)
	{
	}
	LiteralExpr::LiteralExpr(Token *tagToken, int64_t value)
		: Expr(tagToken, AstKind::LITERAL), i64Value(value)
	{
		type = Type(TypeKind::I64,tagToken->sourceLocation);
	}
	LiteralExpr::LiteralExpr(Token *tagToken, double value)
		: Expr(tagToken, AstKind::LITERAL), f64Value(value)
	{
		type = Type(TypeKind::F64,tagToken->sourceLocation);
	}
	LiteralExpr::LiteralExpr(Token *tagToken, bool value)
		: Expr(tagToken, AstKind::LITERAL), boolean(value)
	{
		type = Type(TypeKind::BOOL,tagToken->sourceLocation);
	}
	LiteralExpr::LiteralExpr(Token *tagToken, StringView value)
		: Expr(tagToken, AstKind::LITERAL), str(value)
	{
		type = Type(TypeKind::STR,tagToken->sourceLocation);
	}
	LiteralExpr::~LiteralExpr()
	{
	}
#ifndef NDEBUG
	String LiteralExpr::ToString()
	{
		switch (type.GetKind())
		{
		case TypeKind::I8:
		case TypeKind::I16:
		case TypeKind::I32:
		case TypeKind::I64:
		case TypeKind::U8:
		case TypeKind::U16:
		case TypeKind::U32:
		case TypeKind::U64:
			return std::to_string(i64Value);
		case TypeKind::F32:
		case TypeKind::F64:
			return std::to_string(f64Value);
		case TypeKind::BOOL:
			return boolean ? "true" : "false";
		case TypeKind::CHAR:
			return String({character});
		case TypeKind::STR:
			return str;
		default:
			return "null";
		}
	}
#endif
	IdentifierExpr::IdentifierExpr(Token *tagToken)
		: Expr(tagToken, AstKind::IDENTIFIER)
	{
	}
	IdentifierExpr::IdentifierExpr(Token *tagToken, StringView literal)
		: Expr(tagToken, AstKind::IDENTIFIER), literal(literal)
	{
	}
	IdentifierExpr::~IdentifierExpr()
	{
	}

#ifndef NDEBUG
	String IdentifierExpr::ToString()
	{
		return literal;
	}
#endif
	VarDescExpr::VarDescExpr(Token *tagToken)
		: Expr(tagToken, AstKind::VAR_DESC), name(nullptr)
	{
		type = Type(TypeKind::ANY,tagToken->sourceLocation);
	}
	VarDescExpr::VarDescExpr(Token *tagToken, const Type &type, Expr *name)
		: Expr(tagToken, AstKind::VAR_DESC), name(name)
	{
		this->type = type;
	}
	VarDescExpr::~VarDescExpr()
	{
		SAFE_DELETE(name);
	}
#ifndef NDEBUG
	String VarDescExpr::ToString()
	{
		return name->ToString() + ":" + type.GetName();
	}
#endif
	ArrayExpr::ArrayExpr(Token *tagToken)
		: Expr(tagToken, AstKind::ARRAY)
	{
	}
	ArrayExpr::ArrayExpr(Token *tagToken, const std::vector<Expr *> &elements) : Expr(tagToken, AstKind::ARRAY), elements(elements)
	{
	}
	ArrayExpr::~ArrayExpr()
	{
		std::vector<Expr *>().swap(elements);
	}
#ifndef NDEBUG
	String ArrayExpr::ToString()
	{
		String result = "[";

		if (!elements.empty())
		{
			for (auto e : elements)
				result += e->ToString() + ",";
			result = result.SubStr(0, result.Size() - 1);
		}
		result += "]";
		return result;
	}
#endif

	DictExpr::DictExpr(Token *tagToken)
		: Expr(tagToken, AstKind::DICT)
	{
	}
	DictExpr::DictExpr(Token *tagToken, const std::vector<std::pair<Expr *, Expr *>> &elements)
		: Expr(tagToken, AstKind::DICT), elements(elements)
	{
	}
	DictExpr::~DictExpr()
	{
		std::vector<std::pair<Expr *, Expr *>>().swap(elements);
	}
#ifndef NDEBUG
	String DictExpr::ToString()
	{
		String result = "{";

		if (!elements.empty())
		{
			for (auto [key, value] : elements)
				result += key->ToString() + ":" + value->ToString() + ",";
			result = result.SubStr(0, result.Size() - 1);
		}
		result += "}";
		return result;
	}
#endif

	GroupExpr::GroupExpr(Token *tagToken)
		: Expr(tagToken, AstKind::GROUP), expr(nullptr)
	{
	}
	GroupExpr::GroupExpr(Token *tagToken, Expr *expr)
		: Expr(tagToken, AstKind::GROUP), expr(expr)
	{
	}
	GroupExpr::~GroupExpr()
	{
		SAFE_DELETE(expr);
	}

#ifndef NDEBUG
	String GroupExpr::ToString()
	{
		return "(" + expr->ToString() + ")";
	}
#endif

	PrefixExpr::PrefixExpr(Token *tagToken)
		: Expr(tagToken, AstKind::PREFIX), right(nullptr)
	{
	}
	PrefixExpr::PrefixExpr(Token *tagToken, StringView op, Expr *right)
		: Expr(tagToken, AstKind::PREFIX), op(op), right(right)
	{
	}
	PrefixExpr::~PrefixExpr()
	{
		SAFE_DELETE(right);
	}
#ifndef NDEBUG
	String PrefixExpr::ToString()
	{
		return op + right->ToString();
	}
#endif
	InfixExpr::InfixExpr(Token *tagToken)
		: Expr(tagToken, AstKind::INFIX), left(nullptr), right(nullptr)
	{
	}
	InfixExpr::InfixExpr(Token *tagToken, StringView op, Expr *left, Expr *right)
		: Expr(tagToken, AstKind::INFIX), op(op), left(left), right(right)
	{
	}
	InfixExpr::~InfixExpr()
	{
		SAFE_DELETE(left);
		SAFE_DELETE(right);
	}
#ifndef NDEBUG
	String InfixExpr::ToString()
	{
		return left->ToString() + op + right->ToString();
	}
#endif

	PostfixExpr::PostfixExpr(Token *tagToken)
		: Expr(tagToken, AstKind::POSTFIX), left(nullptr)
	{
	}
	PostfixExpr::PostfixExpr(Token *tagToken, Expr *left, StringView op)
		: Expr(tagToken, AstKind::POSTFIX), left(left), op(op)
	{
	}
	PostfixExpr::~PostfixExpr()
	{
		SAFE_DELETE(left);
	}

#ifndef NDEBUG
	String PostfixExpr::ToString()
	{
		return left->ToString() + op;
	}
#endif

	ConditionExpr::ConditionExpr(Token *tagToken)
		: Expr(tagToken, AstKind::CONDITION), condition(nullptr), trueBranch(nullptr), falseBranch(nullptr)
	{
	}
	ConditionExpr::ConditionExpr(Token *tagToken, Expr *condition, Expr *trueBranch, Expr *falseBranch)
		: Expr(tagToken, AstKind::CONDITION), condition(condition), trueBranch(trueBranch), falseBranch(falseBranch)
	{
	}
	ConditionExpr::~ConditionExpr()
	{
		SAFE_DELETE(condition);
		SAFE_DELETE(trueBranch);
		SAFE_DELETE(falseBranch);
	}
#ifndef NDEBUG
	String ConditionExpr::ToString()
	{
		return condition->ToString() + "?" + trueBranch->ToString() + ":" + falseBranch->ToString();
	}
#endif
	IndexExpr::IndexExpr(Token *tagToken)
		: Expr(tagToken, AstKind::INDEX), ds(nullptr), index(nullptr)
	{
	}
	IndexExpr::IndexExpr(Token *tagToken, Expr *ds, Expr *index)
		: Expr(tagToken, AstKind::INDEX), ds(ds), index(index)
	{
	}
	IndexExpr::~IndexExpr()
	{
		SAFE_DELETE(ds);
		SAFE_DELETE(index);
	}
#ifndef NDEBUG
	String IndexExpr::ToString()
	{
		return ds->ToString() + "[" + index->ToString() + "]";
	}
#endif

	RefExpr::RefExpr(Token *tagToken)
		: Expr(tagToken, AstKind::REF), refExpr(nullptr)
	{
	}
	RefExpr::RefExpr(Token *tagToken, Expr *refExpr)
		: Expr(tagToken, AstKind::REF), refExpr(refExpr)
	{
	}
	RefExpr::~RefExpr()
	{
		SAFE_DELETE(refExpr);
	}
#ifndef NDEBUG
	String RefExpr::ToString()
	{
		return "&" + refExpr->ToString();
	}
#endif
	LambdaExpr::LambdaExpr(Token *tagToken)
		: Expr(tagToken, AstKind::LAMBDA), body(nullptr)
	{
	}
	LambdaExpr::LambdaExpr(Token *tagToken, const std::vector<VarDescExpr *> &parameters, ScopeStmt *body)
		: Expr(tagToken, AstKind::LAMBDA), parameters(parameters), body(body)
	{
	}
	LambdaExpr::~LambdaExpr()
	{
		std::vector<VarDescExpr *>().swap(parameters);
		SAFE_DELETE(body);
	}
#ifndef NDEBUG
	String LambdaExpr::ToString()
	{
		String result = "fn(";
		if (!parameters.empty())
		{
			for (auto param : parameters)
				result += param->ToString() + ",";
			result = result.SubStr(0, result.Size() - 1);
		}
		result += ")";
		result += body->ToString();
		return result;
	}
#endif

	CallExpr::CallExpr(Token *tagToken)
		: Expr(tagToken, AstKind::CALL), callee(nullptr)
	{
	}
	CallExpr::CallExpr(Token *tagToken, Expr *callee, const std::vector<Expr *> &arguments)
		: Expr(tagToken, AstKind::CALL), callee(callee), arguments(arguments)
	{
	}
	CallExpr::~CallExpr()
	{
		SAFE_DELETE(callee);
		std::vector<Expr *>().swap(arguments);
	}
#ifndef NDEBUG
	String CallExpr::ToString()
	{
		String result = callee->ToString() + "(";

		if (!arguments.empty())
		{
			for (const auto &arg : arguments)
				result += arg->ToString() + ",";
			result = result.SubStr(0, result.Size() - 1);
		}
		result += ")";
		return result;
	}
#endif

	DotExpr::DotExpr(Token *tagToken)
		: Expr(tagToken, AstKind::DOT), callee(nullptr), callMember(nullptr)
	{
	}
	DotExpr::DotExpr(Token *tagToken, Expr *callee, IdentifierExpr *callMember)
		: Expr(tagToken, AstKind::DOT), callee(callee), callMember(callMember)
	{
	}
	DotExpr::~DotExpr()
	{
		SAFE_DELETE(callee);
		SAFE_DELETE(callMember);
	}
#ifndef NDEBUG
	String DotExpr::ToString()
	{
		return callee->ToString() + "." + callMember->ToString();
	}
#endif
	NewExpr::NewExpr(Token *tagToken)
		: Expr(tagToken, AstKind::NEW), callee(nullptr)
	{
	}
	NewExpr::NewExpr(Token *tagToken, Expr *callee)
		: Expr(tagToken, AstKind::NEW), callee(callee)
	{
	}
	NewExpr::~NewExpr()
	{
		SAFE_DELETE(callee);
	}
#ifndef NDEBUG
	String NewExpr::ToString()
	{
		return "new " + callee->ToString();
	}
#endif

	ThisExpr::ThisExpr(Token *tagToken)
		: Expr(tagToken, AstKind::THIS)
	{
	}

	ThisExpr::~ThisExpr()
	{
	}
#ifndef NDEBUG
	String ThisExpr::ToString()
	{
		return "this";
	}
#endif

	BaseExpr::BaseExpr(Token *tagToken, IdentifierExpr *callMember)
		: Expr(tagToken, AstKind::BASE), callMember(callMember)
	{
	}
	BaseExpr::~BaseExpr()
	{
		SAFE_DELETE(callMember);
	}
#ifndef NDEBUG
	String BaseExpr::ToString()
	{
		return "base." + callMember->ToString();
	}
#endif

	CompoundExpr::CompoundExpr(Token *tagToken)
		: Expr(tagToken, AstKind::COMPOUND), endExpr(nullptr)
	{
	}
	CompoundExpr::CompoundExpr(Token *tagToken, const std::vector<Stmt *> &stmts, Expr *endExpr)
		: Expr(tagToken, AstKind::COMPOUND), stmts(stmts), endExpr(endExpr)
	{
	}
	CompoundExpr::~CompoundExpr()
	{
		SAFE_DELETE(endExpr);
		std::vector<Stmt *>().swap(stmts);
	}
#ifndef NDEBUG
	String CompoundExpr::ToString()
	{
		String result = "({";
		for (const auto &stmt : stmts)
			result += stmt->ToString();
		result += endExpr->ToString();
		result += "})";
		return result;
	}
#endif

	StructExpr::StructExpr(Token *tagToken)
		: Expr(tagToken, AstKind::STRUCT)
	{
	}
	StructExpr::StructExpr(Token *tagToken, const std::vector<std::pair<String, Expr *>> &elements)
		: Expr(tagToken, AstKind::STRUCT), elements(elements)
	{
	}
	StructExpr::~StructExpr()
	{
		std::vector<std::pair<String, Expr *>>().swap(elements);
	}
#ifndef NDEBUG
	String StructExpr::ToString()
	{
		String result = "{";
		if (!elements.empty())
		{
			for (auto [key, value] : elements)
				result += key + ":" + value->ToString() + ",";
			result = result.SubStr(0, result.Size() - 1);
		}
		result += "}";
		return result;
	}
#endif
	VarArgExpr::VarArgExpr(Token *tagToken)
		: Expr(tagToken, AstKind::VAR_ARG), argName(nullptr)
	{
	}
	VarArgExpr::VarArgExpr(Token *tagToken, IdentifierExpr *argName)
		: Expr(tagToken, AstKind::VAR_ARG), argName(argName)
	{
	}
	VarArgExpr::~VarArgExpr()
	{
		SAFE_DELETE(argName);
	}
#ifndef NDEBUG
	String VarArgExpr::ToString()
	{
		return "..." + (argName ? argName->ToString() : "");
	}
#endif
	FactorialExpr::FactorialExpr(Token *tagToken)
		: Expr(tagToken, AstKind::FACTORIAL), expr(nullptr)
	{
	}
	FactorialExpr::FactorialExpr(Token *tagToken, Expr *expr)
		: Expr(tagToken, AstKind::FACTORIAL), expr(expr)
	{
	}
	FactorialExpr::~FactorialExpr()
	{
		SAFE_DELETE(expr);
	}
#ifndef NDEBUG
	String FactorialExpr::ToString()
	{
		return expr->ToString() + "!";
	}
#endif

	AppregateExpr::AppregateExpr(Token *tagToken)
		: Expr(tagToken, AstKind::APPREGATE)
	{
	}
	AppregateExpr::AppregateExpr(Token *tagToken, const std::vector<Expr *> &exprs)
		: Expr(tagToken, AstKind::APPREGATE), exprs(exprs)
	{
	}
	AppregateExpr::~AppregateExpr()
	{
		std::vector<Expr *>().swap(exprs);
	}
#ifndef NDEBUG
	String AppregateExpr::ToString()
	{
		String result = "(";
		if (!exprs.empty())
		{
			for (const auto &expr : exprs)
				result += expr->ToString() + ",";
			result = result.SubStr(0, result.Size() - 1);
		}

		return result + ")";
	}
#endif
	//----------------------Statements-----------------------------

	ExprStmt::ExprStmt(Token *tagToken)
		: Stmt(tagToken, AstKind::EXPR), expr(nullptr)
	{
	}
	ExprStmt::ExprStmt(Token *tagToken, Expr *expr)
		: Stmt(tagToken, AstKind::EXPR), expr(expr)
	{
	}
	ExprStmt::~ExprStmt()
	{
		SAFE_DELETE(expr);
	}
#ifndef NDEBUG
	String ExprStmt::ToString()
	{
		return expr->ToString() + ";";
	}
#endif
	ReturnStmt::ReturnStmt(Token *tagToken)
		: Stmt(tagToken, AstKind::RETURN), expr(nullptr)
	{
	}
	ReturnStmt::ReturnStmt(Token *tagToken, Expr *expr)
		: Stmt(tagToken, AstKind::RETURN), expr(expr)
	{
	}
	ReturnStmt::~ReturnStmt()
	{
		SAFE_DELETE(expr);
	}
#ifndef NDEBUG
	String ReturnStmt::ToString()
	{
		if (!expr)
			return "return;";
		else
			return "return " + expr->ToString() + ";";
	}
#endif

	IfStmt::IfStmt(Token *tagToken)
		: Stmt(tagToken, AstKind::IF), condition(nullptr), thenBranch(nullptr), elseBranch(nullptr)
	{
	}
	IfStmt::IfStmt(Token *tagToken, Expr *condition, Stmt *thenBranch, Stmt *elseBranch)
		: Stmt(tagToken, AstKind::IF),
		  condition(condition),
		  thenBranch(thenBranch),
		  elseBranch(elseBranch)
	{
	}
	IfStmt::~IfStmt()
	{
		SAFE_DELETE(condition);
		SAFE_DELETE(thenBranch);
		SAFE_DELETE(elseBranch);
	}
#ifndef NDEBUG
	String IfStmt::ToString()
	{
		String result;
		result = "if(" + condition->ToString() + ")" + thenBranch->ToString();
		if (elseBranch != nullptr)
			result += "else " + elseBranch->ToString();
		return result;
	}
#endif

	ScopeStmt::ScopeStmt(Token *tagToken)
		: Stmt(tagToken, AstKind::SCOPE)
	{
	}
	ScopeStmt::ScopeStmt(Token *tagToken, const std::vector<Stmt *> &stmts)
		: Stmt(tagToken, AstKind::SCOPE), stmts(stmts)
	{
	}
	ScopeStmt::~ScopeStmt()
	{
		std::vector<Stmt *>().swap(stmts);
	}
#ifndef NDEBUG
	String ScopeStmt::ToString()
	{
		String result = "{";
		for (const auto &stmt : stmts)
			result += stmt->ToString();
		result += "}";
		return result;
	}
#endif

	WhileStmt::WhileStmt(Token *tagToken)
		: Stmt(tagToken, AstKind::WHILE), condition(nullptr), body(nullptr), increment(nullptr)
	{
	}
	WhileStmt::WhileStmt(Token *tagToken, Expr *condition, ScopeStmt *body, ScopeStmt *increment)
		: Stmt(tagToken, AstKind::WHILE), condition(condition), body(body), increment(increment)
	{
	}
	WhileStmt::~WhileStmt()
	{
		SAFE_DELETE(condition);
		SAFE_DELETE(body);
		SAFE_DELETE(increment);
	}
#ifndef NDEBUG
	String WhileStmt::ToString()
	{
		String result = "while(" + condition->ToString() + "){" + body->ToString();
		if (increment)
			result += increment->ToString();
		return result += "}";
	}
#endif

	BreakStmt::BreakStmt(Token *tagToken)
		: Stmt(tagToken, AstKind::BREAK)
	{
	}
	BreakStmt::~BreakStmt()
	{
	}
#ifndef NDEBUG
	String BreakStmt::ToString()
	{
		return "break;";
	}
#endif

	ContinueStmt::ContinueStmt(Token *tagToken)
		: Stmt(tagToken, AstKind::CONTINUE)
	{
	}
	ContinueStmt::~ContinueStmt()
	{
	}
#ifndef NDEBUG
	String ContinueStmt::ToString()
	{
		return "continue;";
	}
#endif

	AstStmts::AstStmts(Token *tagToken)
		: Stmt(tagToken, AstKind::ASTSTMTS)
	{
	}
	AstStmts::AstStmts(Token *tagToken, std::vector<Stmt *> stmts)
		: Stmt(tagToken, AstKind::ASTSTMTS), stmts(stmts)
	{
	}

	AstStmts::~AstStmts()
	{
		std::vector<Stmt *>().swap(stmts);
	}
#ifndef NDEBUG
	String AstStmts::ToString()
	{
		String result;
		for (const auto &stmt : stmts)
			result += stmt->ToString();
		return result;
	}
#endif
	//----------------------Declarations-----------------------------

	VarDecl::VarDecl(Token *tagToken)
		: Stmt(tagToken, AstKind::VAR), permission(Permission::MUTABLE)
	{
	}
	VarDecl::VarDecl(Token *tagToken, Permission permission, const std::vector<std::pair<Expr *, Expr *>> &variables)
		: Stmt(tagToken, AstKind::VAR), permission(permission), variables(variables)
	{
	}
	VarDecl::~VarDecl()
	{
		std::vector<std::pair<Expr *, Expr *>>().swap(variables);
	}
#ifndef NDEBUG
	String VarDecl::ToString()
	{
		String result;

		if (permission == Permission::IMMUTABLE)
			result += "const ";
		else
			result += "let ";

		if (!variables.empty())
		{
			for (auto [key, value] : variables)
				result += key->ToString() + "=" + value->ToString() + ",";
			result = result.SubStr(0, result.Size() - 1);
		}
		return result + ";";
	}
#endif

	EnumDecl::EnumDecl(Token *tagToken)
		: Stmt(tagToken, AstKind::ENUM), name(nullptr)
	{
	}
	EnumDecl::EnumDecl(Token *tagToken, IdentifierExpr *name, const std::unordered_map<IdentifierExpr *, Expr *> &enumItems)
		: Stmt(tagToken, AstKind::ENUM), name(name), enumItems(enumItems)
	{
	}
	EnumDecl::~EnumDecl()
	{
		SAFE_DELETE(name);
		std::unordered_map<IdentifierExpr *, Expr *>().swap(enumItems);
	}

#ifndef NDEBUG
	String EnumDecl::ToString()
	{
		String result = "enum " + name->ToString() + "{";

		if (!enumItems.empty())
		{
			for (auto [key, value] : enumItems)
				result += key->ToString() + "=" + value->ToString() + ",";
			result = result.SubStr(0, result.Size() - 1);
		}
		return result + "}";
	}
#endif
	ModuleDecl::ModuleDecl(Token *tagToken)
		: Stmt(tagToken, AstKind::MODULE), name(nullptr)
	{
	}
	ModuleDecl::ModuleDecl(Token *tagToken,
						   IdentifierExpr *name,
						   const std::vector<VarDecl *> &varItems,
						   const std::vector<ClassDecl *> &classItems,
						   const std::vector<ModuleDecl *> &moduleItems,
						   const std::vector<EnumDecl *> &enumItems,
						   const std::vector<FunctionDecl *> &functionItems)
		: Stmt(tagToken, AstKind::MODULE), name(name),
		  varItems(varItems),
		  classItems(classItems),
		  moduleItems(moduleItems),
		  enumItems(enumItems),
		  functionItems(functionItems)
	{
	}
	ModuleDecl::~ModuleDecl()
	{
		SAFE_DELETE(name);
		std::vector<VarDecl *>().swap(varItems);
		std::vector<ClassDecl *>().swap(classItems);
		std::vector<ModuleDecl *>().swap(moduleItems);
		std::vector<EnumDecl *>().swap(enumItems);
		std::vector<FunctionDecl *>().swap(functionItems);
	}
#ifndef NDEBUG
	String ModuleDecl::ToString()
	{
		String result = "module " + name->ToString() + "\n{\n";
		for (const auto &item : varItems)
			result += item->ToString() + "\n";
		for (const auto &item : classItems)
			result += item->ToString() + "\n";
		for (const auto &item : moduleItems)
			result += item->ToString() + "\n";
		for (const auto &item : enumItems)
			result += item->ToString() + "\n";
		for (const auto &item : functionItems)
			result += item->ToString() + "\n";
		return result + "}\n";
	}
#endif

	FunctionDecl::FunctionDecl(Token *tagToken)
		: Stmt(tagToken, AstKind::FUNCTION), name(nullptr), body(nullptr)
	{
	}

	FunctionDecl::FunctionDecl(Token *tagToken, IdentifierExpr *name, const std::vector<VarDescExpr *> &parameters, ScopeStmt *body)
		: Stmt(tagToken, AstKind::FUNCTION), name(name), parameters(parameters), body(body)
	{
	}

	FunctionDecl::~FunctionDecl()
	{
		SAFE_DELETE(name);
		std::vector<VarDescExpr *>().swap(parameters);
		SAFE_DELETE(body);
	}
#ifndef NDEBUG
	String FunctionDecl::ToString()
	{
		String result = "fn " + name->ToString() + "(";
		if (!parameters.empty())
		{
			for (auto param : parameters)
				result += param->ToString() + ",";
			result = result.SubStr(0, result.Size() - 1);
		}
		result += ")";
		result += body->ToString();
		return result;
	}
#endif

	ClassDecl::ClassDecl(Token *tagToken)
		: Stmt(tagToken, AstKind::CLASS)
	{
	}

	ClassDecl::ClassDecl(Token *tagToken,
						 String name,
						 const std::vector<std::pair<MemberPrivilege, IdentifierExpr *>> &parents,
						 const std::vector<std::pair<MemberPrivilege, VarDecl *>> &variables,
						 const std::vector<std::pair<MemberPrivilege, FunctionMember>> &functions,
						 const std::vector<std::pair<MemberPrivilege, EnumDecl *>> &enumerations)
		: Stmt(tagToken, AstKind::CLASS),
		  name(name),
		  variables(variables),
		  functions(functions),
		  enumerations(enumerations),
		  parents(parents)
	{
	}

	ClassDecl::~ClassDecl()
	{
		std::vector<std::pair<MemberPrivilege, IdentifierExpr *>>().swap(parents);
		std::vector<std::pair<MemberPrivilege, VarDecl *>>().swap(variables);
		std::vector<std::pair<MemberPrivilege, FunctionMember>>().swap(functions);
		std::vector<std::pair<MemberPrivilege, EnumDecl *>>().swap(enumerations);
	}
#ifndef NDEBUG
	String ClassDecl::ToString()
	{
		String result = "class " + name;
		if (!parents.empty())
		{
			result += ":";
			for (const auto &parent : parents)
				result += parent.second->ToString() + ",";
			result = result.SubStr(0, result.Size() - 1);
		}
		result += "{";
		for (auto enumeration : enumerations)
			result += enumeration.second->ToString();
		for (auto variable : variables)
			result += variable.second->ToString();
		for (auto fn : functions)
			result += fn.second.decl->ToString();
		return result + "}";
	}
#endif
}
