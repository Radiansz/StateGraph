#ifndef STATEEXCEPTION_H
#define STATEEXCEPTION_H
#include <exception>


namespace States {


class StateException : public std::exception
{
protected:
    static StateException* lastException;
public:

    StateException() ;
    StateException(const StateException&);
    StateException& operator= (const StateException&) ;
    virtual ~StateException() throw();
    virtual const char* what() const throw() ;
    static StateException* getLastException();
};
}
#endif // STATEEXCEPTION_H
