#ifndef PLOT_MAKER_DATA_H
#define PLOT_MAKER_DATA_H

#include "boost/filesystem.hpp"
#include "TGraphErrors.h"
#include "TFile.h"
#include "Point.hpp"

namespace PlotMaker{
  
  template <class T>
  struct Data{
    
    std::vector<Point<T>> points;
    void emplace(Point<T> point);
    void write(const boost::filesystem::path& outputPath) const;
    
  };

  template <class T>
  std::ostream& operator<<(std::ostream& output, const Data<T>& data){
    
    for(const auto& point : data.points) output<<point<<"\n";
    return output;
    
  }

  template <class T>
  void Data<T>::emplace(Point<T> point){
    
    points.emplace_back(std::move(point));

  }

  template <class T>
  void Data<T>::write(const boost::filesystem::path& outputPath) const{
    
    std::vector<double> xValues(points.size());
    std::vector<double> yValues(points.size());
    std::vector<double> yErrors(points.size());
    
    for(unsigned k = 0; k !=  points.size(); ++k){
      
      xValues[k] = points[k].xValue;
      yValues[k] = points[k].yValue;
      yErrors[k] = points[k].yError;
      
    }
    
    TFile ouput(outputPath.c_str(), "recreate");
    TGraphErrors graph(points.size(),xValues.data(),yValues.data(), 0, yErrors.data());
    graph.SetTitle("");
    graph.SetLineWidth(2);
    graph.SetLineColor(4);
    graph.SetMarkerStyle(21);
    graph.SetMarkerColor(4);
    graph.Write();
    
  }
  
}

#endif 