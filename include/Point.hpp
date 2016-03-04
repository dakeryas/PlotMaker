#ifndef PLOT_MAKER_POINT_H
#define PLOT_MAKER_POINT_H

#include <iomanip>
#include <iostream>

namespace PlotMaker{
  
  template <class T>
  struct Point{
    
    T xValue;
    T yValue;
    T yError;
    
  };

  template <class T>
  std::ostream& operator<<(std::ostream& output, const Point<T>& point){
    
    output<<std::left<<std::setw(6)<<point.xValue<<"  -->  "<<std::left<<std::setw(8)<<point.yValue<<" +/- "<<std::left<<std::setw(8)<<point.yValue;
    return output;
    
  }
  
}

#endif 