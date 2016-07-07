/**
 * @file	Expression.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "SEAPPExpression.h"


bool SEAPPExpression::isAssignmentOperator(const string op)
{
	if ( (op == "=") || (op == "+=") || (op == "-=") || (op == "*=") || (op == "/=") || (op == "%=") ) {
		return true;
	}
	
	return false;
}


SEAPPExpression::SEAPPExpression(const vector<string> expressionItems) : ActionBase(action_t::EXPRESSION)
{
	this->expressionItems = expressionItems;
	involvedLayer = NONE_LAYER;	
}


SEAPPExpression::~SEAPPExpression()
{  
}


void SEAPPExpression::execute(map<string, Variable*> *variableTable, stack<Variable> *variableStack)
{	
	Variable leftOperand;
	Variable rightOperand;

	for (size_t i = 0; i < expressionItems.size(); i++) {

		map<string, Variable*>::iterator iter = variableTable->find(expressionItems[i]);
		
		// push the found operand into the stack
		if (iter != variableTable->end()) {
			variableStack->push(*(iter->second));
		}
		// found an operator
		else {
			string op = expressionItems[i];

			// is an assignment operator
			if (isAssignmentOperator(op)) {
				
				i++;
				string where = expressionItems[i];

				// find the other operand
				map<string, Variable*>::iterator jter = variableTable->find(where);

				Variable value;

				value = variableStack->top();
				variableStack->pop();

				if ( op == "=")
					*(jter->second) = value;
				else if ( op == "+=")
					*(jter->second) += value;
				else if ( op == "-=")
					*(jter->second) -= value;
				else if ( op == "*=")
					*(jter->second) *= value;
				else if ( op == "/=")
					*(jter->second) /= value;
				else if ( op == "%=")
					*(jter->second) %= value;

			}
			// is not an assignment operator
			else {
				
				// Retrieve the operands
				rightOperand = variableStack->top();
				variableStack->pop();

				leftOperand = variableStack->top();
				variableStack->pop();
				
				// push the result of the operation into the stack
				if ( op == "+")
					variableStack->push( leftOperand + rightOperand );
				else if ( op == "-")
					variableStack->push( leftOperand - rightOperand );
				else if ( op == "*")
					variableStack->push( leftOperand * rightOperand );
				else if ( op == "/")
					variableStack->push( leftOperand / rightOperand );
				else if ( op == "%")
					variableStack->push( leftOperand % rightOperand );
				else if ( op == "==")
					variableStack->push( leftOperand == rightOperand );
				else if ( op == ">")
					variableStack->push( leftOperand > rightOperand );
				else if ( op == "<")
					variableStack->push( leftOperand < rightOperand );
				else if ( op == ">=")
					variableStack->push( leftOperand >= rightOperand );
				else if ( op == "<=")
					variableStack->push( leftOperand <= rightOperand );
				else if ( op == "!=")
					variableStack->push( leftOperand != rightOperand );
				else if ( op == "&&")
					variableStack->push( leftOperand && rightOperand );
				else if ( op == "||")
					variableStack->push( leftOperand || rightOperand );

			}
			
		}

	}

}



