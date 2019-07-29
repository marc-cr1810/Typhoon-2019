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

struct Node
{
	Node* Parent;
	std::vector<Node*> Nodes;
	NodeType Type = NodeType::NODE;

	enum ObjectType
	{
		NAME,
		NUMBER,
		FLOAT,
		STRING,
		BOOL
	};

	enum StatementType
	{
		UNKNOWN_STATEMENT = -1,
		ASSIGN,
		IF
	};

	StatementType StmtType;

	Node* Left;
	Node* Right;
	OperatorType OpType;

	ObjectType ObjType;
	Ty_string_t Value;

	Node(Node* parent)
		: Parent(parent)
	{
		if (parent != nullptr)
			parent->Add(this);
	}

	virtual void Add(Node* node)
	{
		Nodes.push_back(node);
	}
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
		StmtType = type;
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