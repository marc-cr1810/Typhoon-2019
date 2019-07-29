#ifndef Ty_AST_H
#define Ty_AST_H

#include "../Port.h"
#include "Token.h"

struct Node
{
	Node* Parent;

	Node(Node* parent)
		: Parent(parent)
	{}

	virtual void Add(Node node) 
	{}
};

struct Block : Node
{
	std::vector<Node> Nodes;

	Block(Node* parent) : Node(parent)
	{}

	Block(Node* parent, std::vector<Node> nodes) 
		: Node(parent), Nodes(nodes)
	{}

	void Add(Node node) override
	{
		Nodes.push_back(node);
	}
};

struct Statement : Node
{
	enum StatementType
	{
		UNKNOWN_STATEMENT = -1,
		ASSIGN,
		IF
	};

	StatementType Type;
	std::vector<Node> Nodes;

	Statement(StatementType type, Node* parent)
		: Node(parent), Type(type)
	{}

	Statement(StatementType type, Node* parent, std::vector<Node> nodes)
		: Node(parent), Type(type), Nodes(nodes)
	{}
};

struct Expression : Node
{
	Node* Left;
	Node* Right;
	OperatorType OpType;

	Expression(Node* parent)
		: Node(parent)
	{}

	Expression(Node* parent, OperatorType opType, Node* left, Node* right)
		: Node(parent), OpType(opType), Left(left), Right(right)
	{}
};

struct TyObject : Node
{
	enum ObjectType
	{
		NAME,
		NUMBER,
		FLOAT,
		STRING,
		BOOL
	};

	ObjectType Type;
	Ty_string_t Value;

	TyObject(Node* parent)
		: Node(parent)
	{}

	TyObject(Node* parent, ObjectType type)
		: Node(parent), Type(type)
	{}
};

struct AST
{
	Block Program;

	AST()
		: Program(nullptr)
	{}
};

#endif