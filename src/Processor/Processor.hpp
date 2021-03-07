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

#include <Data/Data.hpp>
#include <map>

/**
 * Main data mining namespace
 */
namespace DataMiner {
		
	/**
	 * Main Processor abstract class, all other processors must inherit from this
	 */
	class Processor {
	public:
		/**
		 * Creates a new processor
		 */
		Processor();

		/**
		 * Creates a processor given a dataset to train on
		 * 
		 * @throws A string with a description of why the task failed
		 * @param dataset The dataset to train on
		 */
		virtual void createProcessor(const Data& dataset) = 0;

		/**
		 * Loads a processor given a file a previous processor of the same type was saved to
		 *
		 * @throws A string with a description of why the task failed
		 * @param filename The file the processor was saved to
		 */
		virtual void loadProcessor(const char* filename) = 0;

		/**
		 * Saves the processor to a file
		 * 
		 * @throws A string with a description of why the task failed
		 * @param filename A file name to save the processor to
		 */
		virtual void saveProcessor(const char* filename) = 0;

		/**
		 * Predicts a categorical variable based on a sample row
		 * 
		 * @throws A string with a description of why the task failed
		 * @param sampleRow the sample row to predict
		 */
		virtual std::string predictCategorical(const DataRow& sampleRow) = 0;

		/**
		 * Predicts a numerical variable based on a sample row
		 * 
		 * @throws A string with a description of why the task failed
		 * @param sampleRow the sample row to predict
		 */
		virtual double predictNumerical(const DataRow& sampleRow) = 0;
	};
}