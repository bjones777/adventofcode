
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

string toBinString(const string& hexString)
{
	string retVal;
	retVal.reserve(hexString.size()*4);
	for (char ch : hexString)
	{
		switch(ch)
		{
			case '0':
				retVal += "0000";
				break;
			case '1':
				retVal += "0001";
				break;
			case '2':
				retVal += "0010";
				break;
			case '3':
				retVal += "0011";
				break;
			case '4':
				retVal += "0100";
				break;
			case '5':
				retVal += "0101";
				break;
			case '6':
				retVal += "0110";
				break;
			case '7':
				retVal += "0111";
				break;
			case '8':
				retVal += "1000";
				break;
			case '9':
				retVal += "1001";
				break;
			case 'A':
				retVal += "1010";
				break;
			case 'B':
				retVal += "1011";
				break;
			case 'C':
				retVal += "1100";
				break;
			case 'D':
				retVal += "1101";
				break;
			case 'E':
				retVal += "1110";
				break;
			case 'F':
				retVal += "1111";
				break;
			default:
				assert(false);
				break;
		}
	}
	return retVal;
}

struct Packet
{
	virtual ~Packet() {}
	virtual int64_t GetSumOfVersionNumbers() = 0;
	virtual int64_t Evaluate() = 0;
	int Version;
	int TypeId;
};

struct LiteralPacket : public Packet
{
	LiteralPacket(int64_t ver, int64_t typeId, int64_t v)
	{
		Version = static_cast<int>(ver);
		TypeId = static_cast<int>(typeId);
		Value = v;
		assert(Value >= 0);
	}
	int64_t Value;
	virtual int64_t GetSumOfVersionNumbers() override {
		return Version;
	}

	virtual int64_t Evaluate() override {
		return Value;
	}
};

struct OperatorPacket : public Packet
{
	OperatorPacket(int64_t ver, int64_t typeId)
	{
		Version = static_cast<int>(ver);
		TypeId = static_cast<int>(typeId);
	}

	void AddSubpacket(Packet* packet) {
		SubPackets.emplace_back(unique_ptr<Packet>(packet));
	}

	virtual int64_t GetSumOfVersionNumbers() override {
		int64_t Start = Version;
		for (unique_ptr<Packet>& subPacket : SubPackets) {
			Start += subPacket->GetSumOfVersionNumbers();
		}
		return Start;
	}

	virtual int64_t Evaluate() override {
		switch (TypeId)
		{
			case 0:
			{
				int64_t sum = 0;
				for (unique_ptr<Packet>& subPacket : SubPackets) {
					sum += subPacket->Evaluate();
				}
				return sum;
			}
			break;
			case 1:
			{
				uint64_t pc = 1;
				int64_t product = 1;
				for (unique_ptr<Packet>& subPacket : SubPackets) {
					product *= subPacket->Evaluate();
					assert(product >= 0);
					pc *= (uint64_t)subPacket->Evaluate();
				}
				return product;
			}
			break;
			case 2:
			{
				int64_t minVal = SubPackets.front()->Evaluate();
				for (int i = 1; i < SubPackets.size(); ++i) {
					minVal = min(minVal, SubPackets[i]->Evaluate());
				}
				return minVal;
			}
			case 3:
			{
				int64_t maxVal = SubPackets.front()->Evaluate();
				for (int i = 1; i < SubPackets.size(); ++i) {
					maxVal = max(maxVal, SubPackets[i]->Evaluate());
				}
				return maxVal;
			}
			case 5:
			{
				assert(SubPackets.size() == 2);
				return (int64_t)(SubPackets.front()->Evaluate() > SubPackets.back()->Evaluate());
			}
			break;
			case 6:
			{
				assert(SubPackets.size() == 2);
				return (int64_t)(SubPackets.front()->Evaluate() < SubPackets.back()->Evaluate());
			}	
			break;
			case 7:
			{
				assert(SubPackets.size() == 2);
				return (int64_t)(SubPackets.front()->Evaluate() == SubPackets.back()->Evaluate());
			}
			default:
				assert(false);
				break;
		}
		return -1;
	}

	vector<unique_ptr<Packet>> SubPackets;
};

class Parser
{
public:
	Parser(const string& binString)
	{
		BinString = binString;
	}

	vector<unique_ptr<Packet>> parsePackets();

	
private:
	int64_t nibble(int numChars);
	unique_ptr<Packet> parsePacket();
	unique_ptr<LiteralPacket> parseLiteralPacket(int64_t ver, int64_t typeId);
	unique_ptr<OperatorPacket> parseOperatorPacket(int64_t ver, int64_t typeId);

	int Location = 0;
	string BinString;
};

struct nibble_exception {};

int64_t Parser::nibble(int numChars)
{
	assert(numChars <= 63);
	int64_t retVal = 0;
	for (int i = 0; i < numChars; ++i) {
		retVal *= 2;
		if (Location >= BinString.size()) {
			throw nibble_exception{};
		}
		retVal += BinString.at(Location) == '1' ? 1 : 0;
		++Location;
	}
	return retVal;
}

unique_ptr<Packet> Parser::parsePacket()
{
	int64_t version = nibble(3);
	int64_t type = nibble(3);
	if (type == 4)
	{
		return parseLiteralPacket(version, type);
	}
	return parseOperatorPacket(version, type);
	
}


vector<unique_ptr<Packet>> Parser::parsePackets()
{
	vector<unique_ptr<Packet>> packets;
	try {
		for (;;)
		{
			packets.emplace_back(parsePacket());
		}
	}
	catch (const nibble_exception&)
	{
		cerr << "Nibble Exception\n";
	}
	return packets;
}

unique_ptr<LiteralPacket> Parser::parseLiteralPacket(int64_t version, int64_t typeId)
{
	assert(typeId == 4);
	int64_t val = 0;
	bool isLast = false;
	do 
	{
		isLast = nibble(1) == 0;
		val <<= 4;
		val += nibble(4);
	} while (!isLast);

	return make_unique<LiteralPacket>(version,typeId,val);
}

unique_ptr<OperatorPacket> Parser::parseOperatorPacket(int64_t ver, int64_t typeId)
{
	unique_ptr<OperatorPacket> retVal = make_unique<OperatorPacket>(ver, typeId);
	assert(typeId != 4);
	bool isBits = nibble(1) == 0;
	if (isBits)
	{
		int64_t bitLength = nibble(15);
		Parser p(BinString.substr(Location,bitLength));
		vector<unique_ptr<Packet>> packets = p.parsePackets();
		for (unique_ptr<Packet>& p : packets)
		{
			retVal->AddSubpacket(p.release());
		}
		Location += static_cast<int>(bitLength);
	}
	else
	{
		int childPackets = static_cast<int>(nibble(11));
		for (int i = 0; i < childPackets; ++i) {
			retVal->AddSubpacket(parsePacket().release());
		}
	}
	return retVal;
}

int64_t partA(const vector<unique_ptr<Packet>>& packets)
{
	int64_t retVal = 0;
	for (auto& packet : packets)
	{
		retVal += packet->GetSumOfVersionNumbers();
	}
	return retVal;
}

int64_t partB(const vector<unique_ptr<Packet>>& packets)
{
	assert(packets.size() == 1);
	return packets.front()->Evaluate();
}

int main()
{
	string hexString;
	bool isOk = (bool)getline(cin, hexString);
	assert(isOk);
    Parser p(toBinString(hexString));
	vector<unique_ptr<Packet>> packets = p.parsePackets();

	cout << "Part A: " << partA(packets) << '\n';
	cout << "Part B: " << partB(packets) << '\n';
	return 0;
}