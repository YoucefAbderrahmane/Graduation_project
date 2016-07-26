/*
 * Schedule.cpp
 *
 *  Created on: 10 mai 2016
 *      Author: dell
 */

#include <stdlib.h>
#include <time.h>
#include <iterator>

#include "config.h"
#include "Schedule.h"
#include <libnova/solar.h>
#include <libnova/julian_day.h>
#include "../utilities/time_calculation.h"

class Request;
class Target;


//// HELPING FUNCTIONS
//
//double addSecondsToJD(double JD, double seconds){
//
//	//convert JD to date
//	time_t * d = new time_t();
//	ln_get_timet_from_julian(JD, d);
//
//	//Add seconds to d
//	struct tm d_tm = *localtime(d);
//	d_tm.tm_sec += seconds;
//
//	//normalize tm and convert it to time_t
//	time_t new_d = mktime(&d_tm);
//
//	//return the equivalent julian day
//	return ln_get_julian_from_timet (&new_d);
//}


Schedule::Schedule() : observations(),
		observations_length(),
		teles_alloc_matrix(),
		teles_length(),
		total_duration(),
		conditions(){
	// TODO Auto-generated constructor stub

	conditions = new Obs_conditions();
}

Schedule::Schedule(vector<Observation> observations,
		int observation_length,
		int teles_length) : observations(observations),
				observations_length(observation_length),
				teles_length(teles_length),
				total_duration(),
				conditions(){

	conditions = new Obs_conditions();

	const int width = observation_length;
	const int height = N_TELESCOPE;

	this->teles_alloc_matrix = new int*[width];
	for(int i = 0; i < teles_length; i++){

		teles_alloc_matrix[i] = new int[height];
	}
}

Schedule::Schedule(vector<Observation> observations, 
	int observation_length, 
	int teles_length, 
	double jd, //which represent the date of the schedule
	Obs_conditions * conditions) : observations(observations),
		observations_length(observation_length),
		teles_alloc_matrix(),
		teles_length(teles_length),
		total_duration(),
		conditions(conditions) {

	this->conditions = new Obs_conditions(jd);
}

Schedule::~Schedule() {
	// TODO Auto-generated destructor stub
}


const vector<Observation>& Schedule::getObservations() const {
	return observations;
}

void Schedule::setObservations(const vector<Observation>& observations) {
	this->observations = observations;
}


double Schedule::getTotalDuration() const {
	return total_duration;
}

void Schedule::setTotalDuration(double totalDuration) {
	total_duration = totalDuration;
}


int Schedule::getObservationsLength() const {
	return observations_length;
}

void Schedule::setObservationsLength(int observationsLength) {
	observations_length = observationsLength;
}

int** Schedule::getTelesAllocMatrix() const {
	return teles_alloc_matrix;
}

void Schedule::setTelesAllocMatrix(int** telesAllocMatrix) {
	teles_alloc_matrix = telesAllocMatrix;
}

int Schedule::getTelesLength() const {
	return teles_length;
}

void Schedule::setTelesLength(int telesLength) {
	teles_length = telesLength;
}


double Schedule::calculateTelescopeDuration(int telescope){

	double teles_duration = 0;

	for( int i = 0; i < observations_length ; i++ ){

		if( teles_alloc_matrix[i][telescope] == 1 ){

			teles_duration += observations.at(i).getDuration();
		}
	}

	return teles_duration;
}


double Schedule::calculateTotalDuration() {

	double total = calculateTelescopeDuration(0);;
	double t_duration;

	for( int i = 1; i < teles_length; i++ ){

		t_duration = calculateTelescopeDuration(i);
		if( t_duration > total ){

			total = t_duration;
		}
	}

	return total;
}

int Schedule::targetGenerator(Target * target){

	bool stop = false;
	int circump;

	//Target generation
	do {
		double dec = 0.0;
		double ra = 0.0;

		double ra_range = 360.0 - 0.0;
		double dec_range = 90.0 - (-90.0);

		double div = RAND_MAX / dec_range;
		dec = -90.0 + rand() / div;

		div = RAND_MAX / ra_range;
		ra = rand() / div;

		target->setEqDec(dec);
		target->setEqRAsc(ra);

		circump = target->get_rise_set_transit(conditions->night_horizon.start,
				OBSERVATORY_HORIZON,
				&conditions->observer);

		if( circump != -1 ) stop = true;

	} while (!stop);

	return SUCCESS;
}


int Schedule::timeConstraintGenerator(time_interval * requested){


	double time_const = (double) rand() / (double) RAND_MAX;

	if( time_const <= TIME_CONST_RATIO ){

		//randomly generating start time
		double range = conditions->night_horizon.end - conditions->night_horizon.start;
		double div = RAND_MAX / range;

		requested->start = conditions->night_horizon.start + rand() / div;

		//randomly generating start time second bound
		range = conditions->night_horizon.end - requested->start;
		div = RAND_MAX / range;
		requested->end = requested->start + rand() / div;

		return SUCCESS;
	}

	return FAILURE;
}


int Schedule::heightConstraintGenerator(double * min_height){

	double height_const = (double) rand() / (double) RAND_MAX;
	if( height_const <= MIN_HEIGHT_RATIO ){

		double range = 90.0 - 0.0;
		double div = RAND_MAX / range;
		*min_height = rand() / div;

		return SUCCESS;
	}

	return FAILURE;
}

int Schedule::moonDistConstraintGenerator(double * min_moon_dist){

	double moon_const = (double) rand() / (double) RAND_MAX;
	if( moon_const <= MIN_MOON_DIST_RATIO ){

		double range = 180.0 - MOON_DISK;
		double div = RAND_MAX / range;
		*min_moon_dist = rand() / div;

		return SUCCESS;
	}

	return FAILURE;
}

int Schedule::observationRequestGenerator(Request * request){

	int isTimeConstrainted = 1;
	int isHeightConstrainted = 1;
	int isMoonConstrainted = 1;

	//srand(time(NULL));

	//Target Generation...must be observable
	Target * target = new Target();
	targetGenerator(target);

	//randomly generating exposure time
	int exp_range = MAX_EXPOSURE - MIN_EXPOSURE;
	int exp_div = RAND_MAX / exp_range;
	int exposure = MIN_EXPOSURE + rand() / exp_div;

	//time constraint generation
	time_interval * requested = new time_interval();
	isTimeConstrainted = timeConstraintGenerator(requested);

	//min height constraint generation
	double min_height = 0;
	isHeightConstrainted = heightConstraintGenerator(&min_height);

	//min moon constraint generation
	double min_moon_dist = MOON_DISK;
	isMoonConstrainted = moonDistConstraintGenerator(&min_moon_dist);

	//helping variables...
	double start = requested->start;
	double duration = requested->end - requested->start;

	//Generating the request's observations
	for(int i = 1; i <= request->getLength(); i++){

		Observation * obs = new Observation(request, i, *target, (double) exposure, conditions);
		obs->setTimeConst(isTimeConstrainted);

		obs->setHeightConst(isHeightConstrainted);
		if( !isHeightConstrainted )
			obs->setMinHeight(min_height);

		obs->setMoonConst(isMoonConstrainted);
		if( !isMoonConstrainted )
			obs->setMoonMinSeparation(min_moon_dist);

		requested->start = addSecondsToJD(start, (i-1)*(request->getPeriod()));
		requested->end = requested->start + duration; //in JD

		if( requested->end > conditions->night_horizon.end ){

			request->setLength(i-1);
			break;
		}

		obs->setReqTime(*requested);
		request->addObservation(*obs);
	}

	return SUCCESS;
}

int Schedule::singularRequestGenerator(Request * request){

	int obs_length = 1;
	int period = 0;


	//srand(time(NULL));
	double multi_obs = (double) rand() / (double) RAND_MAX;
	if( multi_obs < PERIODIC_RATIO ){

		obs_length = rand() % MAX_OBS_REQ;
		period = (rand() % MAX_PERIOD) + MIN_PERIOD;
	}

	request->setLength(obs_length);
	request->setPeriod(period);

	//Generating priority
	request->setPriority((double)  ((rand() % MAX_PRIO) + 1.0) / 10.0);

	//generating observations
	//...
	observationRequestGenerator(request);

	return SUCCESS;
}

int Schedule::randomObservationListGenerator(int request_length) {


	srand(time(NULL));

	if( conditions->allSet == 1){
		conditions->calculateNightHorizon();
	}
	else{

		double JD = ln_get_julian_from_sys ();
		conditions = new Obs_conditions(JD);
	}

	Request * request;
	for(int i = 1; i <= request_length; i++){

		//request and observation generation
		request = new Request(i);
		singularRequestGenerator(request);

		std::vector<Observation> obss = request->getObservations();
		for(vector<Observation>::size_type i = 0; i < obss.size(); i++ ){

			observations.push_back(obss[i]);
		}

//		delete request;
	}

	return SUCCESS;
}

int Schedule::randomObservationAllocation(){

	srand(time(NULL));
	double selected;

	for(int i = 0; i < observations_length; i++){

		selected = (double) rand() / (double) RAND_MAX;
		if( selected < (observations[i].getRequest()->getPriority()) ){

			observations[i].setTaken(0);

			//allocate a random telescope
			//...


			//move on to the next observation
			//...
		}
	}

	return SUCCESS;
}

Obs_conditions* Schedule::getConditions() const {
	return conditions;
}

void Schedule::checkObservations() {

	//Use this function to check if observations are generated correctly
	//This function will become useless when the number of observation is too high

	std::cout << "Number of generated observations : " << observations.size() << std::endl;
	std::cout << "Julian day : " << fixed << conditions->JD << std::endl;
	std::cout << "Observer's position : " << "lat " << conditions->observer.lat
			<< " - lng " << conditions->observer.lng << std::endl;
	std::cout << "Night horizon : " << fixed << conditions->night_horizon.start << " - " << fixed << conditions->night_horizon.end <<std::endl;

	for(int i = 0; i < (int) observations.size(); i++){

		std::cout << "Observation " << i << std::endl;
		std::cout << "Obs req id : " << observations[i].getRequest()->getReqId() << std::endl;
		std::cout << "Obs id : " << observations[i].getObsId()<< std::endl;
		std::cout << "Obs priority : " << observations[i].getRequest()->getPriority()<< std::endl;
		std::cout << "Obs exp time : " << fixed << observations[i].getExposureTime() << std::endl;
		std::cout << "Obs min h : " << fixed << observations[i].getMinHeight() << std::endl;
		std::cout << "Obs min moon : " << fixed << observations[i].getMoonMinSeparation() << std::endl;
		std::cout << "Obs req time : " << fixed << observations[i].getReqTime().start << " - "
				<< fixed << observations[i].getReqTime().end << std::endl;
		std::cout << "Obs target : " << "dec " << fixed << observations[i].getTarget().getEqDec() << " - "
				<< "ra " << fixed << observations[i].getTarget().getEqRAsc() << std::endl;
		std::cout << endl;
	}
}

void Schedule::setConditions(Obs_conditions * conditions) {
	this->conditions = conditions;
}

std::vector<Observation> * Schedule::getObs() {

	return &observations;
}
