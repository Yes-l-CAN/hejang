#include "./include/CanCommunicate.hpp"

CanCommunicate::CanCommunicate(){}

CanCommunicate::CanCommunicate(const Communicate& obj){
    *this = obj;
}

CanCommunicate& operator=(const CanCommunicate& obj){

}

CanCommunicate::~CanCommunicate(){}

void CanCommunicate::cRecv(int fd)
{
    int ret = recv(fd, this->buffer, this->bufferSize, 0);
    if (ret < 0)
    {
        std::cerr << "couldn't recv client socket error: " << errno << ": " << std::strerror(errno)
                    << std::endl;
        throw(recvException);
    }
    else if (ret == 0)
    {
        std::cerr << "receive client socket closed " << std::endl;
        throw(recvException);
    }
    FD_CLR(i, &reads);
    close(i);
}

const char* CanCommunicate::recvException::what() const throw(){
    return ("Recv error");
}
