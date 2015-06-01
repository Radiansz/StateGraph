#ifndef BADXMLEXCEPTION_H
#define BADXMLEXCEPTION_H
#include "stateexception.h"
namespace States  {
class BadXMLException : public StateException
{
public:
    BadXMLException();
    BadXMLException(const BadXMLException&);
    virtual ~BadXMLException() throw();
    virtual const char* what() const throw();
};
}

#endif // BADXMLEXCEPTION_H
