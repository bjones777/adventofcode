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

enum class EPart
{
	PART_A,
	PART_B,
};

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
	static const int MAX_LOCATIONS = 16;
	RowCol RowCols[MAX_LOCATIONS];
	int NumStates = 0;

	void AddLocation(const RowCol& rowCol) {
		assert(NumStates < MAX_LOCATIONS);
		RowCols[NumStates++] = rowCol;
	}
	
	bool operator==(const BurrowState& rhs) const {
		if (NumStates != rhs.NumStates)
		{
			return false;
		}
		for (int i = 0; i < NumStates; ++i) {
			if (!(RowCols[i] == rhs.RowCols[i]))
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
			for (int i = 0; i < k.NumStates; ++i) {
				retVal ^= hash<RowCol>()(k.RowCols[i]);
			}
			return retVal;
		}
	};
}

class Burrow
{
public:
    Burrow(EPart part,EConfig Config)
    : Part(part)
	{
        Spaces.reserve(GetWidth() * GetHeight());
        if(Config == EConfig::Example) {
            InitConfigExample();
        }
        else {
            InitConfigPuzzle();
        }
    }

	int GetNumLocations() const {
		return (Part == EPart::PART_A ? ((int)ELocation::DEST_1) + 1 : ((int)ELocation::DEST_3) + 1);
	}

	ELocation GetLocationAtIndex(int index) const {
		assert(index >= 0 && index < GetNumLocations());
		return static_cast<ELocation>(index);
	}


	ELocation GetMaxDestination() const {
		return (Part == EPart::PART_A ? (ELocation::DEST_1) : (ELocation::DEST_3));
	}

	BurrowState GetBurrowState() const {
		BurrowState bs;
		for (int i = 0; i < Amphipods.size(); ++i) {
			const Amphipod& a = Amphipods[i];
			bs.AddLocation(RowCol(a.GetRow(), a.GetColumn()));
		}
		return bs;
	}
	int GetPathCost(const Amphipod* amphipod,int row,int col) const;
	bool IsOpen(int row, int col, const Amphipod* toIgnore) const;

	vector<Burrow> MoveAmphipods() const;
	bool MoveAmphipod(int id, ELocation dest);

    ESpaceType GetSpaceType(int r, int c) const {
        return Spaces.at(r * GetWidth() + c);
    }

    bool IsGood() const;

    void Print() const;

    int GetEnergy() const;

    const Amphipod* GetAmphipodAtLocation(int r,int c) const;

    bool operator>(const Burrow& rhs) const {
        return GetEnergy() > rhs.GetEnergy();
    }

    RowCol GetRowColumnForLocation(ELocation loc, EAmphipodType type) const;

	int GetWidth() const {
		return WIDTH;
	}

	int GetHeight() const {
		if (Part == EPart::PART_A)
		{
			return HEIGHT - 2;
		}
		return HEIGHT;
	}
private:
    void InitBurrow();
    void InitConfigExample();
    void InitConfigPuzzle();
    static const int WIDTH = 13;
    static const int HEIGHT = 7;
    vector<ESpaceType> Spaces;
    vector<Amphipod> Amphipods;
	EPart Part;
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
                for(int r = 0, re = GetHeight()-1;r < re;++r) {
                    for(int c = 0, ce = GetWidth();c < ce;++c) {
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
            case ELocation::DEST_2:
			case ELocation::DEST_3:
				assert(Part == EPart::PART_B);
			case ELocation::DEST_0:
			case ELocation::DEST_1:
                {
                    const ESpaceType spaceType = GetSpaceTypeForAmphipodType(type);
                    int numLeft = (int)loc - (int)ELocation::DEST_0;
                    for(int r = 0, re = GetHeight()-1;r < re;++r) {
                        for(int c = 0, ce = GetWidth();c < ce;++c) {
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
    for(int r = 0, re = GetHeight();r < re;++r) {
        for(int c = 0, ce = GetWidth();c < ce;++c) {
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
    for(int c = 0;c < GetWidth();++c) {
        Spaces.push_back(ESpaceType::BLOCK);
    }
    // row 2
    Spaces.push_back(ESpaceType::BLOCK);
    for(int c = 1;c < GetWidth()-1;++c) {
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
	for(int i = 0, e = (Part == EPart::PART_B ? 3 : 1); i < e;++i) {
		
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
	int row = 2;
    Amphipods.push_back(Amphipod(EAmphipodType::B, row,3, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::C, row,5, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::B, row,7, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::D, row,9, (int)Amphipods.size()));
	++row;

	if(Part == EPart::PART_B)
	{
		Amphipods.push_back(Amphipod(EAmphipodType::D, row, 3, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::C, row, 5, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::B, row, 7, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::A, row, 9, (int)Amphipods.size()));
		++row;
		
		Amphipods.push_back(Amphipod(EAmphipodType::D, row, 3, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::B, row, 5, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::A, row, 7, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::C, row, 9, (int)Amphipods.size()));
		++row;
	}


    Amphipods.push_back(Amphipod(EAmphipodType::A, row,3, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::D, row,5, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::C, row,7, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::A, row,9, (int)Amphipods.size()));
}

void Burrow::InitConfigPuzzle()
{
    InitBurrow();    

	int row = 2;
    Amphipods.push_back(Amphipod(EAmphipodType::B, row,3, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::B, row,5, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::D, row,7, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::A, row,9, (int)Amphipods.size()));
	++row;
			
	if (Part == EPart::PART_B)
	{
		Amphipods.push_back(Amphipod(EAmphipodType::D, row, 3, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::C, row, 5, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::B, row, 7, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::A, row, 9, (int)Amphipods.size()));
		++row;
												
		Amphipods.push_back(Amphipod(EAmphipodType::D, row, 3, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::B, row, 5, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::A, row, 7, (int)Amphipods.size()));
		Amphipods.push_back(Amphipod(EAmphipodType::C, row, 9, (int)Amphipods.size()));
		++row;
	}
												
    Amphipods.push_back(Amphipod(EAmphipodType::D, row,3, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::C, row,5, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::A, row,7, (int)Amphipods.size()));
    Amphipods.push_back(Amphipod(EAmphipodType::C, row,9, (int)Amphipods.size()));
}

void Burrow::Print() const 
{
    for(int r = 0, re = GetHeight();r < re;++r) {
        for(int c = 0, ce = GetWidth();c < ce;++c) {
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
		for (int i = (int)GetMaxDestination(); i >= (int)ELocation::DEST_0; --i)
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
	if (dest >= ELocation::DEST_0 && dest <= GetMaxDestination())
	{
		// Amphipods will never move from the hallway into a room unless that room is their destination room 
		// and that room contains no amphipods which do not also have that room as their own destination.
		// If an amphipod's starting room is not its destination room, it can stay in that room until it leaves the room. 
		// (For example, an Amber amphipod will not move from the hallway into the right three rooms, and will only move into the
		// leftmost room if that room is empty or if it only contains other Amber amphipods.)
		for(int i = (int)ELocation::DEST_0, ie = (int)GetMaxDestination();i <= ie;++i)
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
		for (int i = (int)GetMaxDestination(); i > (int)dest; --i)
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

static int find_min_energy(EPart part, EConfig config)
{
	Burrow burrow(part, config);
	unordered_map<BurrowState, int> enqueued;
	int maxEnergy = 0;
	priority_queue<Burrow, vector<Burrow>, greater<Burrow>> queue;
	queue.push(burrow);
	enqueued[burrow.GetBurrowState()] = burrow.GetEnergy();
	while (!queue.empty()) {
		Burrow b = queue.top();
		queue.pop();
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

static int partA(EConfig config)
{
	return find_min_energy(EPart::PART_A, config);
}

static int partB(EConfig config)
{
	return find_min_energy(EPart::PART_B, config);
}

int main(int argc,char* argv[]) 
{
    const EConfig config = EConfig::Puzzle;
	cout << "Part A: " << partA(config) << '\n';
    cout << "Part B: " << partB(config) << '\n';
    return 0;
}