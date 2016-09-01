/*
 * MyNSGA.cpp
 *
 *  Created on: 12 juil. 2016
 *      Author: root
 */

#include "MyNSGA.h"

MyNSGA::MyNSGA() {
	// TODO Auto-generated constructor stub

}

MyNSGA::~MyNSGA() {
	// TODO Auto-generated destructor stub
}


chromosome MyNSGA::tournamentSelection(int individu1, int individu2, population p)
{
	if(p.get_individual(individu1).pareto_rank < p.get_individual(individu2).pareto_rank) return p.get_individual(individu1);
	if(p.get_individual(individu1).pareto_rank > p.get_individual(individu2).pareto_rank) return p.get_individual(individu2);
	if(p.get_individual(individu1).crowding_dist > p.get_individual(individu2).crowding_dist) return p.get_individual(individu1);
	if(p.get_individual(individu1).crowding_dist < p.get_individual(individu2).crowding_dist) return p.get_individual(individu2);
	if(p.get_individual(individu1).violation_ratio < p.get_individual(individu2).violation_ratio) return p.get_individual(individu1);
	if(p.get_individual(individu1).violation_ratio > p.get_individual(individu2).violation_ratio) return p.get_individual(individu2);
	return ((rand()/RAND_MAX) < 0.5 ?  p.get_individual(individu1) : p.get_individual(individu2));
}

void MyNSGA::crossover(chromosome *enf1, chromosome *enf2, chromosome p1, chromosome p2)
{
	if(rand()/RAND_MAX < 0.5)
	{
		int size = p1.genes.size();
		if(rand()/RAND_MAX < 1/3){// one point crossover

			int i = rand() % size + 1;//make sure to have at least one gene changed
			int k =0;
			for(k =0; k < i; k++)
			{
				enf1->genes.push_back(p1.genes[k]);
				enf2->genes.push_back(p2.genes[k]);

			}
			for(k=1; k< size; k++)
			{
				enf1->genes.push_back(p2.genes[k]);
				enf2->genes.push_back(p1.genes[k]);
			}
		}
		if(1/3 < rand()/RAND_MAX && rand()/RAND_MAX < 2/3)//uniform crossover
		{
			int u =0 ;
			for(int i =0; i < size; i++)
			{
				u = rand()/RAND_MAX;
				if( u < 0.5)
				{
					enf1->genes.push_back(p1.genes[i]);
					enf2->genes.push_back(p2.genes[i]);
				}
				else {
					enf1->genes.push_back(p2.genes[i]);
					enf2->genes.push_back(p1.genes[i]);
				}
			}
		}
		if( rand()/RAND_MAX < 1)//two points crossover
		{
			int i =0, j=0;
			i = rand() % (size-2) + 1;
			j = i + rand() % (size-i);
			for(int k =0; k < i; k++)
			{
				enf1->genes.push_back(p1.genes[k]);
				enf2->genes.push_back(p2.genes[k]);
			}
			for(int k =i; k< j; k++)
			{
				enf1->genes.push_back(p2.genes[k]);
				enf2->genes.push_back(p1.genes[k]);

			}
			for(int k =j; k < size; k++)
			{
				enf1->genes.push_back(p1.genes[k]);
				enf2->genes.push_back(p2.genes[k]);

			}
		}
	}
	}

void MyNSGA::mutation(chromosome *c, chromosome *cr, population p)
{
	if(rand()/RAND_MAX < 0.5)
	{
		int i = rand()% (c->genes.size());
		c->genes[i].setRandomSelection(rand()/RAND_MAX);

		i = rand()% (c->genes.size());
		cr->genes[i].setRandomSelection(rand()/RAND_MAX);
	}
	if(rand()/RAND_MAX < 0.5)
		{
		int i = rand()% (c->genes.size());
		double start = p.schedule.getConditions()->getNightHorizon().start;
		double h = p.schedule.getConditions()->getNightHorizon().start - p.schedule.getConditions()->getNightHorizon().end;

		c->genes[i].setStartDate(start + rand()/RAND_MAX * h);

		i = rand()% (c->genes.size());

		cr->genes[i].setStartDate( start + rand()/RAND_MAX * h);
		}
	if(rand()/RAND_MAX < 0.5)
		{
			int i = rand() % N_TELESCOPE;
			c->genes[i].setTelescopeUsed(i);

			i = rand() % N_TELESCOPE;
			cr->genes[i].setTelescopeUsed(i);

		}
}

void MyNSGA::nsga2(population *p)
{
	int NP = p->get_size();
	std::vector<int> v1(NP), v2(NP);
	chromosome parent1, parent2, enfant1, enfant2;
	for(int i=0;i < NP; i++)
	{
		v1[i] = i;
		v2[i] = i;
	}
	for(int g =0; g< this->nb_gen; g++)
	{
		p->update_pareto_information(); // update fronts and ranks
		p->updateViolation();
		population  cr(p->getIndividuals(),p->get_size()); //Initialization de R
		std::random_shuffle(v1.begin(),v1.end()); //shuffle the vectors to pick the parents randomly
		std::random_shuffle(v2.begin(),v2.end());
		//unsigned seed = std::

		for(int i =0; i < NP; i+=4)
		{
			parent1 = tournamentSelection(v1[i],v1[i+1],*p);
			parent2 = tournamentSelection(v1[i+2],v1[i+3],*p);
			crossover(&enfant1,&enfant2,parent1,parent2);
			mutation(&enfant1, &enfant2, *p);
			cr.repair(&enfant1);
			cr.repair(&enfant2);

			//maj des fitness contrainte et dom
			enfant1.compute_obj_func();
			enfant1.updateViolationRatio();
			cr.addIndividual(enfant1);
			cr.update_dom(cr.get_size()-1);

			enfant2.compute_obj_func();
			enfant2.updateViolationRatio();
			cr.addIndividual(enfant2);
			cr.update_dom(cr.get_size()-1);

			parent1 = tournamentSelection(v2[i],v2[i+1],*p);
			parent2 = tournamentSelection(v2[i+2],v2[i+3],*p);
			crossover(&enfant1,&enfant2,parent1,parent2);
			mutation(&enfant1, &enfant2, *p);
			cr.repair(&enfant1);
			cr.repair(&enfant2);
			//maj des fitness contrainte et dom
			enfant1.compute_obj_func();
			enfant1.updateViolationRatio();
			cr.addIndividual(enfant1);
			cr.update_dom(cr.get_size()-1);


			enfant2.compute_obj_func();
			enfant2.updateViolationRatio();
			cr.addIndividual(enfant2);
			cr.update_dom(cr.get_size()-1);

		}
		//trier et choisir les meilleurs -> pareto information + tri + sélectionnner les NP premier
		cr.bestIndividuals(NP);
		p->setIndividuals(cr.getIndividuals());
	}
}
