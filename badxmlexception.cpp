#include "badxmlexception.h"
namespace States {
    BadXMLException::BadXMLException()
    {
        error = "UnknownStateException";
        lastException = this;
    }

    BadXMLException::~BadXMLException() throw()
    {

    }

    BadXMLException::BadXMLException(const BadXMLException& e) : StateException(e){
    }

    const char* BadXMLException::what() const throw(){
        return "Error in the XML data.";
    }

}
