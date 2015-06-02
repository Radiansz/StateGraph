#include "stateexception.h"
namespace States {

StateException* StateException::lastException;

StateException::StateException() : std::exception()
{
    lastException = this;

}

StateException::~StateException() throw()
{

}


StateException::StateException(const StateException& e) : std::exception(e){

}

StateException& StateException::operator= (const StateException& e){
    return *this;
}

const char* StateException::what() const throw() {
    return "Unknown exception";
}

StateException* StateException::getLastException(){
    return lastException;
}
}
