#include "driver.h"
#include "parser.tab.hh"
using namespace std;
using namespace yy;



int parser_driver::parse(const string &archivo){
    file = archivo;
    scannerInit();
    parser parser(*this);
    parser.set_debug_level(false);
    float res = parser.parse();
    scannerEnd();
    return res;
}
