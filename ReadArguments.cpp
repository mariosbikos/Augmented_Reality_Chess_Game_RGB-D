#include "ReadArguments.h"


bool readArguments(int argc,char** argv)
{
	//If the number of parameters is lower than 3-->wrong
    if (argc<4) 
	{
		cerr<<"Invalid number of arguments"<<endl;
		cerr << 
			"Usage: intrinsics.yml boardConfig.yml boardConfig.yml MarkerSize dictionary.yml \n \
			intrinsics.yml: input camera parameters (in OpenCV format) to allow camera pose estimation \n \
			boardConfig.yml: input board configuration file \n \
			size: markers size in meters to allow camera pose estimation \n \
			dictionary.yml: input marker dictionary used for detection \n "
		<<endl;
        
		return false;
    }
    //Pass to variables the incoming input parameters
    
	
	TheIntrinsicFile=argv[1];
	TheBoardConfigFile=argv[2];
	TheMarkerSize=atof(argv[3]);
    TheDictionaryFile=argv[4];

	if (TheIntrinsicFile != "" && TheBoardConfigFile!= "" &&  TheDictionaryFile!= "") 
	{
		cout<<"Arguments given by user:"<<endl;
		cout<<"Intrinsics of Camera: "<<TheIntrinsicFile<<endl;
		cout<<"Bord Configuration File: "<<TheBoardConfigFile<<endl;
		cout<<"Size of Marker in Meters: "<<TheMarkerSize<<endl;
		cout<<"Dictionary File: "<<TheDictionaryFile<<endl;
	}
	else 
		return false;
	

    return true;
}