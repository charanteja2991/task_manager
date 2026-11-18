#include <string>
#include <source_location>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <fstream>
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





class invalid_time_hour_input: public invalid_time_input
{
public:
    invalid_time_hour_input():invalid_time_input("classes.cpp -> invalid_time_hour_input") {}
    invalid_time_hour_input(const std::string &msg) : invalid_time_input(msg) {}
};





class invalid_time_day_input: public invalid_time_input
{
public:
    invalid_time_day_input(): invalid_time_input("classes.cpp -> invalid_time_day_input") {}
    invalid_time_day_input(const std::string &msg) : invalid_time_input(msg) {}
};





class invalid_time_minute_input: public invalid_time_input 
{
public:
    invalid_time_minute_input(): invalid_time_input("classes.cpp -> invalid_time_minute_input") {}
    invalid_time_minute_input(const std::string &msg) : invalid_time_input(msg) {}
};





struct messenger
{
    std::string filename;
    unsigned int line;
    std::string function_name;
    std::string message;
};





class logger
{
    std::string level;
    std::ofstream &logfile;

    std::string get_current_time_stamp()
    {
        /*************** COPIED FROM AI *********************/
        
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;

        ss << std::put_time(std::localtime(&now_c), "%d-%m-%Y %H:%M:%S");
        return ss.str();

        /****************************************************/ 
    }

public:

    logger(std::ofstream &logfile_, char c) : logfile(logfile_)
    {
        switch(c)
        {
            case 'e': case 'E':
                level = "ERROR";
                break;
            case 'w': case 'W':
                level = "WARNING";
                break;
            case 'i': case 'I':
                level = "INFO";
            default:
                {}
        }
    }

    template <typename data_type>
    logger& operator << (const data_type &data)
    {
        // logformat: [time & date] [level] = message
        logfile << "[" << get_current_time_stamp() << "] [" 
                << level << "] = "
                << data << std::endl;
                      
        return *this;
    }

    logger& operator << (const messenger &m)
    {
        // logformat: [time & date] <filename> (line) 'function_name' [level] = message
        logfile << "[" << get_current_time_stamp() << "] <"
                << m.filename << "> ("
                << m.line << ") '"
                << m.function_name << "' ["
                << level << "] = "
                << m.message << std::endl;

        return *this;
    }
};
