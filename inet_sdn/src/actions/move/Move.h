/**
 * @file	Destroy.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief	Move class makes possible the misplace of a node.
 */



#ifndef MOVE_H
#define MOVE_H

#include "ActionBase.h"
#include "Coord.h"


class Move : public ActionBase{

	private:
		// new position of the node
        Coord targetPosition; 
        // node that will be misplaced
		cModule* targetNode;
		
	public:
		/**
		 * @brief Constructor.
		 * @param targetNode Pointer-to the target node module (which may contain the mobility sub-module)
		 * @param targetPosition Is the target position
		 */
		Move(cModule* targetNode, Coord targetPosition);
		
		/**
		 * @brief Destructor.
		 */
		virtual ~Move();

		/**
		 * @brief Force the new position of the node.
		 */
		void execute() const;
		
};

#endif
