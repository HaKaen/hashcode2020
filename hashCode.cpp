#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<map>
#include<set>
#include<string>
#include <cmath>

using namespace std;
hash<string> hash_fn;

//Indexed object to easy sort and outputs
class Book{
public:
	Book(int,int);
	int index_m;
	int score_m;
	bool isAssigned_m;
};

class Library{
public:
	Library(int,int,int,int,bool);
	int index_m;
	int nbBooks_m;
	int nbDaysToFinish_m;
	int nbBooksShippedByDay_m;
	vector<Book*> vBooksInLibrary_m;
	vector<Book*> vBooksReadHere_m;
	bool isSignedUp_m;
	double score;
	double score2;
};

Book::Book(int index_p, int score_p):
index_m(index_p),
score_m(score_p),
isAssigned_m(false)
{
}

Library::Library(int index_p, int nbBooks_p, int nbDaysToFinish_p, int nbBooksShippedByDay_p, bool signedUp_m):
index_m(index_p),
nbBooks_m(nbBooks_p),
nbDaysToFinish_m(nbDaysToFinish_p),
nbBooksShippedByDay_m(nbBooksShippedByDay_p),
vBooksInLibrary_m(),
vBooksReadHere_m(),
isSignedUp_m(signedUp_m),
score(0.0),
score2(0.0)
{
}

//Object definition
class BookProblem{
public:
	//Constructor
	BookProblem();
	~BookProblem();
	//Different types, int or bool as examples
	int nbBooks_m;
	int nbLibraries_m;
	int nbDays_m;
	//Books in the problem
	vector<Book*> vBooks_m;
	//Solution
	vector<Library*> vLibraries_m;
	//Optimize
	void optimize();
	//Eval
	int eval();
	//Max
	int max();
	//Lecture d'un objet
	friend istream& operator>> (istream&,BookProblem&);
	//Ecriture d'un objet
	friend ostream& operator<< (ostream&,const BookProblem&);
};

BookProblem::BookProblem():
nbBooks_m(),
nbLibraries_m(),
nbDays_m(),
vBooks_m(),
vLibraries_m()
{
}

BookProblem::~BookProblem(){
	for (Book* pBook_l : vBooks_m){
		delete pBook_l;
	}
}

//Reader from input file
istream& operator>>(istream& is_p, BookProblem& bookProblem_p){
	is_p >> bookProblem_p.nbBooks_m >> bookProblem_p.nbLibraries_m >> bookProblem_p.nbDays_m;

	for (int i = 0; i < bookProblem_p.nbBooks_m; i++){
		int score_l;
		is_p >> score_l;
		bookProblem_p.vBooks_m.push_back(new Book(i,score_l));
	}

	for (int i = 0; i < bookProblem_p.nbLibraries_m; i++){
		int nbBooksInLibrary_l;
		int nbDaysToFinish_l;
		int nbBooksShippedByDay_l;
		is_p >> nbBooksInLibrary_l >> nbDaysToFinish_l >> nbBooksShippedByDay_l;
		Library* pNewLibrary_l = new Library(i,nbBooksInLibrary_l,nbDaysToFinish_l,nbBooksShippedByDay_l,false);
		//calcul du score de la lib
		pNewLibrary_l->score = nbBooksShippedByDay_l * nbBooksInLibrary_l;
		pNewLibrary_l->score2 = (bookProblem_p.nbDays_m - nbDaysToFinish_l) * nbBooksShippedByDay_l;
		for (int j = 0; j < nbBooksInLibrary_l; j++){
			int bookIndex_l;
			is_p >> bookIndex_l;
			pNewLibrary_l->vBooksInLibrary_m.push_back(bookProblem_p.vBooks_m[bookIndex_l]);
		}

		bookProblem_p.vLibraries_m.push_back(pNewLibrary_l);
	}

	return is_p;
}

//Writer in output file
ostream& operator<<(ostream& os_p, const BookProblem& bookProblem_p){

/*************************************************** JUST TO CHECK READING ********************************************************
	os_p << bookProblem_p.nbBooks_m << " " << bookProblem_p.nbLibraries_m << " " << bookProblem_p.nbDays_m << endl;

	for (int i = 0; i < bookProblem_p.nbBooks_m; i++){
		os_p << bookProblem_p.vBooks_m[i]->score_m << " ";
	}

	os_p << endl;

	for (int i = 0; i < bookProblem_p.nbLibraries_m; i++){
		Library* pLibrary_l = bookProblem_p.vLibraries_m[i];
		os_p << pLibrary_l->nbBooks_m << " " << pLibrary_l->nbDaysToFinish_m << " " << pLibrary_l->nbBooksShippedByDay_m << endl;
		for (int j = 0; j < pLibrary_l->nbBooks_m; j++){
			os_p << pLibrary_l->vBooksInLibrary_m[j]->index_m << " ";
		}
		os_p << endl;
	}
 ************************************************************************************************************************************/
	int nbLib = 0;
	for (Library* l : bookProblem_p.vLibraries_m){
		if(l->isSignedUp_m){
			nbLib++;
		}
	}
	os_p << nbLib << endl;
	for (int i = 0; i < bookProblem_p.nbLibraries_m; i++){
		os_p << i << " " << bookProblem_p.vLibraries_m[i]->vBooksReadHere_m.size() << endl;

		for (unsigned int j = 0; j < bookProblem_p.vLibraries_m[i]->vBooksReadHere_m.size(); j++){
			os_p << bookProblem_p.vLibraries_m[i]->vBooksReadHere_m[j]->index_m << " ";
		}
		os_p << endl;
	}

	return os_p;
}

int BookProblem::eval(){
	int value_l = 0;

	for (Book* pBook_l : vBooks_m){
		if (pBook_l->isAssigned_m){
			value_l += pBook_l->score_m;
		}
	}

	return value_l;
}

int BookProblem::max(){
	int value_l = 0;

	for (Book* pBook_l : vBooks_m){
		value_l += pBook_l->score_m;
	}

	return value_l;
}

void opt(BookProblem pb_p, int index_p, int max_p){
}

void BookProblem::optimize(){
	//code
	vector<Book*> books_in_lib;
	sort(vLibraries_m.begin(), vLibraries_m.end(),
		[](Library* & a, Library* & b) -> bool
	{
		return a->score > b->score;
	});

//	sort (vLibraries_m.begin(),vLibraries_m.end(),[this](Library* lib1_p, Library* lib2_p){
//		return (nbDays_m - lib1_p->nbDaysToFinish_m)*lib1_p->nbBooksShippedByDay_m
//				< (nbDays_m - lib2_p->nbDaysToFinish_m)*lib2_p->nbBooksShippedByDay_m
//				;});

	for (Library *lib_l : vLibraries_m) {
		sort (lib_l->vBooksInLibrary_m.begin(),lib_l->vBooksInLibrary_m.end(),[](Book* b1_p, Book* b2_p){
				return b1_p->score_m > b2_p->score_m ;});
	}

	int d = nbDays_m;
	for (Library* l : vLibraries_m){
//		cerr << "Days remaining : " << d << endl;
		books_in_lib.clear();
		if (d-l->nbDaysToFinish_m>0){
//			cerr << " Lib : " << l->index_m << endl;
			l->isSignedUp_m = true;
 			d -= l->nbDaysToFinish_m;
			for (Book* b : l->vBooksInLibrary_m){
				if(!b->isAssigned_m){
					books_in_lib.push_back(b);
				}
			}
			auto end = books_in_lib.end();
			int day_to_read = std::ceil((int)books_in_lib.size()/l->nbBooksShippedByDay_m);
			if (d-day_to_read<0){
				end = books_in_lib.begin()+l->nbBooksShippedByDay_m*d;
			}
			for (auto it = books_in_lib.begin(); it!=books_in_lib.end(); it++){
				l->vBooksReadHere_m.push_back(*it);
				(*it)->isAssigned_m = true;
			}
		}
		else {
			break;
		}
	}
}

void readInput(const string& fileName_p,BookProblem& pb_p){
        //Open stream
        ifstream myFile_l;
        myFile_l.open(fileName_p.c_str());
        //Read
        myFile_l >> pb_p;
        //Close file
        myFile_l.close();
}

void writeOutput(const string& inputFilename_p, const BookProblem& pb_p, int eval_p){
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
		BookProblem pb_l;
		//Read file
		readInput(string(argv[i]),pb_l);
		//Optimize
		pb_l.optimize();
//		//Eval
		int eval_l = pb_l.eval();
		int max_l = pb_l.max();
		cerr << "(" << eval_l << " / " << max_l << ")" << endl;
		total_l += eval_l;
//		//Write
		writeOutput(string(argv[i]),pb_l,eval_l);
	}

	cout << "Total = " << total_l << endl;
	return 0;
};
