#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<map>
#include<set>
#include<string>

using namespace std;

//Object definition
class PizzaProblem{
public:
	//Constructor
	PizzaProblem();
	//Different types, int or bool as examples
	int maxSlices_m;
	int nbPizzas_m;
	vector<int> nbSlicesForPizza_m;
	vector<bool> isAssigned_m;
	//Solution
	vector<int> pizzasToOrder_m;
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
nbSlicesForPizza_m(),
isAssigned_m(),
pizzasToOrder_m()
{
}

//Reader from input file
istream& operator>>(istream& is_p, PizzaProblem& pizzaProblem_p){
	is_p >> pizzaProblem_p.maxSlices_m >> pizzaProblem_p.nbPizzas_m;

	for (int i = 0; i < pizzaProblem_p.nbPizzas_m; i++){
		int sliceSize_l;
		is_p >> sliceSize_l;
		pizzaProblem_p.nbSlicesForPizza_m.push_back(sliceSize_l);
		pizzaProblem_p.isAssigned_m.push_back(false);
	}

	return is_p;
}

//Writer in output file
ostream& operator<<(ostream& os_p, const PizzaProblem& pizzaProblem_p){

	os_p << pizzaProblem_p.pizzasToOrder_m.size() << endl;

	os_p << pizzaProblem_p.pizzasToOrder_m[0];

	for (unsigned int i = 1; i < pizzaProblem_p.pizzasToOrder_m.size(); i++){
		os_p << " " << pizzaProblem_p.pizzasToOrder_m[i];
	}

	return os_p;
}

int PizzaProblem::eval(){
	int value_l = 0;
	for (unsigned int index_l = 0; index_l < pizzasToOrder_m.size(); index_l++){
		value_l+= nbSlicesForPizza_m[pizzasToOrder_m[index_l]];
	}
	cout << "eval = " << value_l << "\n";
	return value_l;
}

void PizzaProblem::optimize(){
	int sum_l = 0;
	bool continue_l = true;
	while(continue_l){
		continue_l = false;
		for (int i = 0; i < nbPizzas_m; i++){
			if (!isAssigned_m[i] && sum_l + nbSlicesForPizza_m[i] < maxSlices_m){
				isAssigned_m[i] = true;
				pizzasToOrder_m.push_back(i);
				sum_l += nbSlicesForPizza_m[i];
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
        string fileName_l = inputFilename_l + "_" + to_string(eval_p) + ".out";
        myFile_l.open(fileName_l.c_str());
        //Read
        myFile_l << pb_p;
        //Close file
        myFile_l.close();
}

int main(int argc, char** argv){
	//Problem creation
	PizzaProblem pb_l;
	//Read file
	readInput(string(argv[1]),pb_l);
	//Optimize
	pb_l.optimize();
	//Eval
	int eval_l = pb_l.eval();
	//Write
	writeOutput(string(argv[1]),pb_l,eval_l);
	return 0;
};
