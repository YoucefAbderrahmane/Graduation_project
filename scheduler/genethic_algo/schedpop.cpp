/*
 * schedpop.cpp
 *
 *  Created on: 24 juin 2016
 *      Author: root
 */

#include "schedpop.h"

namespace pagmo {

sched_pop::sched_pop(const problem::base &p, int n, const boost::uint32_t &seed) : population(p, n, seed) {
	// TODO Auto-generated constructor stub
	//m_prob.get()->
}

sched_pop::~sched_pop() {
	// TODO Auto-generated destructor stub
}

void sched_pop::reinit(const size_type &idx)
{

}

} /* namespace algorithm */
