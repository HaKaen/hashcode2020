#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<map>
#include<set>
#include<string>

using namespace std;

//Indexed object to easy sort and outputs
class Pizza{
public:
	Pizza(int,int);
	int index_m;
	int nbSlices_m;
	bool isAssigned_m;
};

Pizza::Pizza(int index_p, int nbSlices_p):
index_m(index_p),
nbSlices_m(nbSlices_p),
isAssigned_m(false)
{
}

//Object definition
class PizzaProblem{
public:
	//Constructor
	PizzaProblem();
	~PizzaProblem();
	//Different types, int or bool as examples
	int maxSlices_m;
	int nbPizzas_m;
	//Pizzas in the problem
	vector<Pizza*> pizzas_m;
	//Solution
	vector<Pizza*> pizzasToOrder_m;
	//Optimize
	void optimize();
	//Eval
	int eval();
	//Lecture d'un objet
	friend istream& operator>> (istream&,PizzaProblem&);
	//Ecriture d'un objet
	friend ostream& operator<< (ostream&,const PizzaProblem&);
};

PizzaProblem::PizzaProblem():
maxSlices_m(),
nbPizzas_m(),
pizzas_m(),
pizzasToOrder_m()
{
}

PizzaProblem::~PizzaProblem(){
	for (Pizza* pPizza_l : pizzas_m){
		delete pPizza_l;
	}
}

//Reader from input file
istream& operator>>(istream& is_p, PizzaProblem& pizzaProblem_p){
	is_p >> pizzaProblem_p.maxSlices_m >> pizzaProblem_p.nbPizzas_m;

	for (int i = 0; i < pizzaProblem_p.nbPizzas_m; i++){
		int sliceSize_l;
		is_p >> sliceSize_l;
		Pizza* pNewPizza_l = new Pizza(i,sliceSize_l);
		pizzaProblem_p.pizzas_m.push_back(pNewPizza_l);
	}

	return is_p;
}

//Writer in output file
ostream& operator<<(ostream& os_p, const PizzaProblem& pizzaProblem_p){

	os_p << pizzaProblem_p.pizzasToOrder_m.size() << endl;

	os_p << pizzaProblem_p.pizzasToOrder_m[0]->index_m;

	for (unsigned int i = 1; i < pizzaProblem_p.pizzasToOrder_m.size(); i++){
		os_p << " " << pizzaProblem_p.pizzasToOrder_m[i]->index_m;
	}

	return os_p;
}

int PizzaProblem::eval(){
	int value_l = 0;
	for (Pizza* pPizza_l : pizzasToOrder_m){
		value_l+= pPizza_l->nbSlices_m;
	}
	cout << "eval = " << value_l << "\n";
	return value_l;
}

void opt(PizzaProblem pb_p, int index_p, int max_p){
}

void PizzaProblem::optimize(){
	int sum_l = 0;
	bool continue_l = true;
	//Sort using a lambda
	sort(pizzas_m.begin(),pizzas_m.end(),[](Pizza* p1_p, Pizza* p2_p){return p1_p->nbSlices_m > p2_p->nbSlices_m;});
	//While loop
	while(continue_l){
		continue_l = false;
		for (Pizza* pPizza_l : pizzas_m){
			if (!pPizza_l->isAssigned_m && sum_l + pPizza_l->nbSlices_m < maxSlices_m){
				pPizza_l->isAssigned_m = true;
				pizzasToOrder_m.push_back(pPizza_l);
				sum_l += pPizza_l->nbSlices_m;
				continue_l = true;
			}
		}
	}
}

void readInput(const string& fileName_p,PizzaProblem& pb_p){
        //Open stream
        ifstream myFile_l;
        myFile_l.open(fileName_p.c_str());
        //Read
        myFile_l >> pb_p;
        //Close file
        myFile_l.close();
}

void writeOutput(const string& inputFilename_p, const PizzaProblem& pb_p, int eval_p){
        //Open stream
        ofstream myFile_l;
        string inputFilename_l = inputFilename_p.substr(0,inputFilename_p.find("."));
        string fileName_l = "out/" + inputFilename_l.substr(3,inputFilename_l.size()) + "_" + to_string(eval_p) + ".out";
        myFile_l.open(fileName_l.c_str());
        //Read
        myFile_l << pb_p;
        //Close file
        myFile_l.close();
}

int main(int argc, char** argv){
	//Problem creation
	int total_l = 0;
	for (int i = 1; i < argc; i++){
		PizzaProblem pb_l;
		//Read file
		readInput(string(argv[i]),pb_l);
		//Optimize
		pb_l.optimize();
		//Eval
		int eval_l = pb_l.eval();
		total_l += eval_l;
		//Write
		writeOutput(string(argv[i]),pb_l,eval_l);
	}
	cout << "Total = " << total_l << endl;
	return 0;
};
