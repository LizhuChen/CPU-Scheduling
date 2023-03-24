#include <iostream> // cout, endl
#include <fstream> // open, is_open, close, ignore
#include <string> // string, find_last_of, substr
#include <string.h> // string, find_last_of, substr
#include <vector> // vector, push_back
#include <cstdlib> // system, atoi
#include <iomanip> // setw
#include <sstream>
#include <stdio.h>


using namespace std;

typedef struct ps {
    string all ;
    int ID = 0 ;
    int Burst = 0 ;
    int Arrival = 0 ;
    int Priority = 0 ;
    int do_Burst = 0 ;
} process;

typedef struct info {
    int ID = 0 ;
    int turnaround = 0 ;
    int waiting = 0 ;
} information;

string input = "" ;
int command = 0 ;
int slice = 0 ;

vector<info> FCFS ;
vector<info> RR ;
vector<info> PSJF ;
vector<info> NSJF ;
vector<info> PP ;
string FCFS_Gantt = "" ;
string RR_Gantt = "" ;
string PSJF_Gantt = "" ;
string NSJF_Gantt = "" ;
string PP_Gantt = "" ;
 
bool readFile( vector<ps> &process) {

	cout << endl << "Input file name : " ;
	cin >> input ;

	fstream file ;
	string tempname = input + ".txt" ;
	file.open( tempname.c_str(), ios::in);	
	
	if (!file.is_open()){
		cout << "檔案無法開啟\n" ;
		return false ;		
	}
	else {
		string temp_com = "" ;
		getline(file,temp_com) ;
		temp_com = temp_com + "\n" ;

   		vector <int> temp_data ;	
	
		for(int i = 0 ; i < temp_com.size() ; i++) {
			if(temp_com[i] != '\n' && temp_com[i] != ' ' && temp_com[i] != '\t'){
				string data = "";
				data = data + temp_com[i] ;
				i = i + 1 ;
				while(temp_com[i] != '\n' && temp_com[i] != ' ' && temp_com[i] != '\t'){
					data = data + temp_com[i] ;
					i = i + 1 ;
				}
    			stringstream ss(data.c_str()); 
    			int num = 0 ;
    			ss >> num ;
				temp_data.push_back(num) ;		
			}
		}
		command = temp_data[0] ;
		slice = temp_data[1] ;

	    string trash = "" ;
	    getline(file,trash) ; //把第二行的文字讀掉 


	    
	    string temp = "" ;
	    getline(file,temp) ;

		while( !file.eof()) { 
			ps temp_process ;
			temp_process.all = temp ;
			temp_process.all = temp_process.all + "\n" ;
			process.push_back(temp_process) ;
			getline(file,temp) ;

		}
		
		file.close() ;
	}
	
	return true ;
} 

    
void Filter( ps &temp ) { 
    int num = 0 ;
    int i = 0 ;
	
    vector <int> temp_data ;

	for(int i = 0 ; i < temp.all.size(); i ++ ) {
		if(temp.all[i] != '\n' && temp.all[i] != ' ' && temp.all[i] != '\t'){
			string data = "";
			data = data + temp.all[i] ;
			i = i + 1 ;
			while(temp.all[i] != '\n' && temp.all[i] != ' ' && temp.all[i] != '\t'){
				data = data + temp.all[i] ;
				i = i + 1 ;
			}
    		stringstream ss(data.c_str()); // s?string??int
    		ss >> num ;
			temp_data.push_back(num) ;		
		}
	}
	
	temp.ID = temp_data[0] ;
	temp.Burst = temp_data[1] ;
	temp.Arrival = temp_data[2] ;
	temp.Priority = temp_data[3] ;		

}

void BubbleSort (vector<ps> &process) {
    
    for( int i = 0 ; i < process.size() ; i++) {
        
        for( int j = 0; j < process.size() ; j++) {
        	
            if( process[j].Arrival > process[i].Arrival ) {
			                
                ps temp = process[j] ;
                process[j] = process[i] ;
                process[i] = temp ;
                
            }
            else if (process[j].Arrival == process[i].Arrival){
            	if(process[j].ID > process[i].ID){
                	ps temp = process[j] ;
                	process[j] = process[i] ;
                	process[i] = temp ;            		
				}
			}
        }
    }
}

string int_to_string(int number){
	char arrMap[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	string str = "" ;
	number = number % 36 ;
	
	if(number < 10){
		str = str + to_string(number);
	} 
	else {
		int index = number - 10 ;
		str = str + arrMap[index] ;

	}
	
	return str ;
}

void do_FCFS(vector<ps> process){
	int nowtime = 0 ;
	for(int i = 0 ; i < process.size() ; i ++ ){
		info temp ;
		temp.ID = process[i].ID ;
		int pre_nowtime = nowtime ; 
		if(process[i].Arrival > nowtime){
			nowtime = process[i].Arrival ;
		}
		
		pre_nowtime = nowtime - pre_nowtime ;
		
		for(int k = 0 ; k < pre_nowtime ; k ++ ){
			FCFS_Gantt = FCFS_Gantt + "-" ;
		}
		
		for(int k = 0 ; k < process[i].Burst ; k ++ ) {
			string str = int_to_string(process[i].ID) ;
			FCFS_Gantt = FCFS_Gantt + str ;
		}
		
		nowtime = nowtime + process[i].Burst ;
		temp.turnaround = nowtime - process[i].Arrival ;
		temp.waiting = temp.turnaround - process[i].Burst ;
		FCFS.push_back(temp) ;
	}

}

void do_RR(vector<ps> process){
	vector<ps> t1_process = process;
	vector<ps> t2_process = t1_process ;
	vector<ps> queue ;
	int nowtime = 0 ;
	for(int i = 0 ; i < t2_process.size(); i++ ) {
		if(t2_process[i].Arrival <= nowtime){
			queue.push_back(t2_process[i]);
			t1_process.erase(t1_process.begin()) ;
		}
	}
	t2_process = t1_process ;
	
	while(t2_process.size() != 0 || queue.size() != 0){

		if(queue.size() != 0) {
			ps temp_process = queue.front() ; // 取出queue第一個並刪除 


			queue.erase(queue.begin()) ;

			int pre_time = nowtime ;
			if( temp_process.Burst - temp_process.do_Burst < slice ) {
				nowtime = nowtime + temp_process.Burst - temp_process.do_Burst ;
				temp_process.do_Burst = temp_process.Burst;
			}
			else {
				temp_process.do_Burst = temp_process.do_Burst + slice ;
				nowtime = nowtime + slice ;
			}

			for(int k = 0 ; k < nowtime - pre_time ; k ++ ) {
				string str = int_to_string(temp_process.ID) ;
				RR_Gantt = RR_Gantt + str ;
			}
		
			for(int i = 0 ; i < t2_process.size(); i++ ) {
				if(t2_process[i].Arrival <= nowtime){
					queue.push_back(t2_process[i]);
					t1_process.erase(t1_process.begin()) ;
				}
			}
			
			t2_process = t1_process ;
		
			
			if(temp_process.do_Burst == temp_process.Burst ) {
				info temp ;
				temp.ID = temp_process.ID ;
				temp.turnaround = nowtime - temp_process.Arrival ;
				temp.waiting = temp.turnaround - temp_process.Burst ;
				RR.push_back(temp) ;			
			}
			else{
				queue.push_back(temp_process) ;
			}		
		}
		else{
			int pre_nowtime = nowtime ;

			for(int j = 1 ; j <= slice && queue.size() == 0 ; j ++ ) {
				nowtime = nowtime + j ;
				for(int i = 0 ; i < t2_process.size(); i++ ) {
					
					if(t2_process[i].Arrival <= nowtime){
						queue.push_back(t2_process[i]);
						t1_process.erase(t1_process.begin()) ;
					}
				}
				
				t2_process = t1_process ;					
			}
	
			pre_nowtime = nowtime - pre_nowtime ;
		
			for(int k = 0 ; k < pre_nowtime ; k ++ ){
				RR_Gantt = RR_Gantt + "-" ;
			}			
		}


	}

}

void BubbleSort_PSJF (vector<ps> &queue) {
    
    for( int i = 0 ; i < queue.size() ; i++) {
        
        for( int j = 0; j < queue.size() ; j++) {
        	
            if( queue[j].do_Burst > queue[i].do_Burst ) {
			                
                ps temp = queue[j] ;
                queue[j] = queue[i] ;
                queue[i] = temp ;
                
            }
            else if (queue[j].do_Burst == queue[i].do_Burst){
            	if(queue[j].Burst > queue[i].Burst){
                	ps temp = queue[j] ;
                	queue[j] = queue[i] ;
                	queue[i] = temp ;            		
				}
				else if(queue[j].Burst == queue[i].Burst) {
				
					if( queue[j].Arrival > queue[i].Arrival ){
	                	ps temp = queue[j] ;
	                	queue[j] = queue[i] ;
	                	queue[i] = temp ;  
					}
					else if (queue[j].Arrival == queue[i].Arrival ) {
						if( queue[j].ID > queue[i].ID ){
		                	ps temp = queue[j] ;
		                	queue[j] = queue[i] ;
		                	queue[i] = temp ;  
						}						
					}
				}

			}
        }
    }
}

void BubbleSort_NSJF (vector<ps> &queue) {
    
    for( int i = 0 ; i < queue.size() ; i++) {
        
        for( int j = 0; j < queue.size() ; j++) {
        	
            if( queue[j].do_Burst > queue[i].do_Burst ) {
			                
                ps temp = queue[j] ;
                queue[j] = queue[i] ;
                queue[i] = temp ;
                
            }
            else if (queue[j].do_Burst == queue[i].do_Burst){
            	if(queue[j].Arrival > queue[i].Arrival){
                	ps temp = queue[j] ;
                	queue[j] = queue[i] ;
                	queue[i] = temp ;            		
				}
				else if(queue[j].Arrival == queue[i].Arrival) {
				

					if( queue[j].ID > queue[i].ID ){
		               	ps temp = queue[j] ;
		               	queue[j] = queue[i] ;
		               	queue[i] = temp ;  
					}						
					
				}

			}
        }
    }
}

void do_NSJF(vector<ps> process){
	vector<ps> t1_process = process;
	vector<ps> t2_process = t1_process ;
	vector<ps> queue ;
	int nowtime = 0 ;
	for(int i = 0 ; i < t2_process.size(); i++ ) {
		if(t2_process[i].Arrival <= nowtime){
			t2_process[i].do_Burst = t2_process[i].Burst ;
			queue.push_back(t2_process[i]);
			t1_process.erase(t1_process.begin()) ;
		}
	}
	t2_process = t1_process ;
	BubbleSort_NSJF(queue) ;
	
	while(t2_process.size() != 0 || queue.size() != 0){

		if(queue.size() != 0) {
			BubbleSort_NSJF(queue) ;
			ps temp_process = queue.front() ; // 取出queue第一個並刪除 
	
			queue.erase(queue.begin()) ;
			
			int pre_time = nowtime ;
			nowtime = nowtime + temp_process.do_Burst ;
			temp_process.do_Burst = 0 ;
			
			for(int k = 0 ; k < nowtime - pre_time ; k ++ ) {
				string str = int_to_string(temp_process.ID) ;
				NSJF_Gantt = NSJF_Gantt + str ;
			}
		
			for(int i = 0 ; i < t2_process.size(); i++ ) {
				if(t2_process[i].Arrival <= nowtime){
					t2_process[i].do_Burst = t2_process[i].Burst ;
					queue.push_back(t2_process[i]);
					t1_process.erase(t1_process.begin()) ;
				}
			}
	
			t2_process = t1_process ;
			BubbleSort_NSJF(queue) ;			
			
			if(temp_process.do_Burst == 0 ) {
				info temp ;
				temp.ID = temp_process.ID ;
				temp.turnaround = nowtime - temp_process.Arrival ;
				temp.waiting = temp.turnaround - temp_process.Burst ;
				NSJF.push_back(temp) ;			
			}
			else{
				queue.push_back(temp_process) ;
			}		
		}
		else{
			int pre_nowtime = nowtime ;
			for(int j = 1 ; j <= slice && queue.size() == 0 ; j ++ ) {
				nowtime = nowtime + j ;
				for(int i = 0 ; i < t2_process.size(); i++ ) {
					
					if(t2_process[i].Arrival <= nowtime){
						t2_process[i].do_Burst = t2_process[i].Burst ;
						queue.push_back(t2_process[i]);
						t1_process.erase(t1_process.begin()) ;
					}
				}
				
				t2_process = t1_process ;					
			}
			
			BubbleSort_NSJF(queue) ;	

			pre_nowtime = nowtime - pre_nowtime ;
		
			for(int k = 0 ; k < pre_nowtime ; k ++ ){
				NSJF_Gantt = NSJF_Gantt + "-" ;
			}
		}


	}

}

void do_PSJF(vector<ps> process){
	vector<ps> t1_process = process;
	vector<ps> t2_process = t1_process ;
	vector<ps> queue ;
	int nowtime = 0 ;
	for(int i = 0 ; i < t2_process.size(); i++ ) {
		if(t2_process[i].Arrival <= nowtime){
			t2_process[i].do_Burst = t2_process[i].Burst ;
			queue.push_back(t2_process[i]);
			t1_process.erase(t1_process.begin()) ;
		}
	}
	t2_process = t1_process ;
	BubbleSort_PSJF(queue) ;
	
	while(t2_process.size() != 0 || queue.size() != 0){

		if(queue.size() != 0) {
			BubbleSort_PSJF(queue) ;
			ps temp_process = queue.front() ; // 取出queue第一個並刪除 

			queue.erase(queue.begin()) ;
			int pre_time = nowtime ;
			temp_process.do_Burst = temp_process.do_Burst - 1 ;
			nowtime = nowtime  + 1 ;			
		
			for(int i = 0 ; i < t2_process.size(); i++ ) {
				if(t2_process[i].Arrival <= nowtime){
					t2_process[i].do_Burst = t2_process[i].Burst ;
					queue.push_back(t2_process[i]);
					t1_process.erase(t1_process.begin()) ;
				}
			}
			
			t2_process = t1_process ;
			BubbleSort_PSJF(queue) ;
				
				
/*			打開後input4對 
			while(temp_process.do_Burst != 0 && temp_process.do_Burst == queue[0].do_Burst){
				temp_process.do_Burst = temp_process.do_Burst - 1 ;
				nowtime = nowtime  + 1 ;		
					
				for(int i = 0 ; i < t2_process.size(); i++ ) {
					if(t2_process[i].Arrival <= nowtime){
						t2_process[i].do_Burst = t2_process[i].Burst ;
						queue.push_back(t2_process[i]);
						t1_process.erase(t1_process.begin()) ;
					}
				}
					
				t2_process = t1_process ;
				BubbleSort_PSJF(queue) ;				
			}			
*/ 


			for(int k = 0 ; k < nowtime - pre_time ; k ++ ) {
				string str = int_to_string(temp_process.ID) ;
				PSJF_Gantt = PSJF_Gantt + str ;
			}					
			
			if(temp_process.do_Burst == 0 ) {
				info temp ;
				temp.ID = temp_process.ID ;
				temp.turnaround = nowtime - temp_process.Arrival ;
				temp.waiting = temp.turnaround - temp_process.Burst ;
				PSJF.push_back(temp) ;			
			}
			else{
				queue.push_back(temp_process) ;
			}		
		}
		else{
			int pre_nowtime = nowtime ;
			for(int j = 1 ; j <= slice && queue.size() == 0 ; j ++ ) {
				nowtime = nowtime + j ;
				for(int i = 0 ; i < t2_process.size(); i++ ) {
					
					if(t2_process[i].Arrival <= nowtime){
						t2_process[i].do_Burst = t2_process[i].Burst ;
						queue.push_back(t2_process[i]);
						t1_process.erase(t1_process.begin()) ;
					}
				}
				
				t2_process = t1_process ;					
			}
			
			BubbleSort_PSJF(queue) ;	
			pre_nowtime = nowtime - pre_nowtime ;
		
			for(int k = 0 ; k < pre_nowtime ; k ++ ){
				PSJF_Gantt = PSJF_Gantt + "-" ;
			}
		}


	}

}

void BubbleSort_PP (vector<ps> &queue) {
    
    for( int i = 0 ; i < queue.size() ; i++) {
        
        for( int j = 0; j < queue.size() ; j++) {
        	
            if( queue[j].Priority > queue[i].Priority ) {
			                
                ps temp = queue[j] ;
                queue[j] = queue[i] ;
                queue[i] = temp ;
                
            }
            else if (queue[j].Priority == queue[i].Priority){
            	int queue_j = queue[j].Burst - queue[j].do_Burst ;
            	int queue_i = queue[i].Burst - queue[i].do_Burst ;
            	if(queue_i == 0 && queue_j != 0){
                	ps temp = queue[j] ;
                	queue[j] = queue[i] ;
                	queue[i] = temp ;            		
				}
				else if(queue_j == 0 && queue_i == 0) {
				
					if( queue[j].Arrival > queue[i].Arrival ){
	                	ps temp = queue[j] ;
	                	queue[j] = queue[i] ;
	                	queue[i] = temp ;  
					}
					else if (queue[j].Arrival == queue[i].Arrival ) {
						if( queue[j].ID > queue[i].ID ){
		                	ps temp = queue[j] ;
		                	queue[j] = queue[i] ;
		                	queue[i] = temp ;  
						}						
					}
				}
				else if(queue_j != 0 && queue_i != 0) {
				
					if( queue[j].Arrival > queue[i].Arrival ){
	                	ps temp = queue[j] ;
	                	queue[j] = queue[i] ;
	                	queue[i] = temp ;  
					}
					else if (queue[j].Arrival == queue[i].Arrival ) {
						if( queue[j].ID > queue[i].ID ){
		                	ps temp = queue[j] ;
		                	queue[j] = queue[i] ;
		                	queue[i] = temp ;  
						}						
					}
				}

			}
        }
    }
}

void do_PP(vector<ps> process){
	vector<ps> t1_process = process;
	vector<ps> t2_process = t1_process ;
	vector<ps> queue ;
	int nowtime = 0 ;
	for(int i = 0 ; i < t2_process.size(); i++ ) {
		if(t2_process[i].Arrival <= nowtime){
			t2_process[i].do_Burst = t2_process[i].Burst ;
			queue.push_back(t2_process[i]);
			t1_process.erase(t1_process.begin()) ;
		}
	}
	t2_process = t1_process ;
	BubbleSort_PP(queue) ;
	
	while(t2_process.size() != 0 || queue.size() != 0){

		if(queue.size() != 0) {
			BubbleSort_PP(queue) ;
			ps temp_process = queue.front() ; // 取出queue第一個並刪除 
			
			int pre_time = nowtime ;
			
			queue.erase(queue.begin()) ;
			temp_process.do_Burst = temp_process.do_Burst - 1 ;
			nowtime = nowtime  + 1 ;

			
		
			for(int i = 0 ; i < t2_process.size(); i++ ) {
				if(t2_process[i].Arrival <= nowtime){
					t2_process[i].do_Burst = t2_process[i].Burst ;
					queue.push_back(t2_process[i]);
					t1_process.erase(t1_process.begin()) ;
				}
			}
			
			t2_process = t1_process ;
			BubbleSort_PP(queue) ;	
			
			
			while(temp_process.do_Burst != 0 && temp_process.Priority == queue[0].Priority){
				temp_process.do_Burst = temp_process.do_Burst - 1 ;
				nowtime = nowtime  + 1 ;		
					
				for(int i = 0 ; i < t2_process.size(); i++ ) {
					if(t2_process[i].Arrival <= nowtime){
						t2_process[i].do_Burst = t2_process[i].Burst ;
						queue.push_back(t2_process[i]);
						t1_process.erase(t1_process.begin()) ;
					}
				}
					
				t2_process = t1_process ;
				BubbleSort_PP(queue) ;				
			}

			for(int k = 0 ; k < nowtime - pre_time ; k ++ ) {
				string str = int_to_string(temp_process.ID) ;
				PP_Gantt = PP_Gantt + str ;
			}	
			
						
			if(temp_process.do_Burst == 0 ) {
				info temp ;
				temp.ID = temp_process.ID ;
				temp.turnaround = nowtime - temp_process.Arrival ;
				temp.waiting = temp.turnaround - temp_process.Burst ;
				PP.push_back(temp) ;			
			}
			else{
				queue.push_back(temp_process) ;
			}		
		}
		else{
			int pre_nowtime = nowtime ;
			for(int j = 1 ; j <= slice && queue.size() == 0 ; j ++ ) {
				nowtime = nowtime + j ;
				for(int i = 0 ; i < t2_process.size(); i++ ) {
					
					if(t2_process[i].Arrival <= nowtime){
						t2_process[i].do_Burst = t2_process[i].Burst ;
						queue.push_back(t2_process[i]);
						t1_process.erase(t1_process.begin()) ;
					}
				}
				
				t2_process = t1_process ;					
			}
			
			BubbleSort_PP(queue) ;	
			pre_nowtime = nowtime - pre_nowtime ;
		
			for(int k = 0 ; k < pre_nowtime ; k ++ ){
				PP_Gantt = PP_Gantt + "-" ;
			}
		}


	}

}

void BubbleSort_process (vector<info> &process) {
    
    for( int i = 0 ; i < process.size() ; i++) {
        
        for( int j = 0; j < process.size() ; j++) {
        	
            if( process[j].ID > process[i].ID ) {
			                
                info temp = process[j] ;
                process[j] = process[i] ;
                process[i] = temp ;
                
            }
        }
    }
}


void saveFile() { //逐一輸出檔案內容 
	string tempname = input +"_output.txt" ;
	fstream file ;
	file.open( tempname.c_str(), ios::out) ; //開啟寫入模式 

	file << "==    FCFS==" << endl ;
	file << FCFS_Gantt << endl ;
	file << "==      RR==" << endl ;
	file << RR_Gantt << endl ;
	file << "==    PSJF==" << endl ;
	file << PSJF_Gantt << endl ;
	file << "==Non-PSJF==" << endl ;
	file << NSJF_Gantt << endl ;	
	file << "== Priority==" << endl ;
	file << PP_Gantt << endl ;
	file << "===========================================================" << endl << endl ;		 

	file << "Waiting Time" << endl ;
	file << "ID      FCFS    RR      PSJF    NPSJF   Priority" << endl ; 
	file << "===========================================================" << endl ;
	for(int i = 0 ; i < FCFS.size(); i ++ ){
		file  << left << setw(8) << FCFS[i].ID << left << setw(8) << FCFS[i].waiting << left << setw(8) << RR[i].waiting << left << setw(8) << PSJF[i].waiting << left << setw(8) << NSJF[i].waiting << left << setw(8) << PP[i].waiting << endl ;
	}

	file << "===========================================================" << endl << endl ;
	file << "Turnaround Time" << endl ;
	file << "ID      FCFS    RR      PSJF    NPSJF   Priority" << endl ; 
	file << "===========================================================" << endl ;
	for(int i = 0 ; i < FCFS.size(); i ++ ){
		file << left << setw(8) <<FCFS[i].ID << left << setw(8) << FCFS[i].turnaround << left << setw(8) << RR[i].turnaround << left << setw(8) << PSJF[i].turnaround << left << setw(8) << NSJF[i].turnaround << left << setw(8) << PP[i].turnaround << endl ;
	}
	file << "===========================================================" << endl ;
	

	file.close() ; 
}

void saveFile_one(vector<info> process,string Gantt ) { //逐一輸出檔案內容 
	string tempname = input +"_output.txt" ;
	fstream file ;
	file.open( tempname.c_str(), ios::out) ; //開啟寫入模式 

	file << "==    Gantt==" << endl ;
	file << Gantt << endl ;

	file << "====================" << endl << endl ;		 

	file << "Waiting Time" << endl ;
	file << "ID      Waiting Time" << endl ; 
	file << "====================" << endl ;
	for(int i = 0 ; i < process.size(); i ++ ){
		file  << left << setw(8) << process[i].ID << left << setw(8) << process[i].waiting  << endl ;
	}

	file << "====================" << endl << endl ;
	file << "Turnaround Time" << endl ;
	file << "ID      Turnaround Time" << endl ; 
	file << "====================" << endl ;
	for(int i = 0 ; i < process.size(); i ++ ){
		file << left << setw(8) << process[i].ID << left << setw(8) << process[i].turnaround << endl ;
	}
	file << "====================" << endl ;
	

	file.close() ; 
}

void Mission( vector<ps> process ){

	switch (command)  { 
		case 0: break;
 		case 1: {
			do_FCFS(process) ;
 			BubbleSort_process (FCFS) ;
 			saveFile_one(FCFS,FCFS_Gantt ) ;
 			break;
 		} 
 		case 2: {	
			do_RR(process) ;
			BubbleSort_process (RR) ;
 			saveFile_one(RR,RR_Gantt ) ;			
 			break;
 		} 	
 		case 3: {
			do_PSJF(process) ;
			BubbleSort_process (PSJF) ;
 			saveFile_one(PSJF,PSJF_Gantt ) ;								
 			break;
 		}
 		case 4: {
			do_NSJF(process) ;
			BubbleSort_process (NSJF) ;
 			saveFile_one(NSJF,NSJF_Gantt ) ;			  			
 			break;
 		}
 		case 5: {
 			do_PP(process) ;
 			BubbleSort_process (PP) ;
  			saveFile_one(PP,PP_Gantt ) ;			
			break;
		 }
 		case 6: {
		 	do_FCFS(process) ;
			do_RR(process) ;
			do_PSJF(process) ;
			do_NSJF(process) ;
			do_PP(process) ;
		
			BubbleSort_process (FCFS) ;
			BubbleSort_process (RR) ;
			BubbleSort_process (PSJF) ;
			BubbleSort_process (NSJF) ;
			BubbleSort_process (PP) ;
			saveFile() ;
			
			break;
		 }
 	}
}

int main(void) {
	vector<ps> process ;
	readFile(process) ;
	for(int i = 0 ; i < process.size(); i ++ ){
		Filter( process[i] ) ;
	}
	BubbleSort( process) ; 
	Mission( process ) ;

	
	return 0;
} // end of main









