#include "GameEngine.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <string>

class LvlFileFormatter : public std::ifstream{

	std::string* fileText = nullptr;
public:
	LvlFileFormatter(std::string filePath) : std::ifstream(filePath){
		std::string sceneText;

		if (!this->is_open())
			return;

		std::ifstream* streamReader = (std::ifstream*)this;

		std::string sceneLine;
		while (std::getline(*streamReader, sceneLine)) {
			sceneText += sceneLine;
		}

		//change all forward slashes which arn't prepended by a < or appended by a > to backslashes
		//otherwise the rapid xml reader breaks
		for (int i = 3; i < sceneText.length() - 1; ++i) {
			char previous = sceneText[i - 1];
			char thisOne = sceneText[i];
			char next = sceneText[i + 1];

			if (thisOne != '/')
				continue;

			if (previous == '<' || next == '>')
				continue;

			sceneText[i] = '\\';
		}

		this->fileText = new std::string;
		*this->fileText = sceneText;
	}

	~LvlFileFormatter() {
		if (this->fileText != nullptr)
			delete this->fileText;
	}

	std::string getFileText();
};

