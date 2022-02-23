#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
using namespace std;


struct Block{
	string overflow;
	string offset;
	string buffer;
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
    const int PAGE_SIZE = 4096;

    vector<int> pageDirectory;  // Where pageDirectory[h(id)] gives page index of block
                                // can scan to pages using index*PAGE_SIZE as offset (using seek function)
    int numBlocks; // n
    int i;
    int numRecords; // Records in index
    int nextFreePage; // Next page to write to
	int block_size;
	string fName;
	void writeBlock(FILE* temp_file, Block* block, int blocknumber)
	{
		// cout << "writeBlock: " << blocknumber << endl;
		
		// Write overflow
		fseek(temp_file, blocknumber*4096, SEEK_SET);
		// cout << ftell(temp_file) << endl;
		fputs(block->overflow.c_str(), temp_file);
		// cout << ftell(temp_file) << endl;
		
		// Write buffer
		fseek(temp_file, ((blocknumber*4096)+4), SEEK_SET);
		//cout << "Move to start of buffer: " << ftell(temp_file) << endl;
		// int puts = fputs(buff_arr, temp_file);
		int puts = fputs(block->buffer.c_str(), temp_file);
		// cout << "buffer: " << block.buffer << endl;
		// cout << "buffer.c_str: " << (block->buffer).c_str() << endl;

		//cout << "puts: " << puts << endl;
		//cout << "Buffer written: " << ftell(temp_file) << endl;
		
		// Write offset
		fseek(temp_file, ((blocknumber*4096)+4092), SEEK_SET);
		//cout << "Move to start of offset: " << ftell(temp_file) << endl;
		fputs(block->offset.c_str(), temp_file);
		// fputs("0000", temp_file);
		//cout << "offset written: " << ftell(temp_file) << endl;
	}


	Block readBlock(FILE* index_file, int blocknumber)
	{
		// cout << "readBlock: " << blocknumber << endl;
		Block newblock;
		//cout << "newblock initiallized" << endl;
		
		//string overflow;
		//string buffer;
		//string offset;
		int buffersize;
		newblock.overflow.resize(4);
		newblock.offset.resize(4);
		//Read block from index file and store in block
		//Get overflow buffer
		fseek(index_file, blocknumber*4096, SEEK_SET);
		//fread(&overflow[0], 1, 4, index_file);
		fread(&newblock.overflow[0], 1, 4, index_file);
		
		// cout << "overflow: " << overflow << endl;

		//Get buffer size
		fseek(index_file, (blocknumber*4096)+4092, SEEK_SET);
		//fread(&offset[0], 1, 4, index_file);
		fread(&newblock.offset[0], 1, 4, index_file);
		// cout << "offset: " << offset << endl;
		buffersize = stoi(newblock.offset);
		//cout << "buffersize: " << buffersize << endl;
		//cout << "newblock.offset: " << newblock.offset << endl;
		//Get buffer
		newblock.buffer.resize(buffersize-4);
		
		fseek(index_file, (blocknumber*4096)+4, SEEK_SET);
		fread(&newblock.buffer[0], 1, buffersize, index_file);
		//cout << "newblock buffer read in" << endl;
		//cout << newblock.buffer << endl;
		// cout << "buffer: " << buffer[buffersize-5] << endl;

		// struct Block newblock;
		// cout << "newblock allocated" << endl;
		//newblock.overflow = overflow;
		//cout << "newblock overflow allocated" << endl;
		// cout << "newblock overflow set" << endl;
		// newblock.buffer = buffer;
		// cout << "newblock buffer set" << endl;
		//newblock.offset = offset;
		//cout << "newblock offset allocated" << endl;
		// cout << "newblock offset set" << endl;

		// cout << "returning new block" << endl;
		return newblock;
	}
	void readBlock2(FILE* index_file, Block* newblock, int blocknumber)
	{
		// cout << "readBlock: " << blocknumber << endl;
		//Block newblock;
		// cout << "newblock initiallized" << endl;
		
		string overflow;
		string buffer;
		string offset;
		int buffersize;
		overflow.resize(4);
		offset.resize(4);
		//Read block from index file and store in block
		//Get overflow buffer
		fseek(index_file, blocknumber*4096, SEEK_SET);
		//fread(&overflow[0], 1, 4, index_file);
		fread(&overflow[0], 1, 4, index_file);
		
		// cout << "overflow: " << overflow << endl;

		//Get buffer size
		fseek(index_file, (blocknumber*4096)+4092, SEEK_SET);
		//fread(&offset[0], 1, 4, index_file);
		fread(&offset[0], 1, 4, index_file);
		// cout << "offset: " << offset << endl;
		buffersize = stoi(offset);
		//cout << "buffersize: " << buffersize << endl;
		//cout << "newblock.offset: " << newblock->offset << endl;
		//Get buffer
		newblock->buffer.resize(buffersize-4);
		
		fseek(index_file, (blocknumber*4096)+4, SEEK_SET);
		fread(&newblock->buffer[0], 1, buffersize, index_file);
		//cout << "newblock buffer read in" << endl;
		//cout << newblock->buffer << endl;
		// cout << "buffer: " << buffer[buffersize-5] << endl;

		// struct Block newblock;
		// cout << "newblock allocated" << endl;
		newblock->overflow = overflow;
		//cout << "newblock overflow allocated" << endl;
		// cout << "newblock overflow set" << endl;
		// newblock.buffer = buffer;
		// cout << "newblock buffer set" << endl;
		newblock->offset = offset;
		//cout << "newblock offset allocated" << endl;
		// cout << "newblock offset set" << endl;

		//cout << "returning new block" << endl;
		//return newblock;
	}

	int h(int id)
	{
		// cout << "h(x)" << endl;
		return (id % int(pow(2.0, 16.0))) % int(pow(2,i));
	}


    // Insert new record into index
    void insertRecord(Record record) {
		FILE* index_file; 
		FILE* temp_file;
		Block newblock;
		Block oldblock;
		
		newblock.overflow = "0000";
		newblock.offset = "0004";
        // No records written to index yet
        if (numRecords == 0) {
			cout << "//////////////////Insert First Record////////////////" << endl;
            // Initialize index with first blocks (start with 2)
			i = 1;
			numBlocks = 2;
			nextFreePage = 2;

			index_file = fopen(fName.c_str(), "w+");
			//Write 2 empty blocks to the file
			writeBlock(index_file, &newblock, 0);
			pageDirectory.push_back(0);
			writeBlock(index_file, &newblock, 1);
			pageDirectory.push_back(1);
			if(fclose(index_file) == 0)
			{
				cout << "index closed" << endl;
			}
			else
			{
				cout << "index failed to close" << endl;
			}
        }
		cout << "//////////////////Insert New Record////////////////" << endl;
		// Add record to the index in the correct block, creating overflow block if necessary
		index_file = fopen(fName.c_str(), "r+");
		temp_file = fopen("temp", "w+");
		bool OFBlock = false;
		int overflowblock = 0;
		int key = record.id;
		//string newbuffer;
		if(h(key) >= numBlocks)
		{
			key = key - pow(2, i-1);
		}
		for(int j=0; j<nextFreePage; j++)
		{
			// cout << "top of loop" << endl;
			
			//oldblock = readBlock(index_file, j);
			readBlock2(index_file, &oldblock, j);
			// cout << "block read in" << endl;
			// cout << oldblock.buffer << endl;

			if(h(record.id) > numBlocks)
			{
	
			}
			if((j == pageDirectory[h(key)]) || (OFBlock && j == overflowblock))
			{
				// cout << "new record goes into this page" << endl;
				//Convert record into string to be added to buffer
				/*cout << "record.id: " << record.id << endl;
				cout << "record.manager_id: " << record.manager_id << endl;
				cout << "record.name : " << record.name << endl;
				cout << "record.bio: " << record.bio << endl;
				cout << "to_string(record.id): " << to_string(record.id) << endl;
				cout << "to_string(record.manager_id): " << to_string(record.manager_id) << endl;
				*/
				string newbuffer = to_string(record.id);
				// cout << "record.id added to newbuffer" << endl;
				newbuffer += ",";
				newbuffer += record.name;
				// cout << "record.name added to newbuffer" << endl;
				newbuffer += ",";
				newbuffer += record.bio;
				// cout << "record.bio added to newbuffer" << endl;
				// cout << "adding ',' after record.bio" << endl;
				newbuffer += ",";
				// cout << "to_string(record.manager_id): " << to_string(record.manager_id) << endl;
				newbuffer += to_string(record.manager_id);
				// cout << "record.manager_id added to newbuffer" << endl;
				newbuffer += "$";
				//newbuffer = to_string(record.id) + "," + record.name + "," + record.bio + "," + to_string(record.manager_id) + "$";
				// cout << "new buffer created" << endl;

				//newbuffer += newbuffer2;
				// cout << "oldblock.offset: " << oldblock.offset << endl;
				// cout << "total buffer length: " << stoi(oldblock.offset)+newbuffer.length() << endl;
				numRecords += newbuffer.length();
				//Check if new record will fit into the block
				if(stoi(oldblock.offset)+newbuffer.length() > 4088)
				{
					// cout << "Overflow block needed" << endl;
					//Check if current block has overflow block
					if(oldblock.overflow != "0000")
					{
						// cout << "block has overflowblock" << endl;
						OFBlock = true;
						overflowblock = stoi(oldblock.overflow);
					}
					else //Create an overflow block
					{
						// cout << "create overflowblock" << endl;
						if(nextFreePage < 10)
						{
							oldblock.overflow = "000" + to_string(nextFreePage);
						}
						else if(nextFreePage < 100)
						{
							oldblock.overflow = "00" + to_string(nextFreePage);
						}
						else if(nextFreePage < 1000)
						{
							oldblock.overflow = "0" + to_string(nextFreePage);
						}
						else
						{
							oldblock.overflow = to_string(nextFreePage);
						}
						// cout << "Write overflowblock" << endl;
						writeBlock(index_file, &newblock, nextFreePage);
						nextFreePage++;
					}
				}
				else
				{
					oldblock.buffer += newbuffer;
					int newoffset = oldblock.buffer.length()+4;
					if(newoffset < 10)
					{
						oldblock.offset = "000" + to_string(newoffset);
					}
					else if(newoffset < 100)
					{
						oldblock.offset = "00" + to_string(newoffset);
					}
					else if(newoffset < 1000)
					{
						oldblock.offset = "0" + to_string(newoffset);
						// cout << newoffset << endl;
						// cout << to_string(newoffset) << endl;
						// cout << oldblock.offset << endl;
					}
					else
					{
						oldblock.offset = to_string(newoffset);
					}
				}
			}
			// cout << "writing block" << endl;
			writeBlock(temp_file, &oldblock, j);
			// cout << "writing finished" << endl;
			//free(oldblock);
		}
		// cout << "All blocks updated" << endl;
		// cout << "attempting to close index file" << endl;
		if(fclose(index_file) == 0)
		{
			cout << "index closed" << endl;
		}
		else
		{
			cout << "index failed to close" << endl;
		}
		
		
		remove(fName.c_str());
		// cout << "EmployeeIndex removed" << endl;
		
		// cout << "attempting to close temp file" << endl;
		if(fclose(temp_file) == 0)
		{
			cout << "temp closed" << endl;
		}
		else
		{
			cout << "temp failed to close" << endl;
		}
		// cout << "both files closed" << endl;
		rename("temp", fName.c_str());
		// cout << "temp renamed to EmployeeIndex" << endl;
		// numRecords++; //This need to be changed to be the number of bits used
		// cout << "number of records updated" << endl;
			


		// Take neccessary steps if capacity is reached
		float capacity = (float)numRecords/(float)(numBlocks*4096);
		cout << "capacity: " << capacity << endl;
		if(capacity > 0.7)
		{
			cout << "Capacity is over 0.7! Adding new block!!!!!!" << endl;
			Block splitblock;
			FILE* new_index_file = fopen("newIndex", "w+");
			index_file = fopen(fName.c_str(), "r+");
			temp_file = fopen("temp", "w+");
			cout << "Add empty block to end of index file" << endl;
			writeBlock(index_file, &newblock, nextFreePage);
			cout << "Empty block added" << endl;
			nextFreePage++;
			numBlocks++;
			if(numBlocks%((int)pow(2,i))==1)
			{
				i++;
			}

			//Intitiallize new_index_file file
			cout << "Initiallizing new_index_file" << endl;
			for(int j=0; j<nextFreePage; j++)
			{
				writeBlock(new_index_file, &newblock, j);
			}
			if(fclose(new_index_file) == 0)
			{
				cout << "index closed" << endl;
			}
			else
			{
				cout << "index failed to close" << endl;
			}
			cout << "new_index_file initiallized" << endl;
			
			//Read each block
			string rec;
			string word;
			
			cout << "/////////////Reading in each block from index_file///////////////////////////" << endl;
			int indexpages = nextFreePage;
			for(int j=0; j<indexpages; j++)
			{
				cout << "reading in page: " << j << endl;
				
				//Why does this cause memeory corruption?
				//oldblock = readBlock(index_file, j);
				readBlock2(index_file, &oldblock, j);
				cout << "page " << j << " read in" << endl;
				//Read each record
				stringstream buff(oldblock.buffer);
				cout << "Reading in each record from page " << j << endl;
				while(getline(buff, rec, '$'))
				{
					vector<std::string> newemp;
					stringstream s(rec);
					//Get id
					getline(s, word,',');
					newemp.push_back(word);
					//get name
					getline(s, word,',');
					newemp.push_back(word);
					//get bio
					getline(s, word, ',');
					newemp.push_back(word);
					//get manager_id
					getline(s, word, ',');
					newemp.push_back(word);

					Record emp(newemp);

					new_index_file = fopen("newIndex", "r+");
					temp_file = fopen("temp", "w+");
					OFBlock = false;
					overflowblock = 0;
					// new_index_file = fopen("newIndex", "w+");
					key = emp.id;
					if(h(key) >= numBlocks)
					{
						key = key - pow(2,i-1);
					}
					//Insert new record into temp_file
					cout << "Inserting record into temp_file" << endl;
					for(int k=0; k<nextFreePage; k++)
					{
						//cout << "top of loop" << endl;
						cout << "reading in page " << k << " from new_index_file" << endl;
						cout << "Splitblock" << endl;
						//splitblock = readBlock(new_index_file, k);
						readBlock2(new_index_file, &splitblock, k);
						cout << "block read in" << endl;
						// cout << oldblock.buffer << endl;
						if((k == pageDirectory[h(key)]) || (OFBlock && k == overflowblock)) // Does not yet account for bit flip
						{
							cout << "/////////////new record goes into page " << k <<  "///////////////////////" << endl;
							//Convert record into string to be added to buffer
							string newbuffer = to_string(emp.id) + ',' + emp.name + ',' + emp.bio + ',' + to_string(emp.manager_id) + "$";
							//cout << "total buffer length: " << stoi(splitblock.offset)+newbuffer.length() << endl;
							// numRecords += newbuffer.length();
							//Check if new record will fit into the block
							if(stoi(splitblock.offset)+newbuffer.length() > 4088)
							{
								cout << "Overflow block needed" << endl;
								//Check if current block has overflow block
								if(splitblock.overflow != "0000")
								{
									cout << "block has overflowblock" << endl;
									OFBlock = true;
									overflowblock = stoi(splitblock.overflow);
								}
								else //Create an overflow block
								{
									cout << "create overflowblock" << endl;
									if(nextFreePage < 10)
									{
										splitblock.overflow = "000" + to_string(nextFreePage);
									}
									else if(nextFreePage < 100)
									{
										splitblock.overflow = "00" + to_string(nextFreePage);
									}
									else if(nextFreePage < 1000)
									{
										splitblock.overflow = "0" + to_string(nextFreePage);
									}
									else
									{
										splitblock.overflow = to_string(nextFreePage);
									}
									cout << "Write overflowblock" << endl;
									//writeBlock(index_file, &newblock, nextFreePage);
									writeBlock(new_index_file, &newblock, nextFreePage);
									nextFreePage++;
								}
							}
							else
							{
								splitblock.buffer += newbuffer;
								int newoffset = splitblock.buffer.length()+4;
								if(newoffset < 10)
								{
									splitblock.offset = "000" + to_string(newoffset);
								}
								else if(newoffset < 100)
								{
									splitblock.offset = "00" + to_string(newoffset);
								}
								else if(newoffset < 1000)
								{
									splitblock.offset = "0" + to_string(newoffset);
									// cout << newoffset << endl;
									// cout << to_string(newoffset) << endl;
									// cout << splitblock.offset << endl;
								}
								else
								{
									splitblock.offset = to_string(newoffset);
								}
							}
						}
						cout << "writing block to temp file" << endl;
						writeBlock(temp_file, &splitblock, k);
						cout << "writing finished" << endl;
					}

					cout << "attempting to close new_index_file" << endl;
					if(fclose(new_index_file) == 0)
					{
						cout << "new index closed" << endl;
					}
					else
					{
						cout << "new index failed to close" << endl;
					}


					remove("newIndex");
					cout << "new index removed" << endl;

					cout << "attempting to close temp file" << endl;
					if(fclose(temp_file) == 0)
					{
						cout << "temp closed" << endl;
					}
					else
					{
						cout << "temp failed to close" << endl;
					}
					cout << "both files closed" << endl;
					rename("temp", "newIndex");
					cout << "temp renamed to newIndex" << endl;
					// numRecords++; //This need to be changed to be the number of bits used
					//cout << "number of records updated" << endl;
				}
			}
			cout << "All blocks updated" << endl;
			cout << "attempting to close index file" << endl;
			if(fclose(index_file) == 0)
			{
				cout << "index closed" << endl;
			}
			else
			{
				cout << "index failed to close" << endl;
			}


			remove(fName.c_str());
			cout << "EmployeeIndex removed" << endl;

			// cout << "attempting to close temp file" << endl;
			// if(fclose(temp_file) == 0)
			// {
			// 	cout << "temp closed" << endl;
			// }
			// else
			// {
			// 	cout << "temp failed to close" << endl;
			// }
			// cout << "both files closed" << endl;
			rename("newIndex", fName.c_str());
			cout << "temp renamed to EmployeeIndex" << endl;
			// numRecords++; //This need to be changed to be the number of bits used
			cout << "number of records updated" << endl;

		}

		
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
			// cout << "Inserting record: " << i << endl;
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
				// i++
			} else {
				eof = false;
			}
		}
    }

    // Given an ID, find the relevant record and print it
    // Record findRecordById(int id) {
        
    // }
};
