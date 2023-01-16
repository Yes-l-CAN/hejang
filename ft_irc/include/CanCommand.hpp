#ifndef CAN_COMMAND_HPP
#define CAN_COMMAND_HPP

#define SUCCESS 1
#define FAIL  0
#define ERROR   -1

class CanCommand{

    private:
        int     result;
        int     isPass = false;

    public:
        CanCommand();
        CanCommand(const CanCommand& obj);
        CanCommand& operator=(const CanCommand& obj);
        ~CanCommand();
        void    Pass(std::vector<std::string> argv);
        int     Nick(std::vector<std::string> argv);
        int     User(std::vector<std::string> argv);
        void    Ping(std::vector<std::string> argv);
        void    Pong(std::vector<std::string> argv);

};

#endif