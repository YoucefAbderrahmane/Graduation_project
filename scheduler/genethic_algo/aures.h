/*
 * aures.h
 *
 *  Created on: 12 juil. 2016
 *      Author: root
 */

#ifndef AURES_H_
#define AURES_H_

#include <string>
#include <algorithm>

#include <config.h>
#include <serialization.h>
#include <types.h>
#include <problem/base.h>

#include "../Model/Observation.h"
#include "../Model/config.h"

namespace pagmo { namespace problem {

class __PAGMO_VISIBLE aures : public base{
public:
	//aures();
	aures(std::vector<Observation> observations, int observation_length, int teles_length);
	virtual ~aures();

	base_ptr clone() const;
	void objfun_impl(fitness_vector &, const decision_vector &) const;
	void compute_constraints_impl(constraint_vector &c, const decision_vector& x) const;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int)
	{
	    ar & boost::serialization::base_object<base>(*this);
	    ar & observations;
	    ar & observations_length;
	    ar & telescopes_used;
	    ar & teles_length;
	    ar & night_horizon;
	    ar & bounds;
	    ar & affectations;
	    ar & total_duration;
	}

	int get_decv_index(int mat_i, int mat_j) const;
	void get_mat_index(int& mat_i, int& mat_j, int dec_ind);

	mutable std::vector<Observation> observations; //Liste des requêtes observables durant la nuit
	int observations_length; //
	mutable std::vector<int> telescopes_used;
	int teles_length;
	time_interval night_horizon;
	time_interval bounds;
	mutable std::vector<int> affectations;
	double total_duration;
};

} /* namespace problem */
} /* namespace pagmo */

#endif /* AURES_H_ */