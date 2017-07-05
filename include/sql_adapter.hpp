#pragma once


class SqlAdapter
{
public:
private:
};

#define NONE 0
#define INSERT 1
#define SELECT 2
#define UPDATE 3
#define DELETE 4

typedef std::vector<std::string> vec_str_t;

vec_str_t sql_operations = {"",
                            "INSERT INTO %s (%s) VALUES (%s)",
                            "SELECT %s FROM %s",
                            "UPDATE %s SET %s WHERE %s",
                            "DELETE FROM %s WHERE %s"};

std::string create_sql(std::string operat, vec_str_t keys, vec_str_t values);
std::string create_sql(std::string operat, std::string keys, std::string values);

std::string create_sql(int operat, vec_str_t keys, vec_str_t values);
std::string create_sql(int operat, std::string keys, std::string values);
