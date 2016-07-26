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

#include <libnova/libnova.h>

#include "../Model/Observation.h"
#include "../Model/Schedule.h"
#include "../Model/Request.h"
#include "../Model/Target.h"

#include "../genethic_algorithm/chromosome.h"
#include "../genethic_algorithm/gene.h"
#include "../genethic_algorithm/population.h"


using namespace std;


int main() {

//	Target vega(279.2347, 38.7836);
//	Target kochab(222.676375, 74.1555);
//	ln_equ_posn * pos = new ln_equ_posn();
//	pos = vega.getEqCord();
//	cout << kochab.getAngularDistance(pos) << endl;

	Schedule * sched = new Schedule();

	sched->randomObservationListGenerator(20);

	cout << "Initializing the population..." << endl;

	population * p = new population(20, *sched);

	cout << "Population initialized..." << endl;

	p->check_init();

	p->get_individual(0)->compute_constraints();

	cout << p->get_individual(0)->get_cst_val(0);

//	sched->checkObservations();



//	chromosome * ch = new chromosome(sched->getObs());
//
//	cout << "fknfi," << endl;
//
//
//	ch->checkObservations();
//	ch->compute_obj_func();
//
//	cout << "fknfi," << endl;
//	cout << fixed << ch->get_obj_func(0) << endl;
//	cout << fixed << ch->get_obj_func(1) << endl;
//	cout << fixed << ch->get_obj_func(2) << endl;
//	cout << fixed << ch->get_obj_func(3) << endl;




//
//	Target RigilKentaurus(219.8795833, -60.81952778);
//
//	Target * Aldebaran = new Target(68.98041667, 16.50841667);

	return 0;
}

