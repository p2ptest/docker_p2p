/**
 *    @file       utf8code.cpp
 *
 *    @brief      
 *    @details
 *
 *    @version    1.0
 *    @date       07/10/2014 10:22:38 PM
 *
 *    @author      wangtiefeng, wangtf418@163.com
 */
#include "utf8code.h"

utf8code::utf8code(const std::string &utf8str) : m_utf8str(utf8str), m_curpos(0)
{

}

utf8code::~utf8code()
{

}

int utf8code::get_next()
{
	if (m_curpos >= m_utf8str.length())
	{
		return (m_curpos == m_utf8str.length())? UTF8CODE_END:UTF8CODE_ERROR;
	}
	int cnt = (unsigned char)m_utf8str[m_curpos];
	m_curpos ++;

	//Zero continuation (0 to 127)
	if ((cnt & 0x80) == 0)
	{
		return cnt;
	}

	//One contination (128 to 2047) 
	int ret = 0;
	if ((cnt & 0xE0) == 0xC0)
	{
		if (m_curpos >= m_utf8str.length()) return UTF8CODE_ERROR;
		int cnt1 = (unsigned char)m_utf8str[m_curpos ++];
		cnt1 = ((cnt1 & 0xC0) == 0x80)? (cnt1 & 0x3F):UTF8CODE_ERROR;
		if (cnt1 < 0) return UTF8CODE_ERROR;
		ret = ((cnt & 0x1F) << 6) | cnt1;
		return ret >= 128 ? ret : UTF8CODE_ERROR;
	}

	//Two continuation (2048 to 55295 and 57344 to 65535)
	if ((cnt & 0xF0) == 0xE0)
	{
		if (m_curpos + 1 >= m_utf8str.length()) return UTF8CODE_ERROR;
		int cnt1 = (unsigned char)m_utf8str[m_curpos ++];
		cnt1 = ((cnt1 & 0xC0) == 0x80)? (cnt1 & 0x3F):UTF8CODE_ERROR;
		int cnt2 = (unsigned char)m_utf8str[m_curpos ++];
		cnt2 = ((cnt2 & 0xC0) == 0x80)? (cnt2 & 0x3F):UTF8CODE_ERROR;
		if (cnt1 < 0 || cnt2 < 0) return UTF8CODE_ERROR;
		ret = ((cnt & 0x0F) << 12) | (cnt1 << 6) | cnt2;
		return ((ret >= 2048) && (ret < 55296 || ret > 57343))? ret:UTF8CODE_ERROR;
	}

	//Three continuation (65536 to 1114111) 0x10FFFF is max Unicode
	if ((cnt & 0xF8) == 0xF0)
	{
		if (m_curpos + 2 >= m_utf8str.length()) return UTF8CODE_ERROR;
		int cnt1 = (unsigned char)m_utf8str[m_curpos ++];
		cnt1 = ((cnt1 & 0xC0) == 0x80)? (cnt1 & 0x3F):UTF8CODE_ERROR;
		int cnt2 = (unsigned char)m_utf8str[m_curpos ++];
		cnt2 = ((cnt2 & 0xC0) == 0x80)? (cnt2 & 0x3F):UTF8CODE_ERROR;
		int cnt3 = (unsigned char)m_utf8str[m_curpos ++];
		cnt3 = ((cnt3 & 0xC0) == 0x80)? (cnt3 & 0x3F):UTF8CODE_ERROR;
		if (cnt1 < 0 || cnt2 < 0 || cnt3 < 0) return UTF8CODE_ERROR;
		ret = ((cnt & 0x0F) << 18) | (cnt1 << 12) | (cnt2 << 6) | cnt3;
		return (ret >= 65536 && ret <= 1114111)? ret:UTF8CODE_ERROR;
	}
	return UTF8CODE_ERROR;
}

int utf8code::utf8_to_utf16(const std::string &utf8str, std::vector<unsigned short> &utf16str)
{
	utf8code code(utf8str);
	int cnt = 0;
	utf16str.clear();
	while (1)
	{
		cnt = code.get_next();
		if (cnt < 0)
		{
			return (cnt == UTF8CODE_END)? 0:-1;
		}
		if (cnt < 0x10000)
		{
			utf16str.push_back((unsigned short)cnt);
		}
		else
		{
			cnt -= 0x10000;
			utf16str.push_back((unsigned short)(0xD800 | (cnt >> 10)));
			utf16str.push_back((unsigned short)(0xDC00 | (cnt & 0x3FF)));
		}
	}
	return 0;
}

