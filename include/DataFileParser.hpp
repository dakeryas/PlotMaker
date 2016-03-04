#ifndef PLOT_MAKER_DATA_PARSER_H
#define PLOT_MAKER_DATA_PARSER_H

#include <regex>
#include "Data.hpp"

namespace PlotMaker{
  
  class DataFileParser{
    
    bool fastMode;
    std::regex separator;//to tokenize each line
    
    template <class T>
    Data<T> parseFast(const boost::filesystem::path& targetPath, bool verbose) const;//do not use the regex separator and assume a space separator
    template <class T>
    Point<T> parseLine(const std::string& line) const;
    template <class T>
    Data<T> parseWithRegex(const boost::filesystem::path& targetPath, bool verbose) const;
    
  public:
    DataFileParser();
    DataFileParser(std::regex separator);
    void setFastMode(bool fastMode);
    void setSeparator(std::regex separator);
    template <class T>
    Data<T> parse(const boost::filesystem::path& targetPath, bool verbose) const;
    
  };
  
  template <class T>
  Point<T> DataFileParser::parseLine(const std::string& line) const{
    
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
  Data<T> DataFileParser::parseWithRegex(const boost::filesystem::path& targetPath, bool verbose) const{

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
  
  template <class T>
  Data<T> DataFileParser::parseFast(const boost::filesystem::path& targetPath, bool verbose) const{

    std::fstream dataFile(targetPath.string());
    
    Data<T> data;
    Point<T> point;
    while(dataFile >> point.xValue >> point.yValue >> point.yError){
      
      if(verbose) std::cout<<point<<"\n";
      data.emplace(std::move(point));
      
    }
    
    return data;
    
  }
  
  template <class T>
  Data<T> DataFileParser::parse(const boost::filesystem::path& targetPath, bool verbose) const{

   if(fastMode) return parseFast<T>(targetPath, verbose);
   else return parseWithRegex<T>(targetPath, verbose);
    
  }
  
}

#endif 