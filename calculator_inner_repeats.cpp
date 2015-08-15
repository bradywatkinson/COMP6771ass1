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

#define DEBUG false
#define REPEAT true

struct number {
	double value;
	bool isInt;
};

std::list<struct number> calc;
std::ifstream in;

void parseLine (std::string line);
std::string to_double_string (double d);
void printStack ();

int main (int argc, char* argv[]) {

	// open the file for reading
	in.open(argv[1]);

	std::string line;
	while (in >> line) {
		parseLine(line);
	}
	//printStack();
}

void splitLine (std::string line) {

	std::stringstream iss;
	std::string tmpStr;
	iss << line;
	while(std::getline(iss,tmpStr,' ')) {
		//std::cout << "test: " << tmpStr << " " << line<< std::endl;
		parseLine(tmpStr);
	}
}

void parseLine (std::string tmpStr) {

	//its an integer
	if (std::regex_match (tmpStr, std::regex("(\\+|-)?[[:digit:]]+"))) {
		struct number new_num;
		new_num.value = std::stoi(tmpStr);
		new_num.isInt = true;
		calc.push_front(new_num);
		if (DEBUG) std::cout << "processed " << int(new_num.value) << " " << new_num.isInt << std::endl;
	//else if it is a double
	} else if (std::regex_match (tmpStr, std::regex ("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"))) {
		struct number new_num;
		new_num.value = std::stod(tmpStr);
		new_num.isInt = false;
		calc.push_front(new_num);
		if (DEBUG) std::cout << "processed " << to_double_string(new_num.value) << " " << new_num.isInt << std::endl;
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
		std::cout << (x.isInt ? std::to_string(int(x.value)) : to_double_string(x.value))
			<< " + " << (y.isInt ? std::to_string(int(y.value)) : to_double_string(y.value))
				<< " = " << (tmpNum.isInt ? std::to_string(int(tmpNum.value)) : std::to_string(tmpNum.value))<< std::endl;
	} else if (tmpStr.compare("sub") == 0) {
		struct number x,y,tmpNum;
		x = calc.front();
		calc.pop_front();
		y = calc.front();
		calc.pop_front();
		tmpNum.isInt = x.isInt && y.isInt;
		tmpNum.value = y.value - x.value;
		calc.push_front(tmpNum);
		//if (DEBUG) std::cout << "after sub: " << x.value << " " << y.value << " " << tmpNum.value << std::endl;
		std::cout << (x.isInt ? std::to_string(int(x.value)) : to_double_string(x.value))
			<< " - " << (y.isInt ? std::to_string(int(y.value)) : to_double_string(y.value))
				<< " = " << (tmpNum.isInt ? std::to_string(int(tmpNum.value)) : to_double_string(tmpNum.value))<< std::endl;
	} else if (tmpStr.compare("mult") == 0) {
		struct number x,y,tmpNum;
		x = calc.front();
		calc.pop_front();
		y = calc.front();
		calc.pop_front();
		tmpNum.isInt = x.isInt && y.isInt;
		tmpNum.value = y.value * x.value;
		calc.push_front(tmpNum);
		std::cout << (x.isInt ? std::to_string(int(x.value)) : to_double_string(x.value))
			<< " * " << (y.isInt ? std::to_string(int(y.value)) : to_double_string(y.value))
				<< " = " << (tmpNum.isInt ? std::to_string(int(tmpNum.value)) : to_double_string(tmpNum.value))<< std::endl;
	} else if (tmpStr.compare("div") == 0) {
		struct number x,y,tmpNum;
		x = calc.front();
		calc.pop_front();
		y = calc.front();
		calc.pop_front();
		tmpNum.isInt = x.isInt && y.isInt;
		tmpNum.value = x.value / y.value;
		calc.push_front(tmpNum);
		std::cout << (x.isInt ? std::to_string(int(x.value)) : to_double_string(x.value))
			<< " / " << (y.isInt ? std::to_string(int(y.value)) : to_double_string(y.value))
				<< " = " << (tmpNum.isInt ? std::to_string(int(tmpNum.value)) : to_double_string(tmpNum.value)) << std::endl;
	} else if (tmpStr.compare("sqrt") == 0) {
		struct number x,tmpNum;
		x = calc.front();
		calc.pop_front();
		tmpNum.isInt = x.isInt;
		tmpNum.value = sqrt(x.value);
		calc.push_front(tmpNum);
		std::cout << "sqrt " << (x.isInt ? std::to_string(int(x.value)) : to_double_string(x.value))
			<< " = " << (tmpNum.isInt ? std::to_string(int(tmpNum.value)) : to_double_string(tmpNum.value)) << std::endl;
	} else if (tmpStr.compare("pop") == 0) {
		struct number tmpNum;
		tmpNum = calc.front();
		calc.pop_front();
		if (DEBUG) std::cout << "popping off: " << tmpNum.value << std::endl;
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

		if (DEBUG) std::cout << "reversing" << std::endl;
		
	} else if (tmpStr.compare("repeat") == 0) {
		struct number tmpNum;
		tmpNum = calc.front();
		calc.pop_front();
		int repeat = tmpNum.value;
		std::string nextCommands = "";
		if (DEBUG || REPEAT) std::cout << "repeating " << tmpNum.value << " times " << std::endl;

		int innerRepeats = 0;
		bool innerRepeatFound = false;
		while (in >> tmpStr) {
			if (DEBUG || REPEAT) std::cout << "repeat received " << tmpStr << std::endl;
			if (tmpStr.compare("repeat") == 0) {
				++innerRepeats;
				innerRepeatFound = true;
			} else if (tmpStr.compare("endrepeat") == 0) {
				if (innerRepeats > 0) {
					if (DEBUG || REPEAT) std::cout << "found inner repeat " << innerRepeats << std::endl;
					--innerRepeats;
				} else {
					if (DEBUG || REPEAT) std::cout << "begin repeat: " << nextCommands << std::endl;
					for (int i=0;i<repeat;++i) {
						splitLine(nextCommands);
					}
					if (DEBUG || REPEAT) std::cout << "end repeat"<< std::endl;
					break;
				}
			} else {
				nextCommands += tmpStr + " ";
			} 
		}
		
	} else {
		std::cout << "|" << tmpStr << "|" << " not recognised" << std::endl;
	}
}

std::string to_double_string (double d) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(3) << d;
	return ss.str();
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





