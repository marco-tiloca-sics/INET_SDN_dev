/**
 * @file	SEAPPExpression.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#ifndef SEAPPEXPRESSION_H
#define SEAPPEXPRESSION_H


#include "ActionBase.h"
#include "Variable.h"

#include <vector>
#include <stack>
#include <map>


class SEAPPExpression : public ActionBase {

	private:
		vector<string> expressionItems;
		
	private:	
		bool isAssignmentOperator(const string op);

	public:
		/**
		 * @brief Constructor
		 */
		SEAPPExpression(const vector<string> expressionItems);
	
		/**
		 * @brief Destructor
		 */
		virtual ~SEAPPExpression();
	
		/**
		 * @brief Execute expression action
		 * @details The execution of an expression relies on the variable table and variable stack, provided by the calling object.
		 *  		The former contains variables considered by the expression, while the latter is used to actually execute the expression.
   		 *			At the end, the variable stored in the variable table and aimed at containing the expression result is updated
   		 */
		 void execute(map<string, Variable*> *variableTable, stack<Variable> *variableStack);

};

#endif
