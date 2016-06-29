/**
 * @file	UnconditionalAttack.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */
 

#ifndef UNCONDITIONALATTACK_H
#define UNCONDITIONALATTACK_H


#include "AttackBase.h"


using namespace std;


class UnconditionalAttack : public AttackBase {

	private:
        // TODO change in period
        // frequency of the unconditional attack
		double frequency;

	public:
		/** 
		 * @brief	Constructor
		 */
		UnconditionalAttack();
		
		/** 
		 * @brief	Destructor
		 */
		virtual ~UnconditionalAttack();
		
		/** 
		 * @brief 	sets frequency attribute
		 */
		void setFrequency(double frequency);

		/** 
		 * @brief 	returns the frequency attribute
		 */
		double getFrequency() const;

		/** 
		 * @brief 	Add an action to the unconditional attack
		 * @detail	The action is added in the base class's attribute action
		 */
		virtual void addAction(ActionBase* action);

		/** 
		 * @brief 	executes the unconditional attacks
		 */
		void execute(vector<cMessage*> &putMessages);

};

#endif
