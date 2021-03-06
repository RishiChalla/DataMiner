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

#pragma once

#include <fstream>

/**
 * Main data mining namespace
 */
namespace DataMiner {
	
	/**
	 * Main logger class
	 */
	class Logger {
	private:
		/**
		 * Main Log file
		 */
		std::ofstream logFile;

	public:
		/**
		 * Creates a new logger given the log output file name (with extension/path)
		 */
		Logger(const char* logOutput);
		
		/**
		 * Prints a info to the console and the log file.
		 * 
		 * @param message The message to print
		 */
		void info(const char* message);
		
		/**
		 * Prints a warn to the console and the log file.
		 * 
		 * @param message The message to print
		 */
		void warn(const char* message);
		
		/**
		 * Prints a error to the console and the log file.
		 * 
		 * @param message The message to print
		 */
		void error(const char* message);
		
		/**
		 * Prints a message to the console and the log file.
		 * 
		 * @param message The message to print
		 */
		void print(const char* message);

		/**
		 * Gets input from the user
		 * 
		 * @returns The user's input
		 */
		template <typename T> T getInput() {
			T input;
			std::cin >> input;
			std::cout << std::endl;
			logFile << "User has inputted: `" << input << "`" << std::endl;
			return input;
		}

		/**
		 * Gets input from the user
		 * 
		 * @param inputStr A message to print before asking for the user's input
		 * @returns The user's input
		 */
		template <typename T> T getInput(const char* inputStr) {
			T input;
			print(inputStr);
			std::cin >> input;
			std::cout << std::endl;
			logFile << "User has inputted: `" << input << "`" << std::endl;
			return input;
		}
	};

	/**
	 * Main logger of the program
	 */
	Logger* logger;
}