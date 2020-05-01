#ifndef Ty_AST_H
#define Ty_AST_H

#include "../Port.h"
#include "Token.h"
#include "Grammar.h"

enum NodeType
{
	UNKNOWN_NODE = -1,
	NODE,
	NODE_BLOCK,
	NODE_STATEMENT,
	NODE_EXPRESSION,
	NODE_OBJECT
};

enum ObjectType
{
	OBJ_UNKNOWN = -1,
	OBJ_NAME = TokenType::NAME,
	OBJ_NUMBER = TokenType::NUMBER,
	OBJ_FLOAT = TokenType::FLOAT,
	OBJ_STRING = TokenType::STRING,
	OBJ_BOOL = TokenType::BOOL,
	OBJ_FUNCTION_CALL = TokenType::FUNCTION_CALL,
	OBJ_NEW_ARRAY = TokenType::NEW_ARRAY,
	OBJ_GET_ARRAY = TokenType::GET_ARRAY
};

enum StatementType
{
	UNKNOWN_STATEMENT = -1,
	EXPRESSION = GrammarType::EXPRESSION_STATEMENT,
	ASSIGN_NEW = GrammarType::CREATE_VAR,
	FUNCTION_DECLARATION = GrammarType::CREATE_FUNCTION,
	IF = GrammarType::IF_STATEMENT,
	ELSE = GrammarType::ELSE_STATEMENT,
	ELSE_IF = GrammarType::ELSE_IF_STATEMENT,
	WHILE_LOOP = GrammarType::WHILE_STATEMENT,
	RETURN_FUNC = GrammarType::RETURN_FUNCTION
};

typedef struct TyNode
{
	NodeType Type = NodeType::UNKNOWN_NODE;
	ObjectType ObjType = ObjectType::OBJ_UNKNOWN;
	OperatorType OpType = OperatorType::UNKNOWN;
	StatementType StmtType = StatementType::UNKNOWN_STATEMENT;

	Ty_string_t Value;
	std::vector<struct TyNode> Children;

	void AddChild(struct TyNode child) { Children.push_back(child); }
} Node;

struct AST
{
	Node Program;

	AST()
	{}
};

static Node NewNode(NodeType type, ObjectType objType, OperatorType opType, StatementType stmtType, Ty_string_t value)
{
	Node n;
	n.Type = type;
	n.ObjType = objType;
	n.OpType = opType;
	n.StmtType = stmtType;
	n.Value = value;
	return n;
}

static Node NewStatementNode(StatementType stmtType)
{
	return NewNode(NodeType::NODE_STATEMENT, ObjectType::OBJ_UNKNOWN, OperatorType::UNKNOWN, stmtType, "");
}

static Node NewExpressionNode(OperatorType opType)
{
	return NewNode(NodeType::NODE_EXPRESSION, ObjectType::OBJ_UNKNOWN, opType, StatementType::UNKNOWN_STATEMENT, "");
}

static Node NewObjectNode(ObjectType type, Ty_string_t value)
{
	return NewNode(NodeType::NODE_OBJECT, type, OperatorType::UNKNOWN, StatementType::UNKNOWN_STATEMENT, value);
}

#endif