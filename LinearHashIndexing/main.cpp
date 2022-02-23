/*
Skeleton code for linear hash indexing
*/

#include <string>
#include <ios>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "classes2.h"
using namespace std;


int main(int argc, char* const argv[]) {

    // Create the index
    LinearHashIndex emp_index("EmployeeIndex");
    emp_index.createFromFile("Employee.csv");
   	
	cout << "enter 'quit' to quit searching" << endl;
	string input="";
	while(input != "quit")
	{
		cout << "Type an id: ";
		cin >> input;
		emp_index.findRecordById(stoi(input)).print();
	}
    // Loop to lookup IDs until user is ready to quit
    // cout << sizeof(double) << endl;

    return 0;
}
