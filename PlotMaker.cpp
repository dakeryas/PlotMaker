#include <fstream>
#include "boost/program_options.hpp"
#include "DataFileParser.hpp"

namespace bpo = boost::program_options;

int main(int argc, char* argv[]){
  
  boost::filesystem::path targetPath, outputPath;
  std::string separatorString;
  
  bpo::options_description optionDescription("PlotMaker usage");
  optionDescription.add_options()
  ("help,h", "Display this help message")
  ("target", bpo::value<boost::filesystem::path>(&targetPath), "Text file containing the data to plot")
  ("output,o", bpo::value<boost::filesystem::path>(&outputPath)->required(), "Output file where to save the plot")
  ("separator,s", bpo::value<std::string>(&separatorString)->default_value("\\s+(\\+/-\\s*)?"), "Regular expression separator for columns")
  ("fast,f", "Assumes 3 columns, space sperator only, no safety checks") 
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
  
  std::istream* dataStream = &std::cin;
  
  if(arguments.count("target")){
    
    if(!boost::filesystem::is_regular_file(targetPath)){
      
      std::cerr<<"Error: "<<targetPath<<" is not a regular file"<<std::endl;
      return 1;
      
    }
    else dataStream = new std::ifstream(targetPath.string());
    
  }
  
  PlotMaker::DataFileParser dataFileParser{std::regex(separatorString)};
  if(arguments.count("fast")) dataFileParser.setFastMode(true);
  auto data = dataFileParser.parse<double>(*dataStream, arguments.count("verbose"));
  data.write(outputPath);
  
  if(dataStream != &std::cin) delete dataStream;
  
}

