#ifndef CANCOMMUNICATION_HPP
#define CANCOMMUNICATION_HPP

class CanCommunication{
	protected:
		int		bufferSize = 300;
		char	buffer[bufferSize];
	public:
		void cRecv(int fd);
		virtual void cSend(int fd) = 0;
        class recvException : public std::exception{
            virtual const char* what() const throw();
        };
};

#define 