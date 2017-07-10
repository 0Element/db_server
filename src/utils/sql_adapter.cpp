#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <stdio.h>

#include "sql_adapter.hpp"


const int BUFF_SIZE = 1024;
const std::string TABLE = "access";
const std::string CONDITION = "1=1";

// INSERT INTO таблиця (колонка1, [колонка2, ... ]) VALUES (значення1, [значення2, ...])
// SELECT C1 FROM T;
// UPDATE table_name SET column1 = value1, column2 = value2, ... WHERE condition;
// DELETE FROM table_name WHERE condition;

//std::string create_sql(int operat, vec_str_t v_keys, vec_str_t v_values);
//std::string create_sql(int operat, std::string str_keys, std::string str_values);

//std::string create_sql(std::string operat, std::string str_keys, std::string str_values);
//std::string create_sql(std::string operat, vec_str_t v_keys, vec_str_t v_values);

char* create_sql(int operat, vec_str_t v_keys, vec_str_t v_values)
{
    std::string str_keys, str_values;

    //std::copy(v_keys.begin(), v_keys.end(), std::ostream_iterator<std::string>(str_keys,", "));
    //std::copy(v_values.begin(), v_values.end(), std::ostream_iterator<std::string>(str_values,", "));
    str_keys = join(v_keys, ", ");
    str_values = join(v_values, ", ");

    if (operat == UPDATE) {
        char* result = (char*)calloc(1, 1024);
        std::ostringstream keys_vals;

        for (size_t i = 0; (i < v_keys.size()) && (i < v_values.size()); i++)
        {
            if (i != 0) keys_vals << ", ";
            keys_vals << (v_keys[i] + " = " + v_values[i]);
        }

        sprintf(result, sql_operations[UPDATE].c_str(),
            TABLE.c_str(), keys_vals.str().c_str(), CONDITION.c_str());

        return result;
    }

    //std::cerr << "str_keys:" << str_keys << ":\n";
    //std::cerr << "str_values:" << str_values << ":\n";

    return create_sql(operat, str_keys, str_values);
}

char* create_sql(int operat, std::string str_keys, std::string str_values)
{
    //std::string result;
    char *result = (char*)calloc(1, 1024);

    if (operat == INSERT) {
        sprintf(result, sql_operations[operat].c_str(),
            TABLE.c_str(), str_keys.c_str(), str_values.c_str());
    }
    else if (operat == SELECT) {
        sprintf(result, sql_operations[operat].c_str(),
            str_keys.c_str(), TABLE.c_str());
    }
    else if (operat == DELETE) {
        sprintf(result, sql_operations[operat].c_str(),
            TABLE.c_str(), CONDITION.c_str());
    }

    //std::cerr << "sql_operations[operat]:" << sql_operations[operat] << ":\n";
    //std::cerr << "result:" << result << ":\n";

    return result;
}

char* create_sql(std::string str_operat, std::string str_keys, std::string str_values)
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

char* create_sql(std::string str_operat, vec_str_t v_keys, vec_str_t v_values)
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
