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

#include <map>
#include <string>

/**
 * Main data mining namespace
 */
namespace DataMiner {

	/**
	 * Types of task actions available
	 */
	enum class TaskAction {
		createModel,
		loadModel
	};
	
	/**
	 * Main Task class
	 */
	class Task {
	private:
		/**
		 * All available tasks with descriptions
		 */
		std::map<TaskAction, std::string> taskActions;

		/**
		 * The action this task is performing
		 */
		TaskAction taskAction;

	public:

		/**
		 * Creates a new task
		 */
		Task();

		/**
		 * Getter method for available task actions
		 */
		const std::map<TaskAction, std::string>& getTaskActions();

		/**
		 * Runs the task
		 */
		void run();
	};
}