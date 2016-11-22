#ifndef _STATIC_FUNCTION_HPP_
#define _STATIC_FUNCTION_HPP_

#include "./node_elem.hpp"
#include "./base_function.hpp"
#include "queue"
#include "string"

using std::queue;
using std::string;

class static_tree_function {
  protected:
    static void pre_ordered_build( node_element **root, queue<char> &build_queue ) {
        if( !build_queue.empty() ) {

            *root = new node_element( build_queue.front() );
            build_queue.pop();

            if( ( *root )->is_type( "oprt" ) ) {

                pre_ordered_build( &( *root )->left_child, build_queue );
                pre_ordered_build( &( *root )->right_child, build_queue );

            } else if( ( *root )->is_type( "func" ) ) {

                pre_ordered_build( &( *root )->right_child, build_queue );

            }
        }
    }

    static void pre_ordered_verify( node_element **root, bool &success ) {
        if( *root == NULL ) {
            return;
        }

        if( ( *root )->is_type( "oprt" ) ) {

            if( ( *root )->left_child == NULL || ( *root )->right_child == NULL ) {
                success = false;
                return;
            }
            pre_ordered_verify( &( *root )->left_child, success );
            pre_ordered_verify( &( *root )->right_child, success );

        } else if( ( *root )->is_type( "func" ) ) {

            if( ( *root )->left_child != NULL || ( *root )->right_child == NULL ) {
                success = false;
                return;
            }
            pre_ordered_verify( &( *root )->right_child, success );

        }
    }

    static string verify_tree( node_element **root, queue<char> &build_queue ) {
        if( !build_queue.empty() ) {

            return "Expresion has too much oprand or operator.";

        } else {

            bool success = true;
            pre_ordered_verify( root, success );
            if( !success ) {
                return "Expresion has not enough oprand or operator.";
            }

        }

        return "No problem.";;
    }

    static void in_ordered_export( node_element **root, string &infix_string ) {
        if( *root == NULL ) {
            return;
        }

        if( ( *root )->is_type( "oprt" ) ) {

            infix_string.push_back( '(' );
            in_ordered_export( &( *root )->left_child, infix_string );

            infix_string.push_back( *( *root )->get_data().oprt );

            in_ordered_export( &( *root )->right_child, infix_string );
            infix_string.push_back( ')' );

        } else if( ( *root )->is_type( "var" ) ) {

            infix_string.push_back( *( *root )->get_data().var );

        } else if( ( *root )->is_type( "opnd" ) ) {

            infix_string += double_to_string( *( *root )->get_data().opnd );

        } else if( ( *root )->is_type( "func" ) ) {

            infix_string += get_function_name( *( *root )->get_data().func );

            if( !( *root )->right_child->is_type( "oprt" ) ) {
                infix_string.push_back( '(' );
            }
            in_ordered_export( &( *root )->right_child, infix_string );
            if( !( *root )->right_child->is_type( "oprt" ) ) {
                infix_string.push_back( ')' );
            }

        }
    }

    static void post_ordered_merge( node_element **root ) {
        if( *root == NULL ) {
            return;
        }
        if( ( *root )->is_type( "func" ) ) {

            post_ordered_merge( &( *root )->right_child );

            if( ( *root )->right_child->is_type( "opnd" ) ) {

                ( *root )->set_opnd(
                    functioning(
                        *( *root )->get_data().func,
                        *( *root )->right_child->get_data().opnd
                    )
                );

                delete &*( *root )->right_child;
                ( *root )->right_child = NULL;

            }


        } else if( ( *root )->is_type( "oprt" ) ) {

            post_ordered_merge( &( *root )->left_child );
            post_ordered_merge( &( *root )->right_child );

            if(
                ( *root )->left_child->is_type( "opnd" ) && \
                ( *root )->right_child->is_type( "opnd" )
            ) {

                ( *root )->set_opnd(
                    operating(
                        *( *root )->left_child->get_data().opnd,
                        *( *root )->get_data().oprt,
                        *( *root )->right_child->get_data().opnd
                    )
                );

                delete &*( *root )->left_child;
                ( *root )->left_child = NULL;
                delete &*( *root )->right_child;
                ( *root )->right_child = NULL;

            }
        }
    }

    static void post_ordered_destory( node_element **root ) {
        if( *root == NULL ) {
            return;
        }
        post_ordered_destory( &( *root )->left_child );
        post_ordered_destory( &( *root )->right_child );

        delete *root;
    }

    static void pre_ordered_copy( node_element **source_root, node_element **target_root ) {
        if( *source_root == NULL ) {
            return;
        }
        *target_root = new node_element( **source_root );

        pre_ordered_copy( &( *source_root )->left_child, &( *target_root )->left_child );
        pre_ordered_copy( &( *source_root )->right_child, &( *target_root )->right_child );
    }

    static void post_ordered_var_replace( node_element **root, const double *var_list ) {
        if( *root == NULL ) {
            return;
        }
        post_ordered_var_replace( &( *root )->left_child, var_list );
        post_ordered_var_replace( &( *root )->right_child, var_list );

        if( ( *root )->is_type( "var" ) ) {

            ( *root )->set_opnd( var_list[*( *root )->get_data().var - 'a'] );

        }
    }

    static void post_ordered_tidy( node_element **root ) {
        if( *root == NULL ) {
            return;
        }
        if( ( *root )->is_type( "oprt" ) ) {
            post_ordered_tidy( &( *root )->left_child );
            post_ordered_tidy( &( *root )->right_child );

            if( ( *root )->is_type( "oprt" ) && *( *root )->get_data().oprt == '+' ) {
                bool right_zero = (
                                      ( *root )->right_child->is_type( "opnd" ) && \
                                      iszero( *( *root )->right_child->get_data().opnd )
                                  );

                bool left_zero = (
                                     ( *root )->left_child->is_type( "opnd" ) && \
                                     iszero( *( *root )->left_child->get_data().opnd )
                                 );

                if( right_zero && !left_zero ) {

                    delete &*( *root )->right_child;
                    ( *root )->right_child = ( *root )->left_child->right_child;
                    node_element *tmp = ( *root )->left_child;
                    ( *root )->left_child = tmp->left_child;
                    ( *root )->copy( *tmp );
                    delete tmp;

                } else if( !right_zero && left_zero ) {

                    delete &*( *root )->left_child;
                    ( *root )->left_child = ( *root )->right_child->left_child;
                    node_element *tmp = ( *root )->right_child;
                    ( *root )->right_child = tmp->right_child;
                    ( *root )->copy( *tmp );
                    delete tmp;

                }
            }

            if( ( *root )->is_type( "oprt" ) && *( *root )->get_data().oprt == '-' ) {
                bool right_zero = (
                                      ( *root )->right_child->is_type( "opnd" ) && \
                                      iszero( *( *root )->right_child->get_data().opnd )
                                  );
                if( right_zero ) {
                    delete &*( *root )->right_child;
                    ( *root )->right_child = ( *root )->left_child->right_child;
                    node_element *tmp = ( *root )->left_child;
                    ( *root )->left_child = tmp->left_child;
                    ( *root )->copy( *tmp );
                    delete tmp;
                }
            }

            if( ( *root )->is_type( "oprt" ) && *( *root )->get_data().oprt == '*' ) {
                bool right_zero = (
                                      ( *root )->right_child->is_type( "opnd" ) && \
                                      iszero( *( *root )->right_child->get_data().opnd )
                                  );

                bool left_zero = (
                                     ( *root )->left_child->is_type( "opnd" ) && \
                                     iszero( *( *root )->left_child->get_data().opnd )
                                 );
                bool right_one = (
                                     ( *root )->right_child->is_type( "opnd" ) && \
                                     iszero( *( *root )->right_child->get_data().opnd - 1 )
                                 );

                bool left_one = (
                                    ( *root )->left_child->is_type( "opnd" ) && \
                                    iszero( *( *root )->left_child->get_data().opnd - 1 )
                                );

                if( left_zero || right_zero ) {

                    post_ordered_destory( &( *root )->left_child );
                    post_ordered_destory( &( *root )->right_child );

                    ( *root )->set_opnd( 0 );
                    ( *root )->left_child = NULL;
                    ( *root )->left_child = NULL;

                } else if( right_one && !left_one ) {

                    delete &*( *root )->right_child;
                    ( *root )->right_child = ( *root )->left_child->right_child;
                    node_element *tmp = ( *root )->left_child;
                    ( *root )->left_child = tmp->left_child;
                    ( *root )->copy( *tmp );
                    delete tmp;

                } else if( !right_one && left_one ) {

                    delete &*( *root )->left_child;
                    ( *root )->left_child = ( *root )->right_child->left_child;
                    node_element *tmp = ( *root )->right_child;
                    ( *root )->right_child = tmp->right_child;
                    ( *root )->copy( *tmp );
                    delete tmp;

                }
            }

            if( ( *root )->is_type( "oprt" ) && *( *root )->get_data().oprt == '/' ) {
                bool left_zero = (
                                     ( *root )->left_child->is_type( "opnd" ) && \
                                     iszero( *( *root )->left_child->get_data().opnd )
                                 );
                bool right_one = (
                                     ( *root )->right_child->is_type( "opnd" ) && \
                                     iszero( *( *root )->right_child->get_data().opnd - 1 )
                                 );

                if( left_zero ) {

                    post_ordered_destory( &( *root )->left_child );
                    post_ordered_destory( &( *root )->right_child );

                    ( *root )->set_opnd( 0 );
                    ( *root )->left_child = NULL;
                    ( *root )->left_child = NULL;

                } else if( right_one ) {

                    delete &*( *root )->right_child;
                    ( *root )->right_child = ( *root )->left_child->right_child;
                    node_element *tmp = ( *root )->left_child;
                    ( *root )->left_child = tmp->left_child;
                    ( *root )->copy( *tmp );
                    delete tmp;

                }
            }

            if( ( *root )->is_type( "oprt" ) && *( *root )->get_data().oprt == '^' ) {
                bool right_zero = (
                                      ( *root )->right_child->is_type( "opnd" ) && \
                                      iszero( *( *root )->right_child->get_data().opnd )
                                  );

                bool left_zero = (
                                     ( *root )->left_child->is_type( "opnd" ) && \
                                     iszero( *( *root )->left_child->get_data().opnd )
                                 );
                bool right_one = (
                                     ( *root )->right_child->is_type( "opnd" ) && \
                                     iszero( *( *root )->right_child->get_data().opnd - 1 )
                                 );

                bool left_one = (
                                    ( *root )->left_child->is_type( "opnd" ) && \
                                    iszero( *( *root )->left_child->get_data().opnd - 1 )
                                );

                if( left_zero ) {

                    post_ordered_destory( &( *root )->left_child );
                    post_ordered_destory( &( *root )->right_child );

                    ( *root )->set_opnd( 0 );
                    ( *root )->left_child = NULL;
                    ( *root )->left_child = NULL;

                } else if( right_zero ) {

                    post_ordered_destory( &( *root )->left_child );
                    post_ordered_destory( &( *root )->right_child );

                    ( *root )->set_opnd( 1 );
                    ( *root )->left_child = NULL;
                    ( *root )->left_child = NULL;

                } else if( right_one ) {

                    delete &*( *root )->right_child;
                    ( *root )->right_child = ( *root )->left_child->right_child;
                    node_element *tmp = ( *root )->left_child;
                    ( *root )->left_child = tmp->left_child;
                    ( *root )->copy( *tmp );
                    delete tmp;

                } else if( left_one ) {

                    post_ordered_destory( &( *root )->left_child );
                    post_ordered_destory( &( *root )->right_child );

                    ( *root )->set_opnd( 1 );
                    ( *root )->left_child = NULL;
                    ( *root )->left_child = NULL;

                }
            }

            if(
                ( *root )->is_type( "oprt" ) && ( *root )->left_child->is_type( "oprt" ) && \
                ( *root )->right_child->is_type( "oprt" ) && \
                ( *( *root )->get_data().oprt == *( *root )->left_child->get_data().oprt ) && \
                ( *( *root )->get_data().oprt == *( *root )->right_child->get_data().oprt ) && \
                ( *( *root )->get_data().oprt == '+' || * ( *root )->get_data().oprt == '*' )
            ) {

                node_element *tmp = ( *root )->right_child;
                ( *root )->right_child = ( *root )->left_child->left_child;
                ( *root )->left_child->left_child = tmp;

                tmp = ( *root )->left_child->left_child;
                ( *root )->left_child->left_child = ( *root )->left_child->right_child;
                ( *root )->left_child->right_child = tmp;

                tmp = ( *root )->right_child;
                ( *root )->right_child = ( *root )->left_child;
                ( *root )->left_child = tmp;

            }

            if(
                ( *root )->is_type( "oprt" ) && ( *root )->left_child->is_type( "oprt" ) && \
                ( *( *root )->get_data().oprt == *( *root )->left_child->get_data().oprt ) && \
                ( *( *root )->get_data().oprt == '+' || *( *root )->get_data().oprt == '*' ) && \
                ( *root )->right_child->is_type( "opnd" ) && \
                !( *root )->left_child->right_child->is_type( "opnd" )
            ) {

                node_element *tmp = ( *root )->left_child->right_child;
                ( *root )->left_child->right_child = ( *root )->right_child;
                ( *root )->right_child = tmp;

            }

            if(
                ( *root )->is_type( "oprt" ) && ( *root )->right_child->is_type( "oprt" ) && \
                ( *( *root )->get_data().oprt == *( *root )->right_child->get_data().oprt ) && \
                ( *( *root )->get_data().oprt == '+' || *( *root )->get_data().oprt == '*' ) && \
                ( *root )->left_child->is_type( "opnd" ) && \
                !( *root )->right_child->left_child->is_type( "opnd" )
            ) {

                node_element *tmp = ( *root )->right_child->left_child;
                ( *root )->right_child->left_child = ( *root )->left_child;
                ( *root )->left_child = tmp;

            }

            if(
                ( *root )->is_type( "oprt" ) && ( *root )->left_child->is_type( "oprt" ) && \
                ( *( *root )->get_data().oprt == *( *root )->left_child->get_data().oprt ) && \
                ( *( *root )->get_data().oprt == '+' || *( *root )->get_data().oprt == '*' ) && \
                ( *root )->right_child->is_type( "opnd" ) && \
                ( *root )->left_child->right_child->is_type( "opnd" )
            ) {

                ( *root )->right_child->set_opnd(
                    operating(
                        *( *root )->left_child->right_child->get_data().opnd,
                        *( *root )->get_data().oprt,
                        *( *root )->right_child->get_data().opnd
                    )
                );

                node_element *tmp = ( *root )->left_child->left_child;

                delete &*( *root )->left_child->right_child;
                ( *root )->left_child->right_child = NULL;
                delete &*( *root )->left_child;
                ( *root )->left_child = NULL;

                ( *root )->left_child = tmp;

            }

            if(
                ( *root )->is_type( "oprt" ) && ( *root )->right_child->is_type( "oprt" ) && \
                ( *( *root )->get_data().oprt == *( *root )->right_child->get_data().oprt ) && \
                ( *( *root )->get_data().oprt == '+' || *( *root )->get_data().oprt == '*' ) && \
                ( *root )->left_child->is_type( "opnd" ) && \
                ( *root )->right_child->left_child->is_type( "opnd" )
            ) {

                ( *root )->left_child->set_opnd(
                    operating(
                        *( *root )->right_child->left_child->get_data().opnd,
                        *( *root )->get_data().oprt,
                        *( *root )->left_child->get_data().opnd
                    )
                );

                node_element *tmp = ( *root )->right_child->right_child;

                delete &*( *root )->right_child->left_child;
                ( *root )->right_child->left_child = NULL;
                delete &*( *root )->right_child;
                ( *root )->right_child = NULL;

                ( *root )->right_child = tmp;

            }
        } else if( ( *root )->is_type( "func" ) ) {

            post_ordered_tidy( &( *root )->right_child );

        }
    }

    static void pre_ordered_compare(
        node_element **root_a,
        node_element **root_b,
        bool &is_equal
    ) {

        if( *root_a == NULL || *root_b == NULL ) {
            if(
                ( *root_a == NULL && *root_b != NULL ) || \
                ( *root_a != NULL && *root_b == NULL )
            ) {
                is_equal = false;
            }
            return;
        }

        if( ( *root_a )->get_type() != ( *root_b )->get_type() ) {
            is_equal = false;
            return;
        } else {

            if(
                ( *root_a )->is_type( "var" ) && \
                ( *( *root_a )->get_data().var != *( *root_b )->get_data().var )
            ) {
                is_equal = false;
                return;
            } else if(
                ( *root_a )->is_type( "opnd" ) && \
                ( *( *root_a )->get_data().opnd != *( *root_b )->get_data().opnd )
            ) {

                if(
                    !isnan( *( *root_a )->get_data().opnd ) && \
                    !isnan( *( *root_b )->get_data().opnd )
                ) {
                    is_equal = false;
                    return;
                }
            } else if(
                ( *root_a )->is_type( "oprt" ) && \
                ( *( *root_a )->get_data().oprt != *( *root_b )->get_data().oprt )
            ) {
                is_equal = false;
                return;
            } else if(
                ( *root_a )->is_type( "func" ) && \
                ( *( *root_a )->get_data().func != *( *root_b )->get_data().func )
            ) {
                is_equal = false;
                return;
            }
        }

        pre_ordered_compare( &( *root_a )->left_child, &( *root_b )->left_child, is_equal );
        pre_ordered_compare( &( *root_a )->right_child, &( *root_b )->right_child, is_equal );

    }

    static void interesting_partial_differential( node_element **root, const char &var ) {
        if( *root == NULL ) {
            return;
        }
        if(
            ( *root )->is_type( "oprt" ) && \
            ( *( *root )->get_data().oprt == '+' || *( *root )->get_data().oprt == '-' )
        ) {
            interesting_partial_differential( &( *root )->left_child, var );
            interesting_partial_differential( &( *root )->right_child, var );
        } else if( ( *root )->is_type( "oprt" ) && *( *root )->get_data().oprt == '*' ) {

            node_element *df, *dg;
            pre_ordered_copy( &( *root )->left_child, &df );
            pre_ordered_copy( &( *root )->right_child, &dg );

            ( *root )->set_data( '+' );

            node_element *mul_left = new node_element( '*' );
            node_element *mul_right = new node_element( '*' );

            mul_left->left_child = df;
            mul_left->right_child = ( *root )->right_child;

            mul_right->left_child = ( *root )->left_child;
            mul_right->right_child = dg;

            ( *root )->left_child = mul_left;
            ( *root )->right_child = mul_right;

            interesting_partial_differential( &df, var );
            interesting_partial_differential( &dg, var );

        } else if( ( *root )->is_type( "oprt" ) && *( *root )->get_data().oprt == '/' ) {

            node_element *df, *dg, *sqrt_g;
            pre_ordered_copy( &( *root )->left_child, &df );
            pre_ordered_copy( &( *root )->right_child, &dg );
            pre_ordered_copy( &( *root )->right_child, &sqrt_g );

            node_element *mul_left = new node_element( '*' );
            node_element *mul_right = new node_element( '*' );

            node_element *sub = new node_element( '-' );
            node_element *squart = new node_element( '^' );

            node_element *two = new node_element( '2' );

            mul_left->left_child = df;
            mul_left->right_child = ( *root )->right_child;

            mul_right->left_child = ( *root )->left_child;
            mul_right->right_child = dg;

            sub->left_child = mul_left;
            sub->right_child = mul_right;

            squart->left_child = sqrt_g;
            squart->right_child = two;

            ( *root )->left_child = sub;
            ( *root )->right_child = squart;

            interesting_partial_differential( &df, var );
            interesting_partial_differential( &dg, var );


        } else if( ( *root )->is_type( "oprt" ) && *( *root )->get_data().oprt == '^' ) {

            node_element *new_g, *new_f;
            pre_ordered_copy( &( *root )->left_child, &new_f );
            pre_ordered_copy( &( *root )->right_child, &new_g );

            node_element *sq_left = new node_element( '^' );
            node_element *mul_right = new node_element( '*' );
            node_element *ln = new node_element( 'L' );

            ln->right_child = new_f;

            mul_right->left_child = new_g;
            mul_right->right_child = ln;

            sq_left->left_child = ( *root )->left_child;
            sq_left->right_child = ( *root )->right_child;

            ( *root )->set_data( '*' );

            ( *root )->left_child = sq_left;
            ( *root )->right_child = mul_right;

            interesting_partial_differential( &mul_right, var );

        } else if( ( *root )->is_type( "func" ) && *( *root )->get_data().func == 'C' ) {

            node_element *du;
            pre_ordered_copy( &( *root )->right_child, &du );

            node_element *ng_one = new node_element;
            node_element *r_sin = new node_element( 'S' );
            node_element *l_mul = new node_element( '*' );
            ng_one->set_opnd( -1 );

            r_sin->right_child = ( *root )->right_child;

            l_mul->left_child = ng_one;
            l_mul->right_child = r_sin;

            ( *root )->set_data( '*' );

            ( *root )->left_child = l_mul;
            ( *root )->right_child = du;

            interesting_partial_differential( &du, var );

        } else if( ( *root )->is_type( "func" ) && *( *root )->get_data().func == 'S' ) {

            node_element *du;
            pre_ordered_copy( &( *root )->right_child, &du );

            node_element *l_cos = new node_element( 'C' );

            l_cos->right_child = ( *root )->right_child;

            ( *root )->set_data( '*' );

            ( *root )->left_child = l_cos;
            ( *root )->right_child = du;

            interesting_partial_differential( &du, var );

        } else if( ( *root )->is_type( "func" ) && *( *root )->get_data().func == 'L' ) {

            node_element *du;
            pre_ordered_copy( &( *root )->right_child, &du );

            node_element *l_div = new node_element( '/' );
            node_element *one = new node_element( '1' );

            l_div->left_child = one;
            l_div->right_child = ( *root )->right_child;

            ( *root )->set_data( '*' );

            ( *root )->left_child = l_div;
            ( *root )->right_child = du;

            interesting_partial_differential( &du, var );

        } else if( ( *root )->is_type( "func" ) && *( *root )->get_data().func == 'E' ) {

            node_element *du;
            pre_ordered_copy( &( *root )->right_child, &du );

            node_element *l_exp = new node_element( 'E' );

            l_exp->right_child = ( *root )->right_child;

            ( *root )->set_data( '*' );

            ( *root )->left_child = l_exp;
            ( *root )->right_child = du;

            interesting_partial_differential( &du, var );

        } else if( ( *root )->is_type( "var" ) && *( *root )->get_data().var == var ) {

            ( *root )->set_data( '1' );

        } else if( ( *root )->is_type( "var" ) && *( *root )->get_data().var != var ) {

            ( *root )->set_data( '0' );

        } else if( ( *root )->is_type( "opnd" ) ) {

            ( *root )->set_data( '0' );

        }
    }
};

#endif