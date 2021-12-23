#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

enum class ELocation
{
    WAIT_0,
    WAIT_1,
    WAIT_2,
    WAIT_3,
    WAIT_4,
    WAIT_5,
    WAIT_6,
    DEST_0,
    DEST_1,
	DEST_2,
	DEST_3,
};

static int GetNumLocations() {
	return ((int)ELocation::DEST_3)+1;
}

static ELocation GetLocationAtIndex(int index) {
	assert(index >= 0 && index < GetNumLocations());
	return static_cast<ELocation>(index);
}

enum class EDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

static int GetNumDirections() {
	return ((int)EDirection::RIGHT)+1;
}

static EDirection GetDirectionAtIndex(int index) {
	assert(index >= 0 && index < GetNumDirections());
	return static_cast<EDirection>(index);
}


struct RowCol
{
	RowCol() : Row(0), Column(0) {}
    RowCol(int r,int c) : Row(r), Column(c) {}
    int Row;
    int Column;

	RowCol Moved(EDirection dir) const;

	bool operator==(const RowCol& rhs) const {
		return Row == rhs.Row && Column == rhs.Column;
	}
};

RowCol RowCol::Moved(EDirection dir) const
{
	switch (dir)
	{
		case EDirection::UP:
			return RowCol(Row-1,Column);
		case EDirection::DOWN:
			return RowCol(Row + 1, Column);
		case EDirection::LEFT:
			return RowCol(Row, Column-1);
	}
	assert(dir == EDirection::RIGHT);
	return RowCol(Row,Column+1);
}

namespace std {

	template <>
	struct hash<RowCol>
	{
		std::size_t operator()(const RowCol& k) const
		{

			return ((hash<int>()(k.Row)
				^ (hash<int>()(k.Column) << 1) >> 1));
		}
	};
}

enum class EAmphipodType
{
    A,
    B,
    C,
    D,
};

static char GetCharacterForType(EAmphipodType type)
{
    switch(type)
    {
        case EAmphipodType::A:
            return 'A';
        case EAmphipodType::B:
            return 'B';
        case EAmphipodType::C:
            return 'C';
    }
    assert(type == EAmphipodType::D);
    return 'D';
}

static int GetMovementCostForType(EAmphipodType type) {
    switch(type)
    {
        case EAmphipodType::A:
            return 1;
        case EAmphipodType::B:
            return 10;
        case EAmphipodType::C:
            return 100;
    }
    assert(type == EAmphipodType::D);
    return 1000;
}

class Amphipod
{
public:
    Amphipod(EAmphipodType type, int row, int col, int id)
    : Type(type), Row(row), Col(col), Id(id) {}

    EAmphipodType GetType() const { return Type; }
    int GetRow() const { return Row; }
    int GetColumn() const { return Col; }
    int GetEnergyUsed() const { return EnergyUsed; }
	int GetId() const { return Id; }

	void SetRow(int row) { Row = row; }
	void SetColumn(int col) { Col = col; }

	void AddMoves(int NumMoves)
	{
		assert(NumMoves >= 0);
		EnergyUsed += NumMoves * GetMovementCostForType(Type);
	}
private:
    EAmphipodType Type; 
    int Row;
    int Col;
	int Id;
    int EnergyUsed = 0;
};

enum class ESpaceType
{
    EMPTY,
    SPACE,
    BLOCK,
    ROOM_A,
    ROOM_B,
    ROOM_C,
    ROOM_D,
};

ESpaceType GetSpaceTypeForAmphipodType(EAmphipodType type) 
{
    switch(type)
    {
        case EAmphipodType::A:
            return ESpaceType::ROOM_A;
        case EAmphipodType::B:
            return ESpaceType::ROOM_B;
        case EAmphipodType::C:
            return ESpaceType::ROOM_C;
    }
    assert(type == EAmphipodType::D);
    return ESpaceType::ROOM_D;
}

enum class EConfig
{
    Example,
    Puzzle,
};

struct BurrowState
{
	RowCol RowCol[16];
	
	bool operator==(const BurrowState& rhs) const {
		for (int i = 0; i < 16; ++i) {
			if (!(RowCol[i] == rhs.RowCol[i]))
			{
				return false;
			}
		}
		return true;
	}
};

namespace std {

	template <>
	struct hash<BurrowState>
	{
		std::size_t operator()(const BurrowState& k) const
		{
			size_t retVal = 0;
			for (int i = 0; i < 16; ++i) {
				retVal ^= hash<RowCol>()(k.RowCol[i]);
			}
			return retVal;
		}
	};
}

class Burrow
{
public:
    Burrow(EConfig Config)
    {
        Spaces.reserve(WIDTH*HEIGHT);
        if(Config == EConfig::Example) {
            InitConfigExample();
        }
        else {
            InitConfigPuzzle();
        }
    }

	BurrowState GetBurrowState() const {
		BurrowState bs;
		assert(Amphipods.size() == 16);
		for (int i = 0; i < Amphipods.size(); ++i) {
			const Amphipod& a = Amphipods[i];
			bs.RowCol[i] = RowCol(a.GetRow(), a.GetColumn());
		}
		return bs;
	}
	int GetPathCost(const Amphipod* amphipod,int row,int col) const;
	bool IsOpen(int row, int col, const Amphipod* toIgnore) const;

	vector<Burrow> MoveAmphipods() const;
	bool MoveAmphipod(int id, ELocation dest);

    ESpaceType GetSpaceType(int r, int c) const {
        return Spaces.at(r * WIDTH + c);
    }

    bool IsGood() const;

    void Print() const;

    int GetEnergy() const;

    const Amphipod* GetAmphipodAtLocation(int r,int c) const;

    bool operator>(const Burrow& rhs) const {
        return GetEnergy() > rhs.GetEnergy();
    }

    RowCol GetRowColumnForLocation(ELocation loc, EAmphipodType type) const;
private:
    void InitBurrow();
    void InitConfigExample();
    void InitConfigPuzzle();
    static const int WIDTH = 13;
    static const int HEIGHT = 7;
    vector<ESpaceType> Spaces;
    vector<Amphipod> Amphipods;
};

int Burrow::GetEnergy() const {
    int energy = 0;
    for(const Amphipod& a : Amphipods) {
        energy += a.GetEnergyUsed();
    }
    return energy;
}

bool Burrow::IsOpen(int row, int col, const Amphipod* toIgnore) const
{
	if (GetSpaceType(row, col) == ESpaceType::BLOCK)
	{
		return false;
	}
	const Amphipod* a = GetAmphipodAtLocation(row, col);
	if (a == nullptr) {
		return true;
	}
	if (a == toIgnore) {
		return true;
	}
	return false;
}

RowCol Burrow::GetRowColumnForLocation(ELocation loc, EAmphipodType type) const
{
    switch(loc) { 
        case ELocation::WAIT_0:
        case ELocation::WAIT_1:
        case ELocation::WAIT_2:
        case ELocation::WAIT_3:
        case ELocation::WAIT_4:
        case ELocation::WAIT_5:
        case ELocation::WAIT_6:
            {
                int numLeft = (int)loc - (int)ELocation::WAIT_0;
                for(int r = 0;r < HEIGHT-1;++r) {
                    for(int c = 0;c < WIDTH;++c) {
                        if(GetSpaceType(r,c) == ESpaceType::SPACE && GetSpaceType(r+1,c) == ESpaceType::BLOCK) {
                            if(numLeft == 0) {
                                return RowCol(r,c);
                            }
                            else {
                                numLeft--;
                            }
                        } 
                    }
                }
                assert(false);
                break;
            }
            case ELocation::DEST_0:
            case ELocation::DEST_1:
			case ELocation::DEST_2:
			case ELocation::DEST_3:
                {
                    const ESpaceType spaceType = GetSpaceTypeForAmphipodType(type);
                    int numLeft = (int)loc - (int)ELocation::DEST_0;
                    for(int r = 0;r < HEIGHT-1;++r) {
                        for(int c = 0;c < WIDTH;++c) {
                            if(GetSpaceType(r,c) == spaceType) {
                                if(numLeft == 0) {
                                    return RowCol(r,c);
                                }
                                else {
                                    numLeft--;
                                }
                            } 
                        }
                    }
                    assert(false);
                    break;
                }
    }
    throw runtime_error("couldn't find dest on map");
}

const Amphipod* Burrow::GetAmphipodAtLocation(int row, int col) const {
    for(auto& a : Amphipods) {
        if(a.GetRow() == row && a.GetColumn() == col) {
            return &a;
        }
    }
    return nullptr;
}

bool Burrow::IsGood() const
{
    for(int r = 0;r < HEIGHT;++r) {
        for(int c = 0;c < WIDTH;++c) {
            switch(GetSpaceType(r,c)) {
                case ESpaceType::ROOM_A:
                    {
                        const Amphipod* a = GetAmphipodAtLocation(r,c);
                        if(a == nullptr || a->GetType() != EAmphipodType::A) {
                            return false;
                        }
                    }
                    break;
                case ESpaceType::ROOM_B:
                    {
                        const Amphipod* a = GetAmphipodAtLocation(r,c);
                        if(a == nullptr || a->GetType() != EAmphipodType::B) {
                            return false;
                        }
                    }
                    break;
                case ESpaceType::ROOM_C:
                    {
                        const Amphipod* a = GetAmphipodAtLocation(r,c);
                        if(a == nullptr || a->GetType() != EAmphipodType::C) {
                            return false;
                        }
                    }
                    break;
                case ESpaceType::ROOM_D:
                    {
                        const Amphipod* a = GetAmphipodAtLocation(r,c);
                        if(a == nullptr || a->GetType() != EAmphipodType::D) {
                            return false;
                        }
                    }
                    break;
            }
        }
    }
    return true;
}

void Burrow::InitBurrow()
{
    // row 1
    for(int c = 0;c < WIDTH;++c) {
        Spaces.push_back(ESpaceType::BLOCK);
    }
    // row 2
    Spaces.push_back(ESpaceType::BLOCK);
    for(int c = 1;c < WIDTH-1;++c) {
        Spaces.push_back(ESpaceType::SPACE);
    }
    Spaces.push_back(ESpaceType::BLOCK);
    // row 3
    for(int c = 0;c < 3;++c) {
        Spaces.push_back(ESpaceType::BLOCK);
    }
    Spaces.push_back(ESpaceType::ROOM_A);
    Spaces.push_back(ESpaceType::BLOCK);
    Spaces.push_back(ESpaceType::ROOM_B);
    Spaces.push_back(ESpaceType::BLOCK);
    Spaces.push_back(ESpaceType::ROOM_C);
    Spaces.push_back(ESpaceType::BLOCK);
    Spaces.push_back(ESpaceType::ROOM_D);
    for(int c = 0;c < 3;++c) {
        Spaces.push_back(ESpaceType::BLOCK);
    }
	// row 4-6
	for(int i = 0; i < 3;++i) {
		
		for(int c = 0;c < 2;++c) {
			Spaces.push_back(ESpaceType::EMPTY);
		}
		Spaces.push_back(ESpaceType::BLOCK);
		Spaces.push_back(ESpaceType::ROOM_A);
		Spaces.push_back(ESpaceType::BLOCK);
		Spaces.push_back(ESpaceType::ROOM_B);
		Spaces.push_back(ESpaceType::BLOCK);
		Spaces.push_back(ESpaceType::ROOM_C);
		Spaces.push_back(ESpaceType::BLOCK);
		Spaces.push_back(ESpaceType::ROOM_D);
		Spaces.push_back(ESpaceType::BLOCK);
		for(int c = 0;c < 2;++c) {
			Spaces.push_back(ESpaceType::EMPTY);
		}
	}
    // row 7
    for(int c = 0;c < 2;++c) {
        Spaces.push_back(ESpaceType::EMPTY);
    }
    for(int c = 0;c < 9;++c) {
        Spaces.push_back(ESpaceType::BLOCK);
    }
    for(int c = 0;c < 2;++c) {
        Spaces.push_back(ESpaceType::EMPTY);
    }
}
    

void Burrow::InitConfigExample()
{
    InitBurrow();    
    //BCBD
	//DCBA
	//DBAC#

    //ADCA
    Amphipods.push_back(Amphipod(EAmphipodType::B, 2,3, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::C, 2,5, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::B, 2,7, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::D, 2,9, (int)Amphipods.size()));

	Amphipods.push_back(Amphipod(EAmphipodType::D, 3, 3, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::C, 3, 5, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::B, 3, 7, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::A, 3, 9, (int)Amphipods.size()));

	Amphipods.push_back(Amphipod(EAmphipodType::D, 4, 3, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::B, 4, 5, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::A, 4, 7, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::C, 4, 9, (int)Amphipods.size()));


    Amphipods.push_back(Amphipod(EAmphipodType::A, 5,3, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::D, 5,5, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::C, 5,7, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::A, 5,9, (int)Amphipods.size()));
}

void Burrow::InitConfigPuzzle()
{
    InitBurrow();    
    Amphipods.push_back(Amphipod(EAmphipodType::B, 2,3, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::B, 2,5, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::D, 2,7, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::A, 2,9, (int)Amphipods.size()));

	Amphipods.push_back(Amphipod(EAmphipodType::D, 3, 3, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::C, 3, 5, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::B, 3, 7, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::A, 3, 9, (int)Amphipods.size()));

	Amphipods.push_back(Amphipod(EAmphipodType::D, 4, 3, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::B, 4, 5, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::A, 4, 7, (int)Amphipods.size()));
	Amphipods.push_back(Amphipod(EAmphipodType::C, 4, 9, (int)Amphipods.size()));


    Amphipods.push_back(Amphipod(EAmphipodType::D, 5,3, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::C, 5,5, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::A, 5,7, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::C, 5,9, (int)Amphipods.size()));
}

void Burrow::Print() const 
{
    for(int r = 0;r < HEIGHT;++r) {
        for(int c = 0;c < WIDTH;++c) {
            const Amphipod* a = GetAmphipodAtLocation(r,c);
            if(a != nullptr)
            {
                cout << GetCharacterForType(a->GetType());
            }
            else 
            {
                switch(GetSpaceType(r,c))
                {
                    case ESpaceType::BLOCK:
                        cout << '#';
                        break;
                    default:
                        cout << ' ';
                        break;
                }
            }
        }
        cout << '\n';
    }
}

struct Entry
{
	Entry(int moves,int row, int col) : NumMoves(moves), Row(row), Column(col) {}
	int NumMoves;
	int Row;
	int Column;
};

int Burrow::GetPathCost(const Amphipod* amphipod, int row, int col) const
{
	queue<Entry> q;
	unordered_set<RowCol> visited;
	q.push(Entry(0, amphipod->GetRow(), amphipod->GetColumn()));
	while (!q.empty()) {
		Entry e = q.front();
		q.pop();
		if (e.Row == row && e.Column == col) {
			return e.NumMoves;
		}
		RowCol rc(e.Row,e.Column);
		auto iter = visited.find(rc);
		if (iter != visited.end()) {
			continue;
		}
		visited.insert(rc);
		for (int i = 0, ie = GetNumDirections(); i < ie; ++i) {
			RowCol nc = rc.Moved(GetDirectionAtIndex(i));
			iter = visited.find(nc);
			if (iter != visited.end()) {
				continue;
			}
			if (IsOpen(nc.Row, nc.Column, amphipod)) {
				q.push(Entry(e.NumMoves+1, nc.Row, nc.Column));
			}
		}
	}
	return -1;
}

vector<Burrow> Burrow::MoveAmphipods() const
{
	vector<Burrow> retVal;
	for (const Amphipod& a : Amphipods) {
		for (int i = 0, e = GetNumLocations(); i < e; ++i) {
			ELocation loc = GetLocationAtIndex(i);
			Burrow nb(*this);
			if (nb.MoveAmphipod(a.GetId(), loc)) {
				retVal.push_back(nb);
			}
		}
	}
	return retVal;
}


bool Burrow::MoveAmphipod(int id, ELocation dest) 
{
	Amphipod* amphipod = &Amphipods.at(id);
	RowCol rowCol = GetRowColumnForLocation(dest, amphipod->GetType());
	// don't bother moving if we are already at the destination
	if (amphipod->GetRow() == rowCol.Row && amphipod->GetColumn() == rowCol.Column)
	{
		return false;
	}
	ESpaceType spaceType = GetSpaceType(amphipod->GetRow(), amphipod->GetColumn());
	if (spaceType == GetSpaceTypeForAmphipodType(amphipod->GetType()))
	{
		for (int i = (int)ELocation::DEST_3; i >= (int)ELocation::DEST_0; --i)
		{
			ELocation e = (ELocation)i;
			RowCol rowCol = GetRowColumnForLocation(e, amphipod->GetType());
			const Amphipod* a = GetAmphipodAtLocation(rowCol.Row, rowCol.Column);
			if (a == amphipod)
			{
				return false;
			}
			else if (a->GetType() != amphipod->GetType()) {
				break;
			}
		}
		
	}

	if (spaceType == ESpaceType::SPACE && dest >= ELocation::WAIT_0 && dest <= ELocation::WAIT_6) {
		// Once an amphipod stops moving in the hallway, it will stay in that spot until it can move into a room. 
		// (That is, once any amphipod starts moving, any other amphipods currently in the hallway are locked in place 
		// and will not move again until they can move fully into a room.)
		return false;
	}
	if (dest >= ELocation::DEST_0 && dest <= ELocation::DEST_3)
	{
		// Amphipods will never move from the hallway into a room unless that room is their destination room 
		// and that room contains no amphipods which do not also have that room as their own destination.
		// If an amphipod's starting room is not its destination room, it can stay in that room until it leaves the room. 
		// (For example, an Amber amphipod will not move from the hallway into the right three rooms, and will only move into the
		// leftmost room if that room is empty or if it only contains other Amber amphipods.)
		for(int i = (int)ELocation::DEST_0;i <= (int)ELocation::DEST_3;++i)
		{
			ELocation e = (ELocation)i;
			RowCol rowCol = GetRowColumnForLocation(e, amphipod->GetType());
			const Amphipod* a = GetAmphipodAtLocation(rowCol.Row, rowCol.Column);
			if (a != nullptr && a->GetType() != amphipod->GetType())
			{
				return false;
			}
		}

		// fill up lower holes before above ones
		for (int i = (int)ELocation::DEST_3; i > (int)dest; --i)
		{
			ELocation e = (ELocation)i;
			RowCol rowCol = GetRowColumnForLocation(e, amphipod->GetType());
			const Amphipod* a = GetAmphipodAtLocation(rowCol.Row, rowCol.Column);
			if (a == nullptr) {
				return false;
			}
		}		
	}
	int pathCost = GetPathCost(amphipod, rowCol.Row, rowCol.Column);
	if (pathCost < 0)
	{
		return false;
	}
	amphipod->SetRow(rowCol.Row);
	amphipod->SetColumn(rowCol.Column);
	amphipod->AddMoves(pathCost);
	return true;
}

int partB(Burrow burrow)
{
	unordered_map<BurrowState, int> enqueued;
	int maxEnergy = 0;
    priority_queue<Burrow,vector<Burrow>,greater<Burrow>> queue;
    queue.push(burrow);
	enqueued[burrow.GetBurrowState()] = burrow.GetEnergy();
    while(!queue.empty()) {
        Burrow b = queue.top();
        queue.pop();
		if (b.GetEnergy() >= maxEnergy + 100) {
			maxEnergy = b.GetEnergy();
			cout << "At Energy Watermark: " << maxEnergy << '\n';
			b.Print();
		}
		if (b.IsGood()) {
			return b.GetEnergy();
		}
		BurrowState bs = b.GetBurrowState();
		vector<Burrow> newBurrows = b.MoveAmphipods();
		for (Burrow& nb : newBurrows) {
			BurrowState nbs = nb.GetBurrowState();
			auto iter = enqueued.find(nbs);
			if (iter == enqueued.end() || nb.GetEnergy() < iter->second) {
				queue.push(nb);
				enqueued[nbs] = nb.GetEnergy();
			}
			
		}
    }
    return -1;
}

int main(int argc,char* argv[]) 
{
    Burrow burrow(EConfig::Puzzle);
    cout << "Part B: " << partB(burrow) << '\n';
    return 0;
}