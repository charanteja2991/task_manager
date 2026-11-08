#include <string>
#include <unordered_set>
#include <iostream>

struct task
{
    std::string title, description;
    unsigned long time_needed, priority;
	bool completed;
	std::unordered_set<std::string> keywords;

    /**************** default ctor & mtor START ****************/

    task() = default;
    task(const task&) = default;
    task(task&&) noexcept= default;
    
    task& operator= (const task&) = default;
    task& operator= (task&&) noexcept= default;

    /************ default ctor & mtor END ****************/

    task(std::string tit, std::string desc = "NULL", unsigned long tm_needed = 0, unsigned long pri = 0, bool isdone = false) :
        title(tit), description(desc), time_needed(tm_needed), priority(pri), completed(isdone) {}
    
};

class exception
{
    const std::string message;
public:
    exception() : message("classes.cpp -> exception::what()") {}
    exception(const std::string &msg) : message(msg) {}
    void what() { std::cerr << message << '\n'; }
};

class invalid_time_input : public exception
{
public:
    invalid_time_input() : exception("classes.cpp -> invalid_time_input") {}
    invalid_time_input(const std::string &msg) : exception(msg) {}
};