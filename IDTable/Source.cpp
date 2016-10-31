#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <iterator>
#include <exception>
#include <regex>

using namespace std;

class InputException : public exception {
	virtual const char * what() const throw() {
		return "Invalid input!";
	}
};

class Statement {
public:
	Statement() {}

	Statement(string ID, string type, string value) {
		this->ID.push_back(ID);
		this->type = type;
		this->value = value;
	}

	virtual ~Statement() {
		this->ID.clear();
		this->value.clear();
		this->type.clear();
	}

	void print() {
		cout << this->type << " " << this->ID.at(0) << " = " << this->value << endl;
		cout << "Linked: ";
		for (int i = 0; i < this->ID.size(); i++) {
			cout << this->ID.at(i);
			if (i != this->ID.size() - 1)
				cout << ", ";
		}
		cout << endl;
	}

	string getID() {
		return this->ID.at(0);
	}

	string getValue() {
		return this->value;
	}

	bool hasID(string name) {
		int size = this->ID.size();
		for (int i = 0; i < size; i++) {
			if (this->ID.at(i) == name)
				return true;
		}
		size = others.size();
		for (int i = 0; i < size; i++) {
			if (this->others.at(i).hasID(name))
				return true;
		}
		return false;
	}

	void setValue(string newValue) {
		this->value = newValue;
	}

	void addLink(string ID) {
		this->ID.push_back(ID);
	}

	bool hasLinks() {
		return ID.size() > 0;
	}

	void deleteLink(string link) {
		for (int i = 0; i <= ID.size(); i++) {
			if (ID.at(i) == link)
				ID.erase(ID.begin() + i);
		}
	}

	void addToOthers(Statement * temp) {
		others.push_back(*temp);
	}

	void deleteOther(string name) {
		for (int i = 0; i <= others.size(); i++) {
			if (others.at(i).hasID(name)) {
				others.erase(others.begin() + i);
				break;
			}
		}
	}

	void addOtherName(string name, string add) {
		for (int i = 0; i <= others.size(); i++) {
			if (others.at(i).hasID(name)) {
				others.at(i).addLink(add);
				break;
			}
		}
	}

	void printOther(string name) {
		for (int i = 0; i <= others.size(); i++) {
			if (others.at(i).hasID(name)) {
				others.at(i).print();
				break;
			}
		}
	}

	string getOtherValue(string name) {
		for (int i = 0; i <= others.size(); i++) {
			if (others.at(i).hasID(name)) {
				return others.at(i).getValue();
			}
		}
	}

	void setOtherValue(string name, string value) {
		for (int i = 0; i <= others.size(); i++) {
			if (others.at(i).hasID(name)) {
				others.at(i).setValue(value);
				break;
			}
		}
	}

private:
	vector <string> ID;
	string type;
	string value;
	vector <Statement> others;
};

class IDTable {
private:
	int hashcode(string name) {
		int code = name.at(0) * 3;
		code *= name.length() * 8;
		code += name.back() * 17;
		return code;
	}
	Statement * elements[1000000];
public:
	void addElement(string name, string type, string value);
	void editElement(string name, string newValue);
	void deleteElement(string name);
	bool searchElement(string name);
	string getValue(string name);
};

void IDTable::addElement(string name, string type, string value) {
	int hash = hashcode(name);
	int valueHash = hashcode(value);
	Statement * temp = new Statement(name, type, value);
	if (this->elements[valueHash]) {
		if (this->elements[valueHash]->hasID(value))
			if (this->elements[valueHash]->getID() == value)
				this->elements[valueHash]->addLink(name);
			else
				this->elements[valueHash]->addOtherName(value, name);
	}
	if (this->elements[hash]) {
		if (this->elements[hash]->getID() == name)
			elements[hash]->setValue(value);
		else if (this->elements[hash]->hasID(name))
			this->elements[hash]->setOtherValue(name, value);
		else
			this->elements[hash]->addToOthers(temp);
	}
	else
		this->elements[hash] = temp;
	
	
}

void IDTable::editElement(string name, string newValue) {
	int hash = hashcode(name);
	if (this->elements[hash]) {
		if (this->elements[hash]->hasID(name)) {
			if (this->elements[hash]->getID() == name)
				this->elements[hash]->setValue(newValue);
			else
				this->elements[hash]->setOtherValue(name, newValue);
		}
	}
	else {
		cout << "Element not found." << endl;
	}
}

void IDTable::deleteElement(string name) {
	int hash = hashcode(name);
	if (this->elements[hash]) {
		if (this->elements[hash]->hasID(name)) {
			if (this->elements[hash]->getID() == name && !this->elements[hash]->hasLinks())
				delete this->elements[hash];
			else if (!this->elements[hash]->hasLinks())
				this->elements[hash]->deleteOther(name);
			else
				this->elements[hash]->deleteLink(name);
			cout << "Element deleted." << endl;
		}
		else
			cout << "Element not found." << endl;
	}
	else {
		cout << "Element not found." << endl;
	}
}

bool IDTable::searchElement(string name) {
	int hash = hashcode(name);
	if (this->elements[hash]) {
		if (this->elements[hash]->hasID(name)) {
			if (this->elements[hash]->getID() == name) {
				this->elements[hash]->print();
				return true;
			}
			else {
				this->elements[hash]->printOther(name);
				return true;
			}
		}
		else {
			return false;
		}
	}
	return false;
}

string IDTable::getValue(string name) {
	int hash = hashcode(name);
	if (this->elements[hash]) {
		if (this->elements[hash]->hasID(name)) {
			if (this->elements[hash]->getID() == name)
				return this->elements[hash]->getValue();
			else
				return this->elements[hash]->getOtherValue(name);
		}
	}
	throw new exception;
}

int adding() {
	IDTable * table = new IDTable();
	while (true) {
		char key;
		cout << "Welcome! Please choose an option: \n"
			"1.Add an element.\n"
			"2.Edit an element.\n"
			"3.Delete an element.\n"
			"4.Search for element.\n"
			"0.Exit program." << endl;
		cin >> key;
		switch (key) {
		case '1': {
			string type, name, value;
			cout << "Enter type: ";
			cin >> type;
			cout << "Enter name: ";
			cin >> name;
			cout << "Enter value: ";
			cin >> value;
			table->addElement(name, type, value);
			cout << "Element added." << endl;
			break;
		}
		case '2': {
			string name, newValue;
			cout << "Enter name: " << endl;
			cin >> name;
			cout << "Enter new value: " << endl;
			cin >> newValue;
			table->editElement(name, newValue);
			break;
		}
		case '3': {
			string name;
			cout << "Enter name: ";
			cin >> name;
			table->deleteElement(name);
			break;
		}
		case '4': {
			string name;
			cout << "Enter name: ";
			cin >> name;
			table->searchElement(name);
			break;
		}
		case '0':
			return 0;
		default:
			break;
		}
	}
}

vector<string> split(string str) {
	istringstream iss(str);
	istringstream space(" ");
	vector<string> tokens{ istream_iterator<string>{iss}, istream_iterator<string>{space} };
	return tokens;
}

bool isColor(string str) {
	return false;
}

bool isDomain(string str) {
	return false;
}

string result(vector<string> tokens, IDTable * vars, stack <string> * elements) {
	string temp;
	float rvalue;
	float lvalue;
	for each (string token in tokens) {
		if (token == "+") {
			if (vars->searchElement(elements->top()))
				rvalue = atof(vars->getValue(elements->top()).c_str());
			else
				rvalue = atof(elements->top().c_str());
			elements->pop();
			if (vars->searchElement(elements->top()))
				lvalue = atof(vars->getValue(elements->top()).c_str());
			else
				lvalue = atof(elements->top().c_str());
			elements->pop();
			temp = to_string(lvalue + rvalue);
			elements->push(temp);
		}
		else if (token == "-") {
			if (vars->searchElement(elements->top()))
				rvalue = atof(vars->getValue(elements->top()).c_str());
			else
				rvalue = atof(elements->top().c_str());
			elements->pop();
			if (vars->searchElement(elements->top()))
				lvalue = atof(vars->getValue(elements->top()).c_str());
			else
				lvalue = atof(elements->top().c_str());
			elements->pop();
			temp = to_string(lvalue - rvalue);
			elements->push(temp);
		}
		else if (token == "*") {
			if (vars->searchElement(elements->top()))
				rvalue = atof(vars->getValue(elements->top()).c_str());
			else
				rvalue = atof(elements->top().c_str());
			elements->pop();
			if (vars->searchElement(elements->top()))
				lvalue = atof(vars->getValue(elements->top()).c_str());
			else
				lvalue = atof(elements->top().c_str());
			elements->pop();
			temp = to_string(lvalue * rvalue);
			elements->push(temp);
		}
		else if (token == "/") {
			if (vars->searchElement(elements->top()))
				rvalue = atof(vars->getValue(elements->top()).c_str());
			else
				rvalue = atof(elements->top().c_str());
			elements->pop();
			if (vars->searchElement(elements->top()))
				lvalue = atof(vars->getValue(elements->top()).c_str());
			else
				lvalue = atof(elements->top().c_str());
			elements->pop();
			temp = to_string(lvalue / rvalue);
			elements->push(temp);
		}
		else if (token == "=") {
			if (vars->searchElement(elements->top()))
				rvalue = atof(vars->getValue(elements->top()).c_str());
			else
				rvalue = atof(elements->top().c_str());
			elements->pop();
			if (vars->searchElement(elements->top()))
				vars->addElement(elements->top(), "float", to_string(rvalue));
			else {
				temp = elements->top();
				vars->addElement(temp, "float", to_string(rvalue));
			}
		}

		else {
			elements->push(token);
		}
	}
	return elements->top();
}

int main() {
	string input = "";
	vector <string> tokens;
	stack <string> elements;
	regex http("(*.*)");
	IDTable * vars = new IDTable();
	cout << "---==Hello. It is  stack calculator.==---\n Enter your expressions now: " << endl;
	while (input != "exit") {
		getline(cin, input);
		if (input == "print") {
			if (vars->searchElement(elements.top()))
				continue;
			cout << "Stack top: " << elements.top();
			continue;
		}
		else if (isColor(input)){
			vector<string> parsed = split(input);
			vars->addElement(parsed[1], parsed[0], parsed[2]);
		}
		else if (vars->searchElement(input)) {
			vector<string> parsed = split(input);
			vars->addElement(parsed[1], parsed[0], parsed[2]);
			break;
		}
		else {
			tokens = split(input);
			cout << "Result: " << result(tokens, vars, &elements) << endl;
		}
	}
	return 0;
}
