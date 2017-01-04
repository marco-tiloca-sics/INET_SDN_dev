/**
 * @file Destroy.h
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief Destroy class extends ActionBase. Destroy class makes possible the destruction of a node.
 */


#ifndef DESTROY_H
#define DESTROY_H


#include "ActionBase.h"


class Destroy : public ActionBase {

	private:
		cModule* targetNode;

	public:
	
		/**
		 * @brief Constructor.
		 * @param isDestroyed Pointer-to the bool variable isDestroyed of a LocalFilter object.
		 */
		Destroy(cModule* targetNode);
		
		/**
		 * @brief Destructor.
		 */
		virtual ~Destroy();

		/**
		 * @brief Destroy the node.
		 */
		void execute() const;
};

#endif
