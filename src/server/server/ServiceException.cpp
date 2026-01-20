#include "ServiceException.h"
#include "microhttpd.h"
namespace server{
ServiceException::ServiceException (HttpStatus status, std::string msg) 
    : httpStatus(status),msg(msg) {    
}

HttpStatus ServiceException::status ()  {
    return httpStatus;
}

const char* ServiceException::what () {
    return msg.c_str();
}
}