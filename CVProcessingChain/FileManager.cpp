//============================================================================
// Name        : FileManager.cpp
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : provedes FileManager functions
//============================================================================

#include "FileManager.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"

#include <iostream>
#include <cstdio>

using namespace rapidjson;

string fileName;

// write #keypoints to json file
/*
path            :  string/path in json
keypoints		:  keypoints
*/
void FileManager::writeKeypointsToJson(string path, vector<KeyPoint>& keypoints) {

	int kpSize = keypoints.size();
	Document doc;
	loadJsonToDocument(::fileName, doc);
	Document::AllocatorType& allocator = doc.GetAllocator();

	//########## create object ##########
	Value kpObject(kObjectType);
	Value kpName;
	kpName.SetString(path.c_str(), allocator);

	doc["keypoints"].AddMember(kpName, keypoints.size(), allocator);
	writeJsonFromDocument(::fileName, doc);
}

// write #matches to json file
/*
path            :  string/path in json
object          :  object in json
matches 		:  matches
*/
void FileManager::writeMatchesToJson(string path, string object, vector<DMatch>& matches) {

	Document doc;
	loadJsonToDocument(::fileName, doc);
	Document::AllocatorType& allocator = doc.GetAllocator();

	//########## create object ##########
	Value mObject(kObjectType);
	Value mName;
	mName.SetString(path.c_str(), allocator);

	doc[object.c_str()].AddMember(mName, matches.size(), allocator);
	writeJsonFromDocument(::fileName, doc);
}

// write distance to json file
/*
path            :  string/path in json
object          :  object in json
distance 		:  distance
*/
void FileManager::writeDistanceToJson(string path, string object, float distance){

	Document doc;
	loadJsonToDocument(::fileName, doc);
	Document::AllocatorType& allocator = doc.GetAllocator();

	//########## create object ##########
	Value mObject(kObjectType);
	Value mName;
	mName.SetString(path.c_str(), allocator);

	doc[object.c_str()].AddMember(mName, distance, allocator);
	writeJsonFromDocument(::fileName, doc);
}

// write time to json file
/*
path            :  string/path in json
object          :  object in json
time	 		:  time
*/
void FileManager::writeTimeToJson(string path, string object, clock_t time) {

	Document doc;
	loadJsonToDocument(::fileName, doc);
	Document::AllocatorType& allocator = doc.GetAllocator();

	//########## create object ##########
	Value tObject(kObjectType);
	Value tName;
	tName.SetString(path.c_str(), allocator);

	doc[object.c_str()].AddMember(tName, (double)time / CLOCKS_PER_SEC, allocator);
	writeJsonFromDocument(::fileName, doc);
}

// load json file and store in a temporary document for editing
/*
path            :  path of json file
doc				:  temporary document
*/
void FileManager::loadJsonToDocument(string path, Document& doc) {
	FILE* fp = fopen(path.c_str(), "rb");
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	doc.ParseStream(is);
	fclose(fp);
}

// write temporary document to json file
/*
path            :  path of json file
doc				:  temporary document
*/
void FileManager::writeJsonFromDocument(string path, Document& doc) {
	FILE* fp = fopen(path.c_str(), "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	doc.Accept(writer);
	fclose(fp);
}

// create and save an json file with all propably needed but empty objects
/*
path            :  path of json file
*/
void FileManager::createEmptyJson(string path) {
	::fileName = path;
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();

	Value kpObj(kObjectType);
	Value ratioBFMObj(kObjectType);
	Value crosscheckBFMObj(kObjectType);
	Value threshMObj(kObjectType);
	Value ransacInlierObj(kObjectType);
	Value realMObj(kObjectType);

	Value avgDistHom(kObjectType);
	Value totalDistHom(kObjectType);

	Value preprocessingTimerObj(kObjectType);
	Value detectorTimerObj(kObjectType);
	Value descriptorTimerObj(kObjectType);
	Value ratioMatchTimerObj(kObjectType);
	Value crosscheckMatchTimerObj(kObjectType);

	doc.AddMember("keypoints", kpObj, allocator);
	doc.AddMember("crosscheckMatches", crosscheckBFMObj, allocator);
	doc.AddMember("ratioMatches", ratioBFMObj, allocator);
	doc.AddMember("thresholdMatches", threshMObj, allocator);
	doc.AddMember("ransacInliers", ransacInlierObj, allocator);
	doc.AddMember("realMatches", realMObj, allocator);

	doc.AddMember("avgDistHomography", avgDistHom, allocator);
	doc.AddMember("totalDistHomography", totalDistHom, allocator);

	doc.AddMember("preprocessingTimer", preprocessingTimerObj, allocator);
	doc.AddMember("detectorTimer", detectorTimerObj, allocator);
	doc.AddMember("descriptorTimer", descriptorTimerObj, allocator);
	doc.AddMember("ratioMatchTimer", ratioMatchTimerObj, allocator);
	doc.AddMember("crosscheckMatchTimer", crosscheckMatchTimerObj, allocator);

	writeJsonFromDocument(::fileName, doc);
}