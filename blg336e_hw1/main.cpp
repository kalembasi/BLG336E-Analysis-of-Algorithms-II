//Omer Malik Kalembasi 150180112
//BLG336E - Algorithm Analysis HW1

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <queue>
#include <climits>
#include <stack>
#include <chrono>

using namespace std;

void print_graph(vector<vector<int>> graph){ //print graph
    // print adjacency matrix to check if it was read correctly
    for (int i = 0; i < (int)graph.size(); i++) {
        for (int j = 0; j < (int)graph[i].size(); j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

string graphToString(const vector<vector<int>>& graph) { //convert vector<vector<int>> graph to string for printing into txt file
    stringstream ss;
    for (int i = 0; i < (int)graph.size(); ++i) {
        for (int j = 0; j < (int)graph[i].size(); ++j) {
            ss << graph[i][j] << " ";
        }
        if(i == (int)graph.size()-1){
            break;
        }
        ss << "\n";
    }
    return ss.str();
}

class kid{ //kid class for push kids into KidVec
    public:
        int x, y, s;    //positions and strength
};

int stepCount = 0; //global variable for counting ball movements

stack<int> reverse_stack(const stack<int>& s){ //reverse function for reversing dfs stack into stirng path correctly
    stack<int> temp = s;
    stack<int> reversed;

    while (!temp.empty()) {
        reversed.push(temp.top());
        temp.pop();
    }

    return reversed;
}

string bfs(vector<vector<int>>& graph, int numOfKids, int source, int target){
    vector<int> dist(numOfKids, INT_MAX);  //distance of each kid from the source kid
    vector<int> prev(numOfKids, -1);  //prev kid on the shortest path to each kid
    queue<int> q;  //bfs queue

    dist[source] = 0;  //distance of source kid from itself is 0
    q.push(source);

    while (!q.empty()){
        int currKid = q.front();
        q.pop();

        //visit the neighbors of the current kid
        for(int neighbor = 0; neighbor < numOfKids; ++neighbor){
            if (graph[currKid][neighbor] == 1 && dist[neighbor] == INT_MAX){
                dist[neighbor] = dist[currKid] + 1;
                prev[neighbor] = currKid;
                q.push(neighbor);
            }
            else if(graph[currKid][neighbor] == 1 && dist[neighbor] > dist[currKid] + 1){
                //found a shorter path to a kid previously visited
                dist[neighbor] = dist[currKid] + 1;
                prev[neighbor] = currKid;
            }
        }
    }

    //path from the source kid to the target kid
    if(prev[target] == -1){
        return "-1";  //no path exists from source kid to target kid
    }
    string path = to_string(target);
    int currKid = target;
    while(prev[currKid] != -1){
        stepCount++;
        path = to_string(prev[currKid]) + "->" + path;
        currKid = prev[currKid];
    }
    
    return path;
}

void dfs(vector<vector<int>>& graph, vector<bool>& visited, stack<int>& path, int currKid, int source, bool& found){
    visited[currKid] = true; //mark current kid as visited
    path.push(currKid); //push current kid into the path

    for(int i = 0; i < (int)graph[currKid].size(); i++){ //loop through all neighbors of the curr kid
        if(found){return;} //if path is found than executes the function
        
        if(graph[currKid][i] == 1) { //check if there is an edge way the curr kid to its neighbor
            if(graph[source][currKid] == graph[currKid][source] && path.size() > 2 && graph[currKid][source] == 1){ //check if cycle found
                found = true; //set found to true if cycle is found
                path.push(source); //add the source kid into the path
                return; //exit the function
            }           
            if(i == source && path.size() > 2){ //check if cycle found and path is correct
                found = true; 
                path.push(source); //add the source kid into the path
                return; //exit the function
            }
            if(!visited[i] && !found){ //check if neighbor is visited and cycle not found
                dfs(graph, visited, path, i, source, found); //recursively call the dfs function on the neighbor kid
            }
        }
    }

    if (!found){ //if cycle not found
        path.pop(); //remove the current kid from the path
    }
}

string findCycle(vector<vector<int>>& graph, int numOfKids, int source) {
    vector<bool> visited(numOfKids, false); //vector to keep track of visited kids
    stack<int> path; //stack to keep path
    bool found = false; //cycle found

    dfs(graph, visited, path, source, source, found); //call dfs

    if(found){ //if cycle is found, add cycle into string path and return path
        string result;
        path = reverse_stack(path); //dfs returns reverse path, avoiding wrong output reverse the stack

        while(!path.empty()){

            if(stepCount == 0){
                result = to_string(path.top());
                path.pop();
                stepCount++; //increase num of steps
            }else{
                result += "->" + to_string(path.top());
                path.pop();
                stepCount++; //increase num of steps
            }
        }

        return to_string(stepCount-1) + " " + result; //return number of steps and path as string
    }

    return "-1"; //if cycle not found
}

int main(int argc, char**argv){

    auto begin = std::chrono::high_resolution_clock::now();

    //open input file
    string line, word;
    fstream inputFile(argv[1], ios::in);

    if(inputFile.is_open()){
        getline(inputFile, line);
        stringstream str(line);
    
        getline(str, word, ' ');    //get first word from first line
        int numOfKids = stoi(word);    //assign num of kids

        getline(str, word, ' ');    
        int source = stoi(word);    //assign source kid

        getline(str, word, ' ');    
        int target = stoi(word);    //assign target kid

        //store kid features into kid class and push them to KidVec
        vector<vector<int>> graph(numOfKids, vector<int>(numOfKids, 0));
        vector<kid> KidVec; //kid vector

        while(getline(inputFile, line)){
            stringstream str(line);

            kid kidToPush;

            //push kid features taken from input into kid class
            getline(str, word, ' ');
            kidToPush.x = stoi(word);
            getline(str, word, ' ');
            kidToPush.y = stoi(word);    
            getline(str, word, ' ');
            kidToPush.s = stoi(word); 

            KidVec.push_back(kidToPush); //push kid class into KidVec
        }


        //graph implementation
        for(int i=0; i < numOfKids; i++){
            for(int j=0; j < numOfKids; j++){
                if(
                    KidVec[i].s >= (pow((KidVec[i].x - KidVec[j].x),2) + pow((KidVec[i].y - KidVec[j].y),2)) 
                    && KidVec[j].s >= (pow((KidVec[i].x - KidVec[j].x),2) + pow((KidVec[i].y - KidVec[j].y),2)) 
                    && i != j
                ){graph[i][j] = 1;}
            }
        }

        //create graph.txt
        ofstream graphFile;
        graphFile.open("graph.txt"); //open file
        if (graphFile.is_open()){
            graphFile << "Graph:\n" + graphToString(graph); //write file
            graphFile.close(); //close the file
        }


        string path = bfs(graph, numOfKids, source, target); //call bfs
        path = to_string(stepCount) + " " + path; //add number of steps into path 
        //create bfs.txt
        ofstream bfsFile; 
        bfsFile.open("bfs.txt"); //open file
        if (bfsFile.is_open()){ 
            bfsFile << "BFS:\n" + path << endl; //write file
            bfsFile.close(); //close file
        }
        stepCount = 0;


        //create dfs.txt     
        ofstream dfsFile;
        dfsFile.open("dfs.txt"); //open file
        if (dfsFile.is_open()){
            dfsFile << "DFS:\n" <<  findCycle(graph, numOfKids, source); //write file
            dfsFile.close(); //close file
        }
        stepCount=0;

        //print_graph(graph);

        //call breadth first search function with count (number of steps)
        //string path = bfs(graph, numOfKids, source, target);
        //cout << count << " " << path << endl;
        //count = 0;

        //string cyclePath = findCycle(graph, numOfKids, source);
        //cout << findCycle(graph, numOfKids, source) << endl;
        //count = 0;

        // close input file
        inputFile.close();
    }

    auto end = std::chrono::high_resolution_clock::now();
    cout <<"Time elapsed (microseconds): " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << endl;
    
    exit(0);
}