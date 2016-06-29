/**
 * @file seapputils.h
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief This file provides utility functions used by INET modules related to attacks evaluation.
 */
 
 
#ifndef SEAPPUTILS_H
#define SEAPPUTILS_H


#include <iostream>
#include <vector>
#include <string>
#include <omnetpp.h>


using namespace std;

/**
 * @brief Splits the string 'str' upon every occurrence of char 'delim'.
 * @param tokens Is the vector of string(s) in which the original string is splitted and stored.
 * @param str Is the string to tokenize.
 * @param delim Is the delimiter character.
 */ 

void tokenize(vector<string>& tokens, const string str, const char delim);

/**
 * @brief Splits the string 'str' upon every occurrence of char 'delim'.
 * @param str Is the string to tokenize.
 * @param delim Is the delimiter character.
 * @return Returns a vector of strings containing the tokens.
 */ 
vector<string> tokenize(const string str, const char delim);

/**
 * @brief Get the deepest encapsulated packet
 */
cPacket* getDeepestEncapsulatedPacket(cPacket* packet);

/**
 * @brief Get the layer of a packet
 */
int getPacketLayer(cPacket* packet);

/**
 * @brief Convert the layer's name (string) to int
 */
int layertoi(const string layer);

/**
 * @brief Set recursively a packet's parameter (and its encapsulated ones)
 */
void setParameterRecursively(cMessage* msg, const string parameterName, const bool parameterValue);

/**
 * @brief Makes an hard copy of a packet.
 * @details	Makes an hard copy of the packetToCopy by creating a perfect copy of the (outer) packet and of all its encapsulated packets.
 * @param 	packetToCopy it's the packet to be copied
 * @return	the pointer to the (new) copied packet, or nullptr if problems occur
 */
cPacket* hardCopy(cPacket* packetToCopy);

/**
 * @brief Compare two elements by means of a specified operator.
 *        Comapared elements must support such operators.
 *  @param field1	is the first parameter
 *  @param field2	is the second parameter
 *  @param oper		is the comparison operator (supported : =, !=,
 */
template<class T>
bool evaluate( T f1, T f2, const string comp_operator) {

	if (comp_operator.compare("==") && comp_operator.compare("!=") && comp_operator.compare(">=") && comp_operator.compare("<=") && comp_operator.compare(">") && comp_operator.compare("<")) {

		EV<<"Operator not supported."<<endl;
		return false;

	}

	if (comp_operator == "==") {
		return f1 == f2;
    }
	
    if (comp_operator == "!=") {
		return f1 != f2;
    }
    
	if (comp_operator == ">") {
		return f1 > f2;
    }
    
	if (comp_operator == "<") {
		return f1 < f2;
    }
		
	if (comp_operator == ">=" || comp_operator == "=>") {
		return f1 >= f2;
    }

	if (comp_operator == "<=" || comp_operator == "=<") {
		return f1 <= f2;
    }
}

/**
 * @brief Evaluate arithmetic expressions between two elements 
 *        by means of a specified operator. The compared elements 
 *        must support such operators.
 * @param field1 Is the first parameter
 * @param field2 Is the second parameter
 * @param oper Is the comparison operator
 */

template<class T>
float evaluateArithmetic(T f1, T f2, const string comp_operator) {

	if (comp_operator.compare("+") && comp_operator.compare("-") && comp_operator.compare("*") && comp_operator.compare("/") && comp_operator.compare("%")) {
		EV << "Operator not supported" <<endl;
		return false;
	}

	if (comp_operator == "+") {
		return (f1 + f2);
	}

	if (comp_operator == "-") {
		return (f1 - f2);
	}
			
	if (comp_operator == "*") {
		return (f1 * f2);
	}
    
    if (comp_operator == "%") {
		return ((int)f1 % (int)f2);
	}
    
}


#endif
