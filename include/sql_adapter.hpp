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

static vec_str_t sql_operations = {"",
                            "INSERT INTO %s (%s) VALUES (%s);",
                            "SELECT %s FROM %s%s;",
                            "UPDATE %s SET %s%s;",
                            "DELETE FROM %s%s;"};
static std::string condition;

void set_condition(std::string);
std::string get_condition();

std::string get_sql(std::string str_msg);

std::string create_sql(std::string operat, vec_str_t keys, vec_str_t values);
std::string create_sql(std::string operat, std::string keys, std::string values);

std::string create_sql(int operat, vec_str_t keys, vec_str_t values);
std::string create_sql(int operat, std::string keys, std::string values);

template <typename T>
std::string join(const T& v, const std::string& delim);
std::vector<std::string> str_to_vec(std::string& str_str, char delim);
