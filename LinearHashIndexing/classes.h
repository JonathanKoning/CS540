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
	const int PAGE_SIZE = 4096; //bucket capacity?
	// Size of each block is 4096 bytes
	// Size of each record is less than the size of a block
	// When total number of bytes used by records is > 70% of total capacity, make new block
	// 

	vector<int> pageDirectory;  // Where pageDirectory[h(id)] gives page index of block //This is kept in main memory
                                // can scan to pages using index*PAGE_SIZE as offset (using seek function)
	int numBlocks; 		// n
	int i;				// number of bits required to address n blocks. If we are using 2^16 for the hash, why do we need i?
						// This could be the page index

	int numRecords; 	// Records in index. Number of records in the page
	int nextFreePage; 	// Next page to write to. This is what determines what to mod against. 
	
	//TODO
	// bucket = block
	// i = which bucket to split
	// n = number of buckets
	// split policy = greater than 70% capacity
	// bucket capacity = PAGE_SIZE
	//1 record = 18 to 716 Bytes (id, name, bio, manager-id) = (int, string, string, int) = (8 bytes, 1-200 bytes, 1-500 bytes, 8 bytes)
	
	 


	// I added these temporarily until I know the correct place to declare them
	int block_size; // Why do we need this? Dynamically allocate block size to match each record? //Record size? Or does it store the aggregate record size per page? 
					// Each page (block) can hold between 5 and 227 records, 4096/716 and 4096/18
	string fName;	// This is the output file
	/////////////////////////////////////////////////////////////////////

    // Insert new record into index
	void insertRecord(Record record) {
		// No records written to index yet
		if (numRecords == 0) {
			fstream index_file1;
			index_file1.open(fName, ios::out);
			index_file1.close();
			FILE* index_file = fopen(fName.c_str(), "w+");
			// Initialize index with first blocks (start with 2)
			numBlocks=2; // n=2
			i=2;
			pageDirectory.push_back(fseek(index_file, 0*PAGE_SIZE, SEEK_SET));
			pageDirectory.push_back(fseek(index_file, 1*PAGE_SIZE, SEEK_SET));
			fclose(index_file);

			cout << pageDirectory[0] << endl;
			cout << pageDirectory[1] << endl;
		}

		// The key, k, is the record.id
		// Hash function h(k) maps a key, k, to {0, ..., n-1}
		// Block h(k) stores the records with key k.

		//Blocksize = PAGE_SIZE = 4096 Bytes
		// long unsigned int s = 3;
		int hk = record.id % int(pow(2.0,16.0));
		std::bitset<32> x(hk);
		// cout << "h(k): " << hk << endl;
		// cout << "bitset: " << x << endl;
		// cout << "hk & 1: " << (bitset<const s> (hk&3)) << endl;
		// cout << "bitset[32]: " <<  << endl;
		// cout << "hk % 2^i: " << (hk% (int)pow(2, i)) << endl;
		// cout << "2^16: " << pow(2, 16) << endl;
		// cout << "msb: " << (((1 << 3)-1)&(hk >> (1-1))) << endl;
		// cout << "msb: " << (1 << ((int)(log2(record.id % int(pow(2.0,16.0)))))) << endl;
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
				
				//Create new record
				Record emp(newemp);
				// emp.print();
				//Insert record into the hash
				insertRecord(emp);

			} else {
				eof = false;
			}
		}
	}	
	// Given an ID, find the relevant record and print it
	// Record findRecordById(int id) {
	// 	return 
	// }
};
