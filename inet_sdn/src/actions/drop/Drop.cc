/**
 * @file	Drop.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "Drop.h"


Drop::Drop() : ActionBase(action_t::DROP)
{
	// the packet filter matching is a sufficient condition to perform the Drop action, further infos are useless
	involvedLayer = NONE_LAYER;
}


Drop::~Drop()
{
}


void Drop::execute(cMessage** packet) const
{
	delete *packet;
	*packet = nullptr;
}
