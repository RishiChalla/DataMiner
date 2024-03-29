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

#include <Algorithms/DecisionTree/DecisionTree.hpp>

/**
 * Main data mining namespace
 */
namespace DataMiner::Algorithm {
		
	/**
	 * Algorithm for decision trees
	 */
	class DecisionTreeChiSquare : public DecisionTree {
	public:

		/**
		 * Create a decision tree based using the Chi Squared splitting method
		 * 
		 * @throws A string description of why the process failed
		 * @param dataset The dataset to use for the creation of the tree
		 */
		void createDecisionTree(const Data& dataset);
	};
}