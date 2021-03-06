/*
   Copyright 2021 Rishi Challa

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "Logger.hpp"
#include <iostream>

using namespace DataMiner;

/**
 * Main logger of the program
 */
Logger* DataMiner::logger = nullptr;

/**
 * Creates a new logger given the log output file name (with extension/path)
 */
DataMiner::Logger::Logger(const char* logOutput) : logFile(logOutput) {
	if (!logFile.is_open()) {
		std::cout << "Aborting program due to invalid log file. Please type anything to exit the program." << std::endl;
		std::string end;
		std::cin >> end;
		std::cout << end;
		exit(0);
	}
}

/**
 * Prints a info to the console and the log file.
 * 
 * @param message The message to print
 */
void DataMiner::Logger::info(const char* message) {
	std::cout << "[Info] : " << message << std::endl;
	logFile << "[Info] : " << message << std::endl;
}

/**
 * Prints a warn to the console and the log file.
 * 
 * @param message The message to print
 */
void DataMiner::Logger::warn(const char* message) {
	std::cout << "[Warn] : " << message << std::endl;
	logFile << "[Warn] : " << message << std::endl;
}

/**
 * Prints a error to the console and the log file.
 * 
 * @param message The message to print
 */
void DataMiner::Logger::error(const char* message) {
	std::cout << "[Error] : " << message << std::endl;
	logFile << "[Error] : " << message << std::endl;
}

/**
 * Prints a message to the console and the log file.
 * 
 * @param message The message to print
 */
void DataMiner::Logger::print(const char* message) {
	std::cout << message << std::endl;
	logFile << message << std::endl;
}

/**
		 * Prints a new line to the console and the log file.
		 */
void DataMiner::Logger::println() {
	std::cout << "\n" << std::endl;
	logFile << "\n" << std::endl;
}