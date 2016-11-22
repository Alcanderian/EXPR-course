#ifndef _NODE_ELEM_HPP_
#define _NODE_ELEM_HPP_

#include "string"
#include "iostream"
#include "cctype"
#include "./base_function.hpp"

using std::string;
using std::ostream;

struct node_element {
  private:
    union node_data {
        double *opnd;
        char *oprt;
        char *var;
        char *func;
    } data;
    int type;

    void set_type( const char &in_data ) {
        if( islower( in_data ) ) {
            type = 0;
        } else if( isdigit( in_data ) ) {
            type = 1;
        } else if( isoprt( in_data ) ) {
            type = 2;
        } else if( isfunc( in_data ) ) {
            type = 3;
        } else {
            type = -1;
        }
    }

    void destory() {
        if( type == 0 ) {
            delete data.var;
        } else if( type == 1 ) {
            delete data.opnd;
        } else if( type == 2 ) {
            delete data.oprt;
        } else if( type == 3 ) {
            delete data.func;
        }

        type = -1;
    }

    void construct( const char &in_data ) {
        this->set_type( in_data );

        if( type == 0 ) {
            data.var = new char( in_data );
        } else if( type == 1 ) {
            data.opnd = new double( in_data - '0' );
        } else if( type == 2 ) {
            data.oprt = new char( in_data );
        } else if( type == 3 ) {
            data.func = new char( in_data );
        }
    }

    void private_copy( const node_element &other ) {
        this->type = other.type;

        if( type == 0 ) {
            data.var = new char( *other.data.var );
        } else if( type == 1 ) {
            data.opnd = new double( *other.data.opnd );
        } else if( type == 2 ) {
            data.oprt = new char( *other.data.oprt );
        } else if( type == 3 ) {
            data.func = new char( *other.data.func );
        }
    }

  public:
    node_element *left_child, *right_child;

    node_element(): type( -1 ), left_child( NULL ), right_child( NULL ) {
    }

    node_element( const char &in_data ): left_child( NULL ), right_child( NULL ) {
        this->construct( in_data );
    }

    node_element( const node_element &other ): left_child( NULL ), right_child( NULL ) {
        this->private_copy( other );
    }

    ~node_element() {
        this->destory();
    }

    void set_data( const char &in_data ) {
        this->destory();
        this->construct( in_data );
    }

    void set_opnd( const double &in_data ) {
        this->destory();
        this->type = 1;
        this->data.opnd = new double( in_data );
        left_child = NULL;
        right_child = NULL;
    }

    node_data &get_data() {
        return data;
    }

    bool is_type( const string &type_name ) {
        return ( type == 0 && type_name == "var" ) || \
               ( type == 1 && type_name == "opnd" ) || \
               ( type == 2 && type_name == "oprt" ) || \
               ( type == 3 && type_name == "func" );
    }

    string get_type() {
        if( type == 0 ) {
            return "var";
        } else if( type == 1 ) {
            return "opnd";
        } else if( type == 2 ) {
            return "oprt";
        } else if( type == 3 ) {
            return "func";
        }

        return "none";
    }

    node_element &operator=( const node_element &other ) {
        this->destory();
        this->private_copy( other );
        return *this;
    }

    void copy( const node_element &other ) {
        this->destory();
        this->private_copy( other );
    }

    friend ostream &operator<<( ostream &os, const node_element &in ) {
        if( in.type == 0 ) {
            os << *in.data.var;
        } else if( in.type == 1 ) {
            os << *in.data.opnd;
        } else if( in.type == 2 ) {
            os << *in.data.oprt;
        } else if( in.type == 3 ) {
            os << *in.data.func;
        }
        return os;
    }
};

#endif