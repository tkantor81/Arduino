#ifndef NOTIMPLEMENTED_H
#define NOTIMPLEMENTED_H

#include <stdexcept>

using namespace std;

class NotImplemented : public logic_error
{
public:
    NotImplemented() : logic_error("Function not yet implemented") { };
};

#endif