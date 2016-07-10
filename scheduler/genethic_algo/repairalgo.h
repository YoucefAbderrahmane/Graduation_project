/*
 * repairalgo.h
 *
 *  Created on: 3 juil. 2016
 *      Author: root
 */

#ifndef REPAIRALGO_H_
#define REPAIRALGO_H_

#include <config.h>
#include <serialization.h>
#include <types.h>
#include <algorithm/base.h>

namespace pagmo {
namespace algorithm {

class repair_algo : public base {
public:
	repair_algo();
	virtual ~repair_algo();

	void evolve(population &p) const;

	base_ptr clone() const;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int)
	{
	    ar & boost::serialization::base_object<base>(*this);
	}
};

} /* namespace algorithm */
} /* namespace pagmo */

#endif /* REPAIRALGO_H_ */
