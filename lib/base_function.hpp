#ifndef _BASE_FUNCTION_HPP_
#define _BASE_FUNCTION_HPP_

#define OPRT_SET_SIZE 5

#include "set"
#include "string"
#include "sstream"
#include "cmath"

using std::string;
using std::stringstream;
using std::set;

const char oprt_list[] = {
    '+', '-', '*', '/', '^'
};

const set<char> oprt_set( oprt_list, oprt_list + OPRT_SET_SIZE );

bool isoprt( const char &c ) {
    return oprt_set.find( c ) != oprt_set.end();
}

bool isfunc( const char &c ) {
    return c == 'C' || c == 'S' || c == 'L' || c == 'E';
}

string double_to_string( const double &input ) {
    stringstream transistor;
    transistor << input;
    string output;
    transistor >> output;
    return output;
}

double operating( const double &opnd_a, const char &oprt, const double &opnd_b ) {
    switch( oprt ) {
    case '+': {
        return opnd_a + opnd_b;
        break;
    }
    case '-': {
        return opnd_a - opnd_b;
        break;
    }
    case '*': {
        return opnd_a * opnd_b;
        break;
    }
    case '/': {
        return opnd_a / opnd_b;
        break;
    }
    case '^': {
        return pow( opnd_a, opnd_b );
        break;
    }
    default: {
        return 0;
        break;
    }
    }
}

double functioning( const char &func, const double &para ) {
    switch( func ) {
    case 'C': {
        return cos( para );
        break;
    }
    case 'S': {
        return sin( para );
        break;
    }
    case 'L': {
        return log( para );
        break;
    }
    case 'E': {
        return exp( para );
        break;
    }
    default: {
        return 0;
        break;
    }
    }
}

string get_function_name( const char &func ) {
    switch( func ) {
    case 'C': {
        return "cos";
        break;
    }
    case 'S': {
        return "sin";
        break;
    }
    case 'L': {
        return "ln";
        break;
    }
    case 'E': {
        return "exp";
        break;
    }
    default: {
        return 0;
        break;
    }
    }
}

bool isnan( const double &d ) {
    return d != d;
}

bool iszero( const double &d ) {
    return fabs( d ) <= 1e-6;
}

#endif