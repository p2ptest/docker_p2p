/**
 *    @file       utf8code.h
 *
 *    @brief      
 *    @details
 *
 *    @version    1.0
 *    @date       07/10/2014 09:49:05 PM
 *
 *    @author      wangtiefeng, wangtf418@163.com
 */
#ifndef _UTF8CODE_H_
#define _UTF8CODE_H_

#include <string>
#include <vector>

#define UTF8CODE_END -1
#define UTF8CODE_ERROR -2

class utf8code
{
public:
	utf8code(const std::string &utf8str);
	virtual ~utf8code();

public:
	int get_next();

public:
	static int utf8_to_utf16(const std::string &utf8str, std::vector<unsigned short> &utf16str);

protected:
	std::string m_utf8str;
	size_t m_curpos;
};

#endif

