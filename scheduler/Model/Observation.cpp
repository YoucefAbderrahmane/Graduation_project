/*
 * Observation.cpp
 *
 *  Created on: 3 mai 2016
 *      Author: YoucefKhaled
 */

#include "Observation.h"

#include <stdlib.h>
#include <math.h>

#include "../libnova/libnova/solar.h"
#include "../libnova/libnova/julian_day.h"
#include "../libnova/libnova/transform.h"
#include "config.h"


Observation::~Observation() {
	// TODO Auto-generated destructor stub
}

Observation::Observation(Request * request, 
	int obs_id, Target target, int exposure_time) {

	this->request = request;
	this->obs_id = obs_id;

	this->target = target;
//	this->target.setEqDec(target.getEqDec());
//	this->target.setEqRAsc(target.getEqRAsc());

	this->exposure_time = exposure_time;
	this->moon_min_separation = MOON_DISK;
}

int Observation::getExposureTime() const {
	return exposure_time;
}

void Observation::setExposureTime(int exposureTime) {
	exposure_time = exposureTime;
}

double Observation::getMinHeight() const {
	return min_height;
}

void Observation::setMinHeight(double minHeight) {
	min_height = minHeight;
}

double Observation::getMoonMinSeparation() const {
	return moon_min_separation;
}

void Observation::setMoonMinSeparation(double moonMinSeparation) {
	if( moonMinSeparation > MOON_DISK ) moon_min_separation = moonMinSeparation;
}

int Observation::getObsId() const {
	return obs_id;
}

void Observation::setObsId(int obsId) {
	obs_id = obsId;
}

const Target& Observation::getTarget() const {
	return target;
}

void Observation::setTarget(const Target& target) {
	this->target = target;
}

const struct time_interval& Observation::getReqTime() const {
	return req_time;
}

void Observation::setReqTime(const struct time_interval& reqTime) {
	req_time = reqTime;
}

const struct time_interval& Observation::getSchedTime() const {
	return sched_time;
}

void Observation::setSchedTime(const struct time_interval& schedTime) {
	sched_time = schedTime;
}

int Observation::getTelescope() const {
	return telescope;
}

void Observation::setTelescope(int telescope) {
	this->telescope = telescope;
}

int Observation::getTaken() const {
	return taken;
}

void Observation::setTaken(int taken) {
	this->taken = taken;
}


void Observation::setTimeConst(int isConst){

	constraints[0] = isConst;
}

int Observation::isTimeConst(){

	return constraints[0];
}

void Observation::setHeightConst(int isConst){

	constraints[1] = isConst;
}
int Observation::isHeightConst(){

	return constraints[1];
}

void Observation::setMoonConst(int isConst){

	constraints[2] = isConst;
}

int Observation::isMoonConst(){

	return constraints[2];
}

//*************

int Observation::calculateVisibility(double JD,
		struct ln_lnlat_posn* observer, std::vector<struct time_interval> * visibility) {

	return calculateVisibilityHorizon(JD, observer, OBSERVATORY_HORIZON, visibility);
}

int Observation::calculateVisibilityHorizon(double JD,
		struct ln_lnlat_posn* observer, double horizon, std::vector<struct time_interval> * visibility) {

	struct time_interval night;
	struct time_interval vis;
	struct time_interval unvis;

	int circumpolar;

	/** GET night horizon **/

	//calculate rise and set of the target

	circumpolar = target.get_rise_set_transit(JD, horizon, observer);

	double rise = target.getRiseSetTransit().rise;
	double set = target.getRiseSetTransit().set;

	/** END **/

	if( circumpolar == -1){

		return FAILURE;
	}



	struct ln_rst_time * solar_rst = (struct ln_rst_time *) malloc(sizeof(struct ln_rst_time));;

	ln_get_solar_rst(JD, observer, solar_rst);

	if( JD > solar_rst->rise ){

		struct ln_rst_time * next_solar_rst = (struct ln_rst_time *) malloc(sizeof(struct ln_rst_time));;
		ln_get_solar_rst(JD+1, observer, next_solar_rst);

		night.start = solar_rst->set;
		night.end = next_solar_rst->rise;
	}
	else{

		night.start = JD;
		night.end = solar_rst->rise;
	}


	//if it is always above horizon...
	if( circumpolar == 1){

		vis.start = night.start;
		vis.end = night.end;
		(*visibility).push_back(vis);
		return SUCCESS;
	}


	if( rise < set ){

		if( set < night.start || rise > night.end ){

			(*visibility).push_back(time_interval());
			return FAILURE;
		}

		vis.start = std::max(rise, night.start);
		vis.end = std::min(set, night.end);

		(*visibility).push_back(vis);
		return SUCCESS;
	}
	else{

		if( set > night.end || rise < night.start ){

			vis.start = night.start;
			vis.end = night.end;
			(*visibility).push_back(vis);
			return SUCCESS;
		}

		if( set < night.start && rise > night.end ){

			(*visibility).push_back(vis);
			return FAILURE;
		}

		unvis.start = std::max(set, night.start);
		unvis.end = std::min(rise, night.end);

		if( unvis.start > night.start ){

			vis.start = night.start;
			vis.end = unvis.start;
			(*visibility).push_back(vis);
		}

		if(unvis.end < night.end ){

			vis.start = unvis.end;
			vis.end = night.end;
			(*visibility).push_back(vis);
		}

		return SUCCESS;
	}
}

int Observation::isAboveMinHeight(double JD) {

	//get height of object

	struct ln_equ_posn eq_coord;
	struct ln_hrz_posn horiz_coord;
	struct ln_lnlat_posn position;

	eq_coord.dec = target.getEqDec();
	eq_coord.ra = target.getEqRAsc();

	position.lat = LATITUDE;
	position.lng = LONGITUDE;

	ln_get_hrz_from_equ(&eq_coord, &position, JD, &horiz_coord);

	if( horiz_coord.alt >= getMinHeight() ) return SUCCESS;
	else return FAILURE;
}

double Observation::getDuration() {

	time_interval interval = getSchedTime();
	return interval.end - interval.start;
}

int Observation::isAwayFromMoon(double JD){

	double dist = target.getMoonAngularDistance(JD);

	if( dist > getMoonMinSeparation() ) return SUCCESS; //good
	else return FAILURE; //bad
}

int Observation::isInReqTime() {

	time_interval scheduled = getSchedTime();
	time_interval requested = getReqTime();

	if( scheduled.start >= requested.start*(1 - TIME_MARGIN)
			&& scheduled.end <= requested.end*(1 + TIME_MARGIN) )

		return SUCCESS; //good
	else return FAILURE; //bad
}

int Observation::isOptimalHeight(double JD) {

	double transit = getTarget().getRiseSetTransit().transit;
	time_interval scheduled = getSchedTime();

	if( transit >= scheduled.start && transit <= scheduled.end ) return SUCCESS;
	else return FAILURE;
}

Request* Observation::getRequest() const {
	return request;
}

void Observation::setRequest(Request* request) {
	this->request = request;
}
