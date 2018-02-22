/**
* @file DataLogger_HDF5_Vector.cpp
* @brief Implementation of the Vector functions for the  DataLogger_HDF5 class
* @author Ben Frazier
* @date 08/12/2017 */

#include "DataLogger_HDF5.h"

  //************************************************************************************************************
  
    std::vector<double> DataLoggerHDF5::ReadVector(std::string fileName, std::string datasetName)
    {
     std::vector<double> data_out;


      //open the file and get the requested dataset
      H5File file( fileName, H5F_ACC_RDONLY);
      DataSet dataset = file.openDataSet( datasetName);

      hsize_t currentSize = dataset.getStorageSize()/8; //Assume 64-bit double value

      //get the dataspace and determine the size and rank
      DataSpace filespace = dataset.getSpace();
      int rank = filespace.getSimpleExtentNdims();
      hsize_t dims;    // dataset dimensions
      rank = filespace.getSimpleExtentDims( &dims );

      DataSpace mspace( rank , &dims);

      data_out.resize(dims);
      dataset.read( &data_out[0], PredType::NATIVE_DOUBLE, mspace, filespace );

      return data_out;
    }


    std::vector<double> DataLoggerHDF5::ReadVector(std::string datasetName)
    {
      std::vector<double> data_out;

      //open the file and get the requested dataset
      DataSet dataset = file_.openDataSet( datasetName);

      hsize_t currentSize = dataset.getStorageSize()/8; //Assume 64-bit double value

      //get the dataspace and determine the size and rank
      DataSpace filespace = dataset.getSpace();
      int rank = filespace.getSimpleExtentNdims();
      hsize_t dims;    // dataset dimensions
      rank = filespace.getSimpleExtentDims( &dims );

      DataSpace mspace( rank , &dims);

      data_out.resize(dims);
      dataset.read( &data_out[0], PredType::NATIVE_DOUBLE, mspace, filespace );

      return data_out;
    }
    
 //************************************************************************************************************
    std::vector<double> DataLoggerHDF5::ReadVector(int index, std::string fileName, std::string datasetName)
    {
     std::vector<double> data_out;
      
    H5File file( fileName, H5F_ACC_RDONLY);
  	 //open the file and get the requested dataset
  	 DataSet dataset = file.openDataSet( datasetName);
  
  	hsize_t currentSize = dataset.getStorageSize()/8; //Assume 64-bit double value

    //get the dataspace and determine the size and rank
  	DataSpace filespace = dataset.getSpace();
  	int rank = filespace.getSimpleExtentNdims();

  	DSetCreatPropList plist = dataset.getCreatePlist();
  
  	//now get the memory size, the new dataset size, and the offset
   	hsize_t dims[2];
   	rank = filespace.getSimpleExtentDims( dims );
   	rank = plist.getChunk(2,dims);

   	hsize_t dsize[2];
   	hsize_t offset[2];
   	offset[0] = 0;
   	offset[1] = index * dims[1];

   	//create the memory space
   	DataSpace mspace(rank, dims);
    
  	if (rank == 1)
   	 	data_out.resize(1,dims[0]);
  	else
    	data_out.resize(dims[1],dims[0]);

   	//get the file space
   	DataSpace fspace = dataset.getSpace();
   	//select the hyperslabs
   	fspace.selectHyperslab( H5S_SELECT_SET, dims,  offset);
    
  	dataset.read( &data_out[0], PredType::NATIVE_DOUBLE, mspace, fspace );
    
  	return data_out;
    }
    
     //************************************************************************************************************
    std::vector<double> DataLoggerHDF5::ReadVector(int index, std::string datasetName)
    {
      std::vector<double> data_out;
      
  	 //open the file and get the requested dataset
  	 DataSet dataset = file_.openDataSet( datasetName);
  
  	hsize_t currentSize = dataset.getStorageSize()/8; //Assume 64-bit double value

    //get the dataspace and determine the size and rank
  	DataSpace filespace = dataset.getSpace();
  	int rank = filespace.getSimpleExtentNdims();

  	DSetCreatPropList plist = dataset.getCreatePlist();
  
  	//now get the memory size, the new dataset size, and the offset
   	hsize_t dims[2];
   	rank = filespace.getSimpleExtentDims( dims );
   	rank = plist.getChunk(2,dims);

   	hsize_t dsize[2];
   	hsize_t offset[2];
   	offset[0] = 0;
   	offset[1] = index * dims[1];

   	//create the memory space
   	DataSpace mspace(rank, dims);
    
  	if (rank == 1)
   	 	data_out.resize(1,dims[0]);
  	else
    	data_out.resize(dims[1],dims[0]);

   	//get the file space
   	DataSpace fspace = dataset.getSpace();
   	//select the hyperslabs
   	fspace.selectHyperslab( H5S_SELECT_SET, dims,  offset);
    
  	dataset.read( &data_out[0], PredType::NATIVE_DOUBLE, mspace, fspace );
    
  	return data_out;
  
    }


  //************************************************************************************************************
   /**
   * \brief Write a std::vector to the File
   *
   * This function writes a data array from a std::vector of data and converts
   * the std::vector to an array of pointers first and then calls the overloaded WriteDataArray function
   * @param data The data to write
   *@param fileName The file name to write to
   * @param datasetName The dataset to use*/
  void DataLoggerHDF5::WriteData(std::vector<double>data, std::string fileName, std::string datasetName)
  {
  
     H5File file;
     file.openFile(fileName, H5F_ACC_RDWR);
     
     DataSet dataset;
     
     //check to see if the requested dataset currently exists
     if (file.exists(datasetName))
     {
		//open the dataset
     	dataset = file.openDataSet( datasetName);
     	hsize_t currentSize = dataset.getStorageSize()/8; //Assume 64-bit double values
     
    	//now get the memory size, the new dataset size, and the offset
    	hsize_t msize = data.size();
    	hsize_t offset = currentSize; 
    	hsize_t dsize = offset + data.size();

    	//create the memory space
    	DataSpace mspace( 1, &msize);

    	//extend the data set
    	dataset.extend( &dsize );
	
    	//get the file space
    	DataSpace fspace = dataset.getSpace();
    	//select the hyperslab
    	fspace.selectHyperslab( H5S_SELECT_SET, &msize, &offset);

		//write the vector
    	dataset.write( &data[0], PredType::NATIVE_DOUBLE, mspace, fspace);

     }
     else
     {
		//need to create a new dataset with Unlimited size to allow extending
        hsize_t      maxdims = H5S_UNLIMITED;
        hsize_t msize = data.size();
        DataSpace mspace( 1, &msize, &maxdims);
    
    	DSetCreatPropList cparms;
        
    	hsize_t chunk_dims;
    	cparms.setChunk( 1, &msize);
        
        int fill_val = 0;
        cparms.setFillValue( PredType::NATIVE_DOUBLE, &fill_val);
       	
     	dataset = file.createDataSet( datasetName, PredType::NATIVE_DOUBLE, mspace,cparms);
		dataset.write( &data[0], PredType::NATIVE_DOUBLE, mspace);
     	
     }
     
   file.close();
  }
  
    //************************************************************************************************************
   /**
   * \brief Write a std::vector to the File
   *
   * This function writes a data array from a std::vector of data to the specified dataset
   * @param data The data to write
   * @param datasetName The dataset to use*/
  void DataLoggerHDF5::WriteData(std::vector<double>data, std::string datasetName)
  {
  
     DataSet dataset;
     
     //check to see if the requested dataset currently exists
     if (file_.exists(datasetName))
     {
		//open the dataset
     	dataset = file_.openDataSet( datasetName);
     	hsize_t currentSize = dataset.getStorageSize()/8; //Assume 64-bit double values
     
    	//now get the memory size, the new dataset size, and the offset
    	hsize_t msize = data.size();
    	hsize_t offset = currentSize; 
    	hsize_t dsize = offset + data.size();

    	//create the memory space
    	DataSpace mspace( 1, &msize);

    	//extend the data set
    	dataset.extend( &dsize );
	
    	//get the file space
    	DataSpace fspace = dataset.getSpace();
    	//select the hyperslab
    	fspace.selectHyperslab( H5S_SELECT_SET, &msize, &offset);

		//write the vector
    	dataset.write( &data[0], PredType::NATIVE_DOUBLE, mspace, fspace);

     }
     else
     {
		//need to create a new dataset with Unlimited size to allow extending
        hsize_t      maxdims = H5S_UNLIMITED;
        hsize_t msize = data.size();
        DataSpace mspace( 1, &msize, &maxdims);
    
    	DSetCreatPropList cparms;
        
    	hsize_t chunk_dims;
    	cparms.setChunk( 1, &msize);
        
        int fill_val = 0;
        cparms.setFillValue( PredType::NATIVE_DOUBLE, &fill_val);
       	
     	dataset = file_.createDataSet( datasetName, PredType::NATIVE_DOUBLE, mspace,cparms);
		dataset.write( &data[0], PredType::NATIVE_DOUBLE, mspace);
     	
     }
     
  }
  
  //************************************************************************************************************
    void DataLoggerHDF5::WriteData(std::vector<double> data, double time, std::string fileName, std::string datasetName)
    {
      
    }
  
  //************************************************************************************************************
    /**
   * \brief Write a std::vector to the File
   *
   * This function writes a data array from a std::vector of data and converts
   * the std::vector to an array of pointers first and then calls the overloaded WriteDataArray function
   * @param data The data to write
   * @param time The time to append to the data vector
   * @param datasetName The dataset to write ("/EField" or "/HField")*/
  void  DataLoggerHDF5::WriteData(std::vector<double>data, double time, std::string datasetName)
  {
   /* DataSet dataset;
    if (datasetName.compare("EField"))
      {
	dataset = datasetE_;
      }
    else if (datasetName.compare("HField"))
      {
        dataset = datasetH_;
      }
    else
      throw std::runtime_error("DataLoggerHDF5::WriteDataArray ... Invalid dataset name requested");
		 
     hsize_t currentSize = dataset.getStorageSize()/8; //Assume 64-bit double values
     
    //now get the memory size, the new dataset size, and the offset
    hsize_t msize = data.size();
    hsize_t offset = currentSize; 
    hsize_t dsize = offset + data.size();

    //create the memory space
    DataSpace mspace( 2, &msize);
    
    //extend the data set
    dataset.extend( &dsize );
	
    //get the file space
    DataSpace fspace = dataset.getSpace();
    //select the hyperslab
    fspace.selectHyperslab( H5S_SELECT_SET, &msize, &offset);

    dataset.write( &data[0], PredType::NATIVE_DOUBLE, mspace, fspace);

    //update the time vector
    hsize_t tSize = 1;
    hsize_t toffset = datasetT_.getStorageSize()/8;
    hsize_t dTsize = toffset + tSize;
    DataSpace mTspace( 1, &tSize);
    datasetT_.extend(&dTsize);

    DataSpace tspace = datasetT_.getSpace();
    tspace.selectHyperslab( H5S_SELECT_SET, &tSize, &toffset);
    datasetT_.write(&time, PredType::NATIVE_DOUBLE, mTspace,tspace);*/
  }


