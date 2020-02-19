/*
 * PizzaProblem.cpp
 *
 *  Created on: Feb 19, 2020
 *      Author: a432172
 */

#include "PizzaProblem.h"


pizzaProblem::pizzaProblem() {
	N_m=0;
	M_m=0;
	pS_m*=nullptr;
	pKsol_m=nullptr;
	K_m=0;
}

pizzaProblem::~pizzaProblem() {
	if (pS_m) {
		delete pS_m;
	}
	if (pKsol_m) {
		delete pKsol_m;
	}
}

void pizzaProblem::getInput(string fileName_p) {

	ifstream myfile_l(fileName_p);

	if (myfile_l.is_open()) {
		char space_l;
		// reading M and N
		myfile_l >> M_m >> space_l >> N_m;
		// reading value in S
		for (int i_l = 0; i_l < N_m; i_l++) {
			myfile_l >> pS_m[i_l] >> space_l;
		}
		myfile_l.close();
	}
	else  {
		cout << "no file found!" << endl;
	}
}

void pizzaProblem::solve() {
	try {
	    GRBEnv env_l = GRBEnv(true);
	    env_l.set("LogFile", "pizzaProblem.log");
	    env_l.start();
	    GRBModel model_l = GRBModel(env_l);

	    // variables
	    GRBVar* x_l = model_l.addVars(getN(),GRB_BINARY);

	    // objective
	    GRBLinExpr expr_l;
	    for (int i_l = 0; i_l < N_m; i_l++)  {
	    	expr_l += x_l[i_l]*pS_m[i_l];
	    }
	    model_l.setObjective(expr_l, GRB_MAXIMIZE);

	    //constraints
	    model_l.addConstr(expr_l <= getM());

	    // to linearize ideally
	    for (int i_l = 0; i_l < N_m; i_l++)  {
	    	for (int j_l = 0; j_l < N_m; j_l++)  {
	    		if (pS_m[i_l] == pS_m[j_l]) {
	    		    model_l.addConstr(x_l[i_l]*x_l[j_l] == 0);
	    		}
	    	}
		}

	    model_l.optimize();

	    // writing solution
	    int idK_l = 0;
	    for (int i_l = 0; i_l < N_m; i_l++)  {
	    	double xi_l = x_l[i_l].get(GRB_DoubleAttr_X);
	    	K_m += xi_l;
	    	if (xi_l != 0) {
	    		pKsol_m[idK_l] = i_l;
	    		idK_l++;
	    	}
	    }

	}
	catch(GRBException &e) {
	    cout << "Error code = " << e.getErrorCode() << endl;
	    cout << e.getMessage() << endl;
	  } catch (...) {
	    cout << "Error during optimization" << endl;
	  }

}
