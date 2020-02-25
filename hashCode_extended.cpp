#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<map>
#include<set>
#include<string>
#include<cmath>

using namespace std;

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
	int nbDaysToSignUp_m;
	int nbBooksShippedByDay_m;
	vector<Book*> vBooksInLibrary_m;
	vector<Book*> vBooksReadHere_m;
	bool isSignedUp_m;
	double score;
	double score2;
	double score3;
};

Book::Book(int index_p, int score_p):
index_m(index_p),
score_m(score_p),
isAssigned_m(false)
{
}

Library::Library(int index_p, int nbBooks_p, int nbDaysToSignUp_p, int nbBooksShippedByDay_p, bool signedUp_m):
index_m(index_p),
nbBooks_m(nbBooks_p),
nbDaysToSignUp_m(nbDaysToSignUp_p),
nbBooksShippedByDay_m(nbBooksShippedByDay_p),
vBooksInLibrary_m(),
vBooksReadHere_m(),
isSignedUp_m(signedUp_m),
score(0.0),
score2(0.0),
score3(0)
{
}

//Object definition
class BookProblem{
public:
	//Constructor
	BookProblem();
	~BookProblem();
	BookProblem(const BookProblem&);
	//Different types, int or bool as examples
	int nbBooks_m;
	int nbLibraries_m;
	int nbDays_m;
	//Books in the problem
	vector<Book*> vBooks_m;
	vector<int> vBookFreq_m;
	//Libraries
	vector<Library*> vLibraries_m;
	//SignedUp Libraries
	vector<Library*> vSignedUpLibraries_m;
	//Optimize
	void optimize(int);
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
vBookFreq_m(),
vLibraries_m()
{
}

BookProblem::BookProblem(const BookProblem& pb_p):
nbBooks_m(pb_p.nbBooks_m),
nbLibraries_m(pb_p.nbLibraries_m),
nbDays_m(pb_p.nbDays_m),
vBooks_m(),
vBookFreq_m(),
vLibraries_m()
{
	for (int i = 0; i < pb_p.nbBooks_m; i++){
		vBooks_m.push_back(new Book(pb_p.vBooks_m[i]->index_m,pb_p.vBooks_m[i]->score_m));
	}

	for (int i = 0; i < pb_p.nbBooks_m; i++){
		vBookFreq_m.push_back(pb_p.vBookFreq_m[i]);
	}

	for (int i = 0; i < pb_p.nbLibraries_m; i++){
		Library* pLib_l = pb_p.vLibraries_m[i];
		vLibraries_m.push_back(new Library(pLib_l->index_m,pLib_l->nbBooks_m,pLib_l->nbDaysToSignUp_m,pLib_l->nbBooksShippedByDay_m,false));
		for (unsigned int j = 0; j < pb_p.vLibraries_m[i]->vBooksInLibrary_m.size(); j++){
			Book* pBook_l = pb_p.vLibraries_m[i]->vBooksInLibrary_m[j];
			vLibraries_m[i]->vBooksInLibrary_m.push_back(vBooks_m[pBook_l->index_m]);
		}
	}
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
		bookProblem_p.vBookFreq_m.push_back(0);
	}

	for (int i = 0; i < bookProblem_p.nbBooks_m; i++){
		int score_l;
		is_p >> score_l;
		bookProblem_p.vBooks_m.push_back(new Book(i,score_l));
	}

	for (int i = 0; i < bookProblem_p.nbLibraries_m; i++){
		int nbBooksInLibrary_l;
		int nbDaysToSignUp_l;
		int nbBooksShippedByDay_l;
		is_p >> nbBooksInLibrary_l >> nbDaysToSignUp_l >> nbBooksShippedByDay_l;
		if (nbBooksShippedByDay_l > nbBooksInLibrary_l){
			nbBooksShippedByDay_l = nbBooksInLibrary_l;
		}
		Library* pNewLibrary_l = new Library(i,nbBooksInLibrary_l,nbDaysToSignUp_l,nbBooksShippedByDay_l,false);
		//calcul du score de la lib
		pNewLibrary_l->score = nbBooksShippedByDay_l * nbBooksInLibrary_l;
		pNewLibrary_l->score2 = (bookProblem_p.nbDays_m - nbDaysToSignUp_l) * nbBooksShippedByDay_l;
		int totalValue_l = 0;
		for (int j = 0; j < nbBooksInLibrary_l; j++){
			int bookIndex_l;
			is_p >> bookIndex_l;
			bookProblem_p.vBookFreq_m[bookIndex_l]++;
			pNewLibrary_l->vBooksInLibrary_m.push_back(bookProblem_p.vBooks_m[bookIndex_l]);
			totalValue_l += bookProblem_p.vBooks_m[bookIndex_l]->score_m;
		}
		pNewLibrary_l->score3 = totalValue_l;

		bookProblem_p.vLibraries_m.push_back(pNewLibrary_l);
	}

	return is_p;
}

//Writer in output file
ostream& operator<<(ostream& os_p, const BookProblem& bookProblem_p){

	os_p << bookProblem_p.vSignedUpLibraries_m.size() << endl;

	for (unsigned int i = 0; i < bookProblem_p.vSignedUpLibraries_m.size(); i++){
		if (bookProblem_p.vSignedUpLibraries_m[i]->isSignedUp_m){
			if (bookProblem_p.vSignedUpLibraries_m[i]->vBooksReadHere_m.size()){
				os_p << bookProblem_p.vSignedUpLibraries_m[i]->index_m << " " << bookProblem_p.vSignedUpLibraries_m[i]->vBooksReadHere_m.size() << endl;
				for (unsigned int j = 0; j < bookProblem_p.vSignedUpLibraries_m[i]->vBooksReadHere_m.size(); j++){
					os_p << bookProblem_p.vSignedUpLibraries_m[i]->vBooksReadHere_m[j]->index_m << " ";
				}
				os_p << endl;
			}
		}
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

int getLibValue(BookProblem* pb_p, int remainingDays_p, Library* pLib_p){

	int remainingDaysAfterSignUp_l = remainingDays_p - pLib_p->nbDaysToSignUp_m;

	if (remainingDaysAfterSignUp_l < 0) return 0;

	int libValue_l = 0;
	int maxBooksShipped_l = remainingDaysAfterSignUp_l * pLib_p->nbBooksShippedByDay_m;
	int nbBooksShipped_l = 0;

	for (Book* pBook_l : pLib_p->vBooksInLibrary_m){
		if (!pBook_l->isAssigned_m){
//			libValue_l += (double)(pBook_l->score_m) / (double)(pb_p->vBookFreq_m[pBook_l->index_m]);
			libValue_l += pBook_l->score_m;
			if (++nbBooksShipped_l == maxBooksShipped_l) break;
		}
	}

	return libValue_l;
}

Library* pickLibrary(BookProblem* pb_p, int remainingDays_p, int nbDaysToWithdraw_p){
	Library* pBestLib_l = nullptr;
	double max_l = 0.0;

	for (Library* pLib_l : pb_p->vLibraries_m){
		if (remainingDays_p < pLib_l->nbDaysToSignUp_m){
			continue;
		}

		if (pLib_l->vBooksReadHere_m.size() == 0){

//			int libValue_l = 1.02*getLibValue(pb_p,remainingDays_p,pLib_l) - getLibValue(pb_p,max(0,remainingDays_p-nbDaysToWithdraw_p),pLib_l);
			int libValue_l = getLibValue(pb_p,remainingDays_p,pLib_l);

			double libRatio_l = (double)(libValue_l) / (pow((double)(pLib_l->nbDaysToSignUp_m),1.0));
//			double libRatio_l = (double)(libValue_l);
			if ( libRatio_l > max_l ){
				pBestLib_l = pLib_l;
				max_l = libRatio_l;
			}
		}

	}
	return pBestLib_l;
}

double getBookScore(Book* pBook_p, const BookProblem& pb_p){
	if (pb_p.vBookFreq_m[pBook_p->index_m] == 0) return 0.0;
	return (double)(pBook_p->score_m) / (double)(pb_p.vBookFreq_m[pBook_p->index_m]);
}

void BookProblem::optimize(int nbDaysToWithdraw_p){

	int nbRemainingDays_l = nbDays_m;
	int total_l = 0;
	int totalBook_l = 0;

//	for (Book* pBook_l : vBooks_m){
//		cout << "Book " << pBook_l->index_m << " has frequency " << vBookFreq_m[pBook_l->index_m] << endl;
//	}

//	for (Library* pLibrary_l : vLibraries_m){
//		cout << "Library " << pLibrary_l->index_m  << endl;
//		for (Book* pBook_l : pLibrary_l->vBooksInLibrary_m){
//			cout << "Book " << pBook_l->index_m << " has frequency " << vBookFreq_m[pBook_l->index_m] << endl;
//		}
//	}

	for (Library *lib_l : vLibraries_m) {
		sort (lib_l->vBooksInLibrary_m.begin(),lib_l->vBooksInLibrary_m.end(),[this](Book* b1_p, Book* b2_p){
			return b1_p->score_m > b2_p->score_m;});
//			return getBookScore(b1_p,*this) > getBookScore(b2_p,*this);});
	}

	for (unsigned int i = 0; i < vLibraries_m.size(); i++){
		Library *pLib_l = pickLibrary(this,nbRemainingDays_l,nbDaysToWithdraw_p);
		if (!pLib_l) break;

		nbRemainingDays_l -= pLib_l->nbDaysToSignUp_m;
		if (nbRemainingDays_l == 0) break;

		pLib_l->isSignedUp_m = true;
		vSignedUpLibraries_m.push_back(pLib_l);
		cout << "Remaining days after signup of library " << pLib_l->index_m << " = " << nbRemainingDays_l << endl;

		int maxBooksToShip_l = nbRemainingDays_l * pLib_l->nbBooksShippedByDay_m;
		int nbBooksShipped_l = 0;
		int libraryValue_l = 0;

		for (Book* pBook_l : pLib_l->vBooksInLibrary_m){
			if(!pBook_l->isAssigned_m){
				if (nbBooksShipped_l < maxBooksToShip_l){
					pBook_l->isAssigned_m = true;
					pLib_l->vBooksReadHere_m.push_back(pBook_l);
					++nbBooksShipped_l;
					libraryValue_l += pBook_l->score_m;
				}
				else{
					vBookFreq_m[pBook_l->index_m]--;
				}
			}
		}
		total_l += libraryValue_l;
		totalBook_l += nbBooksShipped_l;

		cout << "Library " << pLib_l->index_m << " sent " << nbBooksShipped_l << " books, total is " << totalBook_l << ", library value is " << libraryValue_l << " and totalValue is " << total_l << endl;
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
	int bestValue_l = 0;
	int bestIndex_l = 0;

	for (int i = 1; i < argc; i++){
		BookProblem pb_l;
		//Read file
		readInput(string(argv[i]),pb_l);
		//Dyanmic
		//		dynamicProg(&pb_l);
//		Optimize
//		for (int j = 0; j < 100; j++){
//			BookProblem tmp_l(pb_l);
//			tmp_l.optimize(j);
//			//		//Eval
//			int eval_l = tmp_l.eval();
//			int max_l = tmp_l.max();
//			cerr << "(" << eval_l << " / " << max_l << ") for withdraw = " << j << endl;
//			//			total_l += eval_l;
//			if (eval_l > bestValue_l){
//				bestValue_l = eval_l;
//				bestIndex_l = j;
//				cout << "new best = " << bestValue_l << endl;
//			}
//		}
//		cout << "Best Value = " << bestValue_l << " at index " << bestIndex_l << endl;

		int j = 3;
		pb_l.optimize(j);
		//		//Eval
		int eval_l = pb_l.eval();
		total_l += eval_l;
		int max_l = pb_l.max();
		cerr << "(" << eval_l << " / " << max_l << ") for withdraw = " << j << endl;
		//		//Write
		writeOutput(string(argv[i]),pb_l,eval_l);
	}

	cout << "Total = " << total_l << endl;
	return 0;
}


















bool dynamicProg(BookProblem* pb_p){
	bool isOk_l = false;

	vector<vector<pair<int,vector<Library*>>>> vResult_l;

	for (int i = 0; i <= pb_p->nbLibraries_m; i++){
		vResult_l.push_back(vector<pair<int,vector<Library*>>>());
	}

	for (int i = 0; i <= pb_p->nbDays_m; i++){
		vResult_l[0].push_back(make_pair(0,vector<Library*>()));
	}

	for (int i = 1; i < pb_p->nbLibraries_m; i++){
		for (int j = 0; j <= pb_p->nbDays_m; j++){
			vResult_l[i].push_back(vResult_l[i-1][j]);
			Library* pLib_l = pb_p->vLibraries_m[i];
			if (pLib_l->nbDaysToSignUp_m <= j){
				int newWeight_l = vResult_l[i-1][j - pLib_l->nbDaysToSignUp_m].first + getLibValue(pb_p,pb_p->nbDays_m-j,pLib_l);
				if (newWeight_l > vResult_l[i][j].first){
					auto newVect_l = vResult_l[i-1][j - pLib_l->nbDaysToSignUp_m].second;
					newVect_l.push_back(pLib_l);
					vResult_l[i][j] = make_pair(newWeight_l,newVect_l);
				}
			}
		}
	}

	pb_p->vSignedUpLibraries_m = vResult_l[pb_p->nbLibraries_m-1][pb_p->nbDays_m].second;

	int nbRemainingDays_l = pb_p->nbDays_m;
	for (Library* pLib_l : pb_p->vSignedUpLibraries_m){
		pLib_l->isSignedUp_m = true;
		nbRemainingDays_l -= pLib_l->nbDaysToSignUp_m;
		int maxBooksToShip_l = nbRemainingDays_l*pLib_l->nbBooksShippedByDay_m;
		int nbBooksShipped_l = 0;
		for (Book* pBook_l : pLib_l->vBooksInLibrary_m){
			if (nbBooksShipped_l < maxBooksToShip_l){
				pBook_l->isAssigned_m = true;
				pLib_l->vBooksReadHere_m.push_back(pBook_l);
				++nbBooksShipped_l;
			}
			else{
				break;
			}
		}
	}

	return isOk_l;
}
