#pragma once
#include <string>
#include <iostream>
#include <limits>
#include <ctime>

namespace FamilyBudget {

	std::string CurrentDate();
	void ClearInputStream();
	int ReadInt(const std::string& prompt);
	std::string ReadLine(const std::string& prompt);
	std::string HashPassword(const std::string& password);
	void InputDate(std::string& date);

}