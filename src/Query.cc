#include "Query.h"
#include "Tigers.h"

tigers::Query::Query(tigers::Tigers& tigers, const tigers::AttributeFamilyName& af_name)
    : tigers_(tigers), attr_fam_name_(af_name) {

}

