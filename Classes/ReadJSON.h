/*
 * ReadJSON.h
 *
 *  Created on: 2015-12-30
 *      Author: guh
 */

#ifndef READJSON_H_
#define READJSON_H_

#include "cocos2d.h"
#include "Question.h"
#include "dlfcn.h"
#include "cocostudio/CocoStudio.h"

using namespace std;

bool openIcuuc();
const char* gb23122utf8(const char * gb2312);
void WStrToUTF8(std::string& dest, const std::wstring& src);


/**
@class ReadJSON
@brief ReadJSON is a subclass of Ref that implements the reference count management.
@details read json data;
*/
class ReadJSON: public cocos2d::Ref {
public:
	virtual ~ReadJSON();

	static ReadJSON* create(string filepath);

    /** Callback function for get question data from json data.
     * @param which: the question number in this json file.
     * @return a reference of Question object.
     * @js NA
     */
	Question* question(int which);

    /** Callback function for json data change to html code.
     * @param which: the question number in this json file.
     * @return html code string.
     * @js NA
     */
	std::string htmlString(int which);

    /** Callback function for get the question number of this json file.
     * @return the total nubmer of question.
     * @js NA
     */
	int dataSize();

private:

	ReadJSON(string filepath);

	const rapidjson::Value& parse(const rapidjson::Value &val);

    /** Callback function for json data change to html code.
     * @param val: the question content of json.
     * @return html code string.
     * @js NA
     */
	std::string rapidjsonString(const rapidjson::Value &val);

	/** the json file path */
	string m_filepath;

	/** the file of parent path */
	string m_dirpath;

	/** the file of question nubmer */
	int m_data_number;

//	string m_beffer; // 暂存数据

	/** the file of question content */
	rapidjson::Document _doc;
};

#endif /* READJSON_H_ */
