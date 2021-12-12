#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using namespace std;

class Graph;

enum class ECaveType
{
    Big,
    Small,
};

struct VertexId
{
    VertexId(int id, ECaveType caveType, bool isStartEnd) : Id(id), CaveType(caveType)
    , IsStartEnd(isStartEnd) {}
 
    bool operator==(const VertexId& rhs) const {
        return Id == rhs.Id;
    }

    bool IsOkToVisit(int visitCount) const {
        if(CaveType == ECaveType::Big) 
        {
            return true;
        }
        return visitCount == 0;
    }

    bool IsOkToVisit2(int visitCount, bool used) const {
        if(CaveType == ECaveType::Big) 
        {
            return true;
        }
        if(visitCount == 0)
        {
            return true;
        }
        if(IsStartEnd || used) {
            return false;
        }
        return true;
    }

    bool GetIsStartEnd() const {
        return IsStartEnd;
    }

    ECaveType GetCaveType() const {
        return CaveType;
    }
 private:
    int Id;
    ECaveType CaveType;
    bool IsStartEnd;
    friend class Graph;
};

class Graph {
public:
    Graph() {}

    VertexId AddVertexIfNecessary(const string& name);
    void AddLink(const VertexId& id1, const VertexId& id2);
    
    int64_t GetNumPaths(const VertexId& id1, const VertexId& id2) const;
    VertexId GetVertexIdByName(const string& name) const;

    int64_t GetNumPaths2(const VertexId& id1, const VertexId& id2) const;
private:
    int64_t GetNumPaths(const VertexId& id1, const VertexId& id2, vector<int>& visitCounts, int depth) const;
    int64_t GetNumPaths2(const VertexId& id1, const VertexId& id2, vector<int>& visitCounts, int depth, bool usedSmallVisit) const;

    vector<VertexId> vertices;
    vector<vector<VertexId>> adjList;
    unordered_map<string, VertexId> nameToVertexId;
};

static ECaveType GetCaveTypeFromName(const string& name) 
{
    return isupper(name.front()) != 0 ? ECaveType::Big : ECaveType::Small;
} 

VertexId Graph::AddVertexIfNecessary(const string& name)
{
    auto iter = nameToVertexId.find(name);
    if(iter != nameToVertexId.end())
    {
         return iter->second;
    }
    int index = (int)adjList.size();
    adjList.push_back(vector<VertexId>());
    VertexId retVal = VertexId(index, GetCaveTypeFromName(name), name == "start" || name == "end");
    vertices.push_back(retVal);
    nameToVertexId.insert(make_pair(name, retVal));
    return retVal;
}

void Graph::AddLink(const VertexId& id1, const VertexId& id2)
{
    adjList.at(id1.Id).push_back(id2);
    adjList.at(id2.Id).push_back(id1);
}

VertexId Graph::GetVertexIdByName(const string& name) const
{
    auto iter = nameToVertexId.find(name);
    if(iter != nameToVertexId.end())
    {
         return iter->second;
    }
    throw runtime_error("could not find vertex");
}

int64_t Graph::GetNumPaths(const VertexId& id1, const VertexId& id2) const
{
    vector<int> visitCounts(vertices.size());
    return GetNumPaths(id1, id2, visitCounts, 0);
}

int64_t Graph::GetNumPaths2(const VertexId& id1, const VertexId& id2) const
{
    vector<int> visitCounts(vertices.size());
    return GetNumPaths2(id1, id2, visitCounts, 0, false);
}

int64_t Graph::GetNumPaths(const VertexId& id1, const VertexId& id2, vector<int>& visitCounts, int depth) const
{
    if(id1 == id2) {
        return 1;
    }
    if(!id1.IsOkToVisit(visitCounts.at(id1.Id))) {
        return 0;
    }
    visitCounts.at(id1.Id) += 1;
    int64_t sum = 0;
    auto& list = adjList.at(id1.Id);
    for(auto& other : list) {
        sum += GetNumPaths(other, id2, visitCounts, depth+1);
    }
    visitCounts.at(id1.Id) -= 1;
    return sum;
}

int64_t Graph::GetNumPaths2(const VertexId& id1, const VertexId& id2, vector<int>& visitCounts, int depth, bool usedTwice) const
{
    if(id1 == id2) {
        return 1;
    }
    if(!id1.IsOkToVisit2(visitCounts.at(id1.Id),usedTwice)) {
        return 0;
    }
    visitCounts.at(id1.Id) += 1; 
    usedTwice = usedTwice || (visitCounts.at(id1.Id) >= 2 && id1.GetCaveType() == ECaveType::Small); 
    int64_t sum = 0;
    auto& list = adjList.at(id1.Id);
    for(auto& other : list) {
        sum += GetNumPaths2(other, id2, visitCounts, depth+1, usedTwice);
    }
    visitCounts.at(id1.Id) -= 1;
    return sum;
}


static int64_t partA(const Graph& graph)
{
    VertexId start = graph.GetVertexIdByName("start");
    VertexId end = graph.GetVertexIdByName("end");
    
    return graph.GetNumPaths(start, end);
}

static int64_t partB(const Graph& graph)
{
    VertexId start = graph.GetVertexIdByName("start");
    VertexId end = graph.GetVertexIdByName("end");
    
    return graph.GetNumPaths2(start, end);
}

int main(int argc,char* argv[]) 
{
    Graph graph;
    string line;
    while(cin >> line)
    {
        if(line.empty()) continue;
        size_t pos = line.find('-');
        string left = line.substr(0,pos);
        string right = line.substr(pos+1);
        VertexId a = graph.AddVertexIfNecessary(left);
        VertexId b = graph.AddVertexIfNecessary(right);
        graph.AddLink(a,b);
    }
    cout << "Part A: " << partA(graph) << '\n';
    cout << "Part B: " << partB(graph) << '\n';
    return 0;
}