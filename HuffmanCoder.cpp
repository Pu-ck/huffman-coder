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

		double probability;
		char name;

		Symbol(char x, double y)
		{
			name = x;
			probability = y;
		}
};

// Tree branch class
class Branch 
{
	public:
		std::vector<Symbol*> elements_list;
		double probability;

		Branch (double x)
		{
			probability = x;
		}
};

// Sorting symbols by probabilty field
bool sort_symbols_probability(const Symbol* symbol_first, const Symbol* symbol_second)
{
	return symbol_first->probability > symbol_second->probability;
}

// Sorting symbols by size of sequence field
bool sort_symbols_sequence(const Symbol* symbol_first, const Symbol* symbol_second) {
	return symbol_first->sequence.size() < symbol_second->sequence.size();
}

// Sorting branches by probabilty field
bool sort_branches(const Branch* branch_first, const Branch* branch_second) {
	return branch_first->probability < branch_second->probability;
}

// Main Huffman algorithm
void assign_codes(std::vector<Symbol*> symbols_list)
{
	using namespace std;

	vector<double> probabilities_list;
	vector<Branch*> branch_list;

	double min_first, min_second, sum;

	// Create an additional vector with probabilty of each symbol
	// Push each symbol into a new branch 
	if (symbols_list.size() == 1)
	{
		symbols_list.at(0)->sequence.push_back(1);
	}
	else
	{
		for (auto symbol : symbols_list)
		{
			probabilities_list.push_back(symbol->probability);

			Branch* branch = new Branch(symbol->probability);
			branch->elements_list.push_back(symbol);

			branch_list.push_back(branch);
		}

		// Find two minimal probabilties and remove them from probabilties vector
		for (int i = 0; i < symbols_list.size() - 1; i++)
		{
			min_first = *min_element(probabilities_list.begin(), probabilities_list.end());

			for (int i = 0; i < probabilities_list.size(); i++)
			{
				if (probabilities_list[i] == min_first)
				{
					probabilities_list.erase(probabilities_list.begin() + i);
					break;
				}
			}

			min_second = *min_element(probabilities_list.begin(), probabilities_list.end());

			for (int i = 0; i < probabilities_list.size(); i++)
			{
				if (probabilities_list[i] == min_second)
				{
					probabilities_list.erase(probabilities_list.begin() + i);
					break;
				}
			}

			// Add minimal probabilties together and prepare a new branch 
			sum = min_first + min_second;
			Branch* new_branch = new Branch(sum);

			// Push symbols from two branches (with probabilty fields equal to first and second minimal probabilties) into newly created branch
			// Push 0 (smaller minimal value or first found) or 1 (bigger minimal value or second found) to sequence vector of symbols stored in each branch
			// Sort vector with branches and remove redundant branches
			for (auto branch : branch_list)
			{
				if (branch->probability == min_first)
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
				if (branch->probability == min_second)
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
			probabilities_list.push_back(sum);
		}
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
		do 
		{
			cout << "Type message to encode: ";
			getline(cin, message);

			if (message.size() < 1)
			{
				cout << "The message must be at least one character long." << endl;
			}

		} while (message.size() < 1);

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
			cout << setprecision(3);
			cout << "'" << symbol->name << "'" << " -> ";
			cout << symbol->probability << "\n";
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

			average_length = average_length + (symbol->probability * symbol->sequence.size());
			entropy = entropy + (symbol->probability * log2(1 / symbol->probability));
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

		excluded_letters.clear();
		symbols_list.clear();

		// Cleaning up memory
		for (auto symbol : symbols_list)
		{
			delete symbol;
		}
	}
}