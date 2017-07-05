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

std::string create_sql(int operat, vec_str_t v_keys, vec_str_t v_values)
{
    std::ostringstream str_keys, str_values;

    std::copy(v_keys.begin(), v_keys.end(), std::ostream_iterator<std::string>(str_keys,", "));
    std::copy(v_values.begin(), v_values.end(), std::ostream_iterator<std::string>(str_values,", "));

    if (operat == UPDATE) {
        std::string result;
        std::string str_sets;
        for (size_t i = 0; (i < v_keys.size()) && (i < v_values.size()); i++)
        {
            str_sets += v_keys[i] + "=" + v_values[i];
        }
        sprintf(&result[0], sql_operations[UPDATE].c_str(), TABLE, str_sets, CONDITION);
        return result;
    }

    return create_sql(operat, str_keys.str(), str_values.str());
}

std::string create_sql(int operat, std::string str_keys, std::string str_values)
{
    std::string result;

    if (operat == INSERT)
        sprintf(&result[0], sql_operations[INSERT].c_str(), TABLE, str_keys, str_values);
    else if (operat == SELECT)
        sprintf(&result[0], sql_operations[SELECT].c_str(), str_keys, TABLE);
    else if (operat == DELETE) {
        sprintf(&result[0], sql_operations[DELETE].c_str(), TABLE, CONDITION);
    }

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
