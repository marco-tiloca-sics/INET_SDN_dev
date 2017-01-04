/**
 * @file Drop.h
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief Drop packets.
 */


#ifndef DROP_H
#define DROP_H


#include "ActionBase.h"


class Drop : public ActionBase {

	public:
	
		/**
		 * @brief Constructor
		 */
		Drop();
		
		/**
		 * @brief Destructor.
		 */
		virtual ~Drop();

		/**
		 * @brief Execute the drop action.
		 */
		void execute(cMessage** packet) const;
};

#endif
