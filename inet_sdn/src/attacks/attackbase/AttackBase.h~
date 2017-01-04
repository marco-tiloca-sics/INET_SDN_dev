/**
 * @file AttackBase.h
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief AttackBase class provides the representation of a generic attack. An attack is a sequence of actions.
 */
 

#ifndef ATTACKBASE_H
#define ATTACKBASE_H


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>

#include "ActionBase.h"
#include "Variable.h"


using namespace std;


/**
 * @brief Type of action
 */
enum class attack_t {
	PHYSICAL = 0,
	CONDITIONAL,
	UNCONDITIONAL
};


class AttackBase {

	private:
		// type of attack
		attack_t attackType;
		
	protected:
		// sequence of actions to execute during the attack
		vector<ActionBase*> actions;	
		// table of variables
		map<string, Variable*> variableTable;
        // stack for operations
		stack<Variable> variableStack;

	public:
		/** @brief Constructor */
		AttackBase(const attack_t attackType);
		
		/** @brief Destructor */
		virtual ~AttackBase();		
	
        /** @brief Add an action to the attack */
		virtual void addAction(ActionBase* action);
	
		/** @brief Add variable table */
		void initializeVariableTable(const map<string,Variable*> variableTable);

		/** @brief Return the attack type. */
        attack_t getAttackType();

        /** @brief Get an action composing the attack */
        ActionBase* getAction(size_t index) const;

};


/**
 * @brief util functions
 */
string to_string(const attack_t type);
attack_t to_attack_type(const string type); 

#endif
