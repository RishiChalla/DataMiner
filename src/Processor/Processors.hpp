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

// Include list of processors here

// Decision Trees
#include <Algorithms/DecisionTree/ChiSquare/DecisionTreeChiSquare.hpp>
#include <Algorithms/DecisionTree/GiniImpurity/DecisionTreeGiniImpurity.hpp>
#include <Algorithms/DecisionTree/InformationGain/DecisionTreeInformationGain.hpp>
#include <Algorithms/DecisionTree/VarianceReduction/DecisionTreeVarianceReduction.hpp>

/**
 * Main data mining namespace
 */
namespace DataMiner {

	/**
	 * Function to create a specific processor type and return it
	 * (Processor Must be heap allocated)
	 */
	typedef Processor* (*createProcessorFn)(void);
		
	/**
	 * Map containing all available processor algorithms
	 */
	std::map<std::string, createProcessorFn> ProcessorList = {
		{
			"Desicion Tree - Chi Square Splitting Method",
			[](void){
				return (Processor*) new Algorithm::DecisionTreeChiSquare();
			}
		},
		{
			"Desicion Tree - Gini Impurity Splitting Method",
			[](void){
				return (Processor*) new Algorithm::DecisionTreeGiniImpurity();
			}
		},
		{
			"Desicion Tree - Information Gain Splitting Method",
			[](void){
				return (Processor*) new Algorithm::DecisionTreeGiniInformationGain();
			}
		},
		{
			"Desicion Tree - Variance Reduction Splitting Method",
			[](void){
				return (Processor*) new Algorithm::DecisionTreeVarianceReduction();
			}
		}
	};
}