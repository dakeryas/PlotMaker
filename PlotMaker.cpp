#include <fstream>
#include <regex>
#include "boost/program_options.hpp"
#include "Data.hpp"

namespace bpo = boost::program_options;

namespace PlotMaker {
  
  template <class T>
  Point<T> parseLine(const std::string& line){
    
    Point<T> point;
    
    std::regex separator("\\s+(\\+/-\\s*)?");//space or "+/-" separated values
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
  Data<T> parse(const boost::filesystem::path& targetPath, bool verbose){

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

int main(int argc, char* argv[]){
  
  boost::filesystem::path targetPath, outputPath;
  
  bpo::options_description optionDescription("PlotMaker usage");
  optionDescription.add_options()
  ("help,h", "Display this help message")
  ("target", bpo::value<boost::filesystem::path>(&targetPath)->required(), "Text file containing the data to plot")
  ("output,o", bpo::value<boost::filesystem::path>(&outputPath)->required(), "Output file where to save the plot")
  ("verbose,v", "Display read data");  
  bpo::positional_options_description positionalOptions;//to use arguments without "--"
  positionalOptions.add("target", 1);
  
  bpo::variables_map arguments;
  try{
    
    bpo::store(bpo::command_line_parser(argc, argv).options(optionDescription).positional(positionalOptions).run(), arguments);
    
    if(arguments.count("help") || argc == 1){
      
      std::cout<<optionDescription<<std::endl;
      return 0;
      
    }
      
    bpo::notify(arguments);//the arguments are ready to be used
    
  }
  catch(bpo::error& error){
    
    std::cerr<<error.what()<<std::endl;
    return 1;
    
  }
  
  if(!boost::filesystem::is_regular_file(targetPath)){
    
    std::cerr<<"Error: "<<targetPath<<" is not a regular file"<<std::endl;
    return 1;
    
  }
  else{
    
    auto data = PlotMaker::parse<double>(targetPath, arguments.count("verbose"));
    data.write(outputPath);
    
  }
  
}

