

#include <iostream>
#include <string>
#include <fstream>

#include "bloom_filter.hpp"

int main(int argc, char **argv)
{    
	// create bloom filter
	std::cout << "initializing bloom filter parameters" << std::endl;
	bloom_parameters parameters;
	parameters.projected_element_count = 100000;
	parameters.false_positive_probability = 1.0 / parameters.projected_element_count;
	if (!parameters) {
		std::cout << "Error - bloom_parameters invalid" << std::endl;
		return 1;
	}
	parameters.compute_optimal_parameters();
	std::cout << "optimal num hashes: " << parameters.optimal_parameters.number_of_hashes << std::endl;
    std::cout << "optimal table size: " << parameters.optimal_parameters.table_size << std::endl;
	std::cout << "instantiating the bloom filter..." << std::endl;
	bloom_filter filter(parameters);
	std::cout << "allocated " << parameters.optimal_parameters.table_size / 8 << " bytes for table" << std::endl;
	
	// load bloom filter with black list
	{
		std::cout << "loading bloom filter..." << std::endl;
		std::ifstream stream("blacklist.md5");
		if (!stream) {
			std::cout << "Error - failed to open blacklist.md5" << std::endl;
			return 1;
		}
		std::string buffer;
		while (std::getline(stream,buffer)) {
			filter.insert(buffer);
		}
		std::cout << "loading complete" << std::endl;
	}
	
	// check for matches
	{
		std::cout << "checking bloom filter against same data it was loaded with..." << std::endl;
		std::ifstream stream("blacklist.md5");
		if (!stream) {
			std::cout << "Error - failed to open blacklist.md5" << std::endl;
			return 1;
		}
		std::string buffer;
		int num_false_negative = 0;
		while (std::getline(stream,buffer)) {
			if (!filter.contains(buffer)) {
				++num_false_negative;
			}
		}
		std::cout << "	false negative count: " << num_false_negative << std::endl;
		std::cout << "check complete" << std::endl;
	}
	
	// check for non-matches
	{
		std::cout << "checking bloom filter against different data..." << std::endl;
		std::ifstream stream("samples.md5");
		if (!stream) {
			std::cout << "Error - failed to open samples.md5" << std::endl;
			return 1;
		}
		std::string buffer;
		int num_false_positive = 0;
		while (std::getline(stream,buffer)) {
			if (filter.contains(buffer)) {
				++num_false_positive;
			}
		}
		std::cout << "	false positive count: " << num_false_positive << std::endl;
		std::cout << "check complete" << std::endl;
	}
	
	return 0;
}