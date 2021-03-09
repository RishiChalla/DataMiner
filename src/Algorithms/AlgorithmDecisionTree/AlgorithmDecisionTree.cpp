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

using namespace DataMiner;

/**
 * Creates a new decision tree algorithm
 */
DataMiner::AlgorithmDecisionTree::AlgorithmDecisionTree() {

}

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
 * @param filename The file the processor was saved to
 */
void DataMiner::AlgorithmDecisionTree::loadProcessor(const char* filename) {
	throw "Unimplemented Error";
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
	throw "Unimplemented Error";
}

/**
 * Predicts a numerical variable based on a sample row
 * 
 * @throws A string with a description of why the task failed
 * @param sampleRow the sample row to predict
 */
double DataMiner::AlgorithmDecisionTree::predictNumerical(const DataRow& sampleRow) {
	throw "Unimplemented Error";
}