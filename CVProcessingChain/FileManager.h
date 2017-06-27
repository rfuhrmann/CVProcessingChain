//============================================================================
// Name        : FileManager.h
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : header file for FileManager (input/output)
//============================================================================

#include <iostream>
#include<list>
#include <opencv2/opencv.hpp>
#include "rapidjson/document.h"

using namespace std;
using namespace cv;
using namespace rapidjson;

class FileManager {

public:
	// constructor
	FileManager(void) {};
	// destructor
	~FileManager(void) {};



	void writeKeypointsToJson(string path, vector<KeyPoint>& keypoints);
	void writeMatchesToJson(string path, string object, vector<DMatch>& matches);
	void writeTimeToJson(string path, string object, clock_t time);
	void createEmptyJson(string path);


private:

	static string fileName;

	void loadJsonToDocument(string path, Document& doc);
	void writeJsonFromDocument(string path, Document& doc);

};
