/*
 * repairalgo.cpp
 *
 *  Created on: 3 juil. 2016
 *      Author: root
 */

#include "repairalgo.h"

namespace pagmo {
namespace algorithm {

repair_algo::repair_algo() : base() {
	// TODO Auto-generated constructor stub

}

repair_algo::~repair_algo() {
	// TODO Auto-generated destructor stub
}

void repair_algo::evolve(population &p) const{

	/* repair the only individual of the population */

	//Get the individual to repair
	population::individual_type individual = p.get_individual(0);

	//Get the number of observations

	//1- Repairing C1, an observation must be allocated once to a given telescope


	//2- Repairing C2, a non chosen observation must not be allocated to any telescope

	//3- The schedule of each telescope must be withing the night horizon

	//4- All allocated observation on each telescope must not overlap
}

base_ptr repair_algo::clone() const {

	return base_ptr(new repair_algo(*this));
}

} /* namespace algorithm */

}
