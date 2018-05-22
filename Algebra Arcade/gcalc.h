
/*
    Simple calculator

    This program implements a basic expression calculator.
    Input from cin; output to cout.

    This is the integer version!

    The grammar for input is:

    Calculation:
        Statement
        Print
        Quit
        Help
        Calculation Statement

    Statement:
        Declaration
        Expression

    Declaration:
        "let" Name "=" Expression
        "const" name "=" Expression

    Name:
        letter
        letter Sequence

    Sequence:
        letter
        digit
        "_"
        letter Sequence
        digit Sequence
        "_" Sequence

    Print:
        ";"

    Quit:
        "quit"

    Help
        "help"

    Expression:
        Term
        Expression "+" Term
        Expression "-" Term

    Term:
        Primary
        Term "*" Primary
        Term "/" Primary
        Term "%" Primary

    Primary:
        Number
        "(" Expression ")"
        "-" Primary
        "+" Primary
        "sqrt(" Expression ")"
        "pow(" Expression "," Integer ")"
        Name
        Name "=" Expression

    Number:
        floating-point-literal

    Input comes from cin through the Token_stream called ts.
*/

//#include "std_lib_facilities.h"
#include <string>
#include <iostream>
#include <istream>
#include <sstream>


using std::stringstream;


//------------------------------------------------------------------------------

// Helper function to show an error message
inline void error(const std::string& errormessage)
{
    std::cout << errormessage << std::endl;
    throw std::runtime_error(errormessage);
}

//------------------------------------------------------------------------------

inline void error(std::string s1, std::string s2)
{
    error(s1+s2);
}

// -----------------------------------------------------------------------
// a user-defined type to classify input as operator, operand or variable/function
class Token {
public:
    char kind;      // what kind of token
    float value;      // for numbers: a value
    std::string name;    // for variables and functions: a name
    Token(char ch) :kind(ch), value(0) { }              // make a Token from a char
    Token(char ch, float val) :kind(ch), value(val) { }   // make a Token from a char and an float
    Token(char ch, std::string n) :kind(ch), name(n) { } // make a Token from a char and a string
};

// stream of tokens
class Token_stream {
public:
    Token_stream();         // make a Token_stream that reads from readf
    Token get(stringstream& is);            // get a Token
    void putback(Token t);  // put a Token back
    void ignore(char c, stringstream& is);    // discard characters up to and including a c
private:
    bool full;          // is there a Token in the buffer?
    Token buffer;       // here is where we keep a Token put back using putback()

};

// constructor
Token_stream::Token_stream()
    :full(false), buffer(0) { } // no Token in buffer

const char let = 'L';           // declaration token
const char con = 'C';           // constant token
//const char quit = 'q';          // t.kind==quit means that t is a quit Token
//const char help = 'h';          // help token
const char print = ';';         // t.kind==print means that t is a print Token
const char number = '8';        // t.kind==number means that t is a number Token
const char name = 'a';          // name token
const char square_root = 's';   // square root token
const char power = 'p';         // power function token
const char sine = 'S';          // sine function token
const char cosine = 'c';        // cosine function token
const char tangent = 't';
const char absolute = 'b';
const char integer = 'i';
const char arctangent = 'o';

const std::string declkey = "let";   // declaration keyword
const std::string conkey = "const";  // constant keyword
const std::string sqrtkey = "sqrt";  // keyword for square root
const std::string powkey = "pow";    // keywornction
const std::string sinekey = "sin";   // keyword for sine function


const std::string tankey = "tan";
const std::string abskey = "abs";
const std::string intkey = "int";
const std::string arctangentkey = "arctan";

const std::string cosinekey = "cos"; // keyword for cosine function
const std::string quitkey = "quit";  // keyword to quit
const std::string helpkey = "help";  // keyword for help

// read characters from readf and compose a Token
Token Token_stream::get(stringstream& is)
{
    if (full) { // check if we already have a Token ready
        full = false;
        return buffer;
    }

    char ch;
    //cin.get(ch);    // note that cin.get() does NOT skip whitespace
    is.get(ch);
    while (isspace(ch)) {
        if (ch == '\n') return Token(print); // if newline detected, return print Token
        is.get(ch);
        //cin.get(ch);
    }

    switch (ch) {
    case print:
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '=':
    case ',':
        return Token(ch);   // let each character represent itself
    case '.':   // a floating-point-literal can start with a dot
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':   // numeric literal
    {  //cin.putback(ch);    // put digit back into the input stream
        is.putback(ch);
    float val;
    is >> val;         // read an integer number
    return Token(number,val);
    }
    default:
        if (isalpha(ch)) {
            std::string s;
            s += ch;
            while (is.get(ch) && (isalpha(ch) || isdigit(ch) || ch=='_')) s+=ch;
            is.putback(ch);
            if (s == declkey) return Token(let);            // declaration keyword
            if (s == conkey) return Token(con);             // constant keyword
            if (s == sqrtkey) return Token(square_root);    // square root keyword
            if (s == powkey) return Token(power);           // power function keyword
            if (s == sinekey) return Token(sine);           // sine function keyword
            if (s == cosinekey) return Token(cosine);       // cosine function keyword
            if (s == tankey) return Token(tangent);
            if (s == abskey) return Token(absolute);
            if (s == arctangentkey) return Token(arctangent);
            if (s == intkey) return Token(integer);
            //if (s == helpkey) return Token(help);         // help keyword
            //if (s == quitkey) return Token(quit);         // quit keyword
            return Token(name,s);
        }
        std::runtime_error("Bad token");
    }
}

// put Token back into Token stream
void Token_stream::putback(Token t)
{
    if (full) std::runtime_error("putback() into full buffer");
    buffer = t;     // copy t to buffer
    full = true;    // buffer is now full
}

// ignore Tokens up to specific kind
void Token_stream::ignore(char c, stringstream& is)
// c represents the kind of Token
{
    // first look in buffer:
    if (full && c==buffer.kind) {
        full = false;
        return;
    }
    full = false;

    // now search input:
    char ch = 0;
    while (is>>ch)
    if (ch==c) return;
}

// -----------------------------------------------------------------------

// type for (name,value) pairs
class Variable {
public:
    std::string name;
    float value;
    bool is_const;
    Variable(std::string n, float v, bool b) :name(n), value(v), is_const(b) { }
};

// type for var_table and associated functions
class Symbol_table {
public:
    float get(std::string s);                          // return the value of the Variable named s
    void set(std::string s, float d);                  // set the Variable named s to d
    bool is_declared(std::string var);               // is var already in var_table?
    float declare(std::string var, float val,bool b);    // add (var,val) to var_table
    std::vector<Variable> var_table; // vector of Variables
private:
    //vector<Variable> var_table; // vector of Variables
};

// return the value of the Variable named s
float Symbol_table::get(std::string s)
{
    //for (int i = 0; i<var_table.size(); ++i)
    for(Variable& v : var_table)
        //if (var_table[i].name == s) return var_table[i].value;
        if(v.name == s) return v.value;
    std::runtime_error("get: undefined variable "); //,s
}

// set the Variable named s to d
void Symbol_table::set(std::string s, float d)
{
    //for (int i = 0; i<var_table.size(); ++i)
    //cout << "\ncalling st.set(" << s << ", " << d << ")";

    for(Variable& v : var_table)
    if (v.name == s) {
        if (v.is_const) error(s," is a constant");
        //cout << "\nin st.set, d = " << d;
        v.value = d;
        //cout << "\nv.value = " << v.value;
        return;
    }
    error("set: undefined variable ",s);
}

// is var already in var_table?
bool Symbol_table::is_declared(std::string var)
{
    //for (int i = 0; i<var_table.size(); ++i)
    for(Variable v : var_table)
        if (v.name == var) return true;
    return false;
}

// add (var,val) to var_table
float Symbol_table::declare(std::string var, float val, bool b)
{
    if (is_declared(var)) error(var," declared twice");
    var_table.push_back(Variable(var,val,b));
    return val;
}

// -----------------------------------------------------------------------

Token_stream ts;        // provides get() and putback()
Symbol_table st;        // provides get(), set(), is_declared() and declare()
float expression(stringstream& is);    // declaration so that primary() can call expression()

// -----------------------------------------------------------------------

// simple power function
// handles only integers >= 0 as exponents
float my_pow(float base, float expo)
{
    if (expo == 0) {
        if (base == 0) return 0;    // special case: pow(0,0)
        return 1;                   // something to power of 0
    }
    //float res = base;                 // corresponds to power of 1
    //for (int i = 2; i<=expo; ++i)   // powers of 2 and more
    //    res *= base;
    //return res;
    return pow(base,expo);
}

// deal with numbers, unary +/-, parentheses, sqrt, pow, names and assignments
// calls expression()
float primary(stringstream& is)
{
    //cout << "\nContents of st: " << endl << endl;
    //for(Variable v: st.var_table)
    //    cout << "\nv.name = " << v.name << ", v.value = " << v.value;
    Token t = ts.get(is);
    switch (t.kind) {
    case '(':   // handle '(' expression ')'
    {  float d = expression(is);
        t = ts.get(is);
        if (t.kind != ')') error("')' expected");
        return d;
    }
    case '-':
        return -primary(is);
    case '+':
        return primary(is);
    case number:
        return t.value;
    case name:
    {   //cout << "\nt.name = " << t.name << ", st.get(t.name) = " << st.get(t.name);
        Token t2 = ts.get(is);    // check next token
        if (t2.kind == '=') {   // handle name '=' expression
            float d = expression(is);
            st.set(t.name,d);
            //cout << "\nin primary, d = " << d;
            return d;
        }
        else {  // not an assignment
            ts.putback(t2);
            return st.get(t.name);
        }
   }
    case square_root:   // handle 'sqrt(' expression ')'
    {   t = ts.get(is);
        if (t.kind != '(') error("'(' expected");
        float d = expression(is);
        if (d < 0) error("Square roots of negative numbers... nope!");
        t = ts.get(is);
        if (t.kind != ')') error("')' expected");
        //double root = sqrt(d);
        //int root_int = int(root);
        //if (float(root) != root) error("sqrt operation resulted in non-floateger value");
        //return float(root);
        return sqrt(d);
   }
    case power: // handle 'pow(' expression ',' integer ')'
    {   t = ts.get(is);
        if (t.kind != '(') error("'(' expected");
        float d = expression(is);
        t = ts.get(is);
        if (t.kind != ',') error("',' expected");
        t = ts.get(is);
        if (t.kind != number) error("second argument of 'pow' is not a number");
        float e = t.value;
        //if (i != t.value) error("second argument of 'pow' is not an integer");
        t = ts.get(is);
        if (t.kind != ')') error("')' expected");
        return my_pow(d,e);
    }

    case absolute: // handle 'sin(' expression ')'
    {
        t = ts.get(is);
        if(t.kind != '(') error("'(' expected");
        float d = expression(is);
        t = ts.get(is);
        if(t.kind != ')') error ("')' expected");
        return fabs(d);
    }

    case tangent: // handle 'sin(' expression ')'
    {
        t = ts.get(is);
        if(t.kind != '(') error("'(' expected");
        float d = expression(is);
        t = ts.get(is);
        if(t.kind != ')') error ("')' expected");
        return tan(d);
    }

    case integer: // handle 'sin(' expression ')'
    {
        t = ts.get(is);
        if(t.kind != '(') error("'(' expected");
        float d = expression(is);
        t = ts.get(is);
        if(t.kind != ')') error ("')' expected");
        return int(d);

       // reteurn int(d);
        //else return -1.+int(d)
    }

    case arctangent: // handle 'sin(' expression ')'
    {
        t = ts.get(is);
        if(t.kind != '(') error("'(' expected");
        float d = expression(is);
        t = ts.get(is);
        if(t.kind != ')') error ("')' expected");
        return atan(d);
    }


    case sine: // handle 'sin(' expression ')'
    {
        t = ts.get(is);
        if(t.kind != '(') error("'(' expected");
        float d = expression(is);
        t = ts.get(is);
        if(t.kind != ')') error ("')' expected");
        return sin(d);
    }
    case cosine: // handle 'cos(' expression ')'
    {   //cout << "\nt.kind = " << t.kind << ", t.value = " << t.value;
        t = ts.get(is);
        if(t.kind != '(') error("'(' expected");
        float d = expression(is);
        //cout << "\nd = " << d;
        t = ts.get(is);
        if(t.kind != ')') error ("')' expected");
        return cos(d);
    }
    default:
        error("primary expected");
    }
}

// -----------------------------------------------------------------------

// deal with *, / and %
// calls primary()
float term(stringstream& is)
{
    float left = primary(is);
    Token t = ts.get(is); // get the next token from Token_stream

    while (true) {
        switch (t.kind) {
        case '*':
            left *= primary(is);
            t = ts.get(is);
            break;
        case '/':
        {   float d = primary(is);
            if (d == 0) error("divide by zero");
            left /= d;
            float left_float = float(left);
            if (left_float != left) error("division led to non-floateger value");
            t = ts.get(is);
            break;
        }
        case '%':
        {   float d = primary(is);
            //float i1 = float(left);
            //if (i1 != left) error("left-hand operand of % not float");
            //float i2 = float(d);
            //if (i2 != d) error("right-hand operand of % not int");
            if (d == 0) error("%: divide by zero");
            while(left>d)
                left = left - d;
            t = ts.get(is);
            break;
        }
        default:
            ts.putback(t);  // put t back into the Token_stream
            return left;
        }
    }
}

// -----------------------------------------------------------------------

// deal with + and -
// calls term()
float expression(stringstream& is)
{
    float left = term(is);  // read and evaluate a Term
    Token t = ts.get(is); // get the next Token from the Token stream

    while (true) {
        switch (t.kind) {
        case '+':
            left += term(is); // evaluate Term and add
            t = ts.get(is);
            break;
        case '-':
            left -= term(is); // evaluate Term and subtract
            t = ts.get(is);
            break;
        case '=':
            error("use of '=' outside of a declaration");
        default:
            ts.putback(t);  // put t back into the token strea
            return left;    // finally: no more + or -; return the answer
        }
    }
}

// -----------------------------------------------------------------------

// assume we have seen "let" or "const"
// handle: name = expression
// declare a variable called "name" with the initial value "expression"
float declaration(bool b, stringstream& is)
{
    Token t = ts.get(is);
    if (t.kind != name) error("name expected in declaration");
    std::string var_name = t.name;

    Token t2 = ts.get(is);
    if (t2.kind != '=') error("= missing in declaration of ",var_name);
    //cout << "\nt2.kind = " << t2.kind << ", t2.value = " << t2.value;
    float d = expression(is);
    //cout << "\nvar_name = " << var_name << ", d = " << d << ", b = " << b;
    st.declare(var_name,d,b);
    return d;
}

// -----------------------------------------------------------------------

// handles declarations and expressions
float statement(stringstream& is)
{
    Token t = ts.get(is);
    //cout << "\nt.kind = " << t.kind << ", t.value = " << t.value;
    switch (t.kind) {
    case let:
        return declaration(false, is);
    case con:
        return declaration(true, is);
    default:
        ts.putback(t);
        return expression(is);
    }
}

// -----------------------------------------------------------------------

// clean input after error
void clean_up_mess(stringstream& is)
{
    ts.ignore(print, is);
}

// -----------------------------------------------------------------------

/* print help instructions
void print_help()
{
    cout << "Instructions: enter expressions, there are\n";
    cout << "a few functions, you can declare variables using\n";
    cout << "the 'let' keyword and constants with the 'const'\n";
    cout << "keyword.\n";
}*/

const std::string prompt = "> ";
const std::string result = "= "; // used to indicate that what follows is a result



// expression evaluation loop
float calculate(stringstream& is)
{
    while (is)
        try {
        //cout << prompt;
        /*cout << "\nAfter passing to calculate(), is is ";
        string word;
        vector<string> vs;
        while(is>>word) vs.push_back(word);
        for(string s : vs) cout << s << " ";*/
        Token t = ts.get(is);
        //cout << "\nt.kind = " << t.kind << ", t.value = " << t.value;
        while (t.kind == print) t=ts.get(is); // first discard all "prints"
        //if (t.kind == help) print_help();   // print help instructions
        //else {
            //if (t.kind == quit) return 0;
        ts.putback(t);
            //cout << result << statement(is) << endl;
        return statement(is);
        //}
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;   // write error message
        clean_up_mess(is);
    }
}

// -----------------------------------------------------------------------


float calc(std::string fstr)
try
{
    stringstream is(fstr);
    return calculate(is);
}
catch (std::exception& e) {
    std::cout << "exception: " << e.what() << std::endl;
    char c;
    while (std::cin>>c && c!=';');
    return 1;
}
catch (...) {
   std::cout << "exception\n";
    char c;
    while (std::cin>>c && c!=';');
    return 2;
}
