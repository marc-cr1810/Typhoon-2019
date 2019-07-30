#ifndef Ty_AST_H
#define Ty_AST_H

#include "../Port.h"
#include "Token.h"

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
	OBJ_NAME = TokenType::NAME,
	OBJ_NUMBER = TokenType::NUMBER,
	OBJ_FLOAT = TokenType::FLOAT,
	OBJ_STRING = TokenType::STRING,
	OBJ_BOOL = TokenType::BOOL
};

enum StatementType
{
	UNKNOWN_STATEMENT = -1,
	ASSIGN,
	IF
};

struct NodeData;

struct Node
{
	NodeType Type = NodeType::NODE;
	struct NodeData *Data;

	Node(Node* parent)
	{
		Data = new NodeData;
		Data->Parent = parent;
		if (parent != nullptr)
			parent->Add(this);
	}

	virtual void Add(Node* node)
	{
		Data->Nodes.push_back(node);
	}
};

struct NodeData
{
	Node* Parent;
	std::vector<Node> Nodes;

	StatementType StmtType;

	Node Left;
	Node Right;
	OperatorType OpType;

	ObjectType ObjType;
	Ty_string_t Value;

	NodeData()
		: Left(nullptr), Right(nullptr)
	{}
};

struct Block : Node
{
	Block(Node* parent) : Node(parent)
	{
		Type = NodeType::NODE_BLOCK;
	}
};

struct Statement : Node
{
	Statement(StatementType type, Node* parent)
		: Node(parent)
	{
		Type = NodeType::NODE_STATEMENT;
		Data->StmtType = type;
	}
};

struct Expression : Node
{
	Expression(Node* parent)
		: Node(parent)
	{
		Type = NodeType::NODE_EXPRESSION;
	}
};

struct TyObject : Node
{
	TyObject(Node* parent)
		: Node(parent)
	{
		Type = NodeType::NODE_OBJECT;
	}
};

struct AST
{
	Block Program;

	AST()
		: Program(nullptr)
	{}
};

#endif