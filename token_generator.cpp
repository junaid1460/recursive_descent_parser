/*
 needs a c++11(stl) compiler
*/

#include<iostream>
#include<regex>
#include<string>
#include<fstream>

using namespace std;

//class to define lex rule
class lexeme{
    public:
    string rule;//rule name
    string match;//rule match
    //whether we have to skip this token?by default it will not skip
    bool skip;
    lexeme(){}
    lexeme(string x,string y,bool z = false){
        rule = x;
        match = y;
        skip = z;
    }
};

//class to represent a token
class token{
    public:
    string rule;//rule
    string value;//lexval
    token(){}
    token(string x,string y){
        rule= x;
        value=y;
    }
    //display it
    void display(){
        cout<<endl<<value<<"\t is selected for lex rule \t"<<rule;
    }
};

//calculating length of any array
template<typename T, size_t N>
size_t arraySize( T(&)[N] )
{
  return(N);
};

vector<token> lex(string content){

   //perfectly ordered lex rules along with it's regex match
     const lexeme lexemes[] ={
                             lexeme("value","\".*\""),
                             lexeme("print","print"),
                             lexeme("begin","begin"),
                             lexeme("for","for"),
                             lexeme("end","end"),
                             lexeme("to","to"),
                             lexeme("type","integer"),
                             lexeme("type","real"),
                             lexeme("type","string"),
                             lexeme("value","[0-9]+\.[0-9]+[Ee]\-[0-9]+"),
                             lexeme("value","[0-9]+"),
                             lexeme("lb","[\(]"),
                             lexeme("rb","[\)]"),
                             lexeme("sp"," +",true),
                             lexeme("nl","[\n]",true),
                             lexeme("id","[_a-zA-Z]+[a-zA-Z0-9]*"),
                             lexeme("op","[(\>\=)(\<\=)(\!\=)\<\>]"),
                             lexeme("eq","\:\="),
                             lexeme("cmm","\,"),
                             lexeme("sc","\;"),
                        };
//regex match               
smatch m;
//temporary tokens vector
std::vector<token> tokens;
unsigned cc =0;

outer:while(content.length()>0){
for(unsigned j=0;j<arraySize(lexemes);j++){
    regex_search(content,m,regex(lexemes[j].match));
  for (unsigned i=0; i<m.size(); ++i) {
      if(m.position(i)==0){
          if(!lexemes[j].skip)//do not push the token if it should be skipped
          tokens.push_back(token(lexemes[j].rule,m[i]));//push valid token
          content = content.substr(m[i].length(),content.length()-1);//reformat the string by removing matched token from the beginning of the string
          //charecter count
          cc+=m[i].length();
          //skipping two inner for loops and getting back to while loop and starting the process from the beginning, where we fetch fot token
          goto outer;
      }
  }
}
//invalid token found ie: the token for which no rule is defined
cout<<"unexpected token at "<<cc;
exit(1);
}
//return the token vector
return tokens;
}