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

// write #keypoints and keypoints to json
/*
in                   :  name/path in json
					 :  keypoints
return               :  
*/
void FileManager::writeKeypointsToJson(string path, vector<KeyPoint>& keypoints) {

	int kpSize = keypoints.size();
	// if (kpSize < 1) return;
	//string json = "{\"detector\":\"" + path + "\",\"stars\":10}";
	// 1. Parse a JSON string into DOM.
	
	//string jsonTmp = "{\"detector\":\"" + path + "\",\"keypoints\":" + to_string(kpSize) + "}";
	//const char* json = jsonTmp.c_str();
	Document doc;
	loadJsonToDocument(::fileName, doc);
	//d.SetObject(); //needed if creating new doc, without loading file
	Document::AllocatorType& allocator = doc.GetAllocator();

	//########## create object ##########
	Value kpObject(kObjectType);
	//Value kpArray(kArrayType);
	Value kpName;
	kpName.SetString(path.c_str(), allocator);
	//kpObject.AddMember(kpName, 100, allocator);
	
	//########## create array ##########
	//Value array(rapidjson::kArrayType);
	//array.PushBack("hello", allocator).PushBack("world", allocator);

	//Value& points = doc["keypoints"].AddMember(kpName, keypoints.size(), allocator);

	doc["keypoints"].AddMember(kpName, keypoints.size(), allocator);

	writeJsonFromDocument(::fileName, doc);
}

// write #matches and matches to json
/*
in                   :  name/path in json
					 :  matches
return               :
*/
void FileManager::writeMatchesToJson(string path, vector<DMatch>& matches) {

	int kpSize = matches.size();
	Document doc;
	loadJsonToDocument(::fileName, doc);
	//d.SetObject(); //needed if creating new doc, without loading file
	Document::AllocatorType& allocator = doc.GetAllocator();

	//########## create object ##########
	Value mObject(kObjectType);
	Value mName;
	mName.SetString(path.c_str(), allocator);
	doc["matches"].AddMember(mName, matches.size(), allocator);

	writeJsonFromDocument(::fileName, doc);
}

void FileManager::loadJsonToDocument(string path, Document& doc) {
	FILE* fp = fopen(path.c_str(), "rb"); // non-Windows use "r"
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	//Document d;
	doc.ParseStream(is);
	fclose(fp);
}

void FileManager::writeJsonFromDocument(string path, Document& doc) {
	FILE* fp = fopen(path.c_str(), "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	doc.Accept(writer);
	fclose(fp);
}

void FileManager::createEmptyJson(string path) {
	::fileName = path;
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();

	Value kpObj(kObjectType);
	Value mObj(kObjectType);
	//Value kpName, mName;
	//Value objArray(kArrayType);
	//array.PushBack("hello", allocator).PushBack("world", allocator);
	//objName = "keypoints";

	//objName.SetString(path.c_str(), allocator);
	//obj.AddMember(objName, 100, allocator);
	doc.AddMember("keypoints", kpObj, allocator);
	doc.AddMember("matches", mObj, allocator);

	writeJsonFromDocument(::fileName, doc);
}