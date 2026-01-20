#include "AbstractService.h"
#include "ServiceException.h"
namespace server{
AbstractService::AbstractService (const std::string& pattern) {
    setPattern(pattern);
}

AbstractService::~AbstractService() {
    
}

const std::string& AbstractService::getPattern ()  {
    return pattern;
}

void AbstractService::setPattern (const std::string& pattern) {
    this->pattern = pattern;
}

HttpStatus AbstractService::get (Json::Value& out, int id) {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

HttpStatus AbstractService::post (const Json::Value& in, int id) {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

HttpStatus AbstractService::put (Json::Value& out, const Json::Value& in) {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

HttpStatus AbstractService::remove (int id) {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}
}