// Theme:	Project03: State Minimization
// Name:	B10932017 Yu-Zhen Chang
// Date: 		2022/10/29
// Last Update: 2022/10/29 19:11

#include <iostream>
#include <fstream>
#include <vector>

class State
{
public:
	bool removed;
	std::string name;
	std::vector<int> nextState; // if num_i = 1, there will be 2
	std::vector<std::string> nextStateName;
	std::vector<int> nextOutput;

	State() {
		removed = false;
		nextState.resize(State::num);
		nextStateName.resize(State::num);
		nextOutput.resize(State::num);
	}
	State(std::string _name) : name(_name) {
		removed = false;
		nextState.resize(State::num);
		nextStateName.resize(State::num);
		nextOutput.resize(State::num);
	}
	~State() {}
	static int num;	
};
int State::num = 0;
int statesSize;

std::vector< State > states; // all of the states

void ProcessKISS(std::ifstream&);
void StateMinimization();
void SaveKISS(std::string);
void SaveDOT(std::string);

// test print all states
void printStates()
{
	for(int i = 0; i < states.size(); i++)
	{
		if(states[i].removed) std::cout << "[removed] ";
		std::cout << states[i].name << " : \n";
		for(int j = 0; j < State::num; j++)
			std::cout << "\t" << states[states[i].nextState[j]].name << " " << states[i].nextOutput[j] << "\n";
	}
}

int main(int argc, char* argv[])
{
	/*
	if (argc == 4)
	{
		std::ifstream kiss(argv[1]);
		if (!kiss)
		{
			std::cout << "[ERROR] No such a KISS file.\n";
			return 0;
		}
		else
		{
			ProcessKISS(kiss);
			SaveKISS(argv[2]);
			SaveDOT(argv[3]);
		}
	}
	else
	{
		std::cout << "[ERROR] Parameter example: ./a.out input.kiss output.kiss output.dot\n";
		return 0;
	}
*/
	int input;
	std::cout <<"1 or 2 input? ";
	std::cin >> input;
	std::ifstream kiss;
	if(input == 1)
		kiss.open("1_input.kiss");
	else
		kiss.open("2_input.kiss");

	ProcessKISS(kiss);
	printStates();
	StateMinimization();
	//printStates();
}

// binary to decimal
int Bin2Dec(int input)
{
	if (input > 0)
		return input % 10 + 2 * Bin2Dec(input / 10);
	return input;
}

//check if state exists
int findState(std::string now_state_name)
{
	int i = 0;
	for(; i < states.size(); i++)
	{
		if(states[i].name == now_state_name)
			return i;
	}
	states.push_back(State(now_state_name));
	return i;
}


// read the kiss file
void ProcessKISS(std::ifstream& kiss)
{
	std::string op;

	int num_i, num_o;
	int num_p;
	std::string r; // ".r a"

	while (!kiss.eof())
	{
		kiss >> op;

		if (op == ".i")
		{
			kiss >> num_i;	//.i "1"
			if (num_i == 1)
				State::num = 2;
			else if (num_i == 2)
				State::num = 4;
			else
			{
				std::cout << "[ERROR] Wrong .i number (In this case, there should be 1 or 2 input.\n)";
				return;
			}
		}

		else if (op == ".o")
		{
			kiss >> num_o;
			if (num_o != 1)
			{
				std::cout << "[ERROR] Wrong .o number (In this case, there should be only one output.\n)";
				return;
			}
		}

		else if (op == ".p")	//.p "14"
		{
			kiss >> num_p;
		}

		else if (op == ".s")	//.s "7"
		{
			kiss >> statesSize;	// num of states
			states.reserve(statesSize);
		}

		else if (op == ".r")	//.r "a"
		{
			kiss >> r;

			int i, out; // 0/1 & 00/01/10/11
			std::string now_state, next_state;
			for (int p = 0; p < num_p; p++)
			{
				kiss >> i >> now_state >> next_state >> out;
				i = Bin2Dec(i);
				int now = findState(now_state);
				//int next = findState(next_state);
				//states[now].nextState[i] = next;
				states[now].nextStateName[i] = next_state;
				states[now].nextOutput[i] = Bin2Dec(out);
			}

			for(i = 0; i < states.size(); i++)
			{
				for(int j = 0; j < states[i].nextStateName.size(); j++)
				{
					for(int k = 0; k < states.size(); k++)
					{
						if(states[i].nextStateName[j] == states[k].name)
							states[i].nextState[j] = k;
					}
					
				}
			}
		}

		else if (op == ".end_kiss")
		{
			return;
		}
	}
}

int pow(int a, int b) // a^b
{
	if(b == 0)
		return 1;
	return a * pow(a, b - 1);
}

bool equalVector(std::vector<int> a, std::vector<int> b)
{
	if(a.size() != b.size())
	{
		std::cout <<"[ERROR] different size\n";
		return false;
	}
	
	for(int i = 0; i < a.size(); i++)
	{
		if(a[i] != b[i])
			return false;
	}
	return true;
}

void printIncompatible(std::vector< std::vector<bool> > a)
{
	for(int i = 0; i < a.size(); i++)
	{
		for(int j = 0; j < a[i].size(); j++)
			std::cout << (a[i][j] ? "X" : "-");
		std::cout <<"\n";
	}
}

std::vector< std::vector<bool> > checkIncompatible
	(std::vector< std::vector<bool> > incompatible)
{
	printIncompatible(incompatible);
	bool checkAgain = false;
	// a(former state) vs b(latter)
	for(int a = 0; a < statesSize; a++)
	{
		for(int b = a + 1; b < statesSize; b++)
		{
			if(incompatible[a][b - a - 1]) continue;
			std::cout << states[a].name << " " << states[b].name << " 格:\n";
			for(int i = 0; i < State::num; i++) // compare with each pair of output
			{
				//std::cout << "\t" << states[a].nextState[i] << " vs " << states[b].nextState[i] << "\n";
				int x, y; // x is smaller than y
				if(states[a].nextState[i] == states[b].nextState[i])
					continue;

				else if(states[a].nextState[i] < states[b].nextState[i])
				{
					x = states[a].nextState[i];
					y = states[b].nextState[i];
				}
				else
				{
					y = states[a].nextState[i];
					x = states[b].nextState[i];
				}

				if(incompatible[x][y - x - 1] == true)
				{
					incompatible[a][b - a - 1] = true;
					checkAgain = true;
					break;
				}
			}
		}
	}
	//std::cout << "---------after incom--------\n";
	//printIncompatible(incompatible);

	if(checkAgain)
		return checkIncompatible(incompatible);
	else
		return incompatible;
}

void StateMinimization()
{
	std::cout <<"----StateMinimization----\n";
	
	// build incompatible vector
	std::vector< std::vector<bool> > incompatible;
	incompatible.resize(statesSize);
	for(int i = 0; i < statesSize; i++)
		incompatible[i].resize(statesSize - i - 1, false);
	

	// check if incompatible
	std::vector<std::string> sameOutput; // all[i][0]存k
	std::vector< std::vector<int> > compatibleState;

	// check states[i] is equal to sameOutput
	for(int i = 0; i < statesSize; i++)
	{
		bool found = false;
		for(int j = 0; j < compatibleState.size(); j++)
		{
			if(equalVector(states[compatibleState[j][0]].nextOutput, states[i].nextOutput))
			{
				found = true;
				compatibleState[j].push_back(i);
				break;
			}
		}
		if(!found)
		{
			std::vector<int> v(1, i);
			compatibleState.push_back(v);
		}
	}
	
	// test : print sameOutput
	for(int i = 0; i < compatibleState.size(); i++)
	{
		for(int j = 0; j < states[compatibleState[i][0]].nextOutput.size(); j++)
			std::cout << states[compatibleState[i][0]].nextOutput[j] <<" ";
		std::cout << " : ";

		for(int j = 0; j < compatibleState[i].size(); j++)
			std::cout << states[compatibleState[i][j]].name << " ";
		std::cout << "\n";
	}

	//--------------------------------------------------------------------
	// find incompatible states
	int min = INT_MAX;
	for(int i = 0; i < compatibleState.size(); i++)
	{
		if(compatibleState[i].size() < min)
			min = compatibleState[i].size();
	}

	for(int i = 0; i < compatibleState.size(); i++)
	{
		if(compatibleState[i].size() == min)
		{
			for(int j = 0; j < min; j++)
			{
				//state[ compatibleState[i][j] ] 整排 要畫成黑色

				//刪橫的(row)
				if(compatibleState[i][j] < statesSize - 1) // not the last one
				{
					for(int k = 0; k < incompatible[compatibleState[i][j]].size(); k++)
					{
						incompatible[compatibleState[i][j]][k] = true;
					}
				}
				//刪直的
				for(int k = 0; k < compatibleState[i][j]; k++)
				{
					incompatible[k][compatibleState[i][j] - k - 1] = true;
				}
			}
		}
	}

	incompatible = checkIncompatible(incompatible);
	
	std::cout <<"---------------------\n";

	// remove latter state that is not incompatible
	// a(former state) vs b(latter)
	for(int a = 0; a < statesSize; a++)
	{
		if(states[a].removed) continue; // 代表已經刪除

		for(int b = a + 1; b < statesSize; b++)
		{
			if(states[b].removed)
				continue;

			if(incompatible[a][b - a - 1] == false)
			{
				std::cout <<"remove "<<states[b].name << " -> "<<states[a].name<<"\n";
				//states[a].removed = false;
				states[b].removed = true;

				// remove b, change it to a
				for(int i = 0; i < states.size(); i++)
				{
					for(int j = 0; j < State::num; j++)
					{
						if(states[i].nextState[j] == b)
							states[i].nextState[j] = a;
					}
				}
			}
		}
	}

	std::cout <<"----end----\n";
	printStates();
}