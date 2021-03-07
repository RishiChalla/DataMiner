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

#include "Data.hpp"

using namespace DataMiner;

#include <Logger/Logger.hpp>
#include <sstream>

/**
 * Creates a new dataset
 * 
 * @throws A string explaining why the process failed
 */
DataMiner::Data::Data() {
	// Get file name and open it
	std::string filename = logger->getInput<std::string>("Please input the file name of the dataset (csv)", [](const std::string& value, void* selfPtr) {
		Data* self = (Data*) selfPtr;

		// Currently only CSVs are accepted
		if (value.substr(value.find_last_of(".") + 1) == "csv")
			return self->openFile(value.c_str());
		else
			return false;
	}, this);
	file.open(filename);

	try {
		loadCsv(filename.c_str());
	}
	catch (const char* error) {
		throw error;
	}
}

/**
 * Creates a new dataset given the filename and type
 * 
 * @throws A string explaining why the process failed
 * @param filename The file name
 */
DataMiner::Data::Data(const char* filename) : file(filename) {
	std::string filenameStr = filename;
	if (filenameStr.substr(filenameStr.find_last_of(".") + 1) != "csv")
		throw "Invalid data file type (Only csv files are currently supported)";
	if (!file.is_open())
		throw "Unable to Open File Error";
	
	try {
		loadCsv(filename);
	}
	catch (const char* error) {
		throw error;
	}
}

/**
 * Loads a csv file (assumes .csv file extension)
 * 
 * @throws A string explaining why the process failed
 */
void DataMiner::Data::loadCsv(const char* filename) {
	logger->info("This CSV reader does not support commas within fields nor does it support spaces in header names.");

	// Get number of rows and columns in the data
	nrows = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
	file.clear();
	file.seekg(0, std::ios::beg);

	if (nrows < 1) throw "Error, no rows found in table";

	std::string firstLine;
	std::getline(file, firstLine);
	ncols = std::count(firstLine.begin(), firstLine.end(), ',')+1;

	if (ncols < 1) throw "Error, no columns found in table";

	// Check if there is a header or not
	bool hasHeader = logger->getInput<std::string>("Does your dataset have a header row? (Y/N)", [](const std::string& value){
		return value == "Y" || value == "N";
	}) == "Y";

	if (!hasHeader) nrows += 1;

	std::istringstream firstLineStream(firstLine);
	if (hasHeader) {
		// There is a header, go ahead and create columns using header names
		std::string value;
		while (std::getline(firstLineStream, value, ',')) {
			cols.emplace_back(DataType::string, value.c_str(), DataRole::feature);
		}

		// Get second line/row of data for type checking
		std::string secondLine;
		std::getline(file, secondLine);
		std::istringstream secondLineStream(secondLine);

		// Go through second row for checking data types
		size_t i = 0;
		while (std::getline(secondLineStream, value, ',')) {
			if (i >= cols.size())
				throw "Second row has a larger row count than header row";
			if (value.find_first_not_of(".-1234567890") == std::string::npos)
				cols[i].type = DataType::number;
			else
				cols[i].type = DataType::string;
			i++;
		}
	}
	else {
		// There is no header, create columns without header names
		std::string value;
		while (std::getline(firstLineStream, value, ',')) {
			DataType dataType = DataType::string;
			if (value.find_first_not_of(".-1234567890") == std::string::npos)
				dataType = DataType::number;
			
			cols.emplace_back(dataType, "", DataRole::feature);
		}
	}

	// Prompt to change data roles
	bool changeRoles = logger->getInput<std::string>("Would you like to change the data roles of any of your columns? IE: Drop columns/add a target. (Y/N)", [](const std::string& value){
		return value == "Y" || value == "N";
	}) == "Y";

	while (changeRoles) {
		DataColumn* column = nullptr;

		if (hasHeader) {
			std::string columnName = logger->getInput<std::string>("What is the name of the column you'd like to change the role of?");
			try {
				column = &getColumn(columnName.c_str());
			}
			catch (const char*) {
				logger->print("We were unable to find that column.");
				continue;
			}
		}
		else {
			size_t columnId = logger->getInput<size_t>("What is the number of the column you'd like to change the role of (columns start at 1)?");
			try {
				column = &getColumn(columnId - 1);
			}
			catch (const char*) {
				logger->print("We were unable to find that column.");
				continue;
			}
		}

		if (column == nullptr) {
			logger->print("We were unable to find that column.");
			continue;
		}

		std::stringstream str;
		str << "Current role of column is ";
		if (column->role == DataRole::excluded) str << "excluded.";
		if (column->role == DataRole::target) str << "target.";
		if (column->role == DataRole::feature) str << "feature.";
		logger->info(str.str().c_str());

		std::vector<std::string> acceptableValues = {"feature", "excluded", "target"};
		std::string role = logger->getInput<std::string>("What would you like the role of the column to be? (feature/excluded/target)", acceptableValues);

		try {
			if (role == "target") setTarget(*column);
			if (role == "feature") column->role = DataRole::feature;
			if (role == "excluded") column->role = DataRole::excluded;
			logger->print("Role has successfully been changed");
		}
		catch (const char* error) {
			logger->error(error);
		}

		changeRoles = logger->getInput<std::string>("Would you like to change the data roles of any other columns? IE: Drop columns/add a target. (Y/N)", [](const std::string& value){
			return value == "Y" || value == "N";
		}) == "Y";
	}

	{
		std::stringstream str;
		logger->println();

		str << "Current columns are as follows:" << std::endl;
		int counter = 1;
		for (const DataColumn& col : cols) {
			std::string colRole("Feature");

			if (col.role == DataRole::target) colRole = "Target";
			if (col.role == DataRole::excluded) colRole = "Excluded";

			str << counter << ". (Name: " << col.name << ", Type: " << (col.type == DataType::number ? "Number" : "String")
				<< ", Role: " << colRole << ")" << std::endl;
			counter++;
		}

		logger->info(str.str().c_str());
	}

	// Allocate sufficient data on the heap
	size_t dataStrSize = 0;
	size_t dataNumSize = 0;
	for (DataColumn colType : cols) {
		if (colType.type == DataType::number)
			dataNumSize += nrows;
		if (colType.type == DataType::string)
			dataStrSize += nrows;
	}
	strData = new std::string[dataStrSize];
	numData = new double[dataNumSize];

	std::stringstream str;
	str << "Successfully allocated " << (static_cast<double>(dataStrSize*sizeof(std::string) + dataNumSize*sizeof(double)) / 1024.0)
		<< "KB of memory!";
	logger->info(str.str().c_str());

	file.clear();
	file.seekg(0, std::ios::beg);

	std::string line;
	if (hasHeader) std::getline(file, line);

	size_t rowIndex = 0;
	while (std::getline(file, line)) {
		std::string value;
		std::istringstream lineStream(line);
		size_t colIndex = 0;
		size_t numIndex = 0;
		size_t strIndex = 0;
		while (std::getline(lineStream, value, ',')) {
			if (colIndex >= ncols)
				throw "There is a row with more columns than the header/first row";
			if (cols[colIndex].type == DataType::number) {
				numData[numIndex*nrows + rowIndex] = std::stod(value);
				numIndex++;
			}
			if (cols[colIndex].type == DataType::string) {
				strData[strIndex*nrows + rowIndex] = value;
				strIndex++;
			}
			colIndex++;
		}
		rowIndex++;
	}
}

/**
 * Frees resources
 */
DataMiner::Data::~Data() {
	logger->info("Deleted allocated data");
	delete[] strData;
	delete[] numData;
}

/**
 * Gets the index of the column within the specific datatype (useful for retrieving data)
 * 
 * @throws A string with a description of why the process failed
 * @param column The column whose index to retrieve
 * @returns The index of the column within the datatype
 */
size_t DataMiner::Data::getIndex(const char* column) const {
	return getIndex(getColumn(column));
}

/**
 * Gets the index of the column within the specific datatype (useful for retrieving data)
 * 
 * @throws A string with a description of why the process failed
 * @param column The column whose index to retrieve
 * @returns The index of the column within the datatype
 */
size_t DataMiner::Data::getIndex(size_t column) const {
	return getIndex(getColumn(column));
}

/**
 * Gets the index of the column within the specific datatype (useful for retrieving data)
 * 
 * @throws A string with a description of why the process failed
 * @param column The column whose index to retrieve
 * @returns The index of the column within the datatype
 */
size_t DataMiner::Data::getIndex(const DataColumn& column) const {
	size_t index = 0;
	for (const DataColumn& col : cols) {
		if (&col == &column)
			return index;
		if (col.type == column.type)
			index++;
	}
	throw "Column not found in dataset";
}

/**
 * Attempts to open a file
 * 
 * @throws A string explaining why the process failed
 * @param filename The name of the file
 * @returns Whether or not the operation was successful
 */
bool DataMiner::Data::openFile(const char* filename) {
	file.open(filename);
	return file.is_open();
}

/**
 * Returns a column
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @returns The column
 */
const DataColumn& DataMiner::Data::getColumn(const char* column) const {
	for (const DataColumn& col : cols)
		if (col.name == column)
			return col;
	throw "Column not found error";
}

/**
 * Returns numeric data from the dataset
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @param row The row number
 * @returns The data
 */
const double& DataMiner::Data::getNumber(const char* column, size_t row) const {
	if (row > nrows)
		throw "Row out of range";
	size_t i = getIndex(column);
	return numData[i * nrows + row];
}

/**
 * Returns string data from the dataset
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @param row The row number
 * @returns The data
 */
const std::string& DataMiner::Data::getString(const char* column, size_t row) const {
	if (row > nrows)
		throw "Row out of range";
	size_t i = getIndex(column);
	return strData[i * nrows + row];
}

/**
 * Returns a column
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @returns The column
 */
const DataColumn& DataMiner::Data::getColumn(size_t column) const {
	if (column >= cols.size())
		throw "Column not found error";
	return cols[column];
}

/**
 * Returns numeric data from the dataset
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @param row The row number
 * @returns The data
 */
const double& DataMiner::Data::getNumber(size_t column, size_t row) const {
	if (row > nrows)
		throw "Row out of range";
	size_t i = getIndex(column);
	return numData[i * nrows + row];
}

/**
 * Returns string data from the dataset
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @param row The row number
 * @returns The data
 */
const std::string& DataMiner::Data::getString(size_t column, size_t row) const {
	if (row > nrows)
		throw "Row out of range";
	size_t i = getIndex(column);
	return strData[i * nrows + row];
}

/**
 * Returns a column
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @returns The column
 */
DataColumn& DataMiner::Data::getColumn(const char* column) {
	for (DataColumn& col : cols)
		if (col.name == column)
			return col;
	throw "Column not found error";
}

/**
 * Returns numeric data from the dataset
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @param row The row number
 * @returns The data
 */
double& DataMiner::Data::getNumber(const char* column, size_t row) {
	if (row > nrows)
		throw "Row out of range";
	size_t i = getIndex(column);
	return numData[i * nrows + row];
}

/**
 * Returns string data from the dataset
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @param row The row number
 * @returns The data
 */
std::string& DataMiner::Data::getString(const char* column, size_t row) {
	if (row > nrows)
		throw "Row out of range";
	size_t i = getIndex(column);
	return strData[i * nrows + row];
}

/**
 * Returns a column
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @returns The column
 */
DataColumn& DataMiner::Data::getColumn(size_t column) {
	if (column >= cols.size())
		throw "Column not found error";
	return cols[column];
}

/**
 * Returns numeric data from the dataset
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @param row The row number
 * @returns The data
 */
double& DataMiner::Data::getNumber(size_t column, size_t row) {
	if (row > nrows)
		throw "Row out of range";
	size_t i = getIndex(column);
	return numData[i * nrows + row];
}

/**
 * Returns string data from the dataset
 * 
 * @throws A string explaining why the process failed
 * @param column The column name
 * @param row The row number
 * @returns The data
 */
std::string& DataMiner::Data::getString(size_t column, size_t row) {
	if (row > nrows)
		throw "Row out of range";
	size_t i = getIndex(column);
	return strData[i * nrows + row];
}

/**
 * Sets the target of the dataset to a column
 * 
 * @throws A string explaining why the process failed
 * @param column the column to set as the target
 */
void DataMiner::Data::setTarget(size_t column) {
	setTarget(getColumn(column));
}

/**
 * Sets the target of the dataset to a column
 * 
 * @throws A string explaining why the process failed
 * @param column the column to set as the target
 */
void DataMiner::Data::setTarget(const char* column) {
	setTarget(getColumn(column));
}

/**
 * Sets the target of the dataset to a column
 * 
 * @throws A string with a description of why the process failed
 * @param column the column to set as the target
 */
void DataMiner::Data::setTarget(DataColumn& column) {
	if (column.role == DataRole::target)
		throw "This column is already a target";
	for (const DataColumn& col : cols)
		if (col.role == DataRole::target)
			throw "Another column already exists which is set to be a target";
	column.role = DataRole::target;
}