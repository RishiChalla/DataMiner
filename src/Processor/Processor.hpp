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

/**
 * Main data mining namespace
 */
namespace DataMiner {
	
	/**
	 * Main Processor abstract class, all other processors must inherit from this
	 */
	class Processor {
	private:

	protected:
		const Data& dataset;
	public:
		Processor(const Data& dataset);
		Processor(const char* filename);
		~Processor();
		virtual void saveProcessor(const char* filename) = 0;
		virtual std::string predictCategorical() = 0;
		virtual double predictNumerical() = 0;
	};
}