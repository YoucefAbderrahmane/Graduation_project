/*
 * population.cpp
 *
 *  Created on: 16 juil. 2016
 *      Author: root
 */

#include "population.h"

#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include "../Model/config.h"
#include "../utilities/time_calculation.h"

population::population(int population_size, Schedule schedule) : individuals(), population_size(population_size),
		champions(), schedule(schedule)
{
	// TODO Auto-generated constructor stub

	this->individuals.reserve(population_size);
	for(int i = 0; i < population_size; i++){
		individuals.push_back(* new chromosome(*schedule.getObs()));
	}
	init();
}

population::population(std::vector<chromosome> individuals, int population_size) : individuals(individuals),
		population_size(population_size), champions()
{
	// TODO Auto-generated constructor stub
	this->individuals = individuals;
}

population::~population() {
	// TODO Auto-generated destructor stub
}

void population::init() {

	srand(time(NULL));


	//initialize all individuals of the population
	std::vector<gene>::size_type size = individuals.size();
	std::cout<<"__________________size of individuals"<< size<<std::endl;
	std::vector<chromosome>::size_type ind = 0;
	while( ind < size){

		std::cout<<"I am individual ->"<< ind <<std::endl;

		init_individual((int)ind);
		//individuals[i]->compute_obj_func();
		ind++;

		std::cout << "index is now = " << ind << "and size is = " << size << std::endl;
	}

}

void population::init_individual(int index) {

	//individuals[index] = new chromosome(schedule.getObs());

	std::cout << "index = " << index << std::endl;

	//initialize the genes of the chromosome
	std::vector<gene>::size_type size = individuals.at(index).genes.size();

	std::cout << "size " << size << std::endl;

	for(std::vector<gene>::size_type i = 0; i < size; i++){

		init_gene(i, index);
	}

	individuals.at(index).compute_obj_func();
	this->update_dom(index);

}

void population::init_gene(int gene_index, int individual_index) {

	double inf = 0.0;	//the lower bound for the random number
	double sup = 0.0;	//the upper bound for the random number
	double range = 0.0; //needs to be equal to: sup - inf
	double div = 0.0;	//the divisor needed to generate a random number

	//generate the probability of selecting the observation
	sup = 1.0;
	range = sup - inf;
	div = RAND_MAX / range;
	individuals.at(individual_index).genes.at(gene_index).random_selection = inf + rand() / div;

	//generate a random start date within the night horizon
	inf = schedule.getConditions()->getNightHorizon().start;
	sup = schedule.getConditions()->getNightHorizon().end;
	range = sup - inf;
	div = RAND_MAX / range;
	individuals.at(individual_index).genes[gene_index].start_date = inf + rand() / div;
	individuals.at(individual_index).genes[gene_index].duration = schedule.getObs()->at(gene_index).getDuration();

	//randomly chose the allocated telescope
	int low = 0;
	int up = N_TELESCOPE - 1;
	individuals[individual_index].genes[gene_index].telescope_used = rand() % up + low;

}

void population::check_init() {

	//randomly picking an individual from the population
	int i = rand() % individuals.size();

	//check the first, last and a random gene
	check_gene(0, i);

	std::cout << std::endl;
	std::cout << std::endl;

	int gene = rand() % individuals[0].genes.size();
	check_gene(gene, i);

	std::cout << std::endl;
	std::cout << std::endl;

	gene = individuals[i].genes.size() - 1;
	check_gene(gene, i);

	std::cout << std::endl;
	std::cout << std::endl;
}

chromosome  population::get_individual(int chromosome_index) {

	return individuals[chromosome_index];
}

chromosome * population::getIndividualPointeur( int index)
{
	return &individuals[index];}

std::vector<chromosome> * population::getIndividualsPointer() {
		return &individuals;
	}


struct gene_less_comp{

	bool operator()(gene * g1, gene * g2){

		return (g1->get_start_time() < g2->get_start_time());
	}
};

void population::repair(chromosome * individual) {

	//Organize observation by telescope
	std::vector< std::vector<gene *> > obs_per_tel;
	obs_per_tel.resize(N_TELESCOPE);

	for(std::vector<gene>::size_type i = 0; i < individual->genes.size(); i++){

		obs_per_tel[individual->genes[i].telescope_used].push_back(&individual->genes[i]);
	}

	//Sort all generated vectors of observations
	for(std::vector< std::vector<gene> >::size_type i = 0; i < obs_per_tel.size(); i++){

		std::sort(obs_per_tel[i].begin(), obs_per_tel[i].end(), gene_less_comp());
		repair_vect_obs(&obs_per_tel[i]);
	}
}

void population::repair_vect_obs(std::vector<gene *>* genes) {

	int done = 0;
	double lower_bound = schedule.getConditions()->getNightHorizon().start;
	double upper_bound = 0.0;
	double range = 0.0;
	double div = 0.0;

	for(std::vector<gene *>::size_type i = 0; i < genes->size() - 1; i++){

		if( genes->at(i)->get_end_time() > genes->at(i+1)->get_start_time() ){
			//There is an overlapping

			//1- Try to translate the observation i to the left in the timeline
				//This is possible if:
					// lower_bound + duration(i) < end(i+1)

			if( addSecondsToJD(lower_bound, genes->at(i)->duration) < genes->at(i+1)->get_start_time() ){

				//
				upper_bound = addSecondsToJD(genes->at(i+1)->get_start_time(), -genes->at(i)->duration);

				//Randomly generate the new start time between lower_bound and upper_bound
				range = upper_bound - lower_bound;
				div = RAND_MAX / range;
				genes->at(i)->start_date = lower_bound + rand() / div;

				done = 1;
			}

			//2- If 1 is not possible, try to translate observation i+1 to the right

			else if(done == 0){

				if( addSecondsToJD(genes->at(i)->get_end_time(), genes->at(i+1)->duration) < upper_bound ){

					if(i <= genes->size()-3) upper_bound = genes->at(i+2)->get_start_time();
					else upper_bound = schedule.getConditions()->getNightHorizon().end;

					lower_bound = genes->at(i)->get_end_time();
					upper_bound = addSecondsToJD(upper_bound, -genes->at(i+1)->duration);

					//Randomly generate the new start time between lower_bound and upper_bound
					range = upper_bound - lower_bound;
					div = RAND_MAX / range;
					genes->at(i+1)->start_date = lower_bound + rand() / div;

					done = 1;
				}
			}

			//3- Unschedule the observation with the lowest priority
			if(done == 0){
				if(schedule.getObservations().at(genes->at(i)->index).getRequest()->getPriority()
						> schedule.getObservations().at(genes->at(i+1)->index).getRequest()->getPriority()){

					genes->at(i+1)->random_selection = 1.0;
					done = 1;
				}
				else if(schedule.getObservations().at(genes->at(i)->index).getRequest()->getPriority()
						< schedule.getObservations().at(genes->at(i+1)->index).getRequest()->getPriority()) {

					genes->at(i)->random_selection = 1.0;
					done = 1;
				}
			}


			//4- Unschedule the observation which is farther from its requested time (or the one that doesn't have one...)
			if(done == 0){

				double dist_i = ((schedule.getObservations().at(genes->at(i)->index).getReqTime().end
						+ schedule.getObservations().at(genes->at(i)->index).getReqTime().start) / 2.0) - genes->at(i)->start_date;

				double dist_next = ((schedule.getObservations().at(genes->at(i+1)->index).getReqTime().end
						+ schedule.getObservations().at(genes->at(i+1)->index).getReqTime().start) / 2.0) - genes->at(i)->start_date;

				if(dist_i > dist_next){

					genes->at(i)->random_selection = 1.0;
				}
				else{

					genes->at(i+1)->random_selection = 1.0;
				}
			}

			done = 0;
		}
	}
}

int population::get_size() {
	return this->population_size;
}
//checks if indivual1 dominates individual2 which objective functions are f1 and f2 respectively
int population::compare_fitness(std::vector<double> f1,
		std::vector<double> f2) {
	int sup = 0;
	for(int i=0; i < (int) f1.size(); i++)
	{
		if(f1[i] > f2 [i]) return 0;
		if(f1[i] < f2[i]) sup =1;
	}
	return sup;
}


void population::check_gene(int gene_idx, int individual_idx) {

	std::cout << "Gene: " << gene_idx << std::endl;
	std::cout << "Gene's selection probability: " << individuals[individual_idx].genes[gene_idx].random_selection << std::endl;
	std::cout << "Gene's start time: " << fixed << individuals[individual_idx].genes[gene_idx].start_date << std::endl;
	std::cout << "Gene's allocated telescope: " << individuals[individual_idx].genes[gene_idx].telescope_used << std::endl;
	std::cout << std::endl;
}
//ok
void population::update_dom(int index) {
	std::cout<< "-------------------start update_dom, index ="<< index<< std::endl;
	int i;
	chromosome * cr = &(this->individuals[index]);
	if(cr->getDomList().size() != 0){
	for(i =0; i < cr->getDomList().size(); i++)// decrease the number of domination count of every dominated individual by index
	{
		this->individuals[cr->getDomListOf(i)].dom_count--;
		//std::cout<< "mise a jour	";
	}
	}
	cr->clearDomList();
	cr->setDomCount(0);

	for(i=0;i < this->population_size;i++)
	{
		std::cout<< "I'm i = "<< i<< std::endl;
		if(i!=index)
		{
			std::cout<< "I'm i and different from index "<< std::endl;
			//check if i dominates index
			if(compare_fitness(this->individuals[i].getF(),cr->getF()))
			{
				std::cout<<"result of the comparison "<< compare_fitness(this->individuals[i].getF(),cr->getF())<< std::endl;
				//update the domination count of index
				cr->incrementDomCount();
				std::cout << "In the first if I am "<< index << " and I'm dominated by  "<< i<< std::endl;
				//check if index exists in the list of dominated individual of i and add it if not
				if(std::find(this->individuals[i].dom_list.begin(),this->individuals[i].dom_list.end(),index)== this->individuals[i].dom_list.end())
				{
					this->individuals[i].dom_list.push_back(index);
				}
			}
			else {
				//remove index from the list of dominated individuals of i
				std::vector<int>::iterator x = std::find(this->individuals[i].dom_list.begin(),this->individuals[i].dom_list.end(),index);
				if(x !=this->individuals[i].dom_list.end())
				{
					this->individuals[i].dom_list.erase(x);
				}
				std::cout<< "got you"<<std::endl;
			}
			//check if index dominates i
		if(compare_fitness(cr->f,this->individuals[i].f))
			{
				cr->dom_list.push_back(i);
				this->individuals[i].dom_count++;
				std::cout<< "I am "<< index<< " and I dominate "<< i<< std::endl;
			}
		}
	}
	cr= NULL;
	delete cr;
	std::cout<< "-------------find de update_dom----------------"<< std::endl;
}
//ok
struct obj_fct_comp {

	obj_fct_comp(int dim):obj_fct(dim) {};
	bool operator()(chromosome * c1, chromosome * c2){

			return (c1->get_obj_func(obj_fct) < c2->get_obj_func(obj_fct));
		}
int obj_fct;
};

//updates the crowding distance of a front ok
void population::update_crowding_dist(std::vector<chromosome *> front) {
	std::vector<chromosome>::size_type taille  = front.size()-1;
	//std::cout<<"je suis taille "<< taille<< std::endl;
	obj_fct_comp f(0);
	double crow = 0.0;
	//std::cout<<"je suis PROB_DIM "<< PROB_DIM << std::endl;

	 //we loop for each objective function
	int tour =0;
	 for (int i = 0; i < PROB_DIM; ++i)
	 {
		 f.obj_fct=i;

		 std::sort(front.begin(),front.end(),f);

		 front.at(0)->setCrowdingDist(std::numeric_limits<double>::max());

		 front.at(taille)->setCrowdingDist(std::numeric_limits<double>::max());

		 //compute crowding distance
	//	 cout<< "valeur de f(" << i << ") de 0 "<< front.at(0)->get_obj_func(i)<< endl;
		// cout<< "valeur de f(" << i << ") de 0 "<< front.at(taille)->get_obj_func(i)<< endl;

		 double diff = front.at(taille)->get_obj_func(i) - front.at(0)->get_obj_func(i);
		 //cout<< diff << endl;


		 for(std::vector<chromosome>::size_type j = 1; j < taille;j++)
		 {
			 if(diff==0.0)
			 {
				 front.at(j)->setCrowdingDist(0.0);
			//	 std::cout<<"crowd a 0"<<std::endl;
			 }
			 else{
				// std::cout<< "valeur "<< (front.at(j+1)->get_obj_func(i) - front.at(j-1)->get_obj_func(i))/diff<<std::endl;
				 //std::cout<< "former crowding distance"<< front.at(j)->getCrowdingDist()<<std::endl;

				 crow = front.at(j)->getCrowdingDist() + (front.at(j+1)->get_obj_func(i) - front.at(j-1)->get_obj_func(i))/diff;

				 front.at(j)->setCrowdingDist(crow);
				 //std::cout<< "current crowdin distance " << crow <<std::endl;
			 }
		 }
		 //std::cout<<"_________________________e tour numéro"<<tour<<std::endl;
		 tour++;
	 }
	 //std::cout<<"fin"<<std::endl;

}

void population::change(){this->individuals[0].setCrowdingDist(1024);}

//have to update dom count before
void population::update_pareto_information() {
	std::vector<int> list_dom_count(this->get_size(),0);
		//we need the index to update the crowding distance in the population
		std::vector<int> index;
		std::vector<chromosome *> front, S;
	for(int i =0; i < this->get_size();i++){
		this->individuals[i].setCrowdingDist(0);
		this->individuals[i].setParetoRank(0);
		list_dom_count[i] = this->individuals[i].getDomCount();

		//find first pareto front
		if(list_dom_count[i]==0) {
			front.push_back(&(this->individuals[i]));
			index.push_back(i);
		}
	}
	//to reinitalize the fronts, because changes can appear during the resolution
	this->fronts.clear();
	unsigned int rank =1;
	//we search for the other fronts
	while(front.size()!=0)
	{
		//update crowding distance of the current  paerto front
		population::update_crowding_dist(front);

		//update crowding distance of the population
		/*for(std::vector<int>::size_type i =0; i < index.size();i++)
		{
			this->individuals[index[i]].setCrowdingDist(front[i].getCrowdingDist());
		}*/

		population::compute_pareto_fronts(index);
		index.clear();
		//for each individual int the front
		for(std::vector<chromosome *>::size_type i=0; i < front.size();i++)
		{
			//for each individual dominated by front[i]
			for(std::vector<double>::size_type j=0; j< front[i]->getDomList().size();j++)
			{
				//decrease the dom_count
				list_dom_count[front[i]->getDomList()[j]]--;
				//check if there isn't an individual that dominates the individual j
				if(list_dom_count[front[i]->getDomList()[j]]==0)
				{
					S.push_back(&(this->individuals[front[i]->getDomList()[j]]));

					this->individuals[front[i]->getDomList()[j]].setParetoRank(rank);

					//save the index of the individuals to update the crowding distance in the population
					index.push_back(front[i]->getDomList()[j]);
				}
			}
		}
		//std::cout<< "test de taille de front "<< front.size()<< endl;
		front = S;
		//std::cout<< "test 2 de taille de front "<< front.size()<< endl;
		//std::cout<< "test de taille de S "<< S.size()<< endl;
		S.clear();
		//std::cout<< "test 2 de taille de S "<< S.size()<< endl;
		rank++;
	}
}

void population::compute_pareto_fronts(std::vector<int> index) {
	fronts.push_back(index);
}

std::vector<double> population::compute_ideal() {
update_pareto_information();

std::vector<double> ideal (PROB_DIM,numeric_limits<double>::max());
for(std::vector<chromosome>::size_type i = 0; i < this->individuals.size();i++ )
{
	if(this->individuals[i].getParetoRank() == 0)
	{
		for(std::vector<double>::size_type j = 0; j < ideal.size(); j++)
		{
			if(this->individuals[i].get_obj_func(j) < ideal[j])
			{
				//std::cout<< "ideal de j "<< j<< " est "<< this->individuals[i].get_obj_func(j)<< std::endl;
				ideal[j] = this->individuals[i].get_obj_func(j);
			}
		}
	}
}
return ideal;
}

std::vector<double> population::compute_nadir() {
	update_pareto_information();

	std::vector<double> nadir (PROB_DIM,- numeric_limits<double>::max());
	for(std::vector<chromosome>::size_type i = 0; i < this->individuals.size();i++ )
	{
		if(this->individuals[i].getParetoRank() == 0)
		{
			for(std::vector<double>::size_type j = 0; j < nadir.size(); j++)
			{
				if(this->individuals[i].get_obj_func(j) > nadir[j])
				{
					nadir[j] = this->individuals[i].get_obj_func(j);
				}
			}
		}
	}
	return nadir;
}

struct crowding_d_comp {
	crowding_d_comp (population *p):ps(p){};
	bool operator()(int  c1, int  c2){

			return (ps->get_individual(c1).getCrowdingDist() < ps->get_individual(c2).getCrowdingDist());
		}
	population *ps;
};
void population::bestIndividuals(int nb_champ) {
	this->update_pareto_information();
	int nb_champions = 0;
	int size_front = 0;
	std::vector<int> v;
	int index;
	for(index = 0; index < this->fronts.size(); index++)
	{
		nb_champions = (int) this->champions.size();
		size_front = (int) this->fronts[index].size();
		if( (nb_champions + size_front) < nb_champ)
		{
			for(int i =0; i < size_front; i++)
			{
				this->champions.push_back(this->individuals[this->fronts[index][i]]);
			}
		}
		else break;
	}
	v = this->fronts[index];
	crowding_d_comp c(this);
	std::sort(v.begin(),v.end(),c);
	int k =1;
	int t = (int) v.size();
	while(nb_champions < nb_champ)
	{
		champions.push_back(this->get_individual(v[t-k]));
		k++;
		nb_champions++;
	}
}

void population::displayFronts()
{
	for(int i = 0; i< this->fronts.size(); i++)
	{
		std::cout<<"liste des individus du front i = "<< i<< std::endl;
		for(int j = 0; j<fronts[i].size(); j++) std::cout<< this->fronts[i][j]<< std::endl;
		std::cout<< "________________________________"<< std::endl;
	}
}

double population::getDomCountOf(int index)
{
	return  this->individuals[index].dom_count;
}

void population::addIndividual(chromosome ch) {
	this->individuals.push_back(ch);
	this->population_size++;
}

int population::getFrontsSize() {
	return (int) this->fronts.size();
}

void population::updateViolation() {
	for(int i = 0; i < population_size; i++)
	{
		individuals[i].updateViolationRatio();
	}
}
