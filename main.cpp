#include "./lib/expr_tree.hpp"
#include "iostream"
#include "cstdlib"
#include "string"
#include "cstring"
#include "cstdio"
#include "sstream"
using namespace std;

expr_tree *arr[5];
double *var_list = new double[26];


void help() {
    cout << "Welcome to ExT 1.0 NOT_LTS!\n"
         << " * Documentation:\n"
         << " Support operator: +, -, *, /, ^, cos(use C), sin(S), ln(L), exp(E).\n"
         << " Support variable: a-z\n"
         << "\n We have expression container A-D and ~ for result.\n"
         << " How to use: \n\n"
         << "   set               [A-D]            to input an expr.\n"
         << "   show              [A-D|~]          to show an expr.\n"
         << "   set               [a-z]            to set a variable.\n"
         << "   show              [a-z]            to set a variable.\n"
         << "   rm                [A-D|~]          delete sth.\n"
         << "   value             [A-D|~]          get value of expr.\n"
         << "   compound  [+-*/^] [A-D|~] [A-D|~]  compound two expr.\n"
         << "   diff              [A-D|~] [a-z]    partial diff. of value a-z.\n"
         << "   merge             [A-D|~]            merge const.\n\n"
         << "   help - get help.         exit - exit.\n\n"
         << "\n Enjoy --------\n\n";
}

struct command_exception {
    string ex_what;

    command_exception( const string &context ) {
        ex_what = context;
    }

    friend ostream &operator<<( ostream &os, const command_exception &ex ) {
        os << ex.ex_what;
        return os;
    }
};

struct cmd_line {
    string col[20];
    int len;

    void build( const string &in ) {
        len = 0;
        stringstream buf( in );
        for( int i = 0; i < 20; ++i ) {
            buf >> col[i];
            if( col[i] != "" ) {
                len++;
            }
        }
    }

    void clear() {
        len = 0;
        for( int i = 0; i < 20; ++i ) {
            col[i] = "";
        }
    }
};

bool is_container( char s ) {
    return s <= 'D' && s >= 'A';
}

int get_container_index( char s ) {
    return s - 'A';
}

bool is_ans( char s ) {
    return s == '~';
}

int get_ans_index() {
    return 4;
}

bool is_var( char s ) {
    return islower( s );
}

int get_var_index( char s ) {
    return s - 'a';
}

void execute( const cmd_line &cmd ) {
    if( cmd.len < 1 ) {
        throw command_exception( "Empty command." );
    }
    if( cmd.col[0] == "help" ) {
        help();
        return;
    } else if( cmd.col[0] == "exit" ) {
        exit( 0 );
    } else if( cmd.col[0] == "set" ) {
        if( cmd.len < 2 ) {
            throw command_exception( "Not enough param." );
        }
        if( cmd.len > 2 ) {
            throw command_exception( "Too much param." );
        }
        if( cmd.col[1].length() != 1 ) {
            throw command_exception( "Invalid param." );
        }
        if( is_container( cmd.col[1][0] ) ) {
            delete arr[get_container_index( cmd.col[1][0] )];
            string expr;
            cout << " " << cmd.col[1][0] << " = ";
            cin >> expr;
            prefix_expr a( expr );
            arr[get_container_index( cmd.col[1][0] )] = new expr_tree( a );
            arr[get_container_index( cmd.col[1][0] )]->set_var_list( var_list );

            cout << " System: " << arr[get_container_index( cmd.col[1][0] )]->infix_output() << endl;
        } else if( is_var( cmd.col[1][0] ) ) {
            double d;
            cout << " " << cmd.col[1][0] << " = ";
            cin >> d;
            var_list[get_var_index( cmd.col[1][0] )] = d;

            cout << " System: " << cmd.col[1][0] << " = " << d << endl;
        } else {
            throw command_exception( "Invalid param." );

        }
    } else if( cmd.col[0] == "show" ) {
        if( cmd.len < 2 ) {
            throw command_exception( "Not enough param." );
        }
        if( cmd.len > 2 ) {
            throw command_exception( "Too much param." );
        }
        if( cmd.col[1].length() != 1 ) {
            throw command_exception( "Invalid param." );
        }
        if( is_container( cmd.col[1][0] ) ) {
            cout << " System: " << cmd.col[1][0] << " = " << arr[get_container_index( cmd.col[1][0] )]->infix_output() << endl;

        } else if( is_var( cmd.col[1][0] ) ) {
            cout << " System: " << cmd.col[1][0] << " = " << var_list[get_var_index( cmd.col[1][0] )] << endl;

        } else if( is_ans( cmd.col[1][0] ) ) {
            cout << " System: " << cmd.col[1][0] << " = " << arr[get_ans_index()]->infix_output() << endl;
        } else {
            throw command_exception( "Invalid param." );

        }
    } else if( cmd.col[0] == "rm" ) {
        if( cmd.len < 2 ) {
            throw command_exception( "Not enough param." );
        }
        if( cmd.len > 2 ) {
            throw command_exception( "Too much param." );
        }
        if( cmd.col[1].length() != 1 ) {
            throw command_exception( "Invalid param." );
        }
        if( is_container( cmd.col[1][0] ) ) {
            delete arr[get_container_index( cmd.col[1][0] )];
            prefix_expr a( "0" );
            arr[get_container_index( cmd.col[1][0] )] = new expr_tree( a );
            arr[get_container_index( cmd.col[1][0] )]->set_var_list( var_list );

        } else if( is_ans( cmd.col[1][0] ) ) {
            delete arr[get_ans_index()];
            prefix_expr a( "0" );
            arr[get_ans_index()] = new expr_tree( a );
            arr[get_ans_index()]->set_var_list( var_list );

        } else {
            throw command_exception( "Invalid param." );

        }
    } else if( cmd.col[0] == "value" ) {
        if( cmd.len < 2 ) {
            throw command_exception( "Not enough param." );
        }
        if( cmd.len > 2 ) {
            throw command_exception( "Too much param." );
        }
        if( cmd.col[1].length() != 1 ) {
            throw command_exception( "Invalid param." );
        }
        if( is_container( cmd.col[1][0] ) ) {
            cout << " System: " << arr[get_container_index( cmd.col[1][0] )]->calculate() << endl;

        } else if( is_ans( cmd.col[1][0] ) ) {
            cout << " System: " << cmd.col[1][0] << " = " << arr[get_ans_index()]->calculate() << endl;

        } else {
            throw command_exception( "Invalid param." );

        }
    } else if( cmd.col[0] == "compound" ) {
        if( cmd.len < 4 ) {
            throw command_exception( "Not enough param." );
        }
        if( cmd.len > 4 ) {
            throw command_exception( "Too much param." );
        }
        if( cmd.col[1].length() != 1 ) {
            throw command_exception( "Invalid param." );
        }
        if( cmd.col[2].length() != 1 ) {
            throw command_exception( "Invalid param." );
        }
        if( cmd.col[3].length() != 1 ) {
            throw command_exception( "Invalid param." );
        }
        if( !isoprt( cmd.col[1][0] ) ) {
            throw command_exception( "Invalid param.!" );
        }
        int index_a = -1, index_b = -1;
        if( is_container( cmd.col[2][0] ) ) {
            index_a = get_container_index( cmd.col[2][0] );

        } else if( is_ans( cmd.col[2][0] ) ) {
            index_a = get_ans_index();

        }
        if( is_container( cmd.col[3][0] ) ) {
            index_b = get_container_index( cmd.col[3][0] );

        } else if( is_ans( cmd.col[3][0] ) ) {
            index_b = get_ans_index();

        }
        if( index_a != -1 && index_b != -1 ) {
            expr_tree *ans = new expr_tree( *arr[index_a], cmd.col[1][0], *arr[index_b] );
            delete arr[get_ans_index()];
            arr[get_ans_index()] = ans;
            arr[get_ans_index()]->set_var_list( var_list );

            cout << " System: " << cmd.col[2][0] << " " << cmd.col[1][0] << " " << cmd.col[3][0] << " = " << ans->infix_output() << endl;
        } else {
            throw command_exception( "Invalid param.-" );

        }
    } else if( cmd.col[0] == "diff" ) {
        if( cmd.len < 3 ) {
            throw command_exception( "Not enough param." );
        }
        if( cmd.len > 3 ) {
            throw command_exception( "Too much param." );
        }
        if( cmd.col[1].length() != 1 ) {
            throw command_exception( "Invalid param." );
        }
        if( cmd.col[2].length() != 1 ) {
            throw command_exception( "Invalid param." );
        }
        int index_exp = -1;
        if( is_container( cmd.col[1][0] ) ) {
            index_exp = get_container_index( cmd.col[1][0] );

        } else if( is_ans( cmd.col[1][0] ) ) {
            index_exp = get_ans_index();

        }

        if( is_var( cmd.col[2][0] ) ) {
            if( index_exp != -1 ) {
                expr_tree *ans = new expr_tree( *arr[index_exp] );
                ans->partial_differential( cmd.col[2][0] );
                delete arr[get_ans_index()];
                arr[get_ans_index()] = ans;
                arr[get_ans_index()]->set_var_list( var_list );

                cout << " System: d" << cmd.col[1][0] << "/d" << cmd.col[2][0] << " = " << ans->infix_output() << endl;
            }
        } else {
            throw command_exception( "Invalid param." );

        }
    } else if( cmd.col[0] == "merge" ) {
        if( cmd.len < 2 ) {
            throw command_exception( "Not enough param." );
        }
        if( cmd.len > 2 ) {
            throw command_exception( "Too much param." );
        }
        if( cmd.col[1].length() != 1 ) {
            throw command_exception( "Invalid param." );
        }
        if( is_container( cmd.col[1][0] ) ) {
            expr_tree *ans = new expr_tree( *arr[get_container_index( cmd.col[1][0] )] );
            ans->merge_const();
            delete arr[get_ans_index()];
            arr[get_ans_index()] = ans;
            arr[get_ans_index()]->set_var_list( var_list );

            cout << " System: " << cmd.col[1][0] << " = " << ans->infix_output() << endl;

        } else if( is_ans( cmd.col[1][0] ) ) {
            expr_tree *ans = new expr_tree( *arr[get_ans_index()] );
            ans->merge_const();
            delete arr[get_ans_index()];
            arr[get_ans_index()] = ans;
            arr[get_ans_index()]->set_var_list( var_list );

            cout << " System: " << cmd.col[1][0] << " = " << ans->infix_output() << endl;

        } else {
            throw command_exception( "Invalid param." );

        }
    } else {
        throw command_exception( "No such command." );
    }
}

int main() {
    help();
    memset( var_list, 0, sizeof( double ) * 26 );
    for( int i = 0; i < 5; ++i ) {
        prefix_expr a( "0" );
        arr[i] = new expr_tree( a );
        arr[i]->set_var_list( var_list );
    }
    while( 1 ) {
        cmd_line cmd_l;
        try {
            cout << "user@ExT: ";
            char c;
            string cmd;
            do {
                c = cin.get();
                cmd.push_back( c );
            } while( c != '\n' );

            cmd.erase( cmd.length() - 1, 1 );
            cmd_l.clear();
            cmd_l.build( cmd );

            execute( cmd_l );

            if( cmd_l.col[0] == "set" ) {
                cin.get();
            }

        } catch( expr_exception e ) {

            cout << " Expression Exception: ";
            cout << e << endl;
            prefix_expr a( "0" );
            arr[get_container_index( cmd_l.col[1][0] )] = new expr_tree( a );
            cin.get();

        } catch( command_exception e ) {

            cout << " Command Exception: ";
            cout << e << endl;

        } catch( ... ) {

            cout << " Unknown Exception.\n";

        }
    }
    return 0;
}
