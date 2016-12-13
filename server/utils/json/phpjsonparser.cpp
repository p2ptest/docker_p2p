/**
 *    @file       phpjsonparser.cpp
 *
 *    @brief      
 *    @details
 *
 *    @version    1.0
 *    @date       07/10/2014 22:32:56 PM
 *
 *    @author     wangtiefeng , wangtf418@163.com
 */
#include "phpjsonparser.h"
#include "utf8code.h"
#include <string.h>
#include <stdlib.h>

#define __ -1		/* the universal error code */  

enum classes {
    C_SPACE,  /* space */
    C_WHITE,  /* other whitespace */
    C_LCURB,  /* {  */
    C_RCURB,  /* } */
    C_LSQRB,  /* [ */
    C_RSQRB,  /* ] */
    C_COLON,  /* : */
    C_COMMA,  /* , */
    C_QUOTE,  /* " */
    C_BACKS,  /* \ */
    C_SLASH,  /* / */
    C_PLUS,   /* + */
    C_MINUS,  /* - */
    C_POINT,  /* . */
    C_ZERO ,  /* 0 */
    C_DIGIT,  /* 123456789 */
    C_LOW_A,  /* a */
    C_LOW_B,  /* b */
    C_LOW_C,  /* c */
    C_LOW_D,  /* d */
    C_LOW_E,  /* e */
    C_LOW_F,  /* f */
    C_LOW_L,  /* l */
    C_LOW_N,  /* n */
    C_LOW_R,  /* r */
    C_LOW_S,  /* s */
    C_LOW_T,  /* t */
    C_LOW_U,  /* u */
    C_ABCDF,  /* ABCDF */
    C_E,      /* E */
    C_ETC,    /* everything else */
    NR_CLASSES
};

static const int ascii_class[128] = {
/*
    This array maps the 128 ASCII characters into character classes.
    The remaining Unicode characters should be mapped to C_ETC.
    Non-whitespace control characters are errors.
*/
    __,      __,      __,      __,      __,      __,      __,      __,
    __,      C_WHITE, C_WHITE, __,      __,      C_WHITE, __,      __,
    __,      __,      __,      __,      __,      __,      __,      __,
    __,      __,      __,      __,      __,      __,      __,      __,

    C_SPACE, C_ETC,   C_QUOTE, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_PLUS,  C_COMMA, C_MINUS, C_POINT, C_SLASH,
    C_ZERO,  C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT,
    C_DIGIT, C_DIGIT, C_COLON, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,

    C_ETC,   C_ABCDF, C_ABCDF, C_ABCDF, C_ABCDF, C_E,     C_ABCDF, C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_LSQRB, C_BACKS, C_RSQRB, C_ETC,   C_ETC,

    C_ETC,   C_LOW_A, C_LOW_B, C_LOW_C, C_LOW_D, C_LOW_E, C_LOW_F, C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_LOW_L, C_ETC,   C_LOW_N, C_ETC,
    C_ETC,   C_ETC,   C_LOW_R, C_LOW_S, C_LOW_T, C_LOW_U, C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_LCURB, C_ETC,   C_RCURB, C_ETC,   C_ETC
};

static const unsigned char firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

enum number_type
{
	NUM_TYPE_INT,
	NUM_TYPE_DOUBLE
};

phpjsonparser::phpjsonparser()
{
	m_json = NULL;
}

phpjsonparser::~phpjsonparser()
{
	if (m_json)
	{
		delete m_json;
	}
}

phpjson * phpjsonparser::decode(unsigned char *str, int strlen)
{
	if (m_json)
	{
		delete m_json;
		m_json = NULL;
	}

	if (!str || strlen <= 0)
	{
		return NULL;
	}

	std::vector<unsigned short> utr16str;
	int ret = utf8code::utf8_to_utf16(std::string((char*)str, strlen), utr16str);
	if (ret != 0)
	{
		return NULL;	//contain not utf8 char
	}
	m_json = parse(str, strlen);
	return m_json;
}

phpjson * phpjsonparser::parse(unsigned char *str, int strlen)
{
	unsigned char *end = str + strlen;
	unsigned char *begin = trim_begin(str, strlen);
	if (!begin || begin == end) return NULL;

	strlen = strlen - (begin - str);
	str = begin;
	phpjson *json = NULL;
	switch (*str)
	{
		case '{':
			{
				json = parse_object(str, strlen);
				break;
			}
		case '[':
			{
				json = parse_array(str, strlen);
				break;
			}
		default:
			{
				return NULL;
				break;
			}
	}
	return json;
}

phpjson * phpjsonparser::parse_object(unsigned char *str, int strlen)
{
	unsigned char *end = str + strlen - 1;
	while (end != str)
	{
		if (*end >= 128) return NULL;
		if (ascii_class[*end] == C_WHITE || ascii_class[*end] == C_SPACE)
		{
			end --;
		}
		else
		{
			break;
		}
	}
	if (end == str) return NULL;
	if (*end != '}') return NULL;
	phpjson *json = new phpjson;

	str ++;
	strlen = end - str;;
	unsigned char *cur = str;

	int depth = 0;
	int flag = 1;
	while (cur != end)
	{
		switch (*cur)
		{
			case '\\':
				{
					cur += (end - cur >= 2)? 2:1;
					break;
				}
			case '"':
				{
					depth += flag;
					flag = flag * -1;
					cur ++;
					break;
				}
			case '{':
			case '[':
				{
					if (flag > 0)
					{
						depth ++;
					}
					cur ++;
					break;

				}
			case '}':
			case ']':
				{
					if (flag > 0)
					{
						depth --;
					}
					cur ++;
					break;

				}
			case ',':
				{
					if (depth == 0)
					{
						std::string name;
						phpjson *jsontmp = parse_key_value(str, cur - str, name);
						if (jsontmp)
						{
							if (unescape(name) != 0)
							{
								delete jsontmp;
								delete json;
								return NULL;
							}
							else
							{
								json->add(name, jsontmp);
							}
						}
						str = cur + 1;
						cur = str;
					}
					else
					{
						cur ++;
					}
					break;
				}
			default:
				{
					cur ++;
					break;
				}
		}
	}
	if (cur != str && depth == 0)
	{
		if (depth == 0)
		{
			std::string name;
			phpjson *jsontmp = parse_key_value(str, cur - str, name);
			if (jsontmp)
			{
				json->add(name, jsontmp);
			}
		}
	}
	return json;
}

phpjson * phpjsonparser::parse_array(unsigned char *str, int strlen)
{
	unsigned char *end = str + strlen - 1;
	while (end != str)
	{
		if (*end >= 128) return NULL;
		if (ascii_class[*end] == C_WHITE || ascii_class[*end] == C_SPACE)
		{
			end --;
		}
		else
		{
			break;
		}
	}
	if (end == str) return NULL;
	if (*end != ']') return NULL;
	phpjson *json = new phpjson(PJ_ARRAY);

	str ++;
	strlen -= 2;
	unsigned char *cur = str;
	end =  str + strlen;

	int depth = 0;
	int flag = 1;
	while (cur != end)
	{
		switch (*cur)
		{
			case '\\':
				{
					cur += (end - cur >= 2)? 2:1;
					break;
				}
			case '"':
				{
					depth += flag;
					flag = flag * -1;
					cur ++;
					break;
				}
			case '{':
			case '[':
				{
					if (flag > 0)
					{
						depth ++;
					}
					cur ++;
					break;

				}
			case '}':
			case ']':
				{
					if (flag > 0)
					{
						depth --;
					}
					cur ++;
					break;

				}
			case ',':
				{
					if (depth == 0)
					{
						phpjson *jsontmp = parse_value(str, cur - str);
						if (jsontmp)
						{
							json->add(jsontmp);
						}
						str = cur + 1;
						cur = str;
					}
					else
					{
						cur ++;
					}
					break;
				}
			default:
				{
					cur ++;
					break;
				}
		}
	}
	if (cur != str && depth == 0)
	{
		if (depth == 0)
		{
			phpjson *jsontmp = parse_value(str, cur - str);
			if (jsontmp)
			{
				json->add(jsontmp);
			}
		}
	}
	return json;

}


phpjson * phpjsonparser::parse_key_value(unsigned char *str, int strlen, std::string &name)
{
	unsigned char *end = str + strlen;
	unsigned char *cur = str;
	while (cur != end)
	{
		switch (*cur)
		{
			case '\\':
				{
					cur += (end - cur >= 2)? 2:1;
					break;
				}
			case ':':
				{
					unsigned char *name_s = str;
					unsigned char *name_e = cur;
					name_s = trim_begin(name_s, name_e - name_s);
					if (!name_s || name_s == name_e) return NULL;
					name_e = trim_end(name_s, name_e - name_s);
					if (!name_e || name_s == name_e) return NULL;
					if (name_e - name_s < 2 || *name_s != '"' || *(name_e - 1) != '"') return NULL;
					name.assign((char*)(name_s + 1), name_e - name_s - 2);

					cur ++;
					phpjson *json = NULL;
					if (end > cur)
					{
						json = parse_value(cur, end - cur);
					}
					return json;
					break;
				}
			default:
				{
					cur ++;
					break;
				}

		}
	}
	return NULL;
}

phpjson * phpjsonparser::parse_value(unsigned char *str, int strlen)
{
	unsigned char *end = str + strlen;
	unsigned char *begin = trim_begin(str, strlen);
	if (!begin || begin == end) return NULL;

	strlen = strlen - (begin - str);
	str = begin;
	phpjson *json = NULL;
	switch (*str)
	{
		case '{':
			{
				json = parse_object(str, strlen);
				break;
			}
		case '[':
			{
				json = parse_array(str, strlen);
				break;
			}
		case '"':
			{
				end = trim_end(str, end - str);
				if (!end || end == str) return NULL;
				if (end - str < 2 || *str != '"' || *(end - 1) != '"') return NULL;
				std::string strvalue = std::string(std::string((char*)(str + 1), end - str - 2));
				if (unescape(strvalue) != 0)
				{
					return NULL;
				}
				json = new phpjson(strvalue, PJ_STRING);
				break;
			}
		default:
			{
				json = parse_basic_element(str, strlen);
				break;
			}
	}
	return json;
}

phpjson * phpjsonparser::parse_basic_element(unsigned char *str, int strlen)
{
	unsigned char *end = str + strlen;
	unsigned char *begin = trim_begin(str, strlen);
	if (!begin || begin == end) return NULL;

	end = trim_end(begin, end - begin);
	if (!end || end == begin) return NULL;

	strlen = end - begin;
	str = begin;

	switch (strlen)
	{
		case 5:
			{
				if (strncmp("false", (char*)str, 5) == 0)
				{
					return new phpjson(false, PJ_BOOL);
				}
				break;
			}
		case 4:
			{
				if (strncmp("true", (char*)str, 4) == 0)
				{
					return new phpjson(true, PJ_BOOL);
				}
				else if (strncmp("null", (char*)str, 4) == 0)
				{
					return new phpjson(PJ_NULL);
				}
				break;
			}
		default:
			{
				break;
			}
	}

	int numtype = get_numtype(str, strlen);	
	switch (numtype)
	{
		case NUM_TYPE_INT:
			{
				long long value = strtoll((char*)str, NULL, 10);
				return new phpjson(value, PJ_INT);
				break;
			}
		case NUM_TYPE_DOUBLE:
			{
				double value = strtold((char*)str, NULL);
				return new phpjson(value, PJ_DOUBLE);
				break;
			}
		default:
			{
				return NULL;
				break;
			}
	}
	return NULL;
}

int phpjsonparser::get_numtype(unsigned char *str, int strlen)
{
	unsigned char *end = str + strlen;
	if (ascii_class[*str] != C_DIGIT && ascii_class[*str] != C_ZERO) return -1;
	int type = NUM_TYPE_INT;
	int expnum = 0;
	int pointnum = 0;
	while (str != end)
	{
		if (*str >= 128) return -1;
		switch (ascii_class[*str])
		{
			case C_ZERO:
			case C_DIGIT:
				{
					str ++;
					break;
				}
			case C_POINT:
				{
					pointnum ++;
					if (pointnum > 1) return -1;
					str ++;
					type = NUM_TYPE_DOUBLE;
					break;
				}
			case C_LOW_E:
			case C_E:
				{
					str ++;
					expnum ++;
					if (expnum > 1) return -1;
					type = NUM_TYPE_DOUBLE;
					break;
				}
			default:
				{
					return -1;
					break;
				}
		}
	}
	if (pointnum > 0 && expnum > 0)
	{
		type = NUM_TYPE_DOUBLE;
	}
	return type;
}

int phpjsonparser::unescape(std::string &str)
{
	size_t strlen = str.length();
	std::string unesstr;
	size_t i = 0;
	while (i < strlen)
	{
		if (str[i] != '\\')
		{
			unesstr.append(1, str[i]);
			i ++;
			continue;	
		}
		i ++;
		if (i == strlen) break;
		switch (str[i])
		{
			case '"':
				{
					unesstr.append(1, '"');
					i ++;
					break;
				}
			case '\\':
				{
					unesstr.append(1, '\\');
					i ++;
					break;
				}
			case '/':
				{
					unesstr.append(1, '/');
					i ++;
					break;
				}
			case 'b':
				{
					unesstr.append(1, '\b');
					i ++;
					break;
				}
			case 'f':
				{
					unesstr.append(1, '\f');
					i ++;
					break;
				}
			case 'n':
				{
					unesstr.append(1, '\n');
					i ++;
					break;
				}
			case 'r':
				{
					unesstr.append(1, '\r');
					i ++;
					break;
				}
			case 't':
				{
					unesstr.append(1, '\t');
					i ++;
					break;
				}
			case 'u':	//unicode
				{
					if (strlen - i < 5) return -1;
					unsigned utf16 = get_utf16(str.substr(i + 1, 4));
					if (utf16 > 0xFFFF) return -1;
					i += 5; 
					if ((utf16 >= 0xDC00 && utf16 <= 0xDFFF)) return -1;	//trail surrogates
					if ((utf16 >= 0xD800 && utf16 <= 0xDBFF))				//lead surrogates (UTF16 surrogate pairs)
					{
						if (strlen - i < 6) return -1;
						if (str[i] != '\\' || str[i + 1] != 'u') return -1;
						unsigned utf16tail = get_utf16(str.substr(i + 2, 4));
						if (utf16tail > 0xFFFF) return -1;
						i += 6;
						if (utf16tail < 0xDC00 || utf16tail > 0xDFFF) return -1;	//not trail surrogates
						utf16 = 0x10000 + (((utf16tail & 0x3FF) << 10) | (utf16 & 0x3FF));
					}
	int len = 0;
					if (utf16 < 0x80) len = 1;
					else if (utf16 < 0x800) len = 2;
					else if (utf16 < 0x10000) len = 3;
					else len = 4;
					char ch[4];
					switch (len)
					{
						case 4: ch[3] = ((utf16 | 0x80) & 0xBF); utf16 >>= 6;
						case 3: ch[2] = ((utf16 | 0x80) & 0xBF); utf16 >>= 6;
						case 2: ch[1] = ((utf16 | 0x80) & 0xBF); utf16 >>= 6;
						case 1: ch[0] = (utf16 | firstByteMark[len]);
					}
					for (int j = 0; j < len; j ++) unesstr.append(1, ch[j]);

					break;
				}
			default:
				{
					unesstr.append(1, str[i]);
					i ++;
					break;
				}
		}
	}
	str = unesstr;
	return 0;
}

unsigned char * phpjsonparser::trim_begin(unsigned char *str, int strlen)
{
	unsigned char *end = str + strlen;
	while (str != end)
	{
		if (*str >= 128) return NULL;
		if (ascii_class[*str] == C_WHITE || ascii_class[*str] == C_SPACE)
		{
			str ++;
		}
		else
		{
			break;
		}
	}

	return str;
}

unsigned char * phpjsonparser::trim_end(unsigned char *str, int strlen)
{
	unsigned char *end = str + strlen - 1;
	str = str - 1;
	while (str != end)
	{
		if (*end >= 128) return NULL;
		if (ascii_class[*end] == C_WHITE || ascii_class[*end] == C_SPACE)
		{
			end --;
		}
		else
		{
			break;
		}
	}

	return (++end);
}

unsigned int phpjsonparser::get_utf16(const std::string &strunicode)
{
	if (strunicode.size() != 4)
	{
		return -1;
	}
	char c;
	unsigned u = 0;
	for (int i = 0; i < 4; i ++)
	{
		c = strunicode[i];
		if (c >= '0' && c <= '9') u += c - '0';
		else if (c >= 'a' && c <= 'f') u += 10 + c - 'a';
		else if (c >= 'A' && c <= 'F') u += 10 + c - 'A';
		else return -1;
		if (i != 3)	//i=0,1,2
		{
			u = u << 4;
		}
	}
	return u;
}

