#include <string>

struct task
{
    std::string title, description;
    unsigned long time_needed, priority;

    /**************** DEFAULTS ****************/

    task() = default;
    task(const task&) = default;
    task(const task&&) noexcept= default;
    
    task& operator= (const task&) = default;
    task& operator= (const task&&) noexcept= default;

    /************ DEFAULTS END ****************/

    task(std::string tit, std::string desc = "NULL", long tm_needed = 0, long pri = 0) :
        title(tit), description(desc), time_needed(tm_needed), priority(pri) {}
    
};