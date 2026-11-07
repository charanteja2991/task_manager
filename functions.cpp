#include "classes.cpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <thread>

// this is where all the main tasks addresses are lived.
std::vector<std::shared_ptr<task>> VEC_tasks; 

// these sort pointers according to priority and time
std::vector<std::shared_ptr<task>> 
	VEC_task_priority,
	VEC_task_time;

// hash map for string->tasks that makes searching easy through keywords
std::unordered_map<std::string, std::vector<std::shared_ptr<task>>> UOMAP_global_keywords;

task string_to_task_object(std::string &str)
{
	//std::cerr << str << '\n';
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
			std::sort(VEC_task_priority.begin(), VEC_task_priority.end(),
				[](std::shared_ptr<task> &a, std::shared_ptr<task> &b)
				{
					return a->priority > b->priority;
				}
			);
		}
	);
	
	std::thread time_sort_thread(
		[]()
		{
			std::sort(VEC_task_time.begin(), VEC_task_time.end(),
				[](std::shared_ptr<task> &a, std::shared_ptr<task> &b)
				{
					return a->time_needed > b->time_needed;
				}
			);
		}
	);
	
	priority_sort_thread.join();
	time_sort_thread.join(); 
	
	return true;
}

void show_tasks(std::vector<std::shared_ptr<task>> temp, int iterate = 10) noexcept(true)
{
	std::cout << "======================================================================================================\n";
	/************************** temporary code START *********************************/
	for(int i = 0; i < temp.size() && i < iterate; i++)
	{
		std::cout << temp[i]->title << "    "
				  << temp[i]->description << "    "
				  << temp[i]->time_needed << "    "
				  << temp[i]->priority << '\n';
	}
	/************************** temporary code END *********************************/
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
		else if(c == '"' && inside_cotations && !chunk.empty())
		{
			inside_cotations = false;
			tokens.push_back(chunk);
			chunk.clear();
		}

		// condition: this[]<--is a test "string"
		else if(c == ' ' && !inside_cotations && !chunk.empty())
		{
			tokens.push_back(chunk);
			chunk.clear();
		}
		else
		{
			if(c != ' ' && !inside_cotations)
				chunk += c;
		}
	}
	if(!chunk.empty())
		tokens.push_back(chunk);

	for(auto s: tokens)
		std::cout << s << "\n";

	return tokens;
}

bool save_tasks_into_file()
{
	std::ofstream tasks_file_writer("tasks.txt", std::ios::out);
	return true;
}

void start() noexcept(true)
{
	if(load_tasks_into_containers()) // loads task objects into VEC_task, VEC_task_priority, VEC_task_time, UOMAP_global_keywords
		show_tasks(VEC_task_priority);
	else {
		std::cerr << "functions.cpp -> start() -> Couldn't load tasks into containers\n";
		return;
	}

	std::vector<std::string> tokens = get_input();

	/************************** temporary code START *********************************

	if (input == "new")
	{
		std::cout << "test";		
	}
	else if (input == "listall")
	{
		show_tasks(VEC_task_priority, VEC_task_priority.size());
	}
	else if (input == "random")
	{

	}
	else if (input == "help")
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
	else if (input == "exit")
	{
		std::cout << "Saving into file..." << '\n';
	//	save_tasks_into_file();
		return;
	}

	/************************** temporary code END *********************************/
}
