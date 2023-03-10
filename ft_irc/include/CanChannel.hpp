#ifndef CAN_CHANNEL_HPP
#define CAN_CHANNEL_HPP

#include <iostream>
#include <exception>
#include <map>
#include "CanClient.hpp"

class CanClient;

class CanChannel
{

    private:
        std::string                 key;
        std::map<int, CanClient*>	clientList;
    public:
        CanChannel();
        ~CanChannel();
		CanChannel(const CanChannel& ref);
		CanChannel& operator=(const CanChannel& ref);

        const std::map<int, CanClient*>& getClientList(void) const;
        void addClientElement(const int fd, const CanClient *pNewClient);
        void deleteClientElement(const int fd);

    class addClientException: public std::exception
    {
        virtual const char*	what() const throw();
    }

    // class deleteClientException: public std::exception
    // {
    //     virtual const char*	what() const throw();
    // }
};

#endif