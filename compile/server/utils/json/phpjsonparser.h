/**
 *    @file       phpjsonparser.h
 *
 *    @brief      
 *    @details
 *
 *    @version    1.0
 *    @date       07/10/2014 22:32:56 PM
 *
 *    @author     wangtiefeng , wangtf418@163.com
 */
#ifndef _PHPJSONPARSER_H_
#define _PHPJSONPARSER_H_
#include "phpjson.h"

class phpjsonparser
{
public:
	phpjsonparser();
	virtual ~phpjsonparser();

public:
	phpjson * decode(unsigned char *str, int strlen);

protected:
	phpjson * parse(unsigned char *str, int strlen);
	phpjson * parse_object(unsigned char *str, int strlen);
	phpjson * parse_array(unsigned char *str, int strlen);
	phpjson * parse_key_value(unsigned char *str, int strlen, std::string &name);
	phpjson * parse_value(unsigned char *str, int strlen);
	phpjson * parse_basic_element(unsigned char *str, int strlen);

protected:
	int get_numtype(unsigned char *str, int strlen);
	int unescape(std::string &str);
	unsigned char * trim_begin(unsigned char *str, int strlen);
	unsigned char * trim_end(unsigned char *str, int strlen);
	unsigned int get_utf16(const std::string &strunicode);

protected:
	phpjson * m_json;
};

#endif
