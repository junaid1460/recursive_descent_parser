/***
*fragments 
    -lex() //token_generator.cpp
       splits charecter stream into tokens and stores in a vector
       on success it returns the vector of tokens
       on error stops execution 
    -parse() it uses 5 data.. one is the grammar, which is hash map of productions
       production is nothing but array of elements having Terminals and Non-terminals
       second data is set of tokens which is being evaluated for correctness
       third data is start symbol of the grammar
       4th and 5th are start and max index of tokens
***/
#include"token_generator.cpp"

//class representing grammar terminals and non terminals
class Element{
    public:
    //if it is a non-terminal
    bool isVar;
    //name of element
    string name;
    Element(){}
    Element(string x,bool y=false){
       name = x;
       isVar = y;
    }
    //token matching
    bool isMatch(token x){
        return name==x.rule;
    }
};
//hash map of elements which is together called as production
typedef std::map<int, Element> Elements;
//class representing a production
class Production{
    public:
    //hash of production
    Elements elements;
    Production(){}
    Production(const Element x[],int len){
        for(int i=0;i<len;i++)
          {
              //copy
              elements[i]=x[i];
          }  
        
    }
    //getting specified element of production with index
    Element at(int x){
        return elements[x];
    }
};
//hash map to represent grammar
typedef std::map<string, std::map<int,Production>> Grammar;
//a grammar variable kept global to access freely by both main() and parse() function
 Grammar grammar;
 //parse declaration
 int parse(string start,int rhead,int max);
 //vector of tokens. kept global for the ease of access
 vector<token> tokens;
int main(){
    //input file
    std::ifstream file ("./input");
    //read all file content into buffer
    std::string content( (std::istreambuf_iterator<char>(file) ),
                       (std::istreambuf_iterator<char>()    ) );
    //calling lex analyser
    tokens = lex(content);
//display all the tokens 
  for(int i=0;i<tokens.size();i++)
    tokens[i].display();
cout<<endl<<"\n-----Tokens----\n";
 
 //representation of grammar with Element class array
 Element s[] = {Element("B",true)};
 Element b[] = {Element("begin"),Element("STM",true),Element("end")};
 Element stm1[]={Element("type"),Element("L",true),Element("STM",true)};
 Element stm2[]={Element("id"),Element("eq"),Element("value"),Element("STM",true)};
 Element stm3[]={Element("print"),Element("value"),Element("STM",true)};
 Element stm4[]={Element("for"),Element("id"),Element("eq"),Element("value"),Element("to"),Element("value"),Element("STM",true)};
 Element l1[]={Element("id"),Element("cmm"),Element("L",true)};
 Element l2[]={Element("id")};


 //hash map initialization with production heads
 grammar["S"][0] = Production(s,arraySize(s));
 grammar["B"][0] = Production(b,arraySize(b));
 grammar["STM"][0] = Production(stm1,arraySize(stm1));
 grammar["STM"][1] = Production(stm2,arraySize(stm2));
 grammar["STM"][2] = Production(stm3,arraySize(stm3));
 grammar["STM"][3] = Production(stm4,arraySize(stm4));
 grammar["L"][0] = Production(l1,arraySize(l1));
 grammar["L"][1] = Production(l2,arraySize(l2));

 //calling parse function with start symbol, lower token boundary and upper token bounday
 parse("S",0,tokens.size()-1);
 
}
//print the production
void print(string start,int prod){
   cout<<endl<<start<<" -> ";
     for(int i=0;i<grammar[start][prod].elements.size();i++)
         cout<<grammar[start][prod].at(i).name<<" ";
}


//parser logic
 int parse(string start,int rhead,int max){
     int rhtemp=rhead;
     bool match=true;
     
     //iterating thru all the production of non-terminal
     for(int i=0;i<grammar[start].size();i++){
         //iterating thru all elements of a production
         for(int j=0;j<grammar[start][i].elements.size();j++){
             //if token is missing
             if(rhtemp>max)
               {
                   cout<<"\nError: unexpected token "<<tokens[rhead].value;
                    exit(0);
               }
            //if the element is a non-terminal then call the parse()
            //function with the new element which is a non terminal
              if(grammar[start][i].at(j).isVar) 
              {
              
              rhtemp = parse(grammar[start][i].at(j).name,rhtemp,max);
              }
            //if the token matches grammar rule
             else if(tokens[rhtemp].rule==grammar[start][i].at(j).name){
                //increment read count
                 rhtemp++;
                 match=true;
             }
             else{
                 //invalid production. Break and try another production
                match =false;
                 break;
             }
             //if the production matched correctly
             if(grammar[start][i].elements.size()==j+1){
                 //print the selected production
                 print(start,i);
                 return rhtemp;
             }
            
            match = true;
         }
         //or invalid production reinitialize read head to initial symbol
         rhtemp = rhead;
         
     }
     //error state where un expected token found
     if(!match&&rhead!=max){
         cout<<"Error : unexpected token "<<tokens[rhead].value;
         exit(0);
     }else
     {
         //print the global production
         print(start,0);
     }
     return match?rhtemp:rhead;
 }

