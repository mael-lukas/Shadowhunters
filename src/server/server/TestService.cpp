#include "TestService.h"

namespace server{
TestService::TestService () : AbstractService("/test") {
    
}

HttpStatus TestService::get (Json::Value& out, int id) {
    out["1"] = true;
    out["dskj"] =3;
    out["whaaaa"] = "wheeeeee";
    return HttpStatus::OK;
}
}