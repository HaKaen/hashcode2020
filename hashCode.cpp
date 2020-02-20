#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<map>
#include<set>
#include<string>

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
	Library(int,int,int,int);
	int index_m;
	int nbBooks_m;
	int nbDaysToFinish_m;
	int nbBooksShippedByDay_m;
	vector<Book*> vBooksInLibrary_m;
};

Book::Book(int index_p, int score_p):
index_m(index_p),
score_m(score_p),
isAssigned_m(false)
{
}

Library::Library(int index_p, int nbBooks_p, int nbDaysToFinish_p, int nbBooksShippedByDay_p):
index_m(index_p),
nbBooks_m(nbBooks_p),
nbDaysToFinish_m(nbDaysToFinish_p),
nbBooksShippedByDay_m(nbBooksShippedByDay_p),
vBooksInLibrary_m()
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
		Library* pNewLibrary_l = new Library(i,nbBooksInLibrary_l,nbDaysToFinish_l,nbBooksShippedByDay_l);

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

//	os_p << bookProblem_p.BooksToOrder_m.size() << endl;
//
//	os_p << bookProblem_p.BooksToOrder_m[0]->index_m;
//
//	for (unsigned int i = 1; i < bookProblem_p.BooksToOrder_m.size(); i++){
//		os_p << " " << bookProblem_p.BooksToOrder_m[i]->index_m;
//	}

	return os_p;
}

int BookProblem::eval(){
//	int value_l = 0;
//	for (Book* pBook_l : BooksToOrder_m){
//		value_l+= pBook_l->nbSlices_m;
//	}
//	cout << "eval = " << value_l << "\n";
//	return value_l;
	return 0;
}

void opt(BookProblem pb_p, int index_p, int max_p){
}

void BookProblem::optimize(){
//	int sum_l = 0;
//	bool continue_l = true;
//	//Sort using a lambda
//	sort(Books_m.begin(),Books_m.end(),[](Book* p1_p, Book* p2_p){return p1_p->nbSlices_m > p2_p->nbSlices_m;});
//	//While loop
//	while(continue_l){
//		continue_l = false;
//		for (Book* pBook_l : Books_m){
//			if (!pBook_l->isAssigned_m && sum_l + pBook_l->nbSlices_m < maxSlices_m){
//				pBook_l->isAssigned_m = true;
//				BooksToOrder_m.push_back(pBook_l);
//				sum_l += pBook_l->nbSlices_m;
//				continue_l = true;
//			}
//		}
//	}
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
//		pb_l.optimize();
//		//Eval
//		int eval_l = pb_l.eval();
//		total_l += eval_l;
//		//Write
		int eval_l = 0;
		writeOutput(string(argv[i]),pb_l,eval_l);
	}

	cout << "Total = " << total_l << endl;
	return 0;
};
