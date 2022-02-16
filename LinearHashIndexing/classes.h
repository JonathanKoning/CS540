#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
using namespace std;

class Record {
public:
	int id, manager_id;
	std::string bio, name;

	Record(vector<std::string> fields) {
		id = stoi(fields[0]);
		name = fields[1];
		bio = fields[2];
		manager_id = stoi(fields[3]);
	}

	void print() {
		cout << "\tID: " << id << "\n";
		cout << "\tNAME: " << name << "\n";
		cout << "\tBIO: " << bio << "\n";
		cout << "\tMANAGER_ID: " << manager_id << "\n";
	}
};


class LinearHashIndex {

private:
	const int PAGE_SIZE = 4096;

	vector<int> pageDirectory;  // Where pageDirectory[h(id)] gives page index of block
                                // can scan to pages using index*PAGE_SIZE as offset (using seek function)
	int numBlocks; // n
	int i;
	int numRecords; // Records in index
	int nextFreePage; // Next page to write to
	
	//TODO
	// I added these temporarily until I know the correct place to declare them
	int block_size; 
	string fName;
	/////////////////////////////////////////////////////////////////////

    // Insert new record into index
	void insertRecord(Record record) {

		// No records written to index yet
		if (numRecords == 0) {
			// Initialize index with first blocks (start with 2)
			numBlocks=2; // n=2

		}

		// Add record to the index in the correct block, creating overflow block if necessary
		// ONLY keep up to 3 blocks + directory of the hash index in main memory

		// Take neccessary steps if capacity is reached

		// h(id) = id mod 2^16
		//Increment numBlocks if the average number of records per each block exeeds 70% of the block capacity


	}

public:
	LinearHashIndex(string indexFileName) {
		numBlocks = 0;
		i = 0;
		numRecords = 0;
		block_size = 0;
		fName = indexFileName;
	}	
	// Read csv file and add records to the index
	void createFromFile(string csvFName) {
		fstream input_file;
		input_file.open(csvFName, ios::in);
		
		string line, word;
		// Record  emp;
		bool eof = true;
		while (eof)
		{
			if (getline(input_file, line, '\n')) {
				vector<std::string> newemp;
				// turn line into a stream
				stringstream s(line);
				// gets everything in stream up to comma
				getline(s, word,',');
				newemp.push_back(word);
				
				getline(s, word,',');
				newemp.push_back(word);
				getline(s, word, ',');
				newemp.push_back(word);
				getline(s, word, ',');
				newemp.push_back(word);
				// LinearHashIndex emp_index("EmployeeIndex");
				Record emp(newemp);
				emp.print();

			} else {
				eof = false;
			}
		}
	}	
	// Given an ID, find the relevant record and print it
	Record findRecordById(int id) {
	
	}
};
