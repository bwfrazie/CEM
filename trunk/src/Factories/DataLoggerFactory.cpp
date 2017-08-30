/**
* @file DataLoggerFactory.cpp
* @brief Implementation of the DataLogger Factory class
* @author Ben Frazier
* @date 08/16/2017 */

#include "DataLoggerFactory.h"
#include "Interfaces/DataLoggerInterface.h"

namespace CEM
{
  DataLoggerFactory::DataLoggerFactory()
  {
  }

  std::shared_ptr<DataLoggerInterface> DataLoggerFactory::createDataLogger(std::shared_ptr<InputDataInterface> input)
  {
    std::shared_ptr<DataLoggerInterface> dataLogger (new DataLoggerHDF5());
    return dataLogger;
  }
}



