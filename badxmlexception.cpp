#include "badxmlexception.h"
namespace States {
    BadXMLException::BadXMLException()
    {
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
