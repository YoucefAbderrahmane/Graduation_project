//============================================================================
// Name        : scheduler.cpp
// Author      : YoucefKhaled
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iterator>

//#include <libnova/julian_day.h>
//#include <solar.h>
//#include <rise_set.h>
#include <libnova/libnova.h>

#include <pagmo.h>
#include <config.h>
#include <serialization.h>
#include <types.h>
#include <problem/base.h>

#include "../Model/Observation.h"
#include "../Model/Schedule.h"
#include "../Model/Request.h"

#include "../genethic_algo/SchedulingProb.h"


using namespace std;


int main() {

	Target vega(279.2347, 38.7836);

	double jd = ln_get_julian_from_sys();

	Request * req = new Request(0);

	Obs_conditions * conditions = new Obs_conditions(jd);

	Observation * obs = new Observation(req, 0, vega, 20.0, conditions);
	obs->setHeightConst(1);
	obs->setMinHeight(45.0);

	double alt_merit = obs->altituteMerit();

//	Schedule * sched = new Schedule();
//	sched->randomObservationListGenerator(10);
//
	ln_date * d = new ln_date();
//
//	ln_get_date(sched->getObservations().at(0).getReqTime().start, d);

	cout << fixed << alt_merit << endl;

//	Target kochab(222.676375, 74.1555);
//
//	Target RigilKentaurus(219.8795833, -60.81952778);
//
//	Target * Aldebaran = new Target(68.98041667, 16.50841667);
//
//	Schedule * test = new Schedule();
//
//	Request * request = new Request(1);
//
//	test->observationRequestGenerator(&request);
//
//		Schedule * sched = new Schedule();
//		sched->randomObservationAllocation();
//		cout << sched->getObservations().at(0).getReqTime().start << endl;

//		Request * request = new Request(0);
//		int obs_length = 1;
//		int period = 0;
//
//		srand(time(NULL));
//		double multi_obs = (double) rand() / (double) RAND_MAX;
//		//multi_obs = 0.1;
//		if( multi_obs < PERIODIC_RATIO ){
//
//			obs_length = rand() % MAX_OBS_REQ;
//			period = (rand() % MAX_PERIOD) + MIN_PERIOD;
//
//			request->setPeriod(period);
//		}
//
//		request->setLength(obs_length);
//
//		//Generating priority
//		request->setPriority((double) ((rand() % MAX_PRIO) + 1.0) / 10.0);
//
//		//generating observations
//		//...
//
//		int isTimeConstrainted = 1;
//			int isHeightConstrainted = 1;
//			int isMoonConstrainted = 1;
//
//			srand(time(NULL));
//
//			//Target Generation...must be observable
//
//
//			//randomly generating exposure time
//			int exp_range = MAX_EXPOSURE - MIN_EXPOSURE;
//			int exp_div = RAND_MAX / exp_range;
//			int exposure = MIN_EXPOSURE + rand() / exp_div;
//			ln_date * ti = new ln_date();
//			ti->seconds = exposure;
//			double julian_exp = ln_get_julian_day(ti);
//
//			//time constraint
//			time_interval * requested = new time_interval();
//			isTimeConstrainted = timeConstraintGenerator(requested);
//			cout << isTimeConstrainted << endl;
//
//			//min height constraint
//			double min_height = 0;
//			isHeightConstrainted = heightConstraintGenerator(&min_height);
//
//			//min moon constraint
//			double min_moon_dist = MOON_DISK;
//			isMoonConstrainted = moonDistConstraintGenerator(&min_moon_dist);
//
//			double first_end = requested->start;
//
//			cout << "length " << fixed << request->getLength() << endl;
//			cout << "period " << fixed << request->getPeriod() << endl;
//
//			for(int j = 1; j <= request->getLength(); j++){
//
//				Observation * obs = new Observation(request, j, vega, julian_exp);
//				obs->setTimeConst(isTimeConstrainted);
//
//				obs->setHeightConst(isHeightConstrainted);
//				if( !isHeightConstrainted )
//					obs->setMinHeight(min_height);
//
//				obs->setMoonConst(isMoonConstrainted);
//				if( !isMoonConstrainted )
//					obs->setMoonMinSeparation(min_moon_dist);
//
//				requested->start = first_end + (j-1)*(request->getPeriod());
//				requested->end = requested->start + julian_exp;
//				obs->setReqTime(*requested);
//				first_end = requested->end;
//
//				request->addObservation(*obs);
//			}
//
//		cout << "start " << fixed << request->getObservations().at(0).getReqTime().start << endl;

	return 0;
}

