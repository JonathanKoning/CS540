#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
using namespace std;

class slot{
	public:
	vector<int> slots;
};

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

	int numRecords; 	// Records in index.
	int nextFreePage; 	// Next page to write to. This is the index that is stored in pageDirectory
						
	vector<vector<int>> pageslots; // This stores the number of records and bits in each page
	
	//TODO
	// bucket = block
	// i = which bucket to split
	// n = number of buckets
	// split policy = greater than 70% capacity
	// bucket capacity = PAGE_SIZE
	// 1 record = 18 to 716 Bytes (id, name, bio, manager-id) = (int, string, string, int) = (8 bytes, 1-200 bytes, 1-500 bytes, 8 bytes)
	
	 


	// I added these temporarily until I know the correct place to declare them
	int block_size; // Why do we need this? Dynamically allocate block size to match each record? //Record size? Or does it store the aggregate record size per page? 
					// Each page (block) can hold between 5 and 227 records, 4096/716 and 4096/18
	string fName;	// This is the output file
	/////////////////////////////////////////////////////////////////////

    // Insert new record into index
	void insertRecord(Record record) {
		// No records written to index yet
		if (numRecords == 0) {
			nextFreePage = 0;
			fstream index_file1;
			index_file1.open(fName, ios::out);
			index_file1.close();
			//FILE* index_file = fopen(fName.c_str(), "w+");
			// Initialize index with first blocks (start with 2)
			numBlocks=2; // n=2
			i=1;
			pageDirectory.push_back(nextFreePage);
			pageslots.push_back({0,0});
			nextFreePage++;

			pageDirectory.push_back(nextFreePage);
			nextFreePage++;
			pageslots.push_back({0,0});
		}

		FILE* index_file = fopen(fName.c_str(), "a+");
		int hx = (record.id % int(pow(2.0, 16.0))) % int(pow(2,i));
		int Page_index = pageDirectory[hx];
		// cout << Page_index << endl;
		string srecord = to_string(record.id) + ',' + record.name + ',' + record.bio + ',' + to_string(record.manager_id) + "$";
		// cout << srecord << endl;
		int pageoffset = Page_index*PAGE_SIZE + pageslots[hx][1];

		//Place record into correct block
		fseek(index_file, pageoffset, SEEK_SET);
		fputs(srecord.c_str(), index_file);
		fclose(index_file);
		pageslots[hx] = {pageslots[hx][0]+1, (pageslots[hx][1] + (sizeof(srecord.c_str())*srecord.length()))};
		// cout << "pageslots[" << hx << "]: " << pageslots[hx][1] << endl;
		numRecords+=sizeof(srecord.c_str())*srecord.length();
		cout << "numRecords: " << numRecords << endl;
		float capacity = numBlocks*PAGE_SIZE;
		float perfull = numRecords/capacity;
		cout << perfull << endl;
		// if number of records is > 70% capacity, then:
		// split block
		// increment number of blocks
		// if number of blocks is now odd, increase i
		if (perfull > 0.70)
		{
			string line, emp, word;
			cout << "Split" << endl;
			numBlocks++;
			if(numBlocks%2 == 1)
			{
				i++;
			}
			pageDirectory.push_back(nextFreePage);
			nextFreePage++;

			//Loop through each block
			FILE* index_file = fopen(fName.c_str(), "r");
			
			for(int j=0; j<numBlocks-1; j++)
			{
				pageoffset = j*PAGE_SIZE;
				char buffer[pageslots[j][1]];
				// string buffer;
				// Read in one block

				fread(buffer, pageslots[j][1], 1, index_file);

				cout << buffer << endl;

				//Loop through each record
				// for(int k=0; k < pageslots[j][0]; k++)
				// {
					
				// }
			}



		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		// The key, k, is the record.id
		// Hash function h(k) maps a key, k, to {0, ..., n-1}
		// Block h(k) stores the records with key k.


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
				// delete(&emp);

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
