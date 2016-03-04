#include "DataFileParser.hpp"

namespace PlotMaker {
  
  DataFileParser::DataFileParser():fastMode(true),separator("\\s+"){
    
  }
  
  DataFileParser::DataFileParser(std::regex separator):fastMode(false),separator(std::move(separator)){
    
  }
  
  void DataFileParser::setFastMode(bool fastMode){
    
    this->fastMode = fastMode;
    if(fastMode) separator = std::regex("\\s+");
    
  }
  
  void DataFileParser::setSeparator(std::regex separator){
    
    this->separator = std::move(separator);
    fastMode = false;
    
  }
  
}