/**
 *    @file       phpjson.h
 *
 *    @brief      
 *    @details
 *
 *    @version    1.0
 *    @date       07/10/2014 10:08:56 AM
 *
 *    @author     wangtiefeng , wangtf418@163.com
 */
#ifndef _PHPJSON_H_
#define _PHPJSON_H_
#include <string>
#include <vector>
#include <map>

#define PJ_ERROR_OK 0
#define PJ_ERROR_ADD_ARRAY 1
#define PJ_ERROR_ADD_OBJECT 2
#define PJ_ERROR_DECODE 3
#define PJ_ERROR_NOT_UTF8 2

enum pj_type
{
	pjtype_null,	//null
	pjtype_bool,	//Bool
	pjtype_int,		//Integer
	pjtype_double,	//Integer
	pjtype_string,	//String
	pjtype_object,	//Vector
	pjtype_array	//Array

};

class phpjson;

typedef bool pj_bool;
typedef long long pj_int;
typedef double pj_double;
typedef std::string pj_string;

typedef std::vector<phpjson*> pj_array;
typedef std::map<pj_string, phpjson*> pj_object;

union pj_value
{
	pj_bool m_bool;
	pj_int m_int;
	pj_double m_double;
	pj_string *m_string;
	pj_array *m_array;
	pj_object *m_object;
};

enum pj_null_e {PJ_NULL};
enum pj_bool_e {PJ_BOOL};
enum pj_int_e {PJ_INT};
enum pj_double_e {PJ_DOUBLE};
enum pj_string_e {PJ_STRING};
enum pj_array_e {PJ_ARRAY};
enum pj_object_e {PJ_OBJECT};

class phpjson
{
public:
	phpjson(pj_null_e t);									//pjtype_null
	phpjson(pj_bool var, pj_bool_e t = PJ_BOOL);			//pjtype_bool
	phpjson(pj_int var, pj_int_e t = PJ_INT);				//pjtype_int
	phpjson(pj_double var, pj_double_e t = PJ_DOUBLE);		//pjtype_double
	phpjson(const pj_string &var, pj_string_e t = PJ_STRING);		//pjtype_string
	phpjson(pj_array_e t);									//pjtype_array
	phpjson(pj_object_e t = PJ_OBJECT);						//pjtype_object

public:
	virtual ~phpjson();

public:
	phpjson & operator[](const std::string name);
	phpjson & operator[](int index);
	pj_type get_type() {return m_type;}
	pj_value & get_value() {return m_value;}

public:
	//for array
	int add_null();
	int add_bool(pj_bool var);
	int add_int(pj_int var);
	int add_double(pj_double var);
	int add_string(const pj_string& var);
	int add(phpjson* json);

	//for object
	int add_null(const pj_string &name);
	int add_bool(const pj_string &name, pj_bool var);
	int add_int(const pj_string &name, pj_int var);
	int add_double(const pj_string &name, pj_double var);
	int add_string(const pj_string &name, const pj_string& var);
	int add(const pj_string &name, phpjson* json);

public:
	std::string encode();

protected:
	int escape(const pj_string &str, pj_string &esstr);

protected:
	pj_type m_type;
	pj_value m_value;

protected:
	size_t m_objseqno;
	std::vector<pj_object::iterator> m_objseq;
};

#endif
