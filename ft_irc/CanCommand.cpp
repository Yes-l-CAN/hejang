#include "CanCommand.hpp"

CanCommand::CanCommand(){

}

CanCommand::CanCommand(const CanCommand& obj) {
	*this = obj;
}

CanCommand& CanCommand::operator=(const CanCommand& obj){
	if(*this != obj){

	}
	return *this;
}

CanCommand::~CanCommand(){

}

void	CanCommand::Pass(std::vector<std::string> argv){

}

int		CanCommand::Nick(std::vector<std::string> argv){

}

int		CanCommand::User(std::vector<std::string> argv){

}

void	CanCommand::Ping(std::vector<std::string> argv){

}

void	CanCommand::Pong(std::vector<std::string> argv){

}