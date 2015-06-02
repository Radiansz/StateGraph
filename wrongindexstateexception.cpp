#include "wrongindexstateexception.h"
namespace States {


WrongIndexStateException::WrongIndexStateException()
{

    lastException = this;
}



WrongIndexStateException::WrongIndexStateException(const WrongIndexStateException& e) : StateException(e){

}


WrongIndexStateException::~WrongIndexStateException() throw(){

}

const char* WrongIndexStateException::what() const throw(){
    return "Impossible to find state with such an index";
}

}
