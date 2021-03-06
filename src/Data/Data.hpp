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

#include <fstream>
#include <vector>

/**
 * Main data mining namespace
 */
namespace DataMiner {

	/**
	 * All supported base data types in datasets for mining
	 */
	enum class DataType {
		number,
		string
	};

	/**
	 * The role of the data in a dataset
	 */
	enum class DataRole {
		target,
		feature,
		excluded
	};

	/**
	 * Represents a column in the dataset
	 */
	struct DataColumn {
		/**
		 * The type of the data in the column
		 */
		DataType type;

		/**
		 * The name of the column
		 */
		std::string name;

		/**
		 * The role of the column
		 */
		DataRole role;

		/**
		 * Creates a new data column
		 */
		DataColumn() : type(DataType::string), name(""), role(DataRole::feature) {}

		/**
		 * Creates a new data column given the type
		 * 
		 * @param type The data type
		 */
		DataColumn(DataType type) : type(type), name(""), role(DataRole::feature) {}

		/**
		 * Creates a new data column given the type, and role
		 * 
		 * @param type The data type
		 * @param role The role of the column
		 */
		DataColumn(DataType type, DataRole role) : type(type), name(""), role(role) {}

		/**
		 * Creates a new data column given the name, type, and role
		 * 
		 * @param type The data type
		 * @param name The name of the column
		 * @param role The role of the column
		 */
		DataColumn(DataType type, const char* name, DataRole role) : type(type), name(name), role(role) {}

		/**
		 * Checks if the struct is equal to another
		 */
		bool operator==(const DataColumn& other) {
			return other.name == name && other.role == role && other.type == type;
		}

		/**
		 * Checks if the struct is not equal to another
		 */
		bool operator!=(const DataColumn& other) {
			return other.name != name || other.role != role || other.type != type;
		}
	};
	
	/**
	 * Represents a dataset to train/test on
	 */
	class Data {
	private:

		/**
		 * The file storing the dataset
		 */
		std::ifstream file;

		/**
		 * The actual data of the dataset
		 */
		void* data;

		/**
		 * The number of rows in the dataset
		 */
		size_t nrows;

		/**
		 * The number of columns in the dataset
		 */
		size_t ncols;

		/**
		 * The types of data in each column
		 */
		std::vector<DataColumn> cols;

		/**
		 * Loads a csv file (assumes .csv file extension)
		 * 
		 * @throws A string with a description of why the process failed
		 */
		void loadCsv(const char* filename);

	public:
		/**
		 * Creates a new dataset
		 * 
		 * @throws A string with a description of why the process failed
		 */
		Data();

		/**
		 * Creates a new dataset given the filename and type
		 * 
		 * @throws A string with a description of why the process failed
		 * @param filename The file name
		 */
		Data(const char* filename);

		/**
		 * Frees resources
		 */
		~Data();

		/**
		 * Attempts to open a file
		 * 
		 * @throws A string with a description of why the process failed
		 * @param filename The name of the file
		 * @returns Whether or not the operation was successful
		 */
		bool openFile(const char* filename);

		/**
		 * Returns a column
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @returns The column
		 */
		const DataColumn& getColumn(const char* column) const;

		/**
		 * Returns numeric data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		const double& getNumber(const char* column, int row) const;

		/**
		 * Returns string data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		const std::string& getString(const char* column, int row) const;

		/**
		 * Returns a column
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @returns The column
		 */
		const DataColumn& getColumn(size_t column) const;

		/**
		 * Returns numeric data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		const double& getNumber(size_t column, int row) const;

		/**
		 * Returns string data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		const std::string& getString(size_t column, int row) const;

		/**
		 * Returns a column
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @returns The column
		 */
		DataColumn& getColumn(const char* column);

		/**
		 * Returns numeric data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		double& getNumber(const char* column, int row);

		/**
		 * Returns string data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		std::string& getString(const char* column, int row);

		/**
		 * Returns a column
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @returns The column
		 */
		DataColumn& getColumn(size_t column);

		/**
		 * Returns numeric data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		double& getNumber(size_t column, int row);

		/**
		 * Returns string data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		std::string& getString(size_t column, int row);

		/**
		 * Sets the target of the dataset to a column
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column the column to set as the target
		 */
		void setTarget(size_t column);

		/**
		 * Sets the target of the dataset to a column
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column the column to set as the target
		 */
		void setTarget(const char* column);

		/**
		 * Sets the target of the dataset to a column
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column the column to set as the target
		 */
		void setTarget(DataColumn& column);
	};
}