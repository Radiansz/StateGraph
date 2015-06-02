#include "unknownstateexception.h"
namespace States {


UnknownStateException::UnknownStateException()
{
    lastException = this;
}



UnknownStateException::UnknownStateException(const UnknownStateException& e) : StateException(e){
}


UnknownStateException::~UnknownStateException() throw(){

}

const char* UnknownStateException::what() const throw(){
    return "Impossible to find index of a state";
}

}
