#include "classes.cpp"

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <sys/types.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <thread>
#include <utility>

std::ofstream logfile("tmlog.log", std::ios::app);

#define loginfo logger(logfile, 'i', __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define logerr logger(logfile, 'e', __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define logwarn logger(logfile, 'w', __FILE__, __LINE__, __PRETTY_FUNCTION__)





// this is where all the main tasks addresses are lived.
std::vector<std::shared_ptr<task>> VEC_tasks; 

// these sort pointers according to priority and time
std::vector<std::shared_ptr<task>> 
	VEC_task_priority,
	VEC_task_time;

// hash map for string : *tasks that makes searching easy through keywords
std::unordered_map<std::string, std::vector<std::shared_ptr<task>>> UOMAP_global_keywords;










task string_to_task_object(std::string &str)
{
	task task_obj;
	std::string chunk;
	int where = 0;

	for(char &c: str)
	{
		if(c == '|' || c == ',')
		{
			switch(where)
			{
				case 0:
					task_obj.title = chunk;
				 	break;
				case 1:
					task_obj.description = chunk;
				 	break;
				case 2:
					task_obj.time_needed = std::stoul(chunk);
					break;
				case 3:
					task_obj.priority = std::stoul(chunk);
					break;
				case 4:
					task_obj.completed = std::stoi(chunk);
					break;
				case 5:
					task_obj.keywords.insert(chunk);
					break;
				default:
					std::cerr << "functions.cpp -> string_to_task_object() -> faulty variable 'where' values\n";
			}

			if(where != 5)
				where++;

			chunk.clear();
		}
		else
		{
			chunk += c;
		}
	}
	task_obj.keywords.insert(chunk);

	return task_obj;	
}










bool load_tasks_into_containers() noexcept(true)
{
	// opening tasks.txt file
	std::ifstream tasks_file_reader("tasks.txt");
	
	// if opened file doesnt open, this function returns false
	if(!tasks_file_reader.is_open())
	{
		std::cerr << "tasks.txt file not found\n";
		return false;
	}	
	
	std::string STR_line; 

	// string is broken down here and new TASK objects are created and pushed into containers
	while(std::getline(tasks_file_reader, STR_line))
	{
		// creating a temporary task object
		task task_obj = string_to_task_object(STR_line);

		VEC_tasks.push_back(std::make_shared<task>(task_obj)); // -------------------[]
		VEC_task_priority.push_back(VEC_tasks.back()); // __________________________/
        VEC_task_time.push_back(VEC_tasks.back()); // _____________________________/
			

		// keywords to store in unordred map so that searching globally would be easy
		for(auto &s: task_obj.keywords)
		{
			UOMAP_global_keywords[s].push_back(VEC_tasks.back());
		}
	}

	std::thread priority_sort_thread(
		[]()
		{
			std::sort(VEC_task_priority.begin(), VEC_task_priority.end());
		}
	);
	
	std::thread time_sort_thread(
		[]()
		{
			std::sort(VEC_task_time.begin(), VEC_task_time.end());
		}
	);
	
	priority_sort_thread.join();
	time_sort_thread.join(); 

	
	return true;
}










void show_tasks(std::vector<std::shared_ptr<task>> temp, int iterate = 10) noexcept(true)
{
	std::cout << "======================================================================================================\n";
	/************************** temporary code START ********************************/
	for(int i = 0; i < temp.size() && i < iterate; i++)
	{
		std::cout << temp[i]->title << "    "
				  << temp[i]->description << "    "
				  << temp[i]->time_needed << "    "
				  << temp[i]->priority << '\n';
	}
	/************************** temporary code END ********************************/
	std::cout << "======================================================================================================\n";
}










std::vector<std::string> get_input() noexcept(true)
{
	std::string input, chunk;
	std::cout << "\n>> ";
	std::getline(std::cin, input);

	std::vector<std::string> tokens;
	bool inside_cotations = false;

	for(char &c: input)
	{
		// condition: ["]<-- this is a test string"
		if(c == '"' && !inside_cotations)
		{
			inside_cotations = true;
		}

		// condition: "this is a test string -->["]
		else if(c == '"' && inside_cotations)
		{
			inside_cotations = false;
			if(!chunk.empty())
			{
				tokens.push_back(chunk);
				chunk.clear();
			}
		}

		// condition: this[]<--is a test "string"
		else if(c == ' ' && !inside_cotations && !chunk.empty())
		{
			tokens.push_back(chunk);
			chunk.clear();
		}
		else
		{
			// why this condition?
			if(c != '"')
				chunk += c;
		}
	}
	if(!chunk.empty())
		tokens.push_back(chunk);

	return tokens;
}










bool save_tasks_into_file()
{
	std::ofstream tasks_file_writer("tasks.txt", std::ios::out);
	return true;
}










unsigned long input_parse_time()
{
	std::string time, chunk;
	std::getline(std::cin, time);
	unsigned long UL_time = 0, number = 0;
    bool minute = false, hour = false, day = false;


	for(char &c: time)
	{
        if(!(c >= '0' && c <= '9'))
        {
            switch(c)
            {
                case 'd': case 'D':
                {
                    // okavela hour, minute, day false unte, we make day true
                    // this is so that only one time these letters are encountered.
                    // malli encounter aithe we return false
                    if(!hour && !minute && !day) // !minute && !hour is to avoid 12m15h kind... 
                    {
                       day = true;
                       UL_time += (number * 24 * 60);
                       number = 0;
                    }
                    else
                    {
                        logwarn << "throwing 'invalid_time_day_input'";
                        throw invalid_time_day_input("Invalid Time format\n");
                    }
                } break;
                case 'h': case 'H':
                {
                    // okavela hour, minute, day false unte, we make day true
                    // this is so that only one time these letters are encountered.
                    // malli encounter aithe we return false
                    if(!minute && !hour) // !minute is to avoid 12m15h kind... 
                    {
                        hour = true;
                    }
                    else
                    {
                        logwarn << "throwing 'invalid_time_day_input'";
                        throw invalid_time_day_input("Invalid Time format\n");
                    }
                    // to avoid 12d12345d....
                    if(day && number > 24)
                    {
                        logwarn << "throwing 'invalid_time_hour_input'";
                        throw invalid_time_hour_input("Invalid Hour format\n");
                    }
                    else
                    {
                        UL_time += (number * 60);
                        number = 0;
                    }
                } break;
                case 'm': case 'M':
                {   
                    // okavela hour, minute, day false unte, we make day true
                    // this is so that only one time these letters are encountered.
                    // malli encounter aithe we return false
                    if(!minute)
                    {
                        minute = true;
                    }
                    else
                    {
                        logwarn << "throwing 'invalid_time_input'";
                        throw invalid_time_input("Invalid Time format\n");
                    }

                    // to avoid 12d24h12342342m
                    if((day || hour) && number > 60)
                    {
                        logwarn << "throwing 'invalid_time_minute_input'";
                        throw invalid_time_minute_input("Invalid Minute format\n");
                    }
                    else
                    {
                        UL_time += number;
                        number = 0;
                    }
                } break;
                // okavela other than d, m, h letters osthe we return this as false string.
                default:
                {
                    logwarn << "throwing 'invalid_time_input'";
                    throw invalid_time_input("Invalid Time format\n");
                }
            } // switch end
        } // if end
        else
        {
            number *= 10;
            number += (c - '0');

            // to avoid 12d24h12342342 where minute doesnt have 'm' at the end
            if((day || hour) && number > 60)
            {
                logwarn << "throwing 'invalid_time_minute_input'";
                throw invalid_time_minute_input("Invalid Minute format\n");
            }
        } // else end
	} // range based for loop end
    
    /********************** check for 17374 and 12d15h12 kind of inputs... *********************************/ 
    // PROBLEM: will this work for 12345m ?
    // for strings like 1234, 12d15h12m, minute variable doesnt get envoked, so it stays on false... we modify the string based on that now...
    if(!minute && number > 0)
    {
        (day == true || hour == true) ? 
            UL_time += number : // this is for 12d15h12, last m got missed, for that cases, this line is needed
            UL_time = number; // this is for 1234, only numbers that are taken in as minutes
    }

	return UL_time;
}









void add_task_to_containers(task new_task)
{
    // this pushes into main vector, that is VEC_tasks....
    VEC_tasks.push_back(std::make_shared<task>(new_task));
    std::mutex log_writer_mutex;

    auto insert_task = [&log_writer_mutex](std::vector<std::shared_ptr<task>> &container, const char arg) mutable
    {
        int startindex = 0, endindex = container.size()-1, midindex;

        if(arg == 'p' || arg == 'P')
        {
            log_writer_mutex.lock();
            loginfo << "entered into priority (P) condition";
            log_writer_mutex.unlock();

            // this loops locates where should the object be inserted...
            while(startindex < endindex)
            {
                midindex = startindex + (endindex - startindex) / 2;

                if(container[midindex]->priority > VEC_tasks.back()->priority)
                {
                    endindex = midindex - 1;
                }
                else if(container[midindex]->priority < VEC_tasks.back()->priority)
                {
                    startindex = midindex + 1;
                }
            }

			container.insert(container.begin() + midindex, VEC_tasks.back());
        }
        else if(arg == 't' || arg == 'T')
        {
            log_writer_mutex.lock();
            loginfo << "entered into time (T) condition";
            log_writer_mutex.unlock();

            // this loops locates where should the object be inserted...
            while(startindex < endindex)
            {
                midindex = startindex + (endindex - startindex) / 2;

                if(container[midindex]->time_needed > VEC_tasks.back()->time_needed)
                {
                    endindex = midindex - 1;
                }
                else if(container[midindex]->time_needed < VEC_tasks.back()->time_needed)
                {
                    startindex = midindex + 1;
				}
            }

			container.insert(container.begin() + midindex, VEC_tasks.back());
        }
    };

    

    std::thread priority_insert_thread(insert_task, std::ref(VEC_task_priority), 'p');
    std::thread time_insert_thread(insert_task, std::ref(VEC_task_time), 't');

    priority_insert_thread.join();
    time_insert_thread.join();
}









std::unordered_set<std::string> input_parse_keywords()
{
    std::unordered_set<std::string> keywords;
    std::string user_input, chunk;
	std::cin.ignore();
    std::getline(std::cin, user_input);

    for(const char &c: user_input)
    {
        if(c == 32)
        {
            keywords.insert(chunk);
            chunk.clear();
        }
        else
        {
            chunk += c;
        }
    }
    keywords.insert(chunk);
    user_input.clear();
    chunk.clear();

    return keywords;
}










void create_new_task(const std::vector<std::string> &tokens)
{
    task new_task;

	try
	{
		std::cout << "Task Title: ";
		std::getline(std::cin, new_task.title);

		std::cout << "Task Description: ";
		std::getline(std::cin, new_task.description);

		std::cout << "Time needed to complete this task(XdXhXm): ";
		new_task.time_needed = input_parse_time();

		std::cout << "Priority of this task compared to other tasks: ";
		std::cin >> new_task.priority;

        std::cout << "Keyword to identify this task (space seperates keywords): ";
        new_task.keywords = input_parse_keywords();


        VEC_tasks.push_back(std::make_shared<task>(new_task));

        try
        {
            add_task_to_containers(new_task);
        }
        catch(...)
        {
            std::cerr << "Standard Exception: in function.cpp -> create_new_task()\n";
            logwarn << "Caught an exception";
        }
        
		std::cout << "Succesfully added Task into database\n";
	}
	catch(exception &excep)
	{
		excep.what();
	}
	catch(...)
	{
		std::cerr << "functions.cpp -> create_new_task() -> standard error probably...\n";
        logwarn << "Caught an exception";
	}
}










void start() noexcept(true)
{
    loginfo << "\n\n============ NEW START ============\n\n";
	// loads task objects into VEC_task, VEC_task_priority, VEC_task_time, UOMAP_global_keywords
	if(load_tasks_into_containers())
    {
        show_tasks(VEC_task_time);
    }
	else 
	{
		std::cerr << "functions.cpp -> start() -> Couldn't load tasks into containers\n";
        logerr << "Couldn't load task objects into containers";
	}

	while(true)
	{
		std::vector<std::string> tokens = get_input();

		try
		{
			if(tokens.at(0) == "new")
			{
				create_new_task(tokens);
				tokens.clear();
			}
			else if (tokens.at(0) == "listall")
			{
				// shows tasks through priority till its end (thats why VEC_task_priority.size() is given)
				// else only 10 tasks will be printed
				show_tasks(VEC_task_priority, VEC_task_priority.size());
			}
			else if (tokens.at(0) == "random")
			{

			}
			else if (tokens.at(0) == "help")
			{
				std::ifstream help_text_reader("commands.txt");
				std::string line;

				if(help_text_reader.is_open())
				{
					while(std::getline(help_text_reader, line))
					{
						std::cout << line << '\n';
					}	
				}
			}
			else if (tokens.at(0) == "exit")
			{
				std::cout << "Saving into file..." << '\n';
			//	save_tasks_into_file();
                
                loginfo << "============ END ============";
				return;
			}
			else
			{
				std::cerr << "Unknown command: " << tokens[0] << '\n';
			}
		}
		catch(...)
		{
        			
		}
	}
}
