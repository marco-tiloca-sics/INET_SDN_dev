/**
 * @file	AttackBase.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "AttackBase.h"


AttackBase::AttackBase(const attack_t attackType)
{
	this->attackType = attackType;
}


AttackBase::~AttackBase()
{
}		


void AttackBase::initializeVariableTable(const map<string,Variable*> variableTable)
{
	this->variableTable = variableTable;
}


void AttackBase::addAction(ActionBase* action)
{
	actions.push_back(action);
}




string to_string(const attack_t type)
{
	switch(type){
	
		case attack_t::PHYSICAL:{
			return "Physical";
		}
	
		case attack_t::CONDITIONAL:{
			return "Conditional";
		}
			
		case attack_t::UNCONDITIONAL:{
			return "Unconditional";
		}

		default:{
			opp_error("AttackType not recognized during conversion to string");
		}
	
    }
}


attack_t to_attack_type(const string type)
{

	if(type == "Physical"){
		return attack_t::PHYSICAL;
	}
	
	if(type == "Conditional"){
		return attack_t::CONDITIONAL;
	}
	
	if(type == "Unconditional"){
		return attack_t::UNCONDITIONAL;
	}
	
	opp_error("AttackType not recognized during conversion from AttackType to string");

}


