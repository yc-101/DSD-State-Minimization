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
	State();
	~State();
	static int num;

private:
	std::string name;
	std::vector<std::string> next; // if num_i = 1, there will be 2
};

State::State()
{
}

State::~State()
{

}


void ProcessKISS(std::ifstream&);
void SaveKISS(std::string);
void SaveDOT(std::string);

int main(int argc, char* argv[])
{
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

}

// read the kiss file
void ProcessKISS(std::ifstream& kiss)
{
	std::string op;

	int num_i, num_o;
	int num_p, num_s;
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
			kiss >> num_s;	// num of states
		}

		else if (op == ".r")	//.r "a"
		{
			kiss >> r;

			int in, out;
			std::string now_state, next_state;
			for (int i = 0; i < num_p; i++)
			{
				kiss >> in >> now_state >> next_state >> out;
			}
		}

		else if (op == ".end_kiss")
		{
			return;
		}
	}
}