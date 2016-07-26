/*
 * gene.cpp
 *
 *  Created on: 16 juil. 2016
 *      Author: root
 */

#include "gene.h"
#include "../utilities/time_calculation.h"

gene::gene() : index(), random_selection(), start_date(), duration(), end_date(), telescope_used() {
	// TODO Auto-generated constructor stub

}


gene::gene(int index) : index(index), random_selection(), start_date(), duration(), end_date(), telescope_used() {
	// TODO Auto-generated constructor stub

}

gene::~gene() {
	// TODO Auto-generated destructor stub
}

bool gene::is_scheduled(Observation observation) {

	return ( random_selection < observation.getRequest()->getPriority());
}

//Observation* gene::getObs() {
//
//	return observation;
//}

double gene::get_end_time() {

	if( end_date > 0.0 ) end_date = addSecondsToJD(start_date, duration);
	return end_date;
}

double gene::get_start_time() {
	return start_date;
}
