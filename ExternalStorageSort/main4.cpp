/* This is a skeleton code for two-pass multi-way sorting, you can make modifications as long as you meet 
   all question requirements*/  
//Currently this program will just load the records in the buffers (main memory) and print them when filled up.
//And continue this process untill the full Emp.csv is read. 

#include <bits/stdc++.h>
using namespace std;

//defines how many buffers are available in the Main Memory 
#define buffer_size 22

struct EmpRecord {
    int eid;		// 4 bytes
    string ename;	// 40 bytes
    int age;		// 4 bytes
    double salary;	// 8 bytes
};
EmpRecord buffers[buffer_size]; // this structure contains 22 buffers; available as your main memory.

// Grab a single block from the Emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpRecords and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpRecord Grab_Emp_Record(fstream &empin) {
    string line, word;
    EmpRecord  emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        emp.eid = stoi(word); // stoi = string to int
        getline(s, word, ',');
        emp.ename = word;
        getline(s, word, ',');
        emp.age = stoi(word);
        getline(s, word, ',');
        emp.salary = stod(word); //stod = string to double
        return emp;
    } else {
        emp.eid = -1;
        return emp;
    }
}

//Printing whatever is stored in the buffers of Main Memory 
//Can come in handy to see if you've sorted the records in your main memory properly.
void Print_Buffers(int cur_size) {
    for (int i = 0; i < cur_size; i++)
    {
        cout << std::fixed << i << " " << buffers[i].eid << " " << buffers[i].ename << " " << buffers[i].age << " " << std::setprecision(0) << buffers[i].salary << endl;
    }
}

bool comparitor(EmpRecord a, EmpRecord b)
{
	return a.eid < b.eid;
}

//Sorting the buffers in main_memory based on 'eid' and storing the sorted records into a temporary file 
//You can change return type and arguments as you want. 
void Sort_in_Main_Memory(int cur_size){
    // cout << "Sorting Not Implemented" << endl;
	sort(buffers, buffers+cur_size, comparitor);
    return;
}

void Write_run(int run, int cur_size)
{
	fstream output_file;
	string filename = "run"+to_string(run)+".csv";
	output_file.open(filename, ios::out);
	for (int i = 0; i < cur_size; i++)
	{
		output_file << std::fixed << buffers[i].eid << ',' << buffers[i].ename << ',' << buffers[i].age << ',' << std::setprecision(0) << buffers[i].salary << "\n";
	}
	output_file.close();
}

//You can use this function to merge your M-1 runs using the buffers in main memory and storing them in sorted file 'EmpSorted.csv'(Final Output File) 
//You can change return type and arguments as you want. 
void Merge_Runs_in_Main_Memory(){
    cout << "Merging Not Implemented" << endl;
}

int main() {
	// open file streams to read and write
	fstream input_file;
	input_file.open("Emp.csv", ios::in);
 
	// flags check when relations are done being read
	bool flag = true;
	int cur_size = 0;
	int pass = 0;
	int run = 0;
	/*First Pass: The following loop will read each block put it into main_memory 
	  and sort them then will put them into a temporary file for 2nd pass */
	while (flag) {
	    // FOR BLOCK IN RELATION EMP	
	    // grabs a block
	    EmpRecord  single_EmpRecord  = Grab_Emp_Record(input_file);
	    // checks if filestream is empty
	    if (single_EmpRecord.eid == -1) {
	        flag = false;
	        Print_Buffers(cur_size); // The main_memory is not filled up but there are some leftover data that needs to be sorted.
			Sort_in_Main_Memory(cur_size);
			cout << "Sorted" << endl;
			Print_Buffers(cur_size);	
			Write_run(run, cur_size);
	    }
	    if(cur_size + 1 <= buffer_size){
	        //Memory is not full store current record into buffers.
	        buffers[cur_size] = single_EmpRecord ;
	        cur_size += 1;
	    }
	    else{
	        //memory is full now. Sort the blocks in Main Memory and Store it in a temporary file (runs)
	        cout << "Main Memory is full. Time to sort and store sorted blocks in a temporary file" << endl;
	        Print_Buffers(buffer_size);
	        //SortMain("Attributes You Want");
	      	Sort_in_Main_Memory(buffer_size);
			cout << "Sorted" << endl;
			Print_Buffers(buffer_size);	
			Write_run(run, buffer_size);
			run++;
	        //After sorting, start again. Clearing memory and putting the current one into main memory.
	        cur_size = 0;
	        buffers[cur_size] = single_EmpRecord;
	        cur_size += 1;
	    }
	
	}
	input_file.close();

	/* Implement 2nd Pass: Read the temporary sorted files and utilize main_memory to store sorted runs into the EmpSorted.csv*/	
	//Uncomment when you are ready to store the sorted relation
	//fstream sorted_file;  
	//sorted_file.open("EmpSorted.csv", ios::out | ios::app);	
	//Pseudocode
	bool flag_sorting_done = false;
	while(!flag_sorting_done){
	    Merge_Runs_in_Main_Memory();
	    break;
	}
  
  //You can delete the temporary sorted files (runs) after you're done if you want. It's okay if you don't.

  return 0;
}
