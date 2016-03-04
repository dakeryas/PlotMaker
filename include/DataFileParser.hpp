#ifndef PLOT_MAKER_DATA_PARSER_H
#define PLOT_MAKER_DATA_PARSER_H

#include <regex>
#include "Data.hpp"

namespace PlotMaker{
  
  class DataFileParser{
    
    std::regex separator;//to tokenize each line
    
  public:
    DataFileParser(std::regex separator);
    template <class T>
    Point<T> parseLine(const std::string& line);
    template <class T>
    Data<T> parse(const boost::filesystem::path& targetPath, bool verbose);
    
  };
  
  template <class T>
  Point<T> DataFileParser::parseLine(const std::string& line){
    
    Point<T> point;
    
    std::vector<std::string> tokenziedPattern;
    tokenziedPattern.assign(std::sregex_token_iterator(line.begin(), line.end(), separator, -1), std::sregex_token_iterator());
    
    if(tokenziedPattern.size() > 1){
      
      point.xValue = std::stod(tokenziedPattern[0]);
      point.yValue = std::stod(tokenziedPattern[1]);
      if(tokenziedPattern.size() == 2) point.yError = 0;
      else point.yError = std::stod(tokenziedPattern[2]);
      
    }
    else throw std::runtime_error(line + " cannot be parsed to build a point.");
    
    return point;
    
  }
  
  template <class T>
  Data<T> DataFileParser::parse(const boost::filesystem::path& targetPath, bool verbose){

    std::fstream dataFile(targetPath.string());
    
    Data<T> data;
    std::string line;
    while(std::getline(dataFile, line)){
      
      try{
	
	auto point = parseLine<T>(line);
	if(verbose) std::cout<<point<<"\n";
	data.emplace(std::move(point));
	
      }
      catch(std::runtime_error& error){
	
	std::cerr<<"Error: "<<error.what()<<std::endl;
	break;
	
      }
    
    }
    
    return data;
    
  }
  
}

#endif 