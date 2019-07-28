#ifndef Ty_AST_H
#define Ty_AST_H

#include "../Port.h"

struct Node
{
	Node* Parent;

	Node(Node *parent)
		: Parent(parent)
	{ }

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

struct AST
{
	Block Program;
};

#endif