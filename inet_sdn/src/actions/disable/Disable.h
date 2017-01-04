/**
 * @file	Disable.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 *
 * @brief	Disable class extends ActionBase. Disable class disables the communication module of a node.
 */


#ifndef DISABLE_H
#define DISABLE_H


#include "ActionBase.h"
#include "NodeRelease.h"

class Disable : public ActionBase {

	private:
		// pointer to the target node (i.e. the node to disable)
		cModule* targetNode;

	public:
		/**
		 * @brief   Constructor.
		 * @param   targetNode, is the pointer-to the node to disable.
		 */
        Disable(cModule* targetNode);
		virtual ~Disable();

		/**
		 * @brief   Disables the (communication module of the) node.
		 */
		void execute() const;
};

#endif
