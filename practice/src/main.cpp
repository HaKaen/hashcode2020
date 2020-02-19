//==============myfile_l==============================================================
// Name        : practice.cpp
// Author      : sebonnechere
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "PizzaProblem.h"

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	pizzaProblem pizza_l = pizzaProblem();
	string filename_l = "/app/ROPN/travail/sebonnechere/dev/hashcode/datapractice/a_example.in";
	pizza_l.getInput(filename_l);
	cout << "M: " << pizza_l.getM() << endl;
	cout << "N: " << pizza_l.getN() << endl;
	cout << "S[0]: " << pizza_l.getpS()[0] << endl;
	cout << "S[3]: " << pizza_l.getpS()[3] << endl;

	pizza_l.solve();
	cout << "K: " << pizza_l.getK() << endl;
	for (int i_l=0; i_l<pizza_l.getK(); i_l++) {
		cout << "type to order: " << pizza_l.getpKsol()[i_l] << endl;
	}
	return 0;
}
