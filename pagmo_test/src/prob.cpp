/*
 * prob.cpp
 *
 *  Created on: 17 juin 2016
 *      Author: root
 */

#include "prob.h"

namespace pagmo { namespace problem {

	prob::prob(unsigned int dim) : base(dim) {
	// TODO Auto-generated constructor stub

	}

	prob::~prob() {
	// TODO Auto-generated destructor stub
	}

base_ptr prob::clone() const {

	return base_ptr(new prob(*this));
}

void prob::objfun_impl(fitness_vector& f, const decision_vector& x) const {
}

}
}

