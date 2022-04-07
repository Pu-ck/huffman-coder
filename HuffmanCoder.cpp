#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// Symbol class
class Symbol
{
	public:
		std::vector<int> sequence;

		double probabilty;
		char name;

		Symbol(char x, double y)
		{
			name = x;
			probabilty = y;
		}
};

// Tree branch class
class Branch 
{
	public:
		std::vector<Symbol*> elements_list;

		double probabilty;

		Branch (double x)
		{
			probabilty = x;
		}
};

// Sorting symbols by probabilty field
bool sort_symbols_probability(const Symbol* symbol_first, const Symbol* symbol_second)
{
	return symbol_first->probabilty > symbol_second->probabilty;
}

// Sorting symbols by size of sequence field
bool sort_symbols_sequence(const Symbol* symbol_first, const Symbol* symbol_second) {
	return symbol_first->sequence.size() < symbol_second->sequence.size();
}

// Sorting branches by probabilty field
bool sort_branches(const Branch* branch_first, const Branch* branch_second) {
	return branch_first->probabilty < branch_second->probabilty;
}

// Main Huffman algorithm
void assign_codes(std::vector<Symbol*> symbols_list)
{
	using namespace std;

	vector<double> probabilties_list;
	vector<Branch*> branch_list;

	double min_first, min_second, sum;

	// Create an additional vector with probabilty of each symbol
	// Push each symbol into a new branch 
	for (auto symbol : symbols_list) 
	{
		probabilties_list.push_back(symbol->probabilty);

		Branch* branch = new Branch (symbol->probabilty);
		branch->elements_list.push_back(symbol);

		branch_list.push_back(branch);
	}

	// Find two minimal probabilties and remove them from probabilties vector
	for (int i = 0; i < symbols_list.size() - 1; i++) 
	{
		min_first = *min_element(probabilties_list.begin(), probabilties_list.end());

		for (int i = 0; i < probabilties_list.size(); i++) 
		{
			if (probabilties_list[i] == min_first) 
			{
				probabilties_list.erase(probabilties_list.begin() + i);
				break;
			}
		}

		min_second = *min_element(probabilties_list.begin(), probabilties_list.end());

		for (int i = 0; i < probabilties_list.size(); i++)
		{
			if (probabilties_list[i] == min_second)
			{
				probabilties_list.erase(probabilties_list.begin() + i);
				break;
			}
		}

		// Add minimal probabilties together and prepare a new branch 
		sum = min_first + min_second;
		Branch* new_branch = new Branch (sum);

		// Push symbols from two branches (with probabilty fields equal to first and second minimal probabilties) into newly created branch
		// Push 0 (smaller minimal value or first found) or 1 (bigger minimal value or second found) to sequence vector of symbols stored in each branch
		// Sort vector with branches and remove redundant branches
		for (auto branch : branch_list) 
		{
			if (branch->probabilty == min_first) 
			{
				for (auto symbol : branch->elements_list) 
				{
					new_branch->elements_list.push_back(symbol);
					symbol->sequence.push_back(0);
				}

				sort(branch_list.begin(), branch_list.end(), sort_branches);

				branch_list.erase(branch_list.begin());
				break;
			}
		}

		for (auto branch : branch_list) 
		{
			if (branch->probabilty == min_second) 
			{
				for (auto symbol : branch->elements_list) 
				{
					new_branch->elements_list.push_back(symbol);
					symbol->sequence.push_back(1);
				}

				branch_list.erase(branch_list.begin());
				break;
			}
		}

		// Update the vector of probabilties with a sum of previously found minimal probabilties and push new, concatenated branch into a vector of branches
		branch_list.push_back(new_branch);
		probabilties_list.push_back(sum);
	}

	// Cleaning up memory
	for (auto branch : branch_list)
	{
		delete branch;
	}
}

int main()
{
	using namespace std;

	vector<double> probabilties_list;
	vector<char> excluded_letters;
	vector<Symbol*> symbols_list;

	string message;

	double probability;
	char letter;
	double average_length = 0;
	double entropy = 0;
	double sum = 0;

	while (true) 
	{
		cout << "Type message to encode: ";
		getline(cin, message);

		// Determine probabilty for each symbol in input message
		for (int i = 0; i < message.size(); i++)
		{
			letter = message[i];

			if (find(excluded_letters.begin(), excluded_letters.end(), letter) == excluded_letters.end())
			{
				for (int j = 0; j < message.size(); j++)
				{
					if (message[j] == letter)
					{
						sum++;
					}
				}

				probability = sum / message.size();

				Symbol* symbol = new Symbol(letter, probability);
				symbols_list.push_back(symbol);

				excluded_letters.push_back(letter);
				sum = 0;
			}
		}

		sort(symbols_list.begin(), symbols_list.end(), sort_symbols_probability);

		cout << "\nProbabilties: \n";
		for (auto symbol : symbols_list)
		{
			probabilties_list.push_back(symbol->probabilty);
			cout << setprecision(3);
			cout << "'" << symbol->name << "'" << " -> ";
			cout << symbol->probabilty << "\n";
		}

		assign_codes(symbols_list);

		sort(symbols_list.begin(), symbols_list.end(), sort_symbols_sequence);

		cout << "\nSequences: \n";
		for (auto symbol : symbols_list)
		{
			// Each final sequence assigned to a symbol has to be reversed due to the direction of the tree search
			reverse(symbol->sequence.begin(), symbol->sequence.end());
			cout << "'" << symbol->name << "' -> ";
			for (int i : symbol->sequence)
			{
				cout << i << " ";
			}
			cout << "\n";

			average_length = average_length + (symbol->probabilty * symbol->sequence.size());
			entropy = entropy + (symbol->probabilty * log2(1 / symbol->probabilty));
		}

		cout << "\nEncoded message: \n";

		for (char letter : message)
		{
			for (auto symbol : symbols_list)
			{
				if (symbol->name == letter)
				{
					for (int i : symbol->sequence)
					{
						cout << i << "";
					}
					cout << " ";
				}
			}
		}

		cout << "\n\n" << setprecision(3);
		cout << "Average codeword length (Lx) = " << average_length << "\n";
		cout << "Entropy (H(x)) = " << entropy << "\n\n";
		
		average_length = 0;
		entropy = 0;
		sum = 0;

		probabilties_list.clear();
		excluded_letters.clear();
		symbols_list.clear();

		// Cleaning up memory
		for (auto symbol : symbols_list)
		{
			delete symbol;
		}
	}
}