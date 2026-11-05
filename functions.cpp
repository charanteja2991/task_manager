#include "classes.cpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

// this is where all the main tasks addresses are lived.
std::vector<std::shared_ptr<task>> VEC_task; 

// these sort pointers according to priority and time
extern std::vector<std::weak_ptr<task>> 
	VEC_task_priority,
	VEC_task_time;

// hash map for string->tasks that makes searching easy through keywords
extern std::unordered_map<std::string, std::vector<std::weak_ptr<task>>> UOMAP_global_keywords;

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
				{
					task_obj.title = chunk;
				} break;
				case 1:
				{
					task_obj.description = chunk;
				} break;
				case 2:
				{
					//task_obj.time_needed = static_cast<unsigned long>(std::stoi(chunk));
				} break;
				case 3:
				{
					//task_obj.priority = static_cast<unsigned long>(std::stoi(chunk));
				} break;
				case 4:
				{
					//task_obj.completed = static_cast<bool>(std::stoi(chunk));
				} break;
				case 5:
				{
					task_obj.keywords.insert(chunk);
				} break;
				default:
				{
					std::cerr << "functions.cpp -> string_to_task_object() -> faulty variable 'where' values\n";
				}
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
		VEC_task.push_back(std::make_shared<task>(string_to_task_object(STR_line)));	
	}
	return true;
}

void show_tasks() noexcept(true)
{

	/************************** temporary code START *********************************/
	for(auto t: VEC_task)
	{
		std::cout << t->title << '\t'
				  << t->description << '\t'
				  << t->time_needed << '\t'
				  << t->priority << '\t'
			  	  << t->completed << '\n';	  
	}
	/************************** temporary code END *********************************/
}

inline std::string get_input() noexcept(true)
{
	/************************** temporary code START *********************************/

	std::string input;

	std::cout << ">> ";
	std::getline(std::cin, input);
	return input;

	/************************** temporary code END *********************************/
}

void start() noexcept(true)
{
	std::cout << "======================================================================================================\n";

	if(load_tasks_into_containers()) // loads task objects into VEC_task, VEC_task_priority, VEC_task_time, UOMAP_global_keywords
		show_tasks();
	else 
		return;

	/************************** temporary code START *********************************/

	std::string input = get_input(); 

	if (input == "new")
	{
		std::cout << "test";		
	}
	else if (input == "listall")
	{

	}
	else if (input == "random")
	{

	}
	else if (input == "help")
	{

	}
	else if (input == "exit")
	{

	}

	/************************** temporary code END *********************************/
}
