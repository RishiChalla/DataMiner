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

#include "Task.hpp"
#include <Logger/Logger.hpp>
#include <Data/Data.hpp>
#include <sstream>

using namespace DataMiner;

/**
 * Creates a new task
 */
DataMiner::Task::Task() {
	taskActions[TaskAction::createModel] = "Creates a new processor/model given a dataset";
	taskActions[TaskAction::loadModel] = "Applies a processor/model on an existing dataset";

	int counter = 1;
	logger->info("Available Task Actions:");
	TaskAction* actions = (TaskAction*) alloca(sizeof(TaskAction) * taskActions.size());

	for (auto i = taskActions.begin(); i != taskActions.end(); i++) {
		std::stringstream stream;
		stream << counter << ". " << i->second;
		logger->print(stream.str().c_str());
		actions[counter - 1] = i->first;
		counter++;
	}

	int action = logger->getInput<int>("Please choose an action to perform for this task (Input a number):", [](const int& value, void* selfPtr) {
		Task* self = (Task*) selfPtr;
		return value >= 1 && value <= self->getTaskActions().size();
	}, this);

	taskAction = actions[action - 1];
}

const std::map<TaskAction, std::string>& DataMiner::Task::getTaskActions() {
	return taskActions;
}

/**
 * Runs the task
 */
void DataMiner::Task::run() {
	logger->print("Now beginning Data Mining Task, to proceed you must open a dataset.");

	try {
		const Data dataset;
	}
	catch (const char* error) {
		std::stringstream errStream;
		errStream << "Operation ended with the following error: " << error;
		logger->error(errStream.str().c_str());
		logger->print("Now ending Data Mining Task due to an error.");
		return;
	}
}