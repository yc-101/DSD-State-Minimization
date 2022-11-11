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
	bool incompatible;
	std::string name;
	std::vector<std::string> nextState; // if num_i = 1, there will be 2
	std::vector<int> nextOutput;

	State() {
		incompatible = false;
		nextState.resize(State::num);
		nextOutput.resize(State::num);
	}
	State(std::string _name) : name(_name) {
		incompatible = false;
		nextState.resize(State::num);
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
		std::cout << states[i].name << " : \n";
		for(int j = 0; j < State::num; j++)
			std::cout << "\t" << states[i].nextState[j] << " : " << states[i].nextOutput[j] << "\n";
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
int findState(std::string now_state)
{
	int i = 0;
	for(; i < states.size(); i++)
	{
		if(states[i].name == now_state)
			return i;
	}
	states.push_back(State(now_state));
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
				int j = findState(now_state);
				states[j].nextState[i] = next_state;
				states[j].nextOutput[i] = Bin2Dec(out);
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
		return false;
	for(int i = 0; i < a.size(); i++)
	{
		if(a[i] != b[i])
			return false;
	}
	return true;
}

void StateMinimization()
{
	std::cout <<"----StateMinimization----\n";
	bool checkAgain = false;
	// check if incompatible
	std::vector<std::string> sameOutput; // all[i][0]存k
	std::vector< std::vector<int> > compatibleState;

	// check states[i] is equal to sameOutput
	for(int i = 0; i < statesSize; i++)
	{
		std::string k = "";
		for(int temp = 0; temp < State::num; temp++)
		{
			k += states[i].nextOutput[temp]; // 這裡變成string += int，形質不同
		}

		bool found = false;
		for(int j = 0; j < sameOutput.size(); j++)
		{
			if(equalVector(sameOutput[j],states[i].nextOutput)) // sameOutput == k
			{
				found = true;

				compatibleState[j].push_back(i);
			}
		}
		if(!found)
		{
			std::vector<int> v(1, i);
			compatibleState.push_back(v);
			sameOutput.push_back(k);
		}
		std::cout <<"k = "<<k<<"\n";
	}
	
	// test : print sameOutput
	for(int i = 0; i < sameOutput.size(); i++)
	{
		std::cout << sameOutput[i] << " : ";
		for(int j = 0; j < compatibleState[i].size(); j++)
			std::cout << compatibleState[i][j] << " ";
		std::cout << "\n";
	}





	// a(former state) vs b(latter)
	for(int a = 0; a < statesSize; a++)
	{
		for(int b = a + 1; b < statesSize; b++)
		{

		}
	}


	if(checkAgain)
		StateMinimization();
		
	std::cout <<"----end----\n";
	printStates();
}