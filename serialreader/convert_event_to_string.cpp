#include <iostream>
#include <string>
#include <limits>
#include <cassert>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::to_string;

string convert_event_to_string(const vector<uint8_t>& event) {
    
    string to_return{""};

    // FAIL IF SOMETHING WENT WRONG
    assert(event.size() == 14);
    assert(!event[0]);
    assert(!event[13]);
    assert(event[1] == 'e');

    // Check what happened
    to_return += string{"Player "} + to_string(event[2] + 1);
    to_return += (!event[3]) ? (string{" died! "}) : (string{" event! "});
    to_return += (event[4] == std::numeric_limits<uint8_t>::max()) ? 
        (string{""}) : (string{"Killed by player "} + to_string(event[4]));
    
    return to_return;
}
