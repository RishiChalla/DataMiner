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
#include <Processor/Processors.hpp>
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
	std::vector<std::string> algorithms;
	{
		std::stringstream str;
		str << "List of all available processors (Please see readme.md for instructions to add custom processors):" << std::endl;
		unsigned int counter = 0;

		for (auto i = ProcessorList.begin(); i != ProcessorList.end(); i++) {
			counter++;
			str << counter << ". " << i->first << std::endl;
			algorithms.push_back(i->first);
		}

		logger->info(str.str().c_str());
	}

	int algorithm = logger->getInput<int>("Please choose which processor to create a model from. (Input a number)", [](const int& value, void* ctx) {
		const std::vector<std::string>& algorithms = *((std::vector<std::string>*) ctx);
		return value > 0 && value <= algorithms.size();
	}, &algorithms);
	

	try {
		Processor* processor = ProcessorList[algorithms[algorithm - 1]]();

		if (taskAction == TaskAction::createModel) {
			logger->print("Now beginning model creation task, to proceed you must open a dataset to train from");
			const Data dataset;
			processor->createProcessor(dataset);

			bool save = logger->getInput<std::string>("Would you like to save the data processor? (Y/N)", [](const std::string& value) {
				return value == "Y" || value == "N";
			}) == "Y";

			if (save) {
				std::string fileName = logger->getInput<std::string>("Please input the name of the file to save the processor to (include extensions)");
				processor->saveProcessor(fileName.c_str());
			}
		}
		if (taskAction == TaskAction::loadModel) {
			logger->print("To use a model for predictions you must import a dataset containing all colummns and data for predictions (You can leave the target column blank on all rows)");
			const Data dataset;

			logger->print("Now beginning model loading task, to proceed you must open a file to which the processor previously saved to");
			std::string fileName = logger->getInput<std::string>("Please input the name of the file to which the processor was previously saved to");
			processor->loadProcessor(dataset, fileName.c_str());

			DataType targetType = dataset.getTarget().type;

			// TODO - Prediction process
			logger->info("Now showing all predictions:");
			for (size_t row = 0; row < dataset.numRows(); row++) {
				std::stringstream str;
				str << "Row " << (row + 1) << " -> ";

				if (targetType == DataType::number)
					str << processor->predictNumerical(dataset.getRow(row));
				if (targetType == DataType::string)
					str << processor->predictCategorical(dataset.getRow(row));

				logger->print(str.str().c_str());
			}
		}

		delete processor;
	}
	catch (const char* error) {
		std::stringstream errStream;
		errStream << "Operation ended with the following error: " << error;
		logger->error(errStream.str().c_str());
		logger->print("Now ending Data Mining Task due to an error.");
		return;
	}

	logger->print("Data Mining Task has ended successfully.");
}