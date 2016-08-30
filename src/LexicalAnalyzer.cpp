#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer()
{
    //ctor
}


LexicalAnalyzer::LexicalAnalyzer(DFA* minimizedDFA)
{
    this->dfa = minimizedDFA;
    currentIndex=0;
    DFAVector=new vector<DFAState*>();
}

LexicalAnalyzer::~LexicalAnalyzer()
{
    //dtor
}
Token* LexicalAnalyzer::getTokenNextToken()
{

    if((currentIndex >= tokens.size() ) && (currentString.size()==0))
    {
        return NULL;
    }
    int stringIndex=0;
    string tempString="";
    DFAVector=new vector<DFAState*>();
    DFAState* start=dfa->get_mini_start_state();
    DFAVector->push_back(start);
    if(currentString.size()==0)
    {

        currentString=tokens[currentIndex];
        currentIndex++;
    }

   //~~~~~~~~~Check if this is a clear valid input ~~~~~~~~~~~~
    DFAState* tempState=dfa->get_mini_start_state()->move(currentString);
    if(tempState != NULL){
         Token* toke=new Token();
         toke->tokenClass=tempState->get_tokenClass();
         toke->attribute_value=currentString;
         currentString="";
         if(toke->tokenClass->name == "epsilon"){
            toke->tokenClass->name ="Error : Invalid input";

         }
         return toke;
    }

    int i=0;
    DFAState* currentState=start->move_mini(currentString[i]);
    DFAVector->push_back(currentState);
   // if(currentState != NULL){
    i++;
    //}
    bool atend=false;
    while(i<currentString.size() && ( currentState != NULL ) && (currentState->move_mini(currentString[i]) != NULL))
    {
        atend=true;
        DFAState* tempState=currentState->move_mini(currentString[i]);
        currentState=tempState;
        if(currentState != NULL){
            DFAVector->push_back(currentState);
        }
        i++;
    }
    //if(atend){
    /*if(currentState == NULL && i<currentString.size())
        stringIndex=i-1;
    }
    else{
        stringIndex=i;
    }
*/

    stringIndex=i;
    if(stringIndex<currentString.size()) //there exists undefined input
    {
        tempString=currentString.substr(stringIndex);

    }
    //if(i>0){
    int size_=currentString.size()-tempString.size();
    currentString=currentString.substr(0,size_);
    //}

    //if(stringIndex==0 ){ // if an invalid input occured , print an error message in the file
    if(currentState==NULL && i==1){
        Token* error_= new Token();
        error_->tokenClass=new TokenClass("error : invalid input",-1000);
        error_->attribute_value=currentString;
        currentString=tempString;
        return error_;
    }
    DFAState* candidate=new DFAState();
    vector<DFAState*>* tempVector=new vector<DFAState*>();
    for(int i=DFAVector->size()-1; i>-1; i--)
    {
        if(DFAVector->at(i)->get_accepting())
        {
            candidate=DFAVector->at(i);
            break;
        }
    }

    Token* tokenFound=new Token();
    tokenFound->tokenClass=candidate->get_tokenClass();
    tokenFound->attribute_value=currentString;
    DFAVector=tempVector;
    currentString=tempString;
    return tokenFound;
}


void LexicalAnalyzer::setSourceCodeFile(string file)
{
    this->file_name = file;
    source_file.open(file.c_str());
    std::string input;

    int i=0;
    source_file >> input;
    tokens.push_back(input);
    while(!source_file.eof())
    {
        if(input.compare("\n") != 0  || input.compare(" ")){
        //tokens.push_back(input);
        }
        cout<<tokens.at(i)<<endl;
        i++;
        source_file >> input;
        tokens.push_back(input);
    }
    if(input.compare("\n") != 0 || input.compare(" ") ){
      //  tokens.push_back(input);
        }
        cout<<tokens.size()<<endl;
    //tokens.push_back(input);
   // cout<<"ll : "<<tokens.size()<<endl;
}



