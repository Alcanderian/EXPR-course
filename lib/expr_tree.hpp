#ifndef _EXPR_TREE_HPP_
#define _EXPR_TREE_HPP_

#include "./node_elem.hpp"
#include "./base_function.hpp"
#include "./static_function.hpp"
#include "cstring"
#include "iostream"
#include "string"

using std::queue;
using std::ostream;
using std::string;

class prefix_expr {
  public:
    string data;

    prefix_expr( const string &in_data ) {
        this->data = in_data;
    }
};

struct expr_exception {
    string ex_what;

    expr_exception( const string &context ) {
        ex_what = context;
    }

    friend ostream &operator<<( ostream &os, const expr_exception &ex ) {
        os << ex.ex_what;
        return os;
    }
};

class expr_tree: private static_tree_function {
  private:
    node_element *expr_root;
    double *var_list;
    bool is_private_var_list;

  public:
    expr_tree( const prefix_expr &expr ) {
        this->var_list = new double[26];
        memset( this->var_list, 0, 26 * sizeof( double ) );

        is_private_var_list = true;

        queue<char> build_queue;
        for( int i = 0; i < expr.data.length(); ++i ) {
            if(
                islower( expr.data[i] ) || isdigit( expr.data[i] ) || \
                isoprt( expr.data[i] ) || isfunc( expr.data[i] )
            ) {

                build_queue.push( expr.data[i] );

            }
        }

        pre_ordered_build( &this->expr_root, build_queue );

        string verify_result = verify_tree( &this->expr_root, build_queue );

        if( verify_result != "No problem." ) {

            post_ordered_destory( &this->expr_root );
            throw expr_exception( verify_result );

        }
    }

    expr_tree( const expr_tree &other ) {
        if( other.is_private_var_list ) {

            this->var_list = new double[26];
            is_private_var_list = true;

            for( int i = 0; i < 26; ++i ) {
                this->var_list[i] = other.var_list[i];
            }

        } else {

            is_private_var_list = false;
            this->var_list = other.var_list;

        }

        node_element *other_root = other.expr_root;
        pre_ordered_copy( &other_root, &this->expr_root );
    }

    expr_tree( const expr_tree &other_a, const char &oprt, const expr_tree &other_b ) {
        this->var_list = new double[26];
        memset( this->var_list, 0, 26 * sizeof( double ) );

        is_private_var_list = true;

        node_element *other_root_a = other_a.expr_root;
        pre_ordered_copy( &other_root_a, &this->expr_root );

        compound( oprt, other_b );
    }

    ~expr_tree() {
        if( is_private_var_list ) {
            delete[] var_list;
        }
        post_ordered_destory( &this->expr_root );
    }

    void set_var_list( double *other_list ) {
        if( is_private_var_list ) {

            delete[] var_list;
            is_private_var_list = false;

        }

        this->var_list = other_list;
    }

    string infix_output() {
        string ret;
        in_ordered_export( &this->expr_root, ret );

        return ret;
    }

    void assign_var( const char &var_index, const double &var ) {
        if( is_private_var_list ) {
            if( islower( var_index ) ) {

                var_list[var_index - 'a'] = var;

            } else {

                throw expr_exception( "Invalid variable." );

            }
        } else {

            throw expr_exception( "Variable list is not private." );

        }
    }

    double calculate() {
        node_element *value_root;
        pre_ordered_copy( &this->expr_root, &value_root );
        post_ordered_var_replace( &value_root, this->var_list );
        post_ordered_merge( &value_root );

        return *value_root->get_data().opnd;
    }

    void compound( const char &oprt, const expr_tree &other ) {
        node_element *new_root = new node_element( oprt );
        node_element *other_root = other.expr_root;
        new_root->left_child = this->expr_root;
        pre_ordered_copy( &other_root, &new_root->right_child );

        this->expr_root = new_root;
    }

    void merge_const() {
        node_element *tmp = NULL;
        bool is_finish;

        do {

            post_ordered_destory( &tmp );
            pre_ordered_copy( &this->expr_root, &tmp );

            post_ordered_tidy( &this->expr_root );
            post_ordered_merge( &this->expr_root );

            is_finish = true;
            pre_ordered_compare( &this->expr_root, &tmp, is_finish );

        } while( !is_finish );

        post_ordered_destory( &tmp );
    }

    void partial_differential( const char &var ) {
        merge_const();

        interesting_partial_differential( &this->expr_root, var );

        merge_const();
    }
};

#endif