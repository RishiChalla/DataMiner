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

#include <Processor/Processor.hpp>

/**
 * Main data mining namespace
 */
namespace DataMiner {
		
	/**
	 * Algorithm for decision trees
	 */
	class AlgorithmDecisionTree : public Processor {
	protected:

		/**
		 * Represents a single condition in a decision tree rule
		 * 
		 * Ie Col == value
		 */
		struct DecisionTreeCondition {

			/**
			 * The column this condition checks with
			 */
			const DataColumn& conditionColumn;

			/**
			 * The numeric value used for checking the condition
			 */
			double numValue;

			/**
			 * The string value used for checking the condition
			 */
			std::string strValue;

			/**
			 * Creates a condition given the column for the condition
			 * 
			 * @param conditionColumn The column to check with
			 */
			DecisionTreeCondition(const DataColumn& conditionColumn) : conditionColumn(conditionColumn) {}

			/**
			 * Tests if a value passes this condition
			 * 
			 * @throws A string with a description of why the task failed
			 * @param value The value to check for
			 * @returns Whether or not the value passes the condition
			 */
			bool testCondition(double value);

			/**
			 * Tests if a value passes this condition
			 * 
			 * @throws A string with a description of why the task failed
			 * @param value The value to check for
			 * @returns Whether or not the value passes the condition
			 */
			bool testCondition(std::string value);
		};

		/**
		 * Represents a single decision tree rule
		 * 
		 * Ie Col1 == val1 and Col2 == val2 then numOutput/strOutput
		 */
		struct DecisionTreeRule {

			/**
			 * All conditions this rule contains
			 */
			std::vector<DecisionTreeCondition> conditions;

			/**
			 * The target column of the dataset the tree is based on
			 */
			const DataColumn& target;

			/**
			 * The numeric output for the target column if all conditions are satisfied and the target column is a string
			 */
			double numOutput;

			/**
			 * The string output for the target column if all conditions are satisfied and the target column is a string
			 */
			std::string strOutput;

			/**
			 * Creates a decision tree rule given the target column
			 * 
			 * @param target The target column of the dataset the tree is based on
			 */
			DecisionTreeRule(const DataColumn& target) : target(target) {}

			/**
			 * Checks whether a row satisfies all conditions in this rule
			 * 
			 * @throws A string with a description of why the task failed
			 * @param row The row to check for (target column's data will not be checked)
			 * @returns Whether or not the row satisfies all conditions in this rule
			 */
			bool satisfiesConditions(const DataRow& row);
		};
	
	private:
		/**
		 * The list of rules this decision tree contains
		 */
		std::vector<DecisionTreeRule> rules;

		/**
		 * The list of columns of the dataset this algorithm is connected to
		 */
		std::vector<const DataColumn*> columns;

	public:

		/**
		 * Creates a new decision tree algorithm
		 */
		AlgorithmDecisionTree();

		/**
		 * Creates a processor given a dataset to train on
		 * 
		 * @throws A string with a description of why the task failed
		 * @param dataset The dataset to train on
		 */
		void createProcessor(const Data& dataset);

		/**
		 * Loads a processor given a file a previous processor of the same type was saved to
		 *
		 * @throws A string with a description of why the task failed
		 * @param filename The file the processor was saved to
		 */
		void loadProcessor(const char* filename);

		/**
		 * Saves the processor to a file
		 * 
		 * @throws A string with a description of why the task failed
		 * @param filename A file name to save the processor to
		 */
		void saveProcessor(const char* filename);

		/**
		 * Predicts a categorical variable based on a sample row
		 * 
		 * @throws A string with a description of why the task failed
		 * @param sampleRow the sample row to predict
		 */
		std::string predictCategorical(const DataRow& sampleRow);

		/**
		 * Predicts a numerical variable based on a sample row
		 * 
		 * @throws A string with a description of why the task failed
		 * @param sampleRow the sample row to predict
		 */
		double predictNumerical(const DataRow& sampleRow);
	};
}