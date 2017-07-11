#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <exception>

#include <stdio.h>

#include "picojson.h"
#include "sql_adapter.hpp"


const int BUFF_SIZE = 1024;
const std::string TABLE = "access";

// INSERT INTO таблиця (колонка1, [колонка2, ... ]) VALUES (значення1, [значення2, ...])
// SELECT C1 FROM T;
// UPDATE table_name SET column1 = value1, column2 = value2, ... WHERE condition;
// DELETE FROM table_name WHERE condition;

//std::string create_sql(int operat, vec_str_t v_keys, vec_str_t v_values);
//std::string create_sql(int operat, std::string str_keys, std::string str_values);

//std::string create_sql(std::string operat, std::string str_keys, std::string str_values);
//std::string create_sql(std::string operat, vec_str_t v_keys, vec_str_t v_values);

void set_condition(std::string new_cond)
{
    condition = new_cond;
}

std::string get_condition()
{
    if (condition.empty())
        return "";

    return (" WHERE " + condition);
}

std::string get_sql(std::string str_msg) {
    const char* msg = str_msg.c_str();
    std::string sql_line;

    picojson::value v;
    std::string err = picojson::parse(v, msg, msg + str_msg.size());
    if (!err.empty()) {
        std::cerr << "picojson err:" << err << ":\n";
        throw(std::exception());
    }

    try {
        sql_line = v.get("sql_line").to_str();
        std::cerr << "1 sql_line: " << sql_line << "\n";
        if ((sql_line == "null") || sql_line.empty()) {
            throw (std::exception());
        }
    }
    catch (...) {
        std::string type = v.get("type").to_str();
        std::string keys = v.get("keys").to_str();
        std::string vals = v.get("vals").to_str();
        sql_line = create_sql(type, keys, vals);
    }

    return sql_line;
}

std::string create_sql(int operat, vec_str_t v_keys, vec_str_t v_values)
{
    std::string str_keys, str_values;

    //std::copy(v_keys.begin(), v_keys.end(), std::ostream_iterator<std::string>(str_keys,", "));
    //std::copy(v_values.begin(), v_values.end(), std::ostream_iterator<std::string>(str_values,", "));
    str_keys = join(v_keys, ", ");
    str_values = join(v_values, ", ");

    if (operat == UPDATE) {
        char* buff = (char*)calloc(1, BUFF_SIZE);
        std::ostringstream keys_vals;

        for (size_t i = 0; (i < v_keys.size()) && (i < v_values.size()); i++)
        {
            if (i != 0) keys_vals << ", ";
            keys_vals << (v_keys[i] + " = " + v_values[i]);
        }

        sprintf(buff, sql_operations[UPDATE].c_str(),
            TABLE.c_str(), keys_vals.str().c_str(), get_condition().c_str());

        std::string result(buff);
        free(buff);

        return result;
    }

    //std::cerr << "str_keys:" << str_keys << ":\n";
    //std::cerr << "str_values:" << str_values << ":\n";

    return create_sql(operat, str_keys, str_values);
}

std::string create_sql(int operat, std::string str_keys, std::string str_values)
{
    //std::string result;
    char *buff = (char*)calloc(1, BUFF_SIZE);

    if (operat == INSERT) {
        sprintf(buff, sql_operations[operat].c_str(),
            TABLE.c_str(), str_keys.c_str(), str_values.c_str());
    }
    else if (operat == SELECT) {
        sprintf(buff, sql_operations[operat].c_str(),
            str_keys.c_str(), TABLE.c_str(), get_condition().c_str());
    }
    else if (operat == UPDATE) {
        std::vector<std::string> v_keys = str_to_vec(str_keys, ',');
        std::vector<std::string> v_values = str_to_vec(str_values, ',');

        return create_sql(operat, v_keys, v_values);
    }
    else if (operat == DELETE) {
        sprintf(buff, sql_operations[operat].c_str(),
            TABLE.c_str(), get_condition().c_str());
    }

    //std::cerr << "sql_operations[operat]:" << sql_operations[operat] << ":\n";
    //std::cerr << "buff:" << result << ":\n";

    std::string result(buff);
    free(buff);

    return result;
}

std::string create_sql(std::string str_operat, std::string str_keys, std::string str_values)
{
    int operat;

    if (str_operat == "INSERT")
        operat = INSERT;
    else if (str_operat == "SELECT")
        operat = SELECT;
    else if (str_operat == "UPDATE")
        operat = UPDATE;
    else if (str_operat == "DELETE")
        operat = DELETE;
    else
        operat = NONE;

    return create_sql(operat, str_keys, str_values);
}

std::string create_sql(std::string str_operat, vec_str_t v_keys, vec_str_t v_values)
{
    int operat;

    if (str_operat == "INSERT")
        operat = INSERT;
    else if (str_operat == "SELECT")
        operat = SELECT;
    else if (str_operat == "UPDATE")
        operat = UPDATE;
    else if (str_operat == "DELETE")
        operat = DELETE;
    else
        operat = NONE;

    return create_sql(operat, v_keys, v_values);
}

template <typename T>
std::string join(const T& v, const std::string& delim) {
    std::ostringstream s;
    for (const auto& i : v) {
        if (&i != &v[0]) {
            s << delim;
        }
        s << i;
    }
    return s.str();
}

std::vector<std::string> str_to_vec(std::string& str_str, char delim) {
    std::vector<std::string> vec;

    std::string item;
    std::stringstream ss;
    ss.str(str_str);

    while(std::getline(ss, item, delim))
        vec.push_back(item);

    return vec;
}
