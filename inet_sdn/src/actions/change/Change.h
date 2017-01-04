/**
 * @file	Change.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief	Change modifies the value contained in a specified packet field.
 * @details	Change modifies the value contained in a specified packet field. The ASL format is LAYER.FIELD_NAME (or PROTOCOL.FIELD_NAME) 
 *			e.g. APP.sourceNodeID or UDP.sourcePort
 */


#ifndef CHANGE_H
#define CHANGE_H


#include "ActionBase.h"


class Change : public ActionBase {
	private:
		string externalInfo;
		string fieldName;
		string value;

	private:
		void executeOnField(cMessage** packetToChange, string value);
		void executeOnExternalInfo(cMessage** packetToChange, string value);

	public:
		/**
		 * @brief Constructor
		 */
		Change(const string fieldName, const string value);
		
		/**
		 * @brief Destructor
		 */
		virtual ~Change();

		/**
		 * @brief returns the name of the field
		 */
		 string getFieldName() const;
		
		/**
		 * @brief returns the new value of the field
		 */
		 string getValue() const;

		/**
		 * @brief execute the change action
		 */
		void execute(cMessage** packetToChange, string value);
		// <A.S>
		void execute(cMessage **packetToChange, cMessage **payload);
};

#endif
