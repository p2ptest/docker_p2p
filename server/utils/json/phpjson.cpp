/**
 *    @file       phpjson.cpp
 *
 *    @brief      
 *    @details
 *
 *    @version    1.0
 *    @date       07/10/2014 10:10:54 AM
 *
 *    @date       07/10/2014 10:10:54 AM
 */
#include "phpjson.h"
#include "utf8code.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define UTF8CODE_REVERSE16(us) (((us & 0xf) << 12) | (((us >> 4) & 0xf) << 8) | (((us >> 8) & 0xf) << 4) | ((us >> 12) & 0xf))
static const char HEX_DIGITS[] = "0123456789abcdef"; 

phpjson default_phpjson;

phpjson::phpjson(pj_object_e t /* = PJ_OBJECT */)
{
	m_type = pjtype_object;
	m_value.m_object = new pj_object;
}

phpjson::phpjson(pj_null_e t)
{
	m_type = pjtype_null;
}

phpjson::phpjson(pj_bool var, pj_bool_e t /* = PJ_BOOL */)
{
	m_type = pjtype_bool;
	m_value.m_bool = var;
}

phpjson::phpjson(pj_int var, pj_int_e t /* = PJ_INT */)
{
	m_type = pjtype_int;
	m_value.m_int = var;
}

phpjson::phpjson(pj_double var, pj_double_e t /* = PJ_DOUBLE */)
{
	m_type = pjtype_double;
	m_value.m_double = var;
}

phpjson::phpjson(const pj_string &var, pj_string_e t /* = PJ_STRING */)
{
	m_type = pjtype_string;
	m_value.m_string = new pj_string(var);
}

phpjson::phpjson(pj_array_e t)
{
	m_type = pjtype_array;
	m_value.m_array = new pj_array;
}

phpjson::~phpjson()
{
	switch (m_type)
	{
		case pjtype_string:
			{
				delete m_value.m_string;
				break;
			}
		case pjtype_object:
			{
				pj_object::iterator itr = m_value.m_object->begin();
				for (; itr != m_value.m_object->end(); itr ++)
				{
					delete itr->second;
				}
				delete m_value.m_object;
				break;
			}
		case pjtype_array:
			{
				size_t veclen = m_value.m_array->size();
				for (size_t i = 0; i < veclen; i ++)
				{
					delete m_value.m_array->at(i);
				}
				delete m_value.m_array;
				break;
			}
		default:
			{
				break;
			}
	}
}

phpjson & phpjson::operator[](const std::string name)
{
	if (m_type != pjtype_object)
	{
		return default_phpjson;
	}
	pj_object::iterator itr = m_value.m_object->find(name);
	if (itr != m_value.m_object->end())
	{
		return *(itr->second);
	}
	return default_phpjson;
}

phpjson & phpjson::operator[](int index)
{
	if (index < 0 || m_type != pjtype_array || (unsigned)index >= m_value.m_array->size())
	{
		return default_phpjson;
	}
	return *(m_value.m_array->at(index));
}

int phpjson::add_null()
{
	if (m_type != pjtype_array) return PJ_ERROR_ADD_ARRAY;
	m_value.m_array->push_back(new phpjson(PJ_NULL));
	return PJ_ERROR_OK;
}

int phpjson::add_bool(pj_bool var)
{
	if (m_type != pjtype_array) return PJ_ERROR_ADD_ARRAY;
	m_value.m_array->push_back(new phpjson(var, PJ_BOOL));
	return PJ_ERROR_OK;
}

int phpjson::add_int(pj_int var)
{
	if (m_type != pjtype_array) return PJ_ERROR_ADD_ARRAY;
	m_value.m_array->push_back(new phpjson(var, PJ_INT));
	return PJ_ERROR_OK;
}

int phpjson::add_double(pj_double var)
{
	if (m_type != pjtype_array) return PJ_ERROR_ADD_ARRAY;
	m_value.m_array->push_back(new phpjson(var, PJ_DOUBLE));
	return PJ_ERROR_OK;
}

int phpjson::add_string(const pj_string &var)
{
	if (m_type != pjtype_array) return PJ_ERROR_ADD_ARRAY;
	m_value.m_array->push_back(new phpjson(var, PJ_STRING));
	return PJ_ERROR_OK;
}
int phpjson::add(phpjson *json)
{
	if (m_type != pjtype_array || !json) 
	{
		if (json) delete json;
		return PJ_ERROR_ADD_ARRAY;
	}
	m_value.m_array->push_back(json);
	return PJ_ERROR_OK;
}

int phpjson::add_null(const pj_string &name)
{
	if (m_type != pjtype_object) return PJ_ERROR_ADD_OBJECT;
	phpjson *json = new phpjson(PJ_NULL);
	return add(name, json);
}

int phpjson::add_bool(const pj_string &name, pj_bool var)
{
	if (m_type != pjtype_object) return PJ_ERROR_ADD_OBJECT;
	phpjson *json = new phpjson(var, PJ_BOOL);
	return add(name, json);
}

int phpjson::add_int(const pj_string &name, const pj_int var)
{
	if (m_type != pjtype_object) return PJ_ERROR_ADD_OBJECT;
	phpjson *json = new phpjson(var, PJ_INT);
	return add(name, json);
}

int phpjson::add_double(const pj_string &name, const pj_double var)
{
	if (m_type != pjtype_object) return PJ_ERROR_ADD_OBJECT;
	phpjson *json = new phpjson(var, PJ_DOUBLE);
	return add(name, json);
}

int phpjson::add_string(const pj_string &name, const pj_string &var)
{
	if (m_type != pjtype_object) return PJ_ERROR_ADD_OBJECT;
	phpjson *json = new phpjson(var, PJ_STRING);
	return add(name, json);
}

int phpjson::add(const pj_string &name, phpjson *json)
{
	if (m_type != pjtype_object || !json)
	{
		if (json) delete json;
		return PJ_ERROR_ADD_OBJECT;
	}

	pj_object::iterator itr = m_value.m_object->find(name);
	if (itr == m_value.m_object->end())
	{
		json->m_objseqno = m_objseq.size();
		m_objseq.push_back((m_value.m_object->insert(make_pair(name,json))).first);
	}
	else
	{
		json->m_objseqno = itr->second->m_objseqno;
		m_objseq[json->m_objseqno] = itr;
		delete itr->second;
		(*(m_value.m_object))[name] = json;
	}

	return PJ_ERROR_OK;
}

std::string phpjson::encode()
{
	std::string str = "";
	std::string esstr = "";

	if (m_type == pjtype_null)
	{
		str += "null";
	}
	else if  (m_type == pjtype_bool)
	{
		str += m_value.m_bool? "true":"false";
	}
	else if (m_type == pjtype_int)
	{
		char buf[32] = {0};
		snprintf(buf, 32, "%lld", m_value.m_int);
		str += buf; 
	}
	else if (m_type == pjtype_double)
	{
		 
		char buf[32] = {0};
		snprintf(buf, 32, "%lf", m_value.m_double);
		str += buf; 
	}
	else if (m_type == pjtype_string)
	{
		if (escape(*m_value.m_string, esstr) != PJ_ERROR_OK)
		{
			return "";
		}
		str += "\"" + esstr + "\""; 
	}
	else if (m_type == pjtype_object)
	{
		str += "{";
		size_t veclen = m_objseq.size();
		std::string strtmp;
		for (size_t i = 0; i < veclen; i ++)
		{
			if (i != 0) str += ",";
			if (escape(m_objseq[i]->first, esstr) != PJ_ERROR_OK)
			{
				return "";
			}
			str += "\"" + esstr + "\":";
			strtmp = m_objseq[i]->second->encode();
			if (strtmp.empty())
			{
				return "";
			}
			str += strtmp;
		}
		str += "}";
	}
	else if (m_type == pjtype_array)
	{
		str += "[";
		size_t veclen = m_value.m_array->size();
		std::string strtmp;
		for (size_t i = 0; i < veclen; i ++)
		{
			if (i != 0) str += ",";
			strtmp = m_value.m_array->at(i)->encode();
			if (strtmp.empty())
			{
				return "";
			}
			str += strtmp;
		}
		str += "]";

	}
	return str;
}

int phpjson::escape(const pj_string &str, pj_string &esstr)
{
	if (str.empty())
	{
		return PJ_ERROR_OK;
	}

	std::vector<unsigned short> utr16str;
	int ret = utf8code::utf8_to_utf16(str, utr16str);
	if (ret != 0)
	{
		return PJ_ERROR_NOT_UTF8;
	}
	esstr.clear();
	size_t utf16num = utr16str.size();
	unsigned short us = 0;
	for (size_t i = 0; i < utf16num; i ++)
	{
		us = utr16str[i];	
		switch (us)
		{
			case '"':
				{
					esstr += "\\\"";
					break;
				}
			case '\\':
				{
					esstr += "\\\\";
					break;
				}
			case '/':
				{
					esstr += "\\/";
					break;
				}
			case '\b':
				{
					esstr += "\\b";
					break;
				}
			case '\f':
				{
					esstr += "\\f";
					break;
				}
			case '\n':
				{
					esstr += "\\n";
					break;
				}
			case '\r':
				{
					esstr += "\\r";
					break;
				}
			case '\t':
				{
					esstr += "\\t";
					break;
				}
			default:
				{
					if (us >= ' ' && (us & 127) == us)
					{
						esstr.append(1, (char)us);
					}
					else
					{
						esstr += "\\u";
						us = UTF8CODE_REVERSE16(us);
						esstr.append(1, HEX_DIGITS[us & ((1 << 4) - 1)]);
						us >>= 4;
						esstr.append(1, HEX_DIGITS[us & ((1 << 4) - 1)]);
						us >>= 4;
						esstr.append(1, HEX_DIGITS[us & ((1 << 4) - 1)]);
						us >>= 4;
						esstr.append(1, HEX_DIGITS[us & ((1 << 4) - 1)]);
						us >>= 4;
					}
					break;
				}
		}
	}

	return PJ_ERROR_OK;
}

