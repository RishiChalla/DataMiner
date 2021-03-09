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

#include "AlgorithmDecisionTree.hpp"
#include <Logger/Logger.hpp>
#include <sstream>

using namespace DataMiner;

/**
 * Helper function to get double from a string
 * 
 * @throws An error description string if the string couldn't be converted
 */
static double getDouble(const std::string& str) {
	try {
		return std::stod(str);
	}
	catch (const std::invalid_argument&) {
		throw "Unable to convert string to a numeric value";
	}
	catch (const std::out_of_range&) {
		throw "Unable to convert string to a numeric value";
	}
}

// -------------------------- DecisionTreeCondition --------------------------

/**
 * Tests if a value passes this condition
 * 
 * @throws A string with a description of why the task failed
 * @param value The value to check for
 * @returns Whether or not the value passes the condition
 */
bool DataMiner::AlgorithmDecisionTree::DecisionTreeCondition::testCondition(const double& value) const {
	if (conditionColumn.type == DataType::string)
		throw "Invalid Data Type Error (Condition column is a string, number given)";
	
	return numValue == value;
}

/**
 * Tests if a value passes this condition
 * 
 * @throws A string with a description of why the task failed
 * @param value The value to check for
 * @returns Whether or not the value passes the condition
 */
bool DataMiner::AlgorithmDecisionTree::DecisionTreeCondition::testCondition(const std::string& value) const {
	if (conditionColumn.type == DataType::number)
		throw "Invalid Data Type Error (Condition column is a number, string given)";
	
	return strValue == value;
}

// -------------------------- DecisionTreeRule --------------------------

/**
 * Checks whether a row satisfies all conditions in this rule
 * 
 * @throws A string with a description of why the task failed
 * @param row The row to check for (target column's data will not be checked)
 * @returns Whether or not the row satisfies all conditions in this rule
 */
bool DataMiner::AlgorithmDecisionTree::DecisionTreeRule::satisfiesConditions(const DataRow& row) const {
	for (const DataColumn* col : columns) {
		if (col->role != DataRole::feature)
			continue;
		
		for (const DecisionTreeCondition& condition : conditions) {
			if (&(condition.conditionColumn) == col) {
				if (col->type == DataType::number && !condition.testCondition(row.getNumber(condition.conditionColumn)))
					return false;
				if (col->type == DataType::string && !condition.testCondition(row.getString(condition.conditionColumn)))
					return false;
			}
		}
	}

	return true;
}

// -------------------------- AlgorithmDecisionTree --------------------------

/**
 * Creates a processor given a dataset to train on
 * 
 * @throws A string with a description of why the task failed
 * @param dataset The dataset to train on
 */
void DataMiner::AlgorithmDecisionTree::createProcessor(const Data& dataset) {
	throw "Unimplemented Error";
}

/**
 * Loads a processor given a file a previous processor of the same type was saved to
 *
 * @throws A string with a description of why the task failed
 * @param dataset A dataset containing all appropriate columns
 * @param filename The file the processor was saved to
 */
void DataMiner::AlgorithmDecisionTree::loadProcessor(const Data& dataset, const char* filename) {
	columns.reserve(dataset.numColumns());
	const DataColumn* targetColumn = nullptr;
	for (size_t i = 0; i < dataset.numColumns(); i++) {
		columns.push_back(&dataset.getColumn(i));
		if (columns[i]->role == DataRole::target) targetColumn = columns[i];
	}

	if (targetColumn == nullptr)
		throw "Target column not found in dataset";

	std::ifstream file(filename);

	if (!file.is_open())
		throw "Unable to open file";
	
	std::string line;
	while (std::getline(file, line)) {
		// Create rule object
		rules.emplace_back(*targetColumn, columns);
		DecisionTreeRule& rule = rules[rules.size() - 1];

		// Store the line into parts for easier access
		std::string part;
		std::stringstream lineStream(line);
		std::vector<std::string> parts;
		while (std::getline(lineStream, part, ' ')) {
			parts.push_back(part);
		}

		// Make sure the line has valid amounts of parts
		if (parts.size() < 5 || (parts.size() - 1) % 4 != 0)
			throw "Invalid line detected in save file";

		// Last part is always the value - set the value/output of the rule
		if (targetColumn->type == DataType::string)
			rule.strOutput = parts[parts.size() - 1];
		if (targetColumn->type == DataType::number)
			rule.numOutput = getDouble(parts[parts.size() - 1]);

		// We don't need the last 2 parts since thats only for the output - get rid of them
		parts.erase(parts.end() - 2, parts.end());

		// Loop through the rest of the parts and generate conditions
		for (size_t i = 0; i < parts.size(); i += 4) {
			const std::string& column = parts[i];
			const std::string& value = parts[i + 2];
			const DataColumn& conditionColumn = dataset.getColumn(column.c_str());

			rule.conditions.emplace_back(DecisionTreeCondition(conditionColumn));
			DecisionTreeCondition& condition = rule.conditions[rule.conditions.size() - 1];

			if (conditionColumn.type == DataType::string) {
				condition.strValue = value;
			}
			if (conditionColumn.type == DataType::number) {
				condition.numValue = getDouble(value);
			}
		}
	}

	logger->info("Decision Tree successfully imported");
}

/**
 * Saves the processor to a file
 * 
 * @throws A string with a description of why the task failed
 * @param filename A file name to save the processor to
 */
void DataMiner::AlgorithmDecisionTree::saveProcessor(const char* filename) {
	throw "Unimplemented Error";
}

/**
 * Predicts a categorical variable based on a sample row
 * 
 * @throws A string with a description of why the task failed
 * @param sampleRow the sample row to predict
 */
std::string DataMiner::AlgorithmDecisionTree::predictCategorical(const DataRow& sampleRow) {
	for (const DecisionTreeRule& rule : rules)
		if (rule.satisfiesConditions(sampleRow))
			return rule.strOutput;
	
	throw "Unable to create prediction for sample row (Invalid case - this usually happens when a variable outside the domain of the training set appears)";
}

/**
 * Predicts a numerical variable based on a sample row
 * 
 * @throws A string with a description of why the task failed
 * @param sampleRow the sample row to predict
 */
double DataMiner::AlgorithmDecisionTree::predictNumerical(const DataRow& sampleRow) {
	for (const DecisionTreeRule& rule : rules)
		if (rule.satisfiesConditions(sampleRow))
			return rule.numOutput;

	throw "Unable to create prediction for sample row (Invalid case - this usually happens when a variable outside the domain of the training set appears)";
}