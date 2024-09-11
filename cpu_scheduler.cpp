#include <bits/stdc++.h>
#include <fstream>
#include <array>
using namespace std;

float max(float a, float b){
    if(a>b){
        return a;
    } else {
        return b;
    }
}

bool IncArrTime(pair<int, pair<float, float>>& a, pair<int, pair<float, float>>& b){
    return a.second.first < b.second.first;
}

bool DecArrTime(pair<int, pair<float, float>>& a, pair<int, pair<float, float>>& b){
    return a.second.first > b.second.first;
}

bool IncBurstTime(pair<int, pair<float, float>>& a, pair<int, pair<float, float>>& b){
    return a.second.second < b.second.second;
}

bool DecBurstTime(pair<int, pair<float, float>>& a, pair<int, pair<float, float>>& b){
    return a.second.second > b.second.second;
}

void GanttChart(vector<vector<float>> exe, ofstream& outputFile){
    int n = exe.size();
    for(int i=0; i<n; i++){
      if(i==0){
        outputFile << "|   P" << exe[i][0] << "   ";
      } else if(i>0 && exe[i][0]!=exe[i-1][0]){
        if(exe[i][1]!=exe[i-1][4]){
            outputFile << "|********|   P" << exe[i][0] << "   ";
        } else {
            outputFile << "|   P" << exe[i][0] << "   ";
        }
      }
    }
    outputFile << "|\n";
    outputFile << "\n";

    for(int i=0; i<n; i++){
      if(i==0){
        outputFile << exe[i][1] << "       ";
      } else if(i>0 && exe[i][0]!=exe[i-1][0]){
        if(exe[i][1]!=exe[i-1][4]){
            outputFile << exe[i-1][4] << "       " << exe[i][1] << "       ";
        } else {
            outputFile << exe[i][1] << "       ";
        }
      }
      if(i==n-1){
        outputFile << exe[i][4] << "\n";
      }
    }
    outputFile << "\n";
}

void AvgValues(vector<vector<float>> exe, ofstream& outputFile){
    int n = exe.size();
    float avgWT = 0, avgTAT = 0;
    for(int i=0; i<n; i++){
      avgWT += (exe[i][4] - exe[i][2] - exe[i][3]);
      avgTAT += (exe[i][4] - exe[i][3]);
    }
    avgWT /= n;
    avgTAT /= n;
    outputFile << "Average Waiting Time : " << avgWT << ", Average Turn-around Time : " << avgTAT << "\n";
    outputFile << "\n\n\n";
}

vector< vector<float>> FCFS(vector< pair<int, pair<float, float>>> processes, ofstream& outputFile){
    vector< pair<int, pair<float, float>>> original(processes.begin(), processes.end());
    sort(processes.begin(), processes.end(), IncArrTime);
    vector< vector<float>> run;
    float RunTime = 0;

    for(auto& p : processes){
        float pID = p.first;
        float arrTime = p.second.first;
        float burstTime = p.second.second;

        if(RunTime<arrTime){
            RunTime = arrTime;
        }

        float waitTime = max(0.0f, RunTime - arrTime);
        float TATime = RunTime + burstTime;

        run.push_back({pID, RunTime, original[pID-1].second.second, original[pID-1].second.first, TATime});
        RunTime = TATime;
    }
    outputFile << "FCFS :\n";
    GanttChart(run, outputFile);
    return run;
}

vector< vector<float>> SJF(vector< pair<int, pair<float, float>>> processes, ofstream& outputFile){
    vector< pair<int, pair<float, float>>> original(processes.begin(), processes.end());
    sort(processes.begin(), processes.end(), IncArrTime);
    
    multiset <pair<float, pair<float, int>>> ReadyQueue;
    vector< vector<float>> run;

    int idx = 0;
    float RunTime = 0;
    int totalProcesses = processes.size();
    
    while(idx < totalProcesses || !ReadyQueue.empty()){

        while(idx < totalProcesses && processes[idx].second.first <= RunTime){
            ReadyQueue.insert({processes[idx].second.second, {processes[idx].second.first, processes[idx].first}});
            idx++;
        }

        if(ReadyQueue.empty()){
            RunTime = processes[idx].second.first;
            continue;
        }

        pair<float, pair<float, int>> RunningProcess = *ReadyQueue.begin();
        ReadyQueue.erase(ReadyQueue.begin());

        
        float pID = RunningProcess.second.second;
        float arrTime = RunningProcess.second.first;
        float burstTime = RunningProcess.first;

        float waitTime = max(0.0f, RunTime - arrTime);
        float TATime = RunTime + burstTime;

        run.push_back({pID, RunTime, original[pID-1].second.second, original[pID-1].second.first, TATime});
        RunTime += burstTime;
    }
    outputFile << "SJF :\n";
    GanttChart(run, outputFile);
    return run;
}

struct DecSet {
    bool operator()(const pair<float, pair<float, int>>& a, const pair<float, pair<float, int>>& b) const {
        return a.first > b.first;
    }
};

vector< vector<float>> LJF(vector< pair<int, pair<float, float>>> processes, ofstream& outputFile) {
    vector< pair<int, pair<float, float>>> original(processes.begin(), processes.end());
    sort(processes.begin(), processes.end(), IncArrTime);

    multiset< pair<float, pair<float, int>>, DecSet> ReadyQueue;
    vector< vector<float>> run;

    int idx = 0;
    float RunTime = 0;
    int totalProcesses = processes.size();

    while (idx < totalProcesses || !ReadyQueue.empty()) {
        while (idx < totalProcesses && processes[idx].second.first <= RunTime) {
            ReadyQueue.insert({processes[idx].second.second, {processes[idx].second.first, processes[idx].first}});
            idx++;
        }

        if (ReadyQueue.empty()) {
            if (idx < totalProcesses) {
                RunTime = processes[idx].second.first;
            }
            continue;
        }

        auto RunningProcess = *ReadyQueue.begin();
        ReadyQueue.erase(ReadyQueue.begin());

        float pID = RunningProcess.second.second;
        float arrTime = RunningProcess.second.first;
        float burstTime = RunningProcess.first;

        float waitTime = max(0.0f, RunTime - arrTime);
        float TATime = RunTime + burstTime;

        run.push_back({pID, RunTime, original[pID - 1].second.second, original[pID - 1].second.first, TATime});
        RunTime += burstTime;
    }

    outputFile << "LJF :\n";
    GanttChart(run, outputFile);
    return run;
}


vector< vector<float>> SRTF(vector< pair<int, pair<float, float>>> processes, ofstream& outputFile){
    vector< pair<int, pair<float, float>>> original(processes.begin(), processes.end());
    sort(processes.begin(), processes.end(), IncArrTime);
    
    multiset < pair<float, pair<float, int>>> ReadyQueue;
    vector< vector<float>> run, exec;

    int idx = 0, n = processes.size();
    float RunTime = 0;
    
    ReadyQueue.insert({processes[idx].second.second, {processes[idx].second.first, processes[idx].first}});
    RunTime = max(RunTime, processes[idx].second.first);
    idx++;

    while(!ReadyQueue.empty() || idx < n){
        while(idx < n && processes[idx].second.first <= RunTime){
            ReadyQueue.insert({processes[idx].second.second, {processes[idx].second.first, processes[idx].first}});
            idx++;
        }

        if (ReadyQueue.empty()) {
            RunTime = processes[idx].second.first;
            ReadyQueue.insert({processes[idx].second.second, {processes[idx].second.first, processes[idx].first}});
            idx++;
        }

        pair<float, pair<float, int>> RunningProcess = *ReadyQueue.begin();
        ReadyQueue.erase(ReadyQueue.begin());

        float pID = RunningProcess.second.second;
        float arrTime = RunningProcess.second.first;
        float burstTime = RunningProcess.first;
        float remTime = burstTime - 1;

        float waitTime = max(0.0f, RunTime - arrTime);
        float TATime = RunTime + 1;
        
        exec.push_back({pID, RunTime, 1.0f, waitTime, RunTime+1});

        RunTime++;

        while(idx < n && processes[idx].second.first <= RunTime){
            ReadyQueue.insert({processes[idx].second.second, {processes[idx].second.first, processes[idx].first}});
            idx++;
        }

        if(remTime == 0){
            run.push_back({pID, arrTime, original[pID-1].second.second, original[pID-1].second.first, TATime});
        } else {
            ReadyQueue.insert({remTime, {arrTime, pID}});
        }
    }
    outputFile << "SRTF :\n";
    GanttChart(exec, outputFile);
    return run;
}

vector< vector<float>> RoundRobin(vector< pair<int, pair<float, float>>> processes, ofstream& outputFile){
    vector< pair<int, pair<float, float>>> original(processes.begin(), processes.end());
    sort(processes.begin(), processes.end(), IncArrTime);
    
    queue < pair<float, pair<float, int>>> ReadyQueue;
    vector< vector<float>> run, exec;

    int idx = 0, n = processes.size();
    float RunTime = 0, quantum = 2;
    
    ReadyQueue.push({processes[idx].second.second, {processes[idx].second.first, processes[idx].first}});
    RunTime = max(RunTime, processes[idx].second.first);
    idx++;

    while(!ReadyQueue.empty() || idx < n){
        while(idx < n && processes[idx].second.first <= RunTime){
            ReadyQueue.push({processes[idx].second.second, {processes[idx].second.first, processes[idx].first}});
            idx++;
        }
        
        if (ReadyQueue.empty()) {
            RunTime = processes[idx].second.first;
            ReadyQueue.push({processes[idx].second.second, {processes[idx].second.first, processes[idx].first}});
            idx++;
        }
        
        pair<float, pair<float, int>> RunningProcess = ReadyQueue.front();
        ReadyQueue.pop();

        float pID = RunningProcess.second.second;
        float arrTime = RunningProcess.second.first;
        float burstTime = RunningProcess.first;

        float usedTime = min(quantum, burstTime);
        float remTime = burstTime - usedTime;

        float waitTime = max(0.0f, RunTime - arrTime);
        float TATime = RunTime + usedTime;
        
        exec.push_back({pID, RunTime, usedTime, waitTime, RunTime+usedTime});

        RunTime += usedTime;

        while(idx < n && processes[idx].second.first <= RunTime){
            ReadyQueue.push({processes[idx].second.second, {processes[idx].second.first, processes[idx].first}});
            idx++;
        }

        if(remTime == 0){
            run.push_back({pID, arrTime, original[pID-1].second.second, original[pID-1].second.first, TATime});
        } else {
            ReadyQueue.push({remTime, {arrTime, pID}});
        }
    }
    outputFile << "RoundRobin (quantum = 2) :\n";
    GanttChart(exec, outputFile);
    return run;
}

int main(){
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    if(!inputFile.is_open()){
        cerr << "Error reading input file!\n";
        return 0;
    }

    if(!outputFile.is_open()){
        cerr << "Error reading output file!\n";
        return 0;
    }

    vector< pair<int, pair<float, float>>> processes;
    int ProcessID;
    float ArrivalTime, BurstTime;

    while(inputFile >> ProcessID >> ArrivalTime >> BurstTime){
        processes.push_back({ProcessID, {ArrivalTime, BurstTime}});
    }

    float sumBurstTime = 0.0, avgBurstTime = 0.0, varBurstTime = 0.0;
    float sumArrivalTime = 0.0, avgArrivalTime = 0.0, varArrivalTime = 0.0;

    if(processes.empty()){
        cerr << "No processes to schedule. We cool!\n";
        outputFile << "No processes to schedule. We cool!\n";
        return 0;
    }

    map <float, int> countBurstTime;

    for(auto&p : processes){
        cout << "Process ID : " << p.first << ", Arrival Time : " <<
        p.second.first << ", Burst Time : " << p.second.second << "\n";
        countBurstTime[p.second.second]++;
        sumArrivalTime += p.second.first;
        sumBurstTime += p.second.second;
    }

    int totalProcesses = processes.size();
    avgArrivalTime = sumArrivalTime/totalProcesses;
    avgBurstTime = sumBurstTime/totalProcesses;

    for(auto&p : processes){
        varArrivalTime += (p.second.first - avgArrivalTime)*(p.second.first - avgArrivalTime);
        varBurstTime += (p.second.second - avgBurstTime)*(p.second.second - avgBurstTime);
    }

    varArrivalTime /= totalProcesses;
    varBurstTime /= totalProcesses;

    float SDAT = sqrtf(varArrivalTime);
    float SDBT = sqrtf(varBurstTime);

    int long_burst_count = 0;
    int n = processes.size();
    for(int i=0; i<n; i++){
      if(processes[i].second.second > avgBurstTime){
        long_burst_count++;
      }
    }
    if (SDBT < avgBurstTime && SDAT < avgArrivalTime) {
        outputFile << "Predicted Algorithm : SJF or SRTF\n\n";
    } else if (SDBT < avgBurstTime) {
        outputFile << "Predicted Algorithm : SRTF\n\n";
    } else if (long_burst_count > n / 2) {
        outputFile << "Predicted Algorithm : LJF\n\n";
    } else {
        outputFile << "Predicted Algorithm : Round Robin\n\n";
    }

    vector<vector<float>> out_fcfs = FCFS(processes, outputFile);
    AvgValues(out_fcfs, outputFile);
    vector<vector<float>> out_sjf = SJF(processes, outputFile);
    AvgValues(out_sjf, outputFile);
    vector<vector<float>> out_ljf = LJF(processes, outputFile);
    AvgValues(out_ljf, outputFile);
    vector<vector<float>> out_srtf = SRTF(processes, outputFile);
    AvgValues(out_srtf, outputFile);
    vector<vector<float>> out_rr = RoundRobin(processes, outputFile);
    AvgValues(out_rr, outputFile);
}
