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
	 * Represents a single row inside of a dataset
	 */
	struct DataRow {
	private:

		/**
		 * The string data of the row
		 */
		std::vector<const std::string*> strData;

		/**
		 * The numerical data of the row
		 */
		std::vector<const double*> numData;

		/**
		 * The columns of the dataset (direct reference)
		 */
		const std::vector<DataColumn>& columns;

		/**
		 * Creates a new data row given the columns
		 * 
		 * @param columns A reference to the columns of the dataset
		 */
		DataRow(const std::vector<DataColumn>& columns);

		/**
		 * Creates a new data row given the columns and data
		 * 
		 * @param columns A reference to the columns of the dataset
		 * @param strData The string data of the row
		 * @param numData The numerical data of the row
		 */
		DataRow(const std::vector<DataColumn>& columns, const std::vector<const std::string*>& strData, const std::vector<const double*>& numData);
	
	public:

		/**
		 * Gets string data from the row
		 * 
		 * @throws A string representing why the operation failed
		 * @param column The column to retrieve from
		 */
		const std::string& getString(const char* column) const;

		/**
		 * Gets string data from the row
		 * 
		 * @throws A string representing why the operation failed
		 * @param column The column to retrieve from
		 */
		const std::string& getString(size_t column) const;

		/**
		 * Gets string data from the row
		 * 
		 * @throws A string representing why the operation failed
		 * @param column The column to retrieve from
		 */
		const std::string& getString(const DataColumn& column) const;

		/**
		 * Gets double data from the row
		 * 
		 * @throws A string representing why the operation failed
		 * @param column The column to retrieve from
		 */
		const double& getDouble(const char* column) const;

		/**
		 * Gets double data from the row
		 * 
		 * @throws A string representing why the operation failed
		 * @param column The column to retrieve from
		 */
		const double& getDouble(size_t column) const;

		/**
		 * Gets double data from the row
		 * 
		 * @throws A string representing why the operation failed
		 * @param column The column to retrieve from
		 */
		const double& getDouble(const DataColumn& column) const;

		friend class Data;
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
		 * Stores strings from the dataset
		 */
		std::string* strData;

		/**
		 * Stores numeric values from the dataset
		 */
		double* numData;

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
		
		/**
		 * Gets the index of the column within the specific datatype (useful for retrieving data)
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column whose index to retrieve
		 * @returns The index of the column within the datatype
		 */
		size_t getIndex(const char* column) const;
		
		/**
		 * Gets the index of the column within the specific datatype (useful for retrieving data)
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column whose index to retrieve
		 * @returns The index of the column within the datatype
		 */
		size_t getIndex(size_t column) const;
		
		/**
		 * Gets the index of the column within the specific datatype (useful for retrieving data)
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column whose index to retrieve
		 * @returns The index of the column within the datatype
		 */
		size_t getIndex(const DataColumn& column) const;

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
		const double& getNumber(const char* column, size_t row) const;

		/**
		 * Returns string data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		const std::string& getString(const char* column, size_t row) const;

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
		const double& getNumber(size_t column, size_t row) const;

		/**
		 * Returns string data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		const std::string& getString(size_t column, size_t row) const;

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
		double& getNumber(const char* column, size_t row);

		/**
		 * Returns string data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		std::string& getString(const char* column, size_t row);

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
		double& getNumber(size_t column, size_t row);

		/**
		 * Returns string data from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param column The column name
		 * @param row The row number
		 * @returns The data
		 */
		std::string& getString(size_t column, size_t row);

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

		/**
		 * Returns a row from the dataset
		 * 
		 * @throws A string with a description of why the process failed
		 * @param row The row to return
		 * @returns The row
		 */
		const DataRow getRow(size_t row) const;

		/**
		 * Returns the count of columns in the dataset
		 * 
		 * @returns The count of columns in the dataset
		 */
		size_t numColumns() const {
			return ncols;
		}

		/**
		 * Returns the count of rows in the dataset
		 * 
		 * @returns The count of rows in the dataset
		 */
		size_t numRows() const {
			return nrows;
		}
	};
}