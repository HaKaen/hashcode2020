/*
 * PizzaProblem.h
 *
 *  Created on: Feb 19, 2020
 *      Author: a432172
 */

#ifndef PIZZAPROBLEM_H_
#define PIZZAPROBLEM_H_

#include <iostream>
#include <fstream>
#include <string>
#include "gurobi_c++.h"

using namespace std;

class pizzaProblem {

public:
	pizzaProblem();
	virtual ~pizzaProblem ();
	void getInput(string fileName_p);

	int getM() const {return M_m;}
	void setM(int m) {M_m = m;}
	int getN() const {return N_m;}
	void setN(int m) {N_m = m;}
	int* getpS() const {return pS_m;}
	void setpS(int* S_p) {pS_m = S_p;}
	int getK() const {return K_m;}
	void setK(int m) {K_m = m;}
	int* getpKsol() const {return pKsol_m;}
	void setpKsol(int* S_p) {pKsol_m = S_p;}

	void solve();

protected:
	int M_m; // max nb of pizza slices
	int N_m; // max nb of different types
	int* pS_m; // nb of slices by pizza

	int K_m; // nb of different pizzas to order
	int* pKsol_m; // types of pizza to order


};



#endif /* PIZZAPROBLEM_H_ */
