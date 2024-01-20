#include <algorithm>
#include <chrono>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

unsigned long long c = 0;

class Timer
{
public:
	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		std::cout << "\nCZAS WYKONYWANIA ALGORYTMU: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.0 << "s\n";
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

bool isOperator(const char c)
{
	return c == '+' || c == '*' || c == '|' || c == '!' || c == '-' || c == '/' || c == '&';
}

std::stack<char> shuntingYardLog(const std::string_view& str)
{
	std::stack<char> ops;
	std::stack<char> temp_stack;

	for (const char c : str)
	{
		std::cout << "RESOLVING CHAR: " << c << '\n';
		if (std::isalpha(c) || std::isdigit(c))
		{
			ops.push(c);
			std::cout << "PUSHING OPS: " << c << '\n';
		}
		else if (isOperator(c))
		{
			if (!temp_stack.empty() && c == '+' && temp_stack.top() == '*')
			{
				while (!temp_stack.empty() && temp_stack.top() == '*')
				{
					std::cout << "PUSHING OPS: *\n";
					ops.push(temp_stack.top());
					temp_stack.pop();
					std::cout << "POPPING TEMP: *\n";
				}
				std::cout << "PUSHING TEMP: " << c << '\n';
				temp_stack.push(c);

				//while (isOperator(ops.top()))
				//{
				//	temp_stack.push(ops.top());
				//	ops.pop();
				//}
			}
			else if (!temp_stack.empty() && temp_stack.top() == '!')
			{
				if (c == '!')
				{
					temp_stack.push(c);
					std::cout << "PUSHING TEMP: !\n";
				}
				else if (c == '+')
				{
					while (!temp_stack.empty() && (temp_stack.top() == '!' || temp_stack.top() == '*'))
					{
						std::cout << "PUSHING OPS: " << temp_stack.top() << '\n';
						ops.push(temp_stack.top());
						std::cout << "POPPING TEMP: " << temp_stack.top() << '\n';
						temp_stack.pop();
					}
					std::cout << "PUSHING TEMP: " << c << '\n';
					temp_stack.push(c);
				}
				else if (c == '*')
				{
					while (!temp_stack.empty() && temp_stack.top() == '!')
					{
						std::cout << "PUSHING OPS: " << temp_stack.top() << '\n';
						ops.push(temp_stack.top());
						std::cout << "POPPING TEMP: " << temp_stack.top() << '\n';
						temp_stack.pop();
					}
					std::cout << "PUSHING TEMP: " << c << '\n';
					temp_stack.push(c);
				}
			}
			else
			{
				std::cout << "PUSHING TEMP: " << c << '\n';
				temp_stack.push(c);
			}

			//temp_stack.push(c);
		}
		else if (c == '(')
		{
			temp_stack.push(c);
			std::cout << "PUSHING TEMP: (\n";
		}
		else if (c == ')')
		{
			while (temp_stack.top() != '(')
			{
				ops.push(temp_stack.top());
				std::cout << "REMOVING: " << temp_stack.top() << '\n';
				temp_stack.pop();
			}
			std::cout << "REMOVING: " << temp_stack.top() << '\n';
			temp_stack.pop();

			//while (isOperator(ops.top()))
			//{
			//	std::cout << "UNWINDING: " << ops.top() << '\n';
			//	temp_stack.push(ops.top());
			//	ops.pop();
			//}
		}
	}

	while (!temp_stack.empty())
	{
		ops.push(temp_stack.top());
		temp_stack.pop();
	}

	//while(!ops.empty())
	//{
	//	std::cout << ops.top();
	//	ops.pop();
	//}

	std::string temp;

	while (!ops.empty())
	{
		temp += ops.top();
		ops.pop();
	}

	std::cout << "\nRPN NOTATION: " << std::string(temp.rbegin(), temp.rend()) << '\n';

	for (char c : temp)
	{
		ops.push(c);
	}

	return ops;
}

std::stack<char> shuntingYard(const std::string_view& str)
{
	std::stack<char> ops;
	std::stack<char> temp_stack;

	for (const char c : str)
	{
		if (std::isalpha(c) || std::isdigit(c))
		{
			ops.push(c);
		}
		else if (isOperator(c))
		{
			if (!temp_stack.empty() && c == '+' && temp_stack.top() == '*')
			{
				while (!temp_stack.empty() && temp_stack.top() == '*')
				{
					ops.push(temp_stack.top());
					temp_stack.pop();
				}
				temp_stack.push(c);
			}
			else if (!temp_stack.empty() && temp_stack.top() == '!')
			{
				if (c == '!')
				{
					temp_stack.push(c);
				}
				else if (c == '+')
				{
					while (!temp_stack.empty() && (temp_stack.top() == '!' || temp_stack.top() == '*'))
					{
						ops.push(temp_stack.top());
						temp_stack.pop();
					}
					temp_stack.push(c);
				}
				else if (c == '*')
				{
					while (!temp_stack.empty() && temp_stack.top() == '!')
					{
						ops.push(temp_stack.top());
						temp_stack.pop();
					}
					temp_stack.push(c);
				}
			}
			else
			{
				temp_stack.push(c);
			}
		}
		else if (c == '(')
		{
			temp_stack.push(c);
		}
		else if (c == ')')
		{
			while (temp_stack.top() != '(')
			{
				ops.push(temp_stack.top());
				temp_stack.pop();
			}
			temp_stack.pop();
		}
	}

	while (!temp_stack.empty())
	{
		ops.push(temp_stack.top());
		temp_stack.pop();
	}

	std::string temp;

	while (!ops.empty())
	{
		temp += ops.top();
		ops.pop();
	}

	//std::cout << "RPN: " << std::string(temp.rbegin(), temp.rend()) << "\n\n";

	for (char c : temp)
	{
		ops.push(c);
	}

	return ops;
}

int expEval(const std::stack<char>& cops)
{
	std::stack<char> ops = cops;
	std::stack<float> temp_stack;

	while (!ops.empty())
	{
		while (std::isalnum(ops.top()) || std::isdigit(ops.top()))
		{
			temp_stack.push(ops.top() - '0');
			//std::cout << "PUSHING: " << ops.top() - '0' << '\n';
			ops.pop();
		}

		if (isOperator(ops.top()))
		{
			if (ops.top() == '!')
			{
				int num = temp_stack.top();
				temp_stack.pop();
				//std::cout << std::format("operation: !{}", num) << '\n';
				//std::cout << "PUSHING: " << !num << '\n';
				temp_stack.push(!num);
				ops.pop();
				continue;
			}

			int num1 = temp_stack.top();
			temp_stack.pop();

			int num2 = temp_stack.top();
			temp_stack.pop();

			std::swap(num1, num2);

			switch (ops.top())
			{
			case '+':
			{
				temp_stack.push(num1 | num2);
				//std::cout << std::format("operation: {} OR {}", num1, num2) << '\n';
			}
			break;
			case '-':
			{
				temp_stack.push(num1 - num2);
				//std::cout << std::format("operation: {}-{}", num1, num2) << '\n';
			}
			break;
			case '*':
			{
				temp_stack.push(num1 & num2);
				//std::cout << std::format("operation: {} AND {}", num1, num2) << '\n';
			}
			break;
			case '/':
			{
				temp_stack.push(num1 / num2);
				//std::cout << std::format("operation: {}/{}", num1, num2) << '\n';
			}
			}
			ops.pop();
		}
	}

	return temp_stack.top();
}

std::string toBinary(int num, const int length = 0)
{
	std::string res;
	while (num >= 1)
	{
		res += std::to_string(num % 2);
		num /= 2;
	}

	for (int i = res.size(); i < length; i++)
	{
		res += "0";
	}

	return std::string(res.rbegin(), res.rend());
}

int toDecimal(const std::string_view& bin)
{
	int exp = 1;
	int ret = 0;
	for (std::string_view::reverse_iterator it = bin.rbegin(); it != bin.rend(); std::advance(it, 1))
	{
		ret += (*it - '0') * exp;
		exp *= 2;
	}
	return ret;
}

std::vector<int> truthTable(std::stack<char> cops, std::string& var)
{
	std::stack<char> for_swapping = cops;

	std::string swp;
	while (!for_swapping.empty())
	{
		swp += for_swapping.top();
		for_swapping.pop();
	}
	std::string copy_swp = swp;

	std::stack<char> temp_stack;
	std::stack<char> ops;

	std::unordered_map<char, int> map;

	std::string vars;

	while (!cops.empty())
	{
		if (std::isalpha(cops.top()))
		{
			vars += cops.top();
		}
		cops.pop();
	}

	std::ranges::sort(vars, [](char lhs, char rhs) {return lhs < rhs; });
	vars.erase(std::unique(vars.begin(), vars.end()), vars.end());

	std::string order;
	std::string order_s;

	while (vars != order_s)
	{
		std::cout << "wprowadz kolejnosc zmiennych (domyslnie " << vars << "): ";
		std::getline(std::cin, order);
		if (order.empty()) break;
		order_s = order;
		std::ranges::sort(order_s, [](char l, char r) { return l < r; });
	}

	if (!order.empty())
		vars = order;

	std::cout << "\n\n==================== TABLICA PRAWDY ===================\n";

	//std::cout << "table for testing:\n" << vars << '\n';
	std::cout << '\n' << vars << " | " << "X\n";

	for (int i = 0; i < vars.size(); i++)
		std::cout << '-';

	std::cout << " | -\n";

	std::vector<int> minterms;

	for (int i = 0; i <= std::pow(2, vars.size()) - 1; i++)
	{
		std::string bin = toBinary(i, vars.size());
		std::cout << bin << " | ";

		for (int i = 0; i < vars.size(); i++)
		{
			map[vars[i]] = bin[i];
		}

		for (int i = 0; i < copy_swp.size(); i++)
		{
			if (std::isalpha(copy_swp[i]))
			{
				//std::cout << "IS APLHA: " << copy_swp[i] << '\n';
				swp[i] = map[copy_swp[i]];
			}
		}
		//std::cout << "would be evaluated: " << swp << '\n';
		swp = std::string(swp.rbegin(), swp.rend());

		std::stack<char> for_eval;
		for (char c : swp)
		{
			for_eval.push(c);
		}

		int res = expEval(for_eval);

		std::cout << res << '\n';

		if (res)
		{
			minterms.emplace_back(toDecimal(bin));
		}

		swp = copy_swp;
	}

	minterms.push_back(vars.size());
	var = vars;
	return minterms;
}

std::string applyDeMorgans(std::string str)
{
	if (str.find('!') == std::string::npos) return str;

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '!' && i < str.size() - 1 && str[i + 1] != '(')
		{
			str.insert(str.begin() + i, '(');
			str.insert(str.begin() + i + 3, ')');
			std::cout << str << '\n';
			i += 2;
		}
	}

	return str;
}

struct Impl
{
	Impl(const std::string& w, bool ess = true) : word(w), essential(ess), minterms() {}

	std::string word;
	std::vector<int> minterms;
	bool essential;
};

int cmp(Impl& lhs, Impl& rhs)
{
	c++;

	int hamming_distance = 0;
	int pos = -1;
	for (int i = 0; i < lhs.word.size(); i++)
	{
		if (lhs.word[i] == 'x' && rhs.word[i] != 'x' || rhs.word[i] == 'x' && lhs.word[i] != 'x') return -1;

		if (lhs.word[i] != rhs.word[i])
		{
			hamming_distance++;
			pos = i;
		}
	}

	if (hamming_distance == 1)
	{
		lhs.essential = false;
		rhs.essential = false;
		return pos;
	}
	else
	{
		return -1;
	}
}

int hdistance(const std::string& left, const std::string& right)
{
	for (int i = 0; i < left.size(); i++)
	{
		if (left[i] == 'x') continue;
		if (left[i] != right[i]) return -1;
	}

	return 0;
}

std::vector<std::string> getPermutations(const std::string_view& comb)
{
	std::vector<std::string> permute;
	std::vector<std::string> ret;

	std::string temp;
	for (int i = 0; i < comb.size(); i++)
	{
		if (!std::isspace(comb[i]))
		{
			temp += comb[i];
		}
		else
		{
			permute.emplace_back(temp);
			temp.clear();
		}
	}
	std::sort(permute.begin(), permute.end());
	temp.clear();

	do
	{
		for (const std::string& str : permute)
		{
			temp += str + ' ';
		}
		temp.pop_back();
		ret.emplace_back(temp);
		temp.clear();
	} while (std::next_permutation(permute.begin(), permute.end()));
	return ret;
}

void generateCombinations(const std::vector<Impl>& prime_implicants, const std::vector<std::vector<int>>& coverage_minterms, const std::vector<int>& minterms, const std::string_view& vars)
{
	const int max_combs = std::pow(2, prime_implicants.size());

	//std::cout << "POSSIBLE COMBS: " << '\n';

	std::vector<std::string> permutations;
	std::string shortest_comb;
	int least_minterms = std::numeric_limits<int>::max();

	for (int i = 1; i < max_combs; i++)
	{
		std::string bin = toBinary(i, prime_implicants.size());

		std::string comb;
		std::vector<int> minterms_covered;

		for (int j = 0; j < bin.size(); j++)
		{
			if (bin[j] == '1')
			{
				comb += prime_implicants[j].word + ' ';
				minterms_covered.insert(minterms_covered.end(), coverage_minterms[j].begin(), coverage_minterms[j].end());
				std::ranges::sort(minterms_covered, [](const int l, const int r) {return l < r; });
			}
		}
		std::vector<int> unique = std::vector<int>(minterms_covered.begin(), std::unique(minterms_covered.begin(), minterms_covered.end()));

		//std::cout << comb << "total covered: " << minterms_covered.size();

		//std::cout << ", all minterms: " << (unique == minterms) << '\n';

		if (unique == minterms && minterms_covered.size() < least_minterms)
		{
			shortest_comb = comb;
			least_minterms = minterms_covered.size();
		}
	}

	//std::cout << "\n\nshortest comb: " << shortest_comb << "\n\n";
	permutations = getPermutations(shortest_comb);

	std::cout << "\nMOZLIWE FORMY MINIMALNE: " << permutations.size() << '\n';

	for (const std::string& permutation : permutations)
	{
		int counter_var = 0;
		for (int i = 0; i < permutation.size(); i++)
		{
			if (permutation[i] == ' ')
			{
				counter_var = -1;
				std::cout << '+';
			}
			else if (permutation[i] != 'x')
			{
				if (permutation[i] == '1')
				{
					std::cout << vars[counter_var];
				}
				else if (permutation[i] == '0')
				{
					std::cout << vars[counter_var] << '\'';
				}
			}
			counter_var++;
		}
		std::cout << '\n';
	}
}

void qmc(std::vector<int>& minterms, const std::string& vars, std::vector<int> dont_cares = {})
{
	if (minterms.empty())
	{
		std::cout << "\nFORMA MINIMALNA: 0\n";
		return;
	}

	std::vector<Impl> temp_vec = { Impl("w") };
	const int length = minterms.back();
	minterms.pop_back();

	minterms.insert(minterms.end(), dont_cares.begin(), dont_cares.end());
	std::sort(minterms.begin(), minterms.end());

	//   group vec   stage vec   word vec
	std::vector<std::vector<std::vector<Impl>>> groups;

	groups.emplace_back();

	for (int i = 0; i <= length; i++)
		groups[0].emplace_back();

	for (const int n : minterms)
	{
		std::string num = toBinary(n, length);
		Impl impl(num);
		impl.minterms.emplace_back(n);
		groups[0][std::ranges::count(num, '1')].emplace_back(impl);
	}

	groups.emplace_back();

	int g = 0;


	while (!temp_vec.empty())
	{
		std::cout << "ETAP " << g << ": ";

		temp_vec.clear();
		for (int i = 0; i < groups[g].size(); i++)
		{
			for (int j = 0; j < groups[g][i].size(); j++)
			{
				if (i + 1 >= groups[g].size()) break;

				for (int k = 0; k < groups[g][i + 1].size(); k++)
				{
					Impl impl(groups[g][i][j]);

					int ret = cmp(groups[g][i][j], groups[g][i + 1][k]);
					if (ret != -1)
					{
						impl.minterms.clear();
						impl.word[ret] = 'x';
						impl.minterms.insert(impl.minterms.end(), groups[g][i][j].minterms.begin(), groups[g][i][j].minterms.end());
						impl.minterms.insert(impl.minterms.end(), groups[g][i + 1][k].minterms.begin(), groups[g][i + 1][k].minterms.end());
						if (std::ranges::count(impl.word, 'x') == length)
						{
							temp_vec.emplace_back(impl);
							break;
						}
						else
						{
							temp_vec.emplace_back(impl);
						}
						//std::cout << impl.word << '\n';
					}
				}
			}
		}

		for (int j = 0; j <= length; j++)
		{
			//std::cout << "\nLICZBA JEDYNEK: " << j << '\n';

			if (!groups[g][j].empty())
			{
				for (const auto& impl : groups[g][j])
				{
					//std::cout << '(';
					//for (int i = 0; i < impl.minterms.size(); i++)
					//{
					//	std::cout << impl.minterms[i];
					//	if (i < impl.minterms.size() - 1)
					//		std::cout << ',';
					//}
					//std::cout << ") ";
					//
					//std::cout << impl.essential << ' ' << impl.word << '\n';
				}
			}
			else
			{
				//std::cout << "-\n";
			}
		}
		//std::cout << '\n';

		//if (temp_vec.empty()) break;

		std::ranges::sort(temp_vec, [](const Impl& lhs, const Impl& rhs) {return lhs.word > rhs.word; });
		temp_vec.erase(std::unique(temp_vec.begin(), temp_vec.end(), [](const Impl& lhs, const Impl& rhs) {return lhs.word.compare(rhs.word) == 0; }), temp_vec.end());

		groups.emplace_back();

		for (int j = 0; j <= length; j++)
		{
			groups[g + 1].emplace_back();
		}

		for (int j = 0; j < temp_vec.size(); j++)
		{
			Impl impl = temp_vec[j];
			impl.essential = true;
			groups[g + 1][std::ranges::count(temp_vec[j].word, '1')].emplace_back(impl);
		}
		g++;
		std::cout << "OK\n";
	}

	std::cout << "\n================== IMPLIKANTY PROSTE ==================\n\n";

	std::vector<Impl> prime_implicants;
	for (const auto& stage : groups)
	{
		for (const auto& group : stage)
		{
			for (const Impl& implicant : group)
			{
				if (implicant.essential)
				{
					std::cout << implicant.word << '\n';
					prime_implicants.push_back(implicant);
				}
			}
		}
	}

	std::cout << "\n================== TABLICA POKRYCIA ==================\n\n";
	for (int i = 0; i < length; i++) std::cout << ' ';

	prime_implicants.erase(std::remove_if(prime_implicants.begin(), prime_implicants.end(),
		[dont_cares](const Impl& impl)
		{
			for (const int i : impl.minterms)
			{
				if (std::ranges::find(dont_cares, i) == dont_cares.end())
				{
					return false;
				}
			}
			return true;
		}), prime_implicants.end());

	minterms.erase(std::remove_if(minterms.begin(), minterms.end(), [dont_cares](const int i) { return std::ranges::find(dont_cares, i) != dont_cares.end(); }), minterms.end());

	for (const Impl& impl : prime_implicants)
	{
		std::cout << '|' << impl.word;
	}
	//std::cout << '\n';
	//for (int i = 0; i < 3 + prime_implicants.size() * 4; i++) std::cout << '_';

	std::cout << '\n';

	std::vector<std::vector<bool>> coverage_table;
	std::vector<int> covered_minterms;

	for (int i = 0; i < prime_implicants.size(); i++)
	{
		coverage_table.emplace_back();
	}

	for (int i : minterms)
	{
		std::string bin = toBinary(i, length);
		std::cout << bin;
		int implicant = 0;
		for (const Impl& impl : prime_implicants)
		{
			if (hdistance(impl.word, bin) == 0)
			{
				std::cout << '|';
				for (int i = 0; i < std::round(length / 2); i++) std::cout << ' ';
				std::cout << 'X';
				coverage_table[implicant].emplace_back(true);
				for (int i = 0; i < length - length / 2 - 1; i++) std::cout << ' ';
			}
			else
			{
				std::cout << '|';
				coverage_table[implicant].emplace_back(false);
				for (int i = 0; i < length; i++) std::cout << ' ';
			}
			implicant++;
		}
		std::cout << '\n';
	}

	//std::cout << "implicants: " << coverage_table.size() << " minterms: " << coverage_table[0].size() << '\n';

	std::cout << "\n================== IMPLIKANTY ISTOTNE ==================\n\n";

	std::vector<std::string> essential_prime_implicants;
	//std::set<int> for_combinations;
	for (int i = 0; i < coverage_table.front().size(); i++)
	{
		int covered_count = 0;
		int pos = 0;

		for (int j = 0; j < prime_implicants.size(); j++)
		{
			if (coverage_table[j][i])
			{
				pos = j;
				//for_combinations.insert(i);
				covered_count++;
			}
		}

		if (covered_count == 1)
		{
			if (std::ranges::find(essential_prime_implicants, prime_implicants[pos].word) == essential_prime_implicants.end())
			{
				std::cout << prime_implicants[pos].word << '\n';
				essential_prime_implicants.emplace_back(prime_implicants[pos].word);
			}

			for (int k = 0; k < minterms.size(); k++)
			{
				if (coverage_table[pos][k])
				{
					covered_minterms.emplace_back(minterms[k]);
				}
			}
		}
	}

	std::vector<Impl> epi;

	for (const std::string& str : essential_prime_implicants)
	{
		epi.emplace_back(str, true);
	}

	for (const Impl& impl : prime_implicants)
	{
		bool found = false;
		for (int i = 0; i < epi.size(); i++)
		{
			if (epi[i].word == impl.word)
			{
				found = true;
			}
		}

		if (!found)
		{
			epi.emplace_back(impl.word, false);
		}
	}

	std::vector<std::vector<int>> coverage_vector;
	for (int i = 0; i < prime_implicants.size(); i++)
	{
		coverage_vector.emplace_back();
	}

	for (int i = 0; i < prime_implicants.size(); i++)
	{
		for (int j = 0; j < coverage_table[i].size(); j++)
		{
			if (coverage_table[i][j])
			{
				coverage_vector[i].push_back(minterms[j]);
				//std::cout << "implicant: " << i << " minterm: " << minterms[j] << '\n';
			}
		}
	}

	//std::cout << "IMPLICANTS TEST\n";
	//for (const auto& impl : epi)
	//{
	//	std::cout << impl.word << ' ' << impl.essential << '\n';
	//}

	for (Impl& impl : prime_implicants)
	{
		for (int i = 0; i < epi.size(); i++)
		{
			if (epi[i].word == impl.word)
			{
				impl.essential = true;
			}
		}
	}

	if (essential_prime_implicants.size() == 1)
	{
		int count = std::count(essential_prime_implicants.front().begin(), essential_prime_implicants.front().end(), 'x');
		if (count == essential_prime_implicants.front().size())
		{
			std::cout << "\nFORMA MINIMALNA: 1\n";
			return;
		}
	}

	generateCombinations(prime_implicants, coverage_vector, minterms, vars);

	std::ranges::sort(covered_minterms, [](const int left, const int right) { return left < right; });
	covered_minterms.erase(std::unique(covered_minterms.begin(), covered_minterms.end(), [](const int left, const int right) { return left == right; }), covered_minterms.end());


	//std::cout << "\nFORMA MINIMALNA(niekoniecznie jedyna mozliwa): ";
	//if (covered_minterms.size() == minterms.size())
	//{
	//	int counter = 0;
	//
	//	for (const std::string& epi : essential_prime_implicants)
	//	{
	//		for (int i = 0; i < vars.size(); i++)
	//		{
	//			if (epi[i] != 'x')
	//			{
	//				if (epi[i] == '0')
	//				{
	//					std::cout << vars[i] << '\'';
	//				}
	//				else if (epi[i] == '1')
	//				{
	//					std::cout << vars[i];
	//				}
	//			}
	//		}
	//		if (counter < essential_prime_implicants.size() - 1)
	//			std::cout << '+';
	//
	//		counter++;
	//	}
	//}
	//else
	//{
	//
	//}
	//std::cout << '\n';
}

void replace(std::string& exp, const size_t left, const size_t right)
{
	std::cout << exp[left] << ' ' << exp[right] << '\n';
	for (char& c : exp)
	{
		if (c == '[')
		{
			c = exp[left];
		}
		else if (c == ']')
		{
			c = exp[right];
		}
	}

	exp[left] = ' ';
	exp[right] = ' ';
}

std::string replace_operators(std::string exp)
{
	std::unordered_map<std::string, std::string> operator_map;

	std::string labels[] = { "not", "xnor", "and", "or", "nand", "xor", "nor" };

	operator_map["not"] = "!";
	operator_map["xnor"] = "([*])+(![*!])";
	operator_map["and"] = "[*]";
	operator_map["or"] = "[+]";
	operator_map["nand"] = "!([*])";
	operator_map["xor"] = "[*!]+![*]";
	operator_map["nor"] = "!([+])";

	size_t found;

	found = exp.find(labels[5]);
	while (found != std::string::npos)
		if (size_t found = exp.find(labels[5]); found != std::string::npos)
		{
			exp.erase(found, 3);
			exp.insert(found, operator_map[labels[5]]);
			replace(exp, found - 2, found + 10);
		}

	return exp;
}

enum class Type
{
	Operand,
	Not,
	Or,
	Nor,
	And,
	Nand,
	Xor,
	Xnor,
	LeftBracket,
	RightBracket
};

class Token
{
public:
	Token(const Type t, const char let = '-') : type(t), letter(let)
	{
		switch (t)
		{
		case Type::Or:
		case Type::Nor:
		case Type::Xor:
		case Type::Xnor:
		{
			precedence = 0;
		}
		break;
		case Type::And:
		case Type::Nand:
		{
			precedence = 1;
		}
		break;
		case Type::Not:
		{
			precedence = 2;
		}
		}
	}

	Token(std::string& token)
	{
		std::transform(token.begin(), token.end(), token.begin(), ::tolower);

		if (token.size() == 1 && std::isalpha(token[0]))
		{
			type = Type::Operand;
			precedence = -1;
		}
		else if (token == "or")
		{
			type = Type::Or;
			precedence = 0;
		}
		else if (token == "nor")
		{
			type = Type::Nor;
			precedence = 0;
		}
		else if (token == "and")
		{
			type = Type::And;
			precedence = 1;
		}
		else if (token == "nand")
		{
			type = Type::Nand;
			precedence = 1;
		}
		else if (token == "xor")
		{
			type = Type::Xor;
			precedence = 2;
		}
		else if (token == "xnor")
		{
			type = Type::Xnor;
			precedence = 2;
		}
		else if (token == "not")
		{
			type = Type::Not;
			precedence = 3;
		}
	}

	Type getType() const
	{
		return type;
	}

	int getPrecedence() const
	{
		return precedence;
	}
private:
	Type type;
	int precedence;
	char letter;
};

std::vector<Token> tokenize(const std::string& exp)
{
	std::string exp_lower;

	for (const char c : exp)
	{
		if (std::isalpha(c))
		{
			exp_lower += std::tolower(c);
		}
		else
		{
			exp_lower += c;
		}
	}

	std::vector<Token> tokens;
	
	for (int i = 0; i < exp.size(); i++)
	{
		if (exp_lower.find("xor", i) == i)
		{
			std::cout << "xor found at " << i << '\n';
			tokens.emplace_back(Type::Xor);
			i += 2;
		}
		else if (exp_lower.find("nand", i) == i)
		{
			std::cout << "nand found at " << i << '\n';
			tokens.emplace_back(Type::Nand);
			i += 3;
		}
		else if (exp_lower.find("xnor", i) == i)
		{
			std::cout << "xnor found at " << i << '\n';
			tokens.emplace_back(Type::Xnor);
			i += 3;
		}
		else if (exp_lower.find("nor", i) == i)
		{
			std::cout << "nor found at " << i << '\n';
			tokens.emplace_back(Type::Nor);
			i+=2;
		}
		else if (exp_lower.find("or", i) == i)
		{
			std::cout << "or found at " << i << '\n';
			tokens.emplace_back(Type::Or);
			i++;
		}
		else if (exp_lower.find("and", i) == i)
		{
			std::cout << "and found at " << i << '\n';
			tokens.emplace_back(Type::And);
			i+=2;
		}
		else if (exp_lower.find("not", i) == i)
		{
			std::cout << "xnor found at " << i << '\n';
			tokens.emplace_back(Type::Not);
			i += 2;
		}
		else if (exp[i] == '(')
		{
			std::cout << "left bracket found at " << i << '\n';
			tokens.emplace_back(Type::LeftBracket);
		}
		else if (exp[i] == ')')
		{
			std::cout << "right bracket found at " << i << '\n';
			tokens.emplace_back(Type::RightBracket);
		}
		else if (std::isalpha(exp[i]))
		{
			std::cout << "operand " << exp[i] << " found at " << i << '\n';
			tokens.emplace_back(Type::Operand, exp[i]);
		}
	}

	return tokens;
}

int main()
{
	bool log_ops = false;
	
	int op = -1;
	std::cout << "======================== Quine McCluskey ========================\n[1] Wprowadzanie mintermow\n[2] Wprowadzanie zdania logicznego\n";
	
	while (op != 1 && op != 2)
	{
		std::cout << "Wybor: ";
		std::cin >> op;
	}
	
	if (op == 1)
	{
		std::vector<int> minterms;
		std::vector<int> dont_cares;
		int m = 0;
		std::cout << "Wprowadz mintermy (wpisz -1 aby zakonczyc):\n";
		do
		{
			std::cout << ">> ";
			std::cin >> m;
			minterms.emplace_back(m);
	
		} while (m != -1);
		minterms.pop_back();
		m = 0;
		std::cout << "Wprowadz don't care'y (wpisz -1 aby zakonczyc):\n";
		do
		{
			std::cout << ">> ";
			std::cin >> m;
			dont_cares.emplace_back(m);
	
		} while (m != -1);
		dont_cares.pop_back();
		std::cin.ignore();
	
		std::string vars;
		std::string vars_unmod;
		if (minterms.size() > 0)
		{
			std::sort(minterms.begin(), minterms.end());
	
			int vars_size = toBinary(minterms.back()).size();
			minterms.emplace_back(vars_size);
	
			for (int i = 'A'; i < 'A' + vars_size; i++)
			{
				vars_unmod += i;
			}
	
			std::cout << "Wprowadz " << vars_size << " oznaczen(ia) (litery, domyslnie: " << vars_unmod << "):\n>> ";
	
			do
			{
				std::getline(std::cin, vars);
				if (vars.empty())
				{
					break;
				}
				vars_unmod = vars;
				std::sort(vars.begin(), vars.end());
				vars.erase(std::unique(vars.begin(), vars.end()), vars.end());
				if (vars.size() > vars_size)
					std::cout << "ZA DUZO OZNACZEN!\n>>";
				else if (vars.size() < vars_size)
					std::cout << "ZA MALO OZNACZEN!\n>>";
			} while (vars.size() != vars_size);
		}
	
		std::cout << '\n';
		{
			Timer t;
			qmc(minterms, vars_unmod, dont_cares);
		}
		std::cout << "\nPOROWNAN: " << c << '\n';
	}
	else
	{
		std::cout << "Wprowadz zdanie logiczne: ";
		std::string exp;// = "A*B*!C*!D+A*B*!C*D+A*!B*!C*D+A*B*C*D+A*!B*C*D+A*B*C*!D+A*!B*C*!D"; //firgure out
		std::cin >> exp;
		std::cin.ignore();
		exp.erase(std::remove_if(exp.begin(), exp.end(), [](char c) {return c == ' '; }), exp.end());
		std::cout << exp << " = X\n";
		std::stack<char> RPN = log_ops ? shuntingYardLog(exp) : shuntingYard(exp);
		std::string vars;
		std::vector<int> minterms = truthTable(RPN, vars);
	
		std::cout << "\nMINTERMY: ";
		for (int i = 0; i < minterms.size() - 1; i++)
		{
			std::cout << minterms[i] << ' ';
		}
		std::cout << "\n\n";
	
		{
			Timer t;
			qmc(minterms, vars);
		}
		std::cout << "\nPOROWNAN: " << c << '\n';
		//std::cin.get();
	}
}
