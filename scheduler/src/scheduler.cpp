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
struct crowding_d_comp {
	crowding_d_comp (population *p):ps(p){};
	bool operator()(int  c1, int  c2){

			return (ps->get_individual(c1).getCrowdingDist() < ps->get_individual(c2).getCrowdingDist());
		}
	population *ps;
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

	sched->randomObservationListGenerator(10);

	cout << "Initializing the population..." << endl;

	population * p = new population(20, *sched);
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
	/*test si shuffle de deux vecteurs successifs est différent
	vector<int> v1, v2;
	for(int i =0; i < 10; i++)
		{v1.push_back(i);
		v2.push_back(i);
		}
	random_shuffle(v1.begin(),v1.end());
	random_shuffle(v2.begin(),v2.end());
	for(vector<int>::iterator i = v1.begin(); i < v1.end(); i++)
	{
		cout<< *i << endl;
	}
	cout<< "_________________________________"<< endl;
	for(vector<int>::iterator i = v2.begin(); i < v2.end(); i++)
		{
			cout<< *i << endl;
		}*/
//test pareto
	std::vector<chromosome *>  front;
	std::vector<chromosome *> point;
	//front = p->getIndividualsPointer();
	for(int i =0; i < p->getIndividuals().size(); i++)
	{
		front.push_back(p->getIndividualPointeur(i));

	}
	cout<< "taille de pop "<< p->get_size()<< endl;
	//for (int i = 0; i< p->get_size();i++) 	p->update_dom(i);
	p->update_pareto_information();
	for(int i=0; i < p->get_size();i++) cout<< " "<< p->get_individual(i).getDomCount();
	cout<< "\naffichage des fronts";
	p->displayFronts();
	for(int y =0; y < front.size(); y++)
		{
			cout<<"\nI am individual "<< y << " and here are my component:"<< front[y]->get_obj_func(0)<< " "<< front[y]->get_obj_func(1)<< " "<< front[y]->get_obj_func(2)<< " "<< front[y]->get_obj_func(3)<<endl;
		}
	vector<double> ideal = p->compute_nadir();
	cout<< "taille de ideal dans pop "<< ideal.size()<< endl;
	for(int i =0; i< PROB_DIM; i++)
	{
		cout<<ideal[i]<< "  ";
	}
	cout<< endl;
	cout<< "taille de pop	"<< front.size()<< endl;
	for(int i=0; i< p->get_size(); i++)
	{
		cout<< "individu "<< i << " domination cout = "<< p->get_individual(i).getDomCount()<< endl;
	}

	for (int i = 0; i< p->get_size();i++) 	p->update_dom(i);
	p->update_pareto_information();
	cout<< "_______________________________________________________________________________________"<< endl;
	for(int j =1; j < p->get_size();j++)
	{
		int k = p->compare_fitness(  p->get_individual(0).getF(), p->get_individual(j).getF()    );
		cout<< "compare fitness de 0 avec "<< j<< " donne "<< k << endl;
	}
	for(int i=0; i< p->get_size(); i++)
		{
			cout<< "individu "<< i << " domination cout = "<< p->get_individual(i).getDomCount()<< endl;
		}
	cout<< "_______________________________________________________________________________________"<< endl;
	p->displayFronts();

	//test crowding_distance
	p->update_crowding_dist(front);
	for(int i =0; i <p->getIndividuals().size(); i++)
	{
		cout << "crowding distance de  " << i << "	" << p->get_individual(i).getCrowdingDist()<< endl;
	}
	crowding_d_comp c(p);
	vector<int> vv;


		vv = p->getFronts()[0];
		sort(vv.begin(),vv.end(),c);
		for(int j =0; j < vv.size(); j++)
		{
		cout<< "nombre d'individus dans le fronts est : "<< p->get_individual(vv[j]).getCrowdingDist()<< endl;
		}

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
//test fonction pareto
	/*
	//front[1].setDomCount(2); ok
	//cout << front[1].getDomCount(); ok
	//front[1].incrementDomCount(); ok
	//front[1].decrementDomCount(); ok
	//p->update_dom(i); //ok
	//obj_fct_comp f(0); ok
	//p->update_crowding_dist(front); ok
	//p->update_pareto_information(); ok*/

	//various tests
		/*/(*front)[0].setDomCount(1500);

	//p->displayFronts();

			//p->getIndividualPointeur(0)->setDomCount(1500);

			//(p->getIndividualPointeur(0))->setCrowdingDist(120);

			//std::vector<chromosome>::iterator i = p->getIndividuals().begin();
			for(std::vector<chromosome>::iterator i = p->getIndividuals().begin(); i < p->getIndividuals().end(); i++)
			{
				front.insert(i,p->get_individual(i));
			}*/

//	Target RigilKentaurus(219.8795833, -60.81952778);
//
//	Target * Aldebaran = new Target(68.98041667, 16.50841667);
	cout<< "fin"<< endl;
	return 0;
}

