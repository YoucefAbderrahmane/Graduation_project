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

#include <algorithm>
#include <limits>
using namespace std;

struct obj_fct_comp {

	obj_fct_comp(int dim):obj_fct(dim) {};
	bool operator()(chromosome * c1, chromosome * c2){

			return (c1->get_obj_func(obj_fct) < c2->get_obj_func(obj_fct));
		}
int obj_fct;
};
int k;
int * get_inter()
{

	return &k;
}

int main() {

//	Target vega(279.2347, 38.7836);
//	Target kochab(222.676375, 74.1555);
//	ln_equ_posn * pos = new ln_equ_posn();
//	pos = vega.getEqCord();
//	cout << kochab.getAngularDistance(pos) << endl;

	Schedule * sched = new Schedule();

	sched->randomObservationListGenerator(3);

	cout << "Initializing the population..." << endl;

	population * p = new population(3, *sched);
//
//	cout << "Population initialized..." << endl;
//
//	p->check_init();
//
//	p->get_individual(0).compute_constraints();
//
//	cout << p->get_individual(0).get_cst_val(0);

//	sched->checkObservations();



	chromosome * ch = new chromosome(*sched->getObs());

	cout << "fknfi," << endl;



	ch->compute_obj_func();


	std::vector<chromosome *>  front;
	std::vector<chromosome *> point;
	//front = p->getIndividualsPointer();
	for(int i =0; i < p->getIndividuals().size(); i++)
	{
		front.push_back(p->getIndividualPointeur(i));

	}
	p->update_crowding_dist(front);
	cout << "crowding distance de 0 " << p->get_individual(0).getCrowdingDist()<< endl;
	cout << "crowding distance de 1 " << p->get_individual(1).getCrowdingDist()<< endl;
	cout << "crowding distance de 2 " << p->get_individual(2).getCrowdingDist()<< endl;
	//(*front)[0].setDomCount(1500);


	//p->getIndividualPointeur(0)->setDomCount(1500);

	//(p->getIndividualPointeur(0))->setCrowdingDist(120);

	//std::vector<chromosome>::iterator i = p->getIndividuals().begin();
	/*for(std::vector<chromosome>::iterator i = p->getIndividuals().begin(); i < p->getIndividuals().end(); i++)
	{
		front.insert(i,p->get_individual(i));
	}*/

// test de f.obj

	/*cout<< f.obj_fct<<endl;
	cout <<"si obj_fct égale 0 :"<< front[0]->get_obj_func(f.obj_fct) << "	"<< front[0]->get_obj_func(0)<<endl;
	cout<<"_______________________________"<<endl;

	for(int y =0; y < front.size(); y++)
	{
		cout<<"I am individual "<< y << " and here are my component:"<< front[y]->get_obj_func(0)<< " "<< front[y]->get_obj_func(1)<< " "<< front[y]->get_obj_func(2)<< " "<< front[y]->get_obj_func(3)<<endl;
	}

	//f.obj_fct++;
	sort(front.begin(),front.end(),f);
	for(int y =0; y < front.size(); y++)
		{
		cout<<"I am individual new "<< y << " and here are my component:"<< front[y]->get_obj_func(0)<< " " << front[y]->get_obj_func(1)<< " "<<front[y]->get_obj_func(2)<< " "<< front[y]->get_obj_func(3) <<endl;
		}*/

	//cout<<front[1].getCrowdingDist()<<endl;


// test de update dom
/*	for(int i =0; i < (*front).size(); i++)
	{
		point.push_back(&(*front)[i]);
		cout<<"je suis l'individu "<< i<< "		";
		p->update_dom(i); //ok
		cout<< endl;
	}


	/*cout<< "dom count "<< point[0]->getDomCount()<< " front aussi "<< (*front)[0].getDomCount()<<endl;
		cout<< "dom count "<< point[1]->getDomCount()<< " front aussi "<< (*front)[1].getDomCount()<<endl;
		cout<< "dom count "<< point[2]->getDomCount()<< " front aussi "<< (*front)[2].getDomCount()<<endl;
	cout<< "___________________"<< endl;
	//p->update_crowding_dist(point); // ok
	for(int i =0; i < (*front).size(); i++)
	{
		point.push_back(&(*front)[i]);
		cout<<"je suis l'individu "<< i<< "		";
		p->update_dom(i); //ok
		cout<< endl;
	}

	cout<< "dom count "<< point[0]->getDomCount()<< " front aussi "<< (*front)[0].getDomCount()<<endl;
	cout<< "dom count "<< point[1]->getDomCount()<< " front aussi "<< (*front)[1].getDomCount()<<endl;
	cout<< "dom count "<< point[2]->getDomCount()<< " front aussi "<< (*front)[2].getDomCount()<<endl;*/
	//cout<< "premier élément de dom_list de 0 "<< point[0]->getDomList()[0]<< endl;
	cout<< "taille de pop	"<< front.size()<< endl;

	//front[1].setDomCount(2); ok
	//cout << front[1].getDomCount(); ok
	//front[1].incrementDomCount(); ok
	//front[1].decrementDomCount(); ok
	//p->update_dom(i); //ok
	//obj_fct_comp f(0); ok

//
//	Target RigilKentaurus(219.8795833, -60.81952778);
//
//	Target * Aldebaran = new Target(68.98041667, 16.50841667);

	return 0;
}

