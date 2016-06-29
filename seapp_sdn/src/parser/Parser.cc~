/**
 * @file	Parser.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "Parser.h"

#include <stdlib.h>
#include <map>

#include "PhysicalAttack.h"
#include "ConditionalAttack.h"
#include "UnconditionalAttack.h"

#include "Destroy.h"
#include "Move.h"
#include "Drop.h"
#include "Clone.h"
#include "Change.h"
#include "Send.h"
#include "Put.h"
#include "Retrieve.h"
#include "SEAPPExpression.h"
#include "Create.h"

#include "Variable.h"


Parser::Parser(cModule* node)
{
	this->node = node;	
}


Parser::~Parser()
{
}


void Parser::parseConfigurationFile(const attack_t attackTypeTarget, vector<AttackEntry*>& attackEntries)
{
	string msg ("Parser::parse invoked to find '" + to_string(attackTypeTarget) + "' attacks");
	EV_INFO << msg << endl;
	
	string xmlTagTarget = to_string(attackTypeTarget);
	
	// TODO precheck if the nodeID is in the xmlConfigurationFile
	
	string configurationFileName = (( node -> getParentModule()) -> par("attackConfigurationFile")).stringValue();

	if (configurationFileName == "none") {
		EV_WARN << "Parser::parseConfigurationFile hasn't found the configuration file" << endl;
		return;
	}
	
	msg.clear();
	msg.append("Parser::parseConfigurationFile has found the configuration file '" + configurationFileName + "'");
	EV_INFO << msg << endl;
	
	// parsing engine
	try {
		xmlpp::DomParser parser;

		parser.parse_file(configurationFileName);
		
		// retrieve the root node
		xmlpp::Node* rootNode = parser.get_document() -> get_root_node();
	
		string xmlTagRoot = rootNode -> get_name();
		msg.clear();
		msg.append("Parser::parse has found the level 0 (root) node '" + xmlTagRoot + "'");
		EV_INFO << msg <<endl;
	
		// retrieve childrens of root node, i.e. attack type tags
		xmlpp::Node::NodeList nodeListLevel1 = rootNode -> get_children();
	
		for (xmlpp::Node::NodeList::iterator iter = nodeListLevel1.begin(); iter != nodeListLevel1.end(); ++iter) {
		
			xmlpp::Node* nodeLevel1 = *iter;
			string xmlTagLevel1 = nodeLevel1 -> get_name();
			
			msg.clear();
			msg.append("Parser::parse has found a level 1 node '" + xmlTagLevel1 + "'");
			EV_INFO << msg <<endl;
						
			// if the tag parsed is equal to xmlTagTarget then build the Attack object, else ignore it
			if (xmlTagLevel1 == xmlTagTarget) {
			
				xmlpp::Node::NodeList nodeListLevel2 = nodeLevel1 -> get_children();
				
				for (xmlpp::Node::NodeList::iterator subIter = nodeListLevel2.begin(); subIter != nodeListLevel2.end(); ++subIter) {
				
					SimTime occurrenceTime;
					AttackBase* attack;
					
					xmlpp::Node* nodeLevel2 = *subIter;
					
					string xmlTagLevel2 = nodeLevel2 -> get_name();
					tag_t tagLevel2 = to_tag_t(xmlTagLevel2);
					
					msg.clear();
					msg.append("Parser::parse has found a level 2 node '" + xmlTagLevel2 + "'");
					EV_INFO << msg <<endl;
					
					
					// skip text tag
					if (tagLevel2 == tag_t::TEXT) {
						EV_INFO << "Parser::parse has discarded the 'text' node just found" << endl;
						continue;
					}
				
					// create the attack object
					switch (attackTypeTarget) {
					
						case attack_t::PHYSICAL: {
							attack = new PhysicalAttack();
							EV_INFO << "Parser::parse has created a PhysicalAttack object" << endl;
							break;
						}
					
						case attack_t::CONDITIONAL: {
							attack = new ConditionalAttack();
							EV_INFO << "Parser::parse has created a ConditionalAttack object" << endl;
							break;
						}
					
						case attack_t::UNCONDITIONAL: {
							attack = new UnconditionalAttack();
							EV_INFO << "Parser::parse has created a UnconditionalAttack object" << endl;
							break;
						}				
				
					}
					
					// initialize the attack object just created
					bool isInitialized = false;
					isInitialized = initializeAttack(nodeLevel2, attack, occurrenceTime);
					
					if (isInitialized) {
						AttackEntry* attackEntry = new AttackEntry(attack, occurrenceTime);
						attackEntries.push_back(attackEntry);
						EV_INFO << "attack initialized" << endl;
					}
					else {
						delete attack;
						EV_INFO << "Parser::parse can't initialize the attack object so it was deleted" << endl;
					}
				
				}
			
			}
			else {
				msg.clear();
				msg.append("Parser::parse has discarded the '" + xmlTagLevel1 + "' node just found");
				EV_INFO << msg << endl;
			}
			
		}
	
	}
	catch (const exception& exc) {
		EV_ERROR << "Exception caught " << string(exc.what()) << endl;
	}
	
}


bool Parser::initializeAttack (const xmlpp::Node* nodeLevel2, AttackBase* attack, SimTime& occurrenceTime) {

	string msg("Parser::initializeAttack invoked");
	EV_INFO << msg << endl;

	bool isInvolved = false;		// if true the node takes part in the attack as victim or performer
	bool isGlobalFilter = false;	// if true the calling node is the global filter
	
	string startTime;

	map<string,Variable*> variableTable;

	variableTable["SELF"] = new Variable(to_string(node->getId()), variable_t::NUMBER);
	variableTable["MIN"] = new Variable("100", variable_t::NUMBER);
	variableTable["MAX"] = new Variable("0", variable_t::NUMBER);
	 
	// check if the calling node is the global filter
	string className = node->getClassName();
	if (className == "GlobalFilter"){
		isGlobalFilter = true;
	}
	
	xmlpp::Node::NodeList nodeListLevel3 = nodeLevel2 -> get_children();
	
	// Retrieve attack details
	for( xmlpp::Node::NodeList::iterator iter = nodeListLevel3.begin(); iter != nodeListLevel3.end(); ++iter ){
	
		xmlpp::Node* nodeLevel3 = *iter;
		string xmlTagLevel3 = nodeLevel3 -> get_name();
		tag_t tagLevel3 = to_tag_t(xmlTagLevel3);
		
		msg.clear();
		msg.append("Parser::initializeAttack has found a level 3 node '" + xmlTagLevel3 + "'");
		EV_INFO << msg << endl;
		
		switch(tagLevel3){
		
			case tag_t::TEXT:{
				EV_INFO << "Parser::initializeAttack has discarded the 'text' node just found" << endl;
				break;
			}
		
			case tag_t::START_TIME:{
				startTime = get_node_value(nodeLevel3);		
				EV_INFO << "Parser::initializeAttack founded a 'start_time' tag" << endl;
				break;
			}
			
			// packet filter (used only in conditional attacks)
			case tag_t::FILTER:{
				((ConditionalAttack*) attack)->setPacketFilter( get_node_value(nodeLevel3) );
				EV_INFO << "Parser::initializeAttack founded a 'filter' tag" << endl;
				break;
			}
			
			// attack frequency (used only in unconditional attacks)
			case tag_t::FREQUENCY:{
				double frequency = atof( get_node_value(nodeLevel3).c_str() );
				((UnconditionalAttack*)attack)->setFrequency(frequency);
				EV_INFO << "Parser::initializeAttack founded a 'frequency' tag" << endl;
				break;
			}
			
			// nodes involved in the attack as victim and/or performer (used only in physical and conditional attacks)
			case tag_t::NODE:{
				
				vector<string> involvedNodes;
				tokenize(involvedNodes, string(get_node_value(nodeLevel3)), ':' );
				
				// check if node is involved in the attack
				for( size_t i = 0; i < involvedNodes.size(); i++ ){
				
					if( to_string(node->getId()) == involvedNodes[i] ){
						
						isInvolved = true;
						
						msg.clear();
						msg.append("Parser::initializeAttack the node " + involvedNodes[i]);
						msg.append(" is involved in the attack");
						EV_INFO << msg << endl;
				
					}
				
				}
				
				break;
			}
		
		
			// variable
			case tag_t::VAR:{
			
				xmlpp::Node::NodeList nodeListLevel4 = nodeLevel3->get_children();

				string variableName;
				string variableValue;
				variable_t variableType;
				
				// retrieve variable details
				for( xmlpp::Node::NodeList::iterator varIter = nodeListLevel4.begin(); varIter != nodeListLevel4.end(); ++varIter ){
				
					xmlpp::Node* nodeLevel4 = *varIter;
					
					string xmlTagLevel4 = nodeLevel4 -> get_name();
					tag_t tagLevel4 = to_tag_t(xmlTagLevel4);
					
					switch(tagLevel4){
					
						case tag_t::NAME:{
							variableName = get_node_value(nodeLevel4);
							break;
						}
						
						case tag_t::VALUE:{
							variableValue = get_node_value(nodeLevel4);
							break;
						}
						
						case tag_t::TYPE:{
							variableType = to_variable_t( get_node_value(nodeLevel4) );	
							break;
						}
					
					}
				
				}
				
				msg.clear();
				msg.append("Parser::initializeAttack found the variable '" + variableName + "'");
				EV_INFO << msg << endl;
				
				// add theretrieved variable in the varTable
				variableTable[variableName] = new Variable(variableValue, variableType);
				break;
			}
			
			
			// retrieve action details, parameters and item
			case tag_t::ACTION:{
				xmlpp::Node::NodeList nodeListLevel4 = nodeLevel3 -> get_children();
				
				action_t actionType;
				string actionParams;

				vector<string> tokens;
				vector<string> expressionItems;
			
				// action details -- scan level 4
				for(xmlpp::Node::NodeList::iterator actionIter = nodeListLevel4.begin(); actionIter != nodeListLevel4.end(); ++actionIter){
				
					xmlpp::Node* nodeLevel4 = *actionIter;
					
					string xmlTagLevel4 = nodeLevel4 -> get_name();
					tag_t tagLevel4 = to_tag_t (xmlTagLevel4);
					
					switch(tagLevel4){
					
						case tag_t::NAME:{
							actionType = to_action_t (get_node_value(nodeLevel4));
							break;
						}
					
						case tag_t::PARAMETERS:{
							actionParams = get_node_value(nodeLevel4);
							break;
						}
					
						case tag_t::ITEM:{
							expressionItems.push_back(get_node_value(nodeLevel4));
							break;
						}				
					
					}
				
				}

				tokenize(tokens, actionParams, ':');
					
				// retrieve action specific parameters
				switch(actionType){
					
					// Destroy
					case action_t::DESTROY: {	
						Destroy* destroy = new Destroy(node);
						attack -> addAction(destroy);
							
						msg.clear();
						msg.append("Parser::initializeAttack has added a destroy action to the vector PhysicalAttack::action");
						EV_INFO << msg << endl;
						
						break;
					}
						
					// Move
					case action_t::MOVE: {
						double x = (atof) (tokens[0].c_str());
						double y = (atof) (tokens[1].c_str());
						double z = (atof) (tokens[2].c_str());
						
						Coord targetPosition(x, y, z);
							
						Move* move = new Move(node, targetPosition);
						attack->addAction(move);
							
						msg.clear();
						msg.append("Parser::initializeAttack has added a move action to the vector ConditionalAttacks");
						EV_INFO << msg << endl;
							
						break;
					}
					
					// Drop
					case action_t::DROP: {
						Drop* drop = new Drop();
						drop->setPacketName(tokens[1]);
						attack->addAction(drop);
						
						msg.clear();
						msg.append("Parser::initializeAttack has added a Drop action to the vector ConditionalAttacks");
						EV_INFO << msg << endl;

						break;
					}
						
					// Clone
					case action_t::CLONE: {	
						string originalPacketName;
						string clonePacketName;
						
						for (size_t i = 0; i < tokens.size(); i++) {
							if (tokens[i] == "packetName" ) {
								originalPacketName.assign(tokens[i+1]);
							}
							if (tokens[i] == "newPacketName") {
									clonePacketName.assign(tokens[i+1]);
							}	
						}
						
						Clone* clone = new Clone(clonePacketName, originalPacketName);
						attack->addAction(clone);
						
						msg.clear();
						msg.append("Parser::initializeAttack has added a Clone action to the vector ConditionalAttacks");
						EV_INFO << msg << endl;
						
						break;
					}
					
					// Change
					case action_t::CHANGE: {
						string fieldName;
						string value;
						string packetName;
						
						for (size_t i = 0; i < tokens.size(); i++) {
							if (tokens[i] == "field_name") {
								fieldName.assign(tokens[i+1]);
							}
							if (tokens[i] == "value") {
								value.assign(tokens[i+1]);
							}
							if (tokens[i] == "packetName") {
								packetName.assign(tokens[i+1]);
							}
						}

						Change *change = new Change(fieldName, value);
						change->setPacketName(packetName);
						attack->addAction(change);
						
						msg.clear();
						msg.append("Parser::initializeAttack has added a Change action to the vector ConditionalAttacks");
						EV_INFO << msg << endl;

						break;
					}
					
					// Send
					case action_t::SEND: {
						double delay;
						string packetName;

						for (size_t i=0; i < tokens.size(); i++) {
							if (tokens[i] == "delay") {
								delay = atof( (tokens[i+1]).c_str() );
							}
							if (tokens[i] == "packetName") {
								packetName = tokens[i+1];
							}
						}
				
						Send* send = new Send(delay);
						send->setPacketName(packetName);
				
						attack->addAction(send);
					
						msg.clear();
						msg.append("Parser::initializeAttack has added a Send action to the vector ConditionalAttacks");
						EV_INFO << msg << endl;
					
						break;
					}
					
					// Put
					case action_t::PUT: {
					
						direction_t direction;
						vector<string> tokenizedRecipientNodes;						
						bool isStatUpdated = false;
						double forwardingDelay;
						string packetName;

						for (size_t i = 0; i < tokens.size(); i++) {
						
							if (tokens[i] == "direction") {
								if (tokens[i+1] == "TX"){
									direction = direction_t::TX;
								}
								else {
									direction = direction_t::RX;
								}
							}
						
							if (tokens[i] == "nodes") {
								tokenize(tokenizedRecipientNodes, tokens[i+1], '|');
							}
							
							if (tokens[i] == "delay") {
								forwardingDelay = atof((tokens[i+1]).c_str());
							}
							
							if (tokens[i] == "throughWC") {
								if ((tokens[i+1] == "true") || (tokens[i+1] == "TRUE")) {
									isStatUpdated = true;
								}
							}
							
							if (tokens[i] == "packetName") {
								packetName.assign(tokens[i+1]);
							}
						
						}
						
						Put* put = new Put(direction, forwardingDelay, isStatUpdated);
						put->setPacketName(packetName);
						
						// populate the list of recipient nodes
						for (size_t i = 0; i < tokenizedRecipientNodes.size(); i++) {
							put->addRecipientNode(atoi(tokenizedRecipientNodes[i].c_str()));
						}

						attack->addAction(put);
					
						break;
					}
					
					// Retrieve
					case action_t::RETRIEVE: {
					
						string packetName;
						string fields;
						string variableName;

						for (size_t i=0; i < tokens.size() ; i++) {
							if (tokens[i] == "packetName") {
								packetName = tokens[i+1];
							}
							if (tokens[i] == "field_name") {
								fields = tokens[i+1];
							}
							if (tokens[i] == "varName") {
								variableName = tokens[i+1];
							}					
						}
						
						Retrieve* retrieve = new Retrieve(fields, variableName);
						retrieve->setPacketName(packetName);
						attack->addAction(retrieve);
						
						break;
					}
					
					// Expression
					case action_t::EXPRESSION: {
					
						SEAPPExpression* expression = new SEAPPExpression(expressionItems);
						attack->addAction(expression);
					
						break;
					}

					// Create
					case action_t::CREATE: {
					
						string packetName;
						string layer5Type = "-1";
						string layer4Type = "-1";
						string layer3Type = "-1";
						string layer2Type = "-1";
					
						for (size_t i = 0; i < tokens.size(); i++) {
						
							if (tokens[i] == "packetName") {
								packetName.assign(tokens[i+1]);
							}
							
							if (tokens[i] == "APP.type") {
								layer5Type.assign(tokens[i+1]);
							}
					
							if(tokens[i] == "TRA.type") {
								layer4Type.assign(tokens[i+1]);
							}

							if(tokens[i] == "NET.type") {
								layer3Type.assign(tokens[i+1]);
							}
					
							if(tokens[i] == "MAC.type") {
								layer2Type.assign(tokens[i+1]);
							}
							
						}
						
						Create* create = new Create(layer5Type, layer4Type, layer3Type, layer2Type);
						create->setPacketName(packetName);
						attack->addAction(create);
					
						break;
					}
					
				}
					 				
				break;
			} // end action	
		
		}
	
	} // end for to retrieve attack detail

	attack -> initializeVariableTable(variableTable);
	
	if( isInvolved || isGlobalFilter ) {
		occurrenceTime = atol( startTime.c_str() );
		return true;
	}
	else{
		return false;
	}

}


string to_sting (const tag_t type) {

	switch (type) {
	
		case tag_t::TEXT: {
			return "text";
		}
		
		case tag_t::ATTACK: {
			return "Attack";
		}
	
		case tag_t::START_TIME: {
			return "start_time";
		}
		
		case tag_t::FILTER: {
			return "filter";
		}
		
		case tag_t::FREQUENCY: {
			return "frequency";
		}
		
		case tag_t::NODE: {
			return "node";
		}
		
		case tag_t::VAR: {
			return "var";
		}
		
		case tag_t::NAME: {
			return "name";
		}
		
		case tag_t::VALUE: {
			return "value";
		}
		
		case tag_t::TYPE: {
			return "type";
		}
		
		case tag_t::STRING: {
			return "STRING";
		}
		
		case tag_t::NUMBER: {
			return "NUMBER";
		}
		
		case tag_t::NONE: {
			return "NONE";
		}
		
		case tag_t::ACTION: {
			return "action";
		}
		
		case tag_t::PARAMETERS: {
			return "parameters";
		}
		
		case tag_t::ITEM: {
			return "item";
		}
	
		// add new tags here to extend the parser
	
	}
	
	opp_error("Function to_string (from type_t to string) doesn't recognize the argument");
	
}


tag_t to_tag_t(const string type){

	if (type == "action") {
		return tag_t::ACTION;
	}
	
	if (type == "Attack") {
		return tag_t::ATTACK;
	}
	
	if (type == "filter") {
		return tag_t::FILTER;
	}
	
	if (type == "frequency") {
		return tag_t::FREQUENCY;
	}

	if (type == "item") {
		return tag_t::ITEM;
	}

	if (type == "name") {
		return tag_t::NAME;
	}

	if (type == "node") {
		return tag_t::NODE;
	}
	
	if (type == "NONE") {
		return tag_t::NONE;
	}	
	
	if (type == "NUMBER") {
		return tag_t::NUMBER;
	}
	
	if (type == "parameters") {
		return tag_t::PARAMETERS;
	}
	
	if (type == "start_time") {
		return tag_t::START_TIME;
	}
	
	if (type == "STRING") {
		return tag_t::STRING;
	}

	if (type == "text") {
		return tag_t::TEXT;
	}

	if (type == "type") {
		return tag_t::TYPE;
	}
	
	if (type == "value") {
		return tag_t::VALUE;
	}
	
	if (type == "var") {
		return tag_t::VAR;
	}

	// add new tags here to extend the parser

	opp_error("Function to_tag_t (from string to tag_t) doesn't recognize the argument");
	
}


string get_node_value (const xmlpp::Node* node) {

	xmlpp::Node::NodeList values = node->get_children();

	const xmlpp::TextNode* textNode = dynamic_cast<const xmlpp::TextNode*>(values.front());

	if (textNode) {
		return textNode->get_content();
	}
	else {
		return "";
	}

}

