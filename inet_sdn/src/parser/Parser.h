/**
 * @file	Parser.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief	Parser class makes possible to parse the xml file that contains the attack description.
 * @details	Parser class relies on the xml++ library. It is used by LocalFilter modules and the GlobalFilter module, 
 *			to initialize the data structures that are used to perform physical, conditional and unconditional attacks.
 */


#ifndef PARSE_H
#define PARSE_H


#include <iostream>
#include "omnetpp.h"
#include <libxml++/libxml++.h>
#include <cstdint>
#include <string>
#include <vector>

#include "INETDefs.h"

#include "seapputils.h"

#include "AttackBase.h"
#include "AttackEntry.h"


using namespace std;


// TODO remove this
// declare LocalFilter class to avoid cyclic dependency
class LocalFilter;


// tag types
enum class tag_t : uint8_t{
	ACTION = 0,
	ATTACK,
	FILTER,
	FREQUENCY,
	ITEM,
	NAME,
	NODE,
	NONE,
	NUMBER,	
	PARAMETERS,
	START_TIME,
	STRING,
	TEXT,
	TYPE,
	VALUE,
	VAR
};


class Parser{

	private:
		// owner of the parser
        cModule* node;

	private:
		/**
		 * @brief	Initialize an Attack object
		 */
		bool initializeAttack(const xmlpp::Node* nodeLevel2, AttackBase* attack, SimTime& occurrenceTime);
		
	public:
		/**
		 * @brief	Constructor 
		 * @param	TODO
		 */
		Parser(cModule* node);
	
		/** @brief	Destructor */
		virtual ~Parser();
	
		/** 
		 * @brief 	Parse the xml configuration file to find tags corresponding to attackType parameter
		 * @param	attakType 
		 *
		 * @detail	Convert the attakType parameter to string by using the seapputil function toString() and 
		 *			parses the xml configuration file to search the corresponding tag
		 */
		void parseConfigurationFile(const attack_t attackType, vector<AttackEntry*>& attackEntries);

};


string get_node_value(const xmlpp::Node* node); 
string to_string(const tag_t type);
tag_t to_tag_t(const string type);

#endif
