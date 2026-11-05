#include <string>
#include <unordered_set>

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

template <typename data_type1, typename data_type2, typename data_type3>
struct triplet
{
    data_type1 first;
    data_type2 second;
    data_type3 third;
};
