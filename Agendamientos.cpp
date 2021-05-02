#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <stdio.h>
#include <limits>
#include <thread>
#include <tuple>
using namespace std;
const int cambCtx = 1;

vector< vector<int> > leerEntrada(string archivo){
    string myText;
    ifstream MyReadFile("procesos.txt");
    vector< vector<int> > procs;

    //Lee la primera linea
    getline (MyReadFile, myText);
    cout << myText << endl;

    while (getline (MyReadFile, myText)) {
        int id, prioridad, burst, arrival;
        sscanf(myText.c_str() , "%d %d %d %d" , &id, &prioridad, &burst, &arrival );
        int aux[] = {id, prioridad, burst, arrival};
		int n = sizeof(aux) / sizeof(aux[0]);
     	vector<int> t(aux, aux + n);
        procs.push_back(t);
        cout << myText << endl;
    }

    MyReadFile.close();
    return procs;
}


int indice(int id, vector< vector<int> > vec){ // retorna el indice donde se encutra el proceso con un id especifico
	for(int i = 0; i < vec.size(); i++){
		if(id == vec[i][0]){
			return i;
		}
	}
}

vector<int> extraer(vector< vector<int> > procs, int tiempo){
	vector< vector<int> > posibles;
	int max;
	vector<int> res;
	
	for(int i = 0; i < procs.size(); i++){ //calculo de las actividades que pueden ser ejecutadas
		if(procs[i][3] <= tiempo){
			posibles.push_back(procs[i]);
		}
	}
	
	if(posibles.empty()){ //si no hay actividades posibles, se retorna {-1, -1, -1, -1}
		int aux[] = {-1,-1,-1,-1};
		res.assign(aux,aux+4);
	}else{
		max = -1;
		for(int i = 0; i < posibles.size(); i++){ //de las posibles se toma la de mayor prioridad. Si dos tienen la mayor prioridad, se toma 
			if(posibles[i][1] > max){			 //la que se deba ejecutar primero
				res = posibles[i];
				max = posibles[i][1];
			}else if(posibles[i][1] == max and posibles[i][3] < res[3]){
				res = posibles[i];
			}
		}	
	}
	return res;
}

bool compareFCFS(vector<int> v1, vector<int> v2){
    return(v1[3]> v2[3]);
}

bool compareSJF(vector<int> v1, vector<int> v2){
    return(v1[2] < v2[2]);
}

void sjf(vector< vector<int> > procs){
    string res;
    float wait = 0;
    float burst = 0;
    float cant = 0;
    int id;
    float turn = 0;
    float totalTurn = 0;
    float totalWait = 0;
    int clock = 0;
    bool encontro = false;
    string gant;
    //Tabla de gant
    res += "==(SJF)============================================= \n";
    res += "ID | Burst | Waiting-Time | Response-Time \n";
    sort(procs.begin(), procs.end(), compareSJF);

    while(!(procs.empty())){
        
        for(auto it = begin(procs); it != end(procs); it++) {
            if (clock >= (*it)[3]){
                encontro = true;
                clock += cambCtx; //agrega el tiempo del cambio de contexto al entrar y salir el proceso
                id = (*it)[0];
                burst = (*it)[2];
                wait = clock - (*it)[3];
                turn = burst + clock;
                res += to_string((int)id) + " | " + to_string((int)burst) + " | "  + to_string((int)wait) +  " | " + to_string((int)turn) + "\n";
                //Contruiur GANT
                for(int j =0; j < cambCtx;  j++){ //se simboliza con c el cambio de contexto
            	gant += "c";
        		}
                gant += "P[" + to_string(id) + "]";
                for(int j = 0; j < burst;  j++){
                    gant += "-";
                    clock++;
                }
                cant++;
                procs.erase(it);
                totalTurn += turn;
                totalWait += wait;
                break;
            }
        }
        if(!encontro){
            clock++;
            gant += "*";
        }
        encontro = false;
    }
    //Imprimir 
            res += "-Promedio Waiting-Time: " +  to_string(totalWait / cant) + "\n";
            res += "-Promedio Turnaraound-Time: " + to_string( totalTurn / cant ) + "\n";
            res +=  gant  + "\n";
            res += "======================================================= \n ";

	cout << res << endl;
}

void fcfs(vector< vector<int> > procs)
{
    float wait = 0;
    float burst = 0;
    float cant = 0;
    int id;
    float turn = 0;
    float totalTurn = 0;
    float totalWait = 0;
    int clock = 0;
    string res;
    string gant;
    //Tabla de gant
    res += "==(FCFS)============================================= \n";
    res += "ID | Burst | Waiting-Time | Response-Time \n";
    sort(procs.begin(), procs.end(), compareFCFS);

    while(!(procs.empty())){
        //Si ya esta en la hora de ejecucion
        if ((procs.back())[3] <= clock) {
        	clock += cambCtx; //agrega el tiempo del cambio de contexto al entrar y salir el proceso
            id = (procs.back())[0];
            burst = (procs.back())[2];
            wait = clock - (procs.back())[3];
            turn = burst + clock;
            res += to_string((int)id) + " | " + to_string((int)burst) + " | "  + to_string((int)wait) +  " | " + to_string((int)turn) + "\n";
            //Contruiur GANT
            
            for(int j =0; j < cambCtx;  j++){ //se simboliza con c el cambio de contexto
            	gant += "c";
        	}
            gant += "P[" + to_string(id) + "]";
            for(int j = 0; j < burst;  j++){
                gant += "-";
                clock++;
            }
            cant++;
            procs.pop_back();
            totalTurn += turn;
            totalWait += wait;
        }
        else{
            clock++;
            gant += "*";

        }
    }
    //Imprimir 
            res += "-Promedio Waiting-Time: " +  to_string(totalWait / cant) + "\n";
            res += "-Promedio Turnaraound-Time: " + to_string( totalTurn / cant ) + "\n";
            res +=  gant  + "\n";
            res += "======================================================= \n ";

    cout << res << endl;
}




void priority(vector< vector<int> > procs)
{	
	string ans;
	int cant = procs.size();
	float tiempo = 0;
	vector<int> proceso;
	int ind;	
    float burst;
    int id;
    float turn = 0;
    string gant;
    
    ans = "==(Priority)=============================================\n";
    ans += "ID | Burst | Waiting-Time | Response-Time\n";

    
    while(!procs.empty()){ //hasta que haya agendado todas las actividades
    	proceso = extraer(procs, tiempo);
    	if(proceso[0] == -1 and proceso[1] == -1 and proceso[2] == -1 and proceso[3] == -1){ //si no hay proceso continuar con el reloj
    		tiempo += 1;
    		gant += "*";
		}else{
			tiempo += cambCtx; //agrega el tiempo del cambio de contexto al entrar y salir el proceso
			id = proceso[0];
			burst = proceso[2];
			turn += tiempo + burst;
			
			ans += to_string((int)id) + " | " + to_string((int)burst) + " | " + to_string((int)(tiempo - proceso[3])) + " | " + to_string((int)(burst + tiempo)) + "\n";
			
			for(int j =0; j < cambCtx;  j++){ //se simboliza con c el cambio de contexto
            	gant += "c";
        	}
			
			gant += "p[" + to_string(id) + "]";
			
			tiempo += burst;
			
			for(int j =0; j < burst;  j++){
            	gant += "-";
        	}
			
			ind = indice(proceso[0], procs);  
			procs.erase(procs.begin() + ind);
		}
	}
	
	ans += "-Promedio Waiting-Time: " + to_string(tiempo / cant) + "\n";
	ans += "-Promedio Turnaraound-Time: " + to_string(turn / cant) + "\n";
	ans += gant + "\n";
	ans += "=======================================================\n";
    cout << ans << endl;
}

int main(){
    string arch = "procesos.txt";
    vector< vector<int> > proc = leerEntrada(arch);
    vector< vector<int> > copia1 = proc;
    vector< vector<int> > copia2 = proc;
    vector< vector<int> > copia3 = proc;
    
    /*
    fcfs(copia1);
    sjf(copia2);
    priority(copia3);
    */
    
    thread t1(fcfs, ref(copia1));
    thread t2(sjf, ref(copia2));
    thread t3(priority, ref(copia3));
    
    t1.join();
    t2.join();
    t3.join();
    
    return 0;
}
