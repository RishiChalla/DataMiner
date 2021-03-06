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

#include <iostream>
#include <sstream>
#include <Logger/Logger.hpp>
#include <Task/Task.hpp>

using namespace DataMiner;

/**
 * Main function of the program
 */
int main(int argc, const char* argv[]) {
	// Begin the program
    std::cout << "Beginning Data Miner Program" << std::endl;

	// Get the name and location of the output file
	std::cout << "Please input the relative name and path of a log file (include the file extension)" << std::endl;
	std::string logFile;
	std::cin >> logFile;

	// Create the logger
	Logger logger(logFile.c_str());
	DataMiner::logger = &logger;

	// Perform data mining tasks
	bool shouldContinue = true;
	while (shouldContinue) {
		Task task;
		task.run();

		std::string inp = logger.getInput<std::string>("Do you have another Data Mining task to perform? (Y/N)", [](const std::string& val) {
			return val == "Y" || val == "N";
		});
		shouldContinue = inp == "Y";
	}

	// Exit the program after all tasks are complete
	std::cout << "Type anything to close the program." << std::endl;
	std::string end;
	std::cin >> end;
	std::cout << end;
}
