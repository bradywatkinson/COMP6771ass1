#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <vector>
#include <list>
#include <regex>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <ctype.h>


struct number {
	double value;
	bool isInt;
};

std::list<struct number> calc;
std::ifstream in;

void parseLine (std::string line);
void printStack ();

int main (int argc, char* argv[]) {

	// setup the print out format for the precision required.
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
	std::cout.precision(3);

	// open the file for reading
	
	in.open(argv[1]);

	std::string line;

	while (in >> line) {
		parseLine(line);
	}
	printStack();
}

void parseLine (std::string line) {

	std::stringstream iss;
	iss << line;

	std::cout << "parseLine received: " << line << std::endl;

	std::string tmpStr;
	while (std::getline(iss,tmpStr,' ')) {
		//its an integer
		if (std::regex_match (tmpStr, std::regex("(\\+|-)?[[:digit:]]+"))) {
			struct number new_num;
			new_num.value = std::atoi(tmpStr.c_str());
			new_num.isInt = true;
			calc.push_front(new_num);
			std::cout << "processed " << tmpStr << std::endl;
		//else if it is a double
		} else if (std::regex_match (tmpStr, std::regex ("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"))) {
			struct number new_num;
			new_num.value = std::atof(tmpStr.c_str());
			new_num.isInt = false;
			calc.push_front(new_num);
			std::cout << "processed " << tmpStr << std::endl;
		//else process the command
		} else if (tmpStr.compare("add") == 0) {
			struct number x,y,tmpNum;
			x = calc.front();
			calc.pop_front();
			y = calc.front();
			calc.pop_front();
			tmpNum.isInt = x.isInt && y.isInt;
			tmpNum.value = x.value + y.value;
			calc.push_front(tmpNum);
			std::cout << "after add: " << x.value << " " << y.value << " " << tmpNum.value << std::endl;
		} else if (tmpStr.compare("sub") == 0) {
			struct number x,y,tmpNum;
			x = calc.front();
			calc.pop_front();
			y = calc.front();
			calc.pop_front();
			tmpNum.isInt = x.isInt && y.isInt;
			tmpNum.value = y.value - x.value;
			calc.push_front(tmpNum);
			std::cout << "after sub: " << x.value << " " << y.value << " " << tmpNum.value << std::endl;
		} else if (tmpStr.compare("mult") == 0) {
			struct number x,y,tmpNum;
			x = calc.front();
			calc.pop_front();
			y = calc.front();
			calc.pop_front();
			tmpNum.isInt = x.isInt && y.isInt;
			tmpNum.value = y.value * x.value;
			calc.push_front(tmpNum);
			std::cout << "after mult: " << x.value << " " << y.value << " " << tmpNum.value << std::endl;
		} else if (tmpStr.compare("div") == 0) {
			struct number x,y,tmpNum;
			x = calc.front();
			calc.pop_front();
			y = calc.front();
			calc.pop_front();
			tmpNum.isInt = x.isInt && y.isInt;
			tmpNum.value = x.value / y.value;
			calc.push_front(tmpNum);
			std::cout << "after div: " << x.value << " " << y.value << " " << tmpNum.value << std::endl;
		} else if (tmpStr.compare("sqrt") == 0) {
			struct number x,tmpNum;
			x = calc.front();
			calc.pop_front();
			tmpNum.isInt = x.isInt;
			tmpNum.value = sqrt(x.value);
			calc.push_front(tmpNum);
			std::cout << "after sqrt: " << x.value << " " << tmpNum.value << std::endl;
		} else if (tmpStr.compare("pop") == 0) {
			struct number tmpNum;
			tmpNum = calc.front();
			calc.pop_front();
			std::cout << "popping off: " << tmpNum.value << std::endl;
		} else if (tmpStr.compare("reverse") == 0) {
			struct number tmpNum;
			tmpNum = calc.front();
			calc.pop_front();

			//set up iterator
			std::list<struct number>::iterator it=calc.begin();
			std::advance(it,tmpNum.value); 
			//set up target
			std::list<struct number> calc_new;
			//splice and reverse
			calc.splice (calc_new.begin(), calc_new, calc.begin(), it);
			calc_new.reverse();
			//splice back
			calc_new.splice (calc.begin(), calc, calc_new.begin(), calc_new.end());

			std::cout << "reversing" << std::endl;
		} else if (tmpStr.compare("repeat") == 0) {
			struct number tmpNum;
			tmpNum = calc.front();
			calc.pop_front();
			int repeat = tmpNum.value;
			std::string nextCommands = "";
			std::cout << "repeating " << tmpNum.value << " times " << line << std::endl;
			//in >> line;
			std::cout << "testing line " << line << std::endl;
			std::cout << "testing tmpStr " << tmpStr << std::endl;
			//nextCommands += 
			//std::getline(iss,tmpStr);

			std::cout << "the rest of tmpStr is " << tmpStr << std::endl;
			while (in >> line) {
				iss << line;
				while (std::getline(iss,tmpStr,' ')) {
					std::cout << "repeat received " << tmpStr << std::endl;
					if (tmpStr.compare("endrepeat") == 0) {
						for (int i=0;i<repeat;++i) {
							parseLine(nextCommands);
						}
						std::cout << "ending repeat: " << std::endl;
						goto endRepeat;
					} else {
						nextCommands += tmpStr + " ";
					} 
				}
				iss.clear();
			}
			endRepeat:
			iss.clear();
			//std::cout << "test" << std::endl;
		} else {
			std::cout << "|" << tmpStr << "|" << " not recognised" << std::endl;
		}
	}
	iss.clear();
}

void printStack () {
	while (!calc.empty()) {
		struct number n = calc.front();
		std::string tmp;
		if (n.isInt) {
			tmp = std::to_string(int(n.value));
		} else {
			tmp = std::to_string(n.value);
		}
		//std::cout << n.isInt << " <value result> " << std::to_string(n.isInt ? int(n.value) : n.value) << std::endl;
		std::cout << tmp << std::endl;
		calc.pop_front();
	}
}





