/*
 * ReadJSON.cpp
 *
 *  Created on: 2015-12-30
 *      Author: guh
 */

#include "ReadJSON.h"
#include "GameElement.h"

ReadJSON::ReadJSON(string filepath) {
	m_filepath = FileUtils::getInstance()->fullPathForFilename(filepath);
	m_dirpath = m_filepath.substr(0, m_filepath.find_last_of('/', m_filepath.length()));
	string m_beffer = FileUtils::getInstance()->getStringFromFile(m_filepath);
	_doc.Parse<0>(m_beffer.c_str());
	if (!_doc.HasParseError()) {
		if (_doc.IsObject() && _doc.HasMember("data")) {
			const rapidjson::Value &a=_doc["data"];
			if (a.IsArray()) {
				m_data_number = a.Size();
			}
		}
	}

	log("-------- filepath is %s, m_diroath is %s ", m_filepath.c_str(), m_dirpath.c_str());
}

ReadJSON::~ReadJSON() {
	// TODO Auto-generated destructor stub
	release();
}


ReadJSON* ReadJSON::create(string filepath) {
	ReadJSON* ret = new (std::nothrow) ReadJSON(filepath);
    ret->autorelease();
    ret->retain();
    return ret;
}

int ReadJSON::dataSize() {
	return m_data_number;
}

Question* ReadJSON::question(int which) {
	Question* qst = Question::create();
	if (!_doc.HasParseError()) {
		if (_doc.IsObject() && _doc.HasMember("data")) {
			 const rapidjson::Value &a=_doc["data"];

			 if (a.IsArray()) {
				 const rapidjson::Value &val = a[which];

				 // 获得题干
				 if (val.HasMember("content")) {
					 qst->setSubject(val["content"].GetString());
				 }

				 Vector<__Integer*> vector = MyRandom::randomVector(3);
				 // 获取选项
				 if (val.HasMember("options")) { // 判断有没有某个键
					 const rapidjson::Value &val_options = val["options"];

					 if (val_options.IsArray()) {
						 for (int i=0; i<val_options.Size(); i++) {
							 qst->setOption(val_options[i].GetString(), vector.at(i)->getValue());
							 log(" i is %d , value is %d, string is %s", i, vector.at(i)->getValue(), val_options[i].GetString());
						 }
					 }
				 }

				 // 获得答案
				 if (val.HasMember("correctAnswers")) {
					 const rapidjson::Value &val_ans = val["correctAnswers"];
					 const char* right_ans = val_ans.GetString();
					 int right_id = right_ans[0] - 'A';
					 if (val_ans.IsArray()) {
						 const char* right_zero = val_ans[0].GetString();
						 right_id = right_zero[0] - 'A';
					 }

					 log(" right_is is %d ", right_id);
					 qst->setRightId(vector.at(right_id)->getValue());
				 }
			 }
		}
	}

	return qst;
}

string ReadJSON::htmlString(int which) {
	std::string htmlStr = "<body style=\"font-size:50px;\">Hello World <img src=\"HelloWorld.png\"/> </body>";
	if (!_doc.HasParseError()) {
		int font_size = 24;
		if (_doc.IsObject() && _doc.HasMember("fontSize")) {
			if (_doc["fontSize"].IsInt()) {
				font_size = _doc["fontSize"].GetInt();
			}
		}

		CCString* ns=CCString::createWithFormat("<head><link href='style.css' rel='stylesheet' type='text/css'/></head><body>");

		if (_doc.IsObject() && _doc.HasMember("data")) {
			const rapidjson::Value &qsts=_doc["data"];
			if (qsts.IsArray()) {
				const rapidjson::Value &qst = qsts[which];

				if (qst.HasMember("content")) {
					ns->append(rapidjsonString(qst["content"]));
				}
				ns->append(" </body>");
			}
		}
		htmlStr = ns->_string;
	}
	log("htmlString is %s ", htmlStr.c_str());
	return htmlStr;
}

string ReadJSON::rapidjsonString(const rapidjson::Value &val) {
	std::string reback = "";
	if (val.IsArray()) {
		std::string biaopinyin = "";
		for (int i=0; i<val.Size(); i++) {
			const rapidjson::Value &item = val[i];

			if (item.IsArray()) {
				int j = 0;
				const rapidjson::Value &sub_item_content = item[j];
				j++;
				std::string tempcon = gb23122utf8(sub_item_content.GetString());
				if (item.Size() > 1) {
				while (j<item.Size()){
					const rapidjson::Value &sub_item_eff = item[j];
					std::string eff = sub_item_eff.GetString();
					if (eff.compare("b") == 0) {
//						unit->_curEffect = Effect::BOLD;
						CCString* ns=CCString::createWithFormat("<b>%s</b>", tempcon.c_str());
						reback.append(ns->_string);
					} else if (eff.compare("i") == 0) {
//						unit->_curEffect = Effect::ITALIC;
						CCString* ns=CCString::createWithFormat("<em>%s</em>", tempcon.c_str());
						reback.append(ns->_string);
					} else if (eff.compare("u") == 0) {
//						unit->_curEffect = Effect::UNDERLINE;
						CCString* ns=CCString::createWithFormat("<u>%s</u>", tempcon.c_str());
						reback.append(ns->_string);
					} else if (eff.compare("rt") == 0) {
//						unit->_type = Type::PINYIN;
						CCString* ns=CCString::createWithFormat("<ruby>%s<rp>(</rp><rt>%s</rt><rp>)</rp></ruby>", biaopinyin.c_str(), tempcon.c_str());
						reback.append(ns->_string);
					} else if (eff.compare("img") == 0) {
//						unit->_type = Type::PICTURE;
						std::string str = m_dirpath+"/res/"+tempcon;
						CCString* ns=CCString::createWithFormat("<img class='center' src=\"%s\"/>", str.c_str());
						reback.append(ns->_string);
					} else if (eff.compare("tex") == 0) {
//							unit->_type = Type::PICTURE;
						std::string str = m_dirpath+"/res/"+tempcon;
						CCString* ns=CCString::createWithFormat("<img class='center' src=\"%s\"/>", str.c_str());
						reback.append(ns->_string);
					} else if (eff.compare("sup") == 0) {
//							unit->_type = Type::SUPERSCRIPT;
						CCString* ns=CCString::createWithFormat("<SUP>%s</SUP>", tempcon.c_str());
						reback.append(ns->_string);
					} else if (eff.compare("sub") == 0) {
//							unit->_type = Type::SUBSCRIPT;
						CCString* ns=CCString::createWithFormat("<SUB>%s</SUB>", tempcon.c_str());
						reback.append(ns->_string);
					} else if (eff.compare("e") == 0) {
//							unit->_curEffect = Effect::DOT;
						CCString* ns=CCString::createWithFormat("<span class='dot'>%s</span>", tempcon.c_str());
						reback.append(ns->_string);
					} else if (eff.compare("snd") == 0) {
//							unit->_type = Type::SOUND;

					}
					j++;
				}
				} else {
					if (tempcon.length() > 9) {

						std::string pinyin = tempcon.substr(0, 9);
						if (pinyin.compare("拼音：") == 0) {
							biaopinyin = tempcon.substr(9, (tempcon.length()-1));
							log("pinyin: is %s ", biaopinyin.c_str());
							tempcon = pinyin;
						}
					}
					reback.append(tempcon);
				}
			}

		}
	}
	return reback;
}


const rapidjson::Value& ReadJSON::parse(const rapidjson::Value &val) {
	const rapidjson::Value &reback = val;
	if (val.IsArray()) {
		int n = 0;
		log(" -------- n is %d ", n);
		const rapidjson::Value &temp = val[n];
		const rapidjson::Value &reback1 = parse(temp);
		return reback1;
	} else {
		if (reback.IsString()) {
			log(" -------- string is %s ", reback.GetString());
//			log(" -------- change string is %s ", gb23122utf8(reback.GetString()));
		}
	}
	return reback;
}

//UnitLabel* ReadJson::checkEffect();

void (*ucnv_convert)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*) = 0;
//android2.1, ucnv_convert_3_8
 //android2.2, ucnv_convert_4_2
 //android2.3, ucnv_convert_44
 //android4.0, ucnv_convert_46
bool openIcuuc()
{
	void* libFile = dlopen("/system/lib/libicuuc.so", RTLD_LAZY);
//    void* libFile = dlopen(/system/lib/libicuuc.so, RTLD_LAZY);
    if (libFile)
    {
        ucnv_convert = (void (*)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*))dlsym(libFile, "ucnv_convert_3_8");

        int index = 8;
        char fun_name[64];
        int tenIdnex = 3;
        while (ucnv_convert == NULL)
        {
            sprintf(fun_name, "ucnv_convert_%d%d", tenIdnex, index);
            ucnv_convert = (void (*)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*))dlsym(libFile, fun_name);
            if (ucnv_convert) {
            	log(" -------- fun_name  is %s ", fun_name);
            	return true;
            }

            sprintf(fun_name, "ucnv_convert_%d_%d", tenIdnex, index);
            ucnv_convert = (void (*)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*))dlsym(libFile, fun_name);
            if (ucnv_convert) {
            	log(" -------- fun_name  is %s ", fun_name);
               	return true;
            }
            index++;
            if (index > 9) {
            	index = 0;
            	tenIdnex++;
            	if (tenIdnex > 5)
					break;
            }
        }
        dlclose(libFile);
    }
    return false;
}

const char* gb23122utf8(const char * gb2312)
{
    if (ucnv_convert == NULL)
    {
        openIcuuc();
    }
    if (ucnv_convert)
    {
        int err_code = 0;
        int len = strlen(gb2312);
        char* str = new char[len * 2 + 10];
        memset(str, 0, len * 2 + 10);
        //utf8是目标编码，ucs2是原字符编码
        //buffer是存放转换出来的字符的缓冲，给了100字节
        //cbuf是要转换的字符串指针
        //errcode是错误编码，具体可网上搜索
        ucnv_convert("utf-8", "gb2312", str, len * 2 + 10, gb2312, len, &err_code);
        if (err_code == 0)
        {
            return str;
        }
    }
    char test[256] = "error";
    char* str = new char[30];
    strcpy(str, test);
    return str;
}

void WStrToUTF8(std::string& dest, const std::wstring& src) {
	dest.clear();
	for (size_t i = 0; i < src.size(); i++){
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff){
			dest.push_back(0xc0 | ((w >> 6)& 0x1f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0xffff){
			dest.push_back(0xe0 | ((w >> 12)& 0x0f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (sizeof(wchar_t) > 2 && w <= 0x10ffff){
			dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation
			dest.push_back(0x80| ((w >> 12) & 0x3f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else
			dest.push_back('?');
	}
}



