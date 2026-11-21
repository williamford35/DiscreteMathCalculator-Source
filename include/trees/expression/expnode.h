#ifndef EXPNODE_H
#define EXPNODE_H

struct expnode
{
public:
	enum nodeType
	{
		operand,
		binaryOPerator,
		unaryOperator
	};

	wxString nodeValue;
	expnode *left;
	expnode *right;
	nodeType operatorType;

	expnode(const wxString& item, expnode *lchild,
	        expnode *rchild, nodeType nt = operand):
		nodeValue(item), left(lchild),
		right(rchild), operatorType(nt)
	{}
};

#endif
