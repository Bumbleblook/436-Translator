#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
using namespace std;

// CS421 File translator.cpp 

// ** Be sure to put the name of the programmer above each function
// ** Be sure to put the corresponding rule with semantic routines
//    above each function

//=================================================                                                                                                                                 
// File translator.cpp written by Group Number: 16                              
//=================================================  
enum tokentype{ ERROR,WORD1,WORD2,PERIOD,VERB,VERBPAST,VERBNEG,VERBPASTNEG,IS,WAS,OBJECT,SUBJECT,DESTINATION,PRONOUN,CONNECTOR,EOFM};


string reservedWords[30][2] = {
  {"masu",         ""},
  {"masen",        ""},
  {"mashita",      ""},
  {"masendeshita", ""},
  {"desu",         ""},
  {"deshita",      ""},
  {"o",            ""},
  {"wa",           ""},
  {"ni",           ""},
  {"watashi",      "I/me"},
  {"anata",        "you"},
  {"kare",         "he/him"},
  {"kanojo",       "she/her"},
  {"sore",         "it"},
  {"mata",         "Also"},
  {"soshite",      "Then"},
  {"shikashi",     "However"},
  {"dakara",       "Therefore"},
  {"eofm",         ""}  
};

tokentype reservedWordsType[30] = {VERB,VERBNEG,VERBPAST,VERBPASTNEG,IS,WAS,OBJECT,SUBJECT,DESTINATION,PRONOUN,PRONOUN,PRONOUN,PRONOUN,PRONOUN,CONNECTOR,CONNECTOR,CONNECTOR,CONNECTOR,EOFM};

string tokenName[30] = {"ERROR","WORD1","WORD2","PERIOD","VERB","VERBPAST","VERBNEG","VERBPASTNEG","IS","WAS","OBJECT","SUBJECT","DESTINATION","PRONOUN","CONNECTOR","EOFM"};

string wordTable[50][2] = {{"watashi","I/me"},{"anata","you"},{"kare","he/him"},{"kanojo","she/her"},{"sore","it"},
{"mata","also"},{"soshite","then"},{"shikashi","however"},{"dakara","therefore"},{"daigaku","college"},
{"kurasu","class"},{"hon","book"},{"tesuto","test"},{"ie","home"},{"isu","chair"},{"seito","student"},
{"sensei","teacher"},{"tomodachi","friend"},{"jidoosha","car"},{"ayuunyuu","milk"},{"biiru","beer"},
{"choucho","butterfly"},{"ryouri","cooking"},{"toire","restroom"},{"gohan","meal"},{"yasashii","easy"},
{"muzukashii","difficult"},{"ureshii","pleased"},{"shiawase","happy"},{"kanashii","sad"},{"omoi","heavy"},
{"oishii","delicious"},{"tennen","natural"},{"nakI","cry"},{"ikI","go"},{"tabE","eat"},{"ukE","take"},{"kakI","write"},
{"yamI","read"},{"nami","drink"},{"agE","give"},{"moraI","receive"},{"butsI","hit"},{"kerI","kick"},{"shaberI","talk"},
{"yarI","do"},{"yorokobI","enjoy"},{"sore","it"},{"sore","it"},{"sore","it"}};

tokentype next_token();
bool word(string s);
bool period(string s);
int scanner(tokentype &a, string &w);
bool match(tokentype expected);
void getEword();
void gen(string);
void Story();
void S();
void AfterSubject();
void AfterNoun();
void AfterObject();
void Noun();
void Verb();
void Be();
void Tense();
void Syntax_Error1(tokentype);
void Syntax_Error2(string);
tokentype  saved_token;     // global buffer for the scanner token
bool   token_available;          // global flag indicating whether
string saved_lexeme;
string savedEword;
bool error1 = false;
bool error2 = false;
// we have saved a token to eat up or not
// ** Be sure to put the name of the programmer above each function
// i.e. Done by:

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)

// ** Need the updated match and next_token (with 2 global vars)


ifstream fin;
ofstream outfile;
ofstream errors ("errors.txt");

/** Additions to parser.cpp here:
 getEword - using the current lexeme, look up the English word in the Lexicon if it is there -- save the result in saved_E_word gen(line_type) - using the line type, display a line of an IR (saved_E_word or saved_token is used)
 The test driver to start the parser
 Done by  Riya Patel **/
int main()
{
  tokentype thetype;
  string theword;
  string filename;

  //- opens the output file translated.txt
  outfile.open("lexicon.txt");
  //- opens the input file  
  cout << "Enter the input file name: ";
  cin >> filename;
  fin.open(filename.data());
  //- calls the <story> to start parsing
  Story();
  cout << endl;
  if(error1 == false && error2 == false)
    cout << "succesfully parsed <Story>" << endl;
  //- closes the input file 
  fin.close();
  //- closes traslated.txt
  outfile.close();

  return 0;
}// end

//** should require no other input files!

// ** Be sure to put the name of the programmer above each function                                                                                                                
// i.e. Done by:                                                                                                                                                                   
// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)                                                                                                             
// ** Need the updated match and next_token (with 2 global vars)                                                                                                                   

// Done by:Akash Nemade
tokentype next_token()
{  
  if (!token_available)   // if there is no saved token from previous lookahead                                                                                                    
    {
      scanner(saved_token, saved_lexeme);  // call scanner to grab a new token                                                                 
      cout << "Scanner called using word: " << saved_lexeme << endl;
      if(saved_token == ERROR)
	{
	  cout << "LEXICAL ERROR: " << saved_lexeme << " is not a valid token" << endl;
	}
      token_available = true;  // mark that fact that you have saved it                                                                                           
    }
  return saved_token;    // return the saved token                                                                                                                                 
}

// i.e. Done by:Riya Patel                                                                                                 
bool match(tokentype expected)
{
 
  if (next_token() != expected)  // mismatch has occurred with the next token                                                                                                      
    {
      Syntax_Error1(expected); // generate a syntax error message here                                                                                                                   
      scanner(saved_token, saved_lexeme);
      match(saved_token);
      // do error handling here if any                                                                                                                                             
    }
  else  // match has occurred                                                                                                                                                      
    {
      token_available = false;  // eat up the token                                                                                                                               
      return true;              // say there was a match                                                                                                                           
    }
}

/** Make each non-terminal into a function here getEword - using the current lexeme, 
look up the English word in the Lexicon if it is there -- save the result in saved_E_word  
Done by: Eric Perez**/
void getEword(){
	for (int i=0;i<48;i++){
		if(wordTable[i][0] == saved_lexeme){
			savedEword = wordTable[i][1];
			return;
		}
	}
	savedEword = saved_lexeme;
}	

// gen(line_type) - using the line type, display a line of an IR (saved_E_word or saved_token is used)                                                                                                                                                                                                                                                       

// i.e. Done by:Akash Nemade
void gen(string word)
{
  if(word != "TENSE"){
      outfile << word << ": " << savedEword << endl;
    }
  else{
      outfile << word << ": " << tokenName[saved_token] << endl;
    }
}

//RE:<story>  ::= <s> { <s> }                                                                                                                
// i.e. Done by:Akash Nemade                                                                                                             
void Story()
{
  cout << "Processing <Story> \n\n";
  S();
  while(next_token() != EOFM)
    {
      switch(next_token())
        {
        case CONNECTOR:
        case WORD1:
        case PRONOUN: S();
          break;
        default: return;
        }
    }
}

//RE:<s>  ::=  [CONNECTOR #getEword# #gen(“CONNECTOR”)#] <noun> #getEword SUBJECT #gen(“ACTOR”)# <afterSubject> 
// i.e. Done by:Akash Nemade                                                                                                             
void S()
{
  cout << "Processing <S>" << endl;
  if(next_token() == CONNECTOR)
    {
      match(CONNECTOR);
      cout << "Matched CONNECTOR\n";
      getEword();
      gen("CONNECTOR");
    }
  Noun(); 
  getEword();
  match(SUBJECT);
  gen("Actor"); 
  cout << "Matched SUBJECT" << endl;
  AfterSubject(); 
}

//RE:<afterSubject>  ::= <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | <noun>  #getEword# <afterNoun>                                                         
// i.e. Done by:Akash Nemade  
void AfterSubject()
{
  cout << "Processing <After-Subject>" << endl;
  switch(next_token())
    {
    case WORD2: Verb(); getEword(); gen("ACTION"); Tense(); gen("TENSE"); match(PERIOD); cout << "Matched PERIOD" << endl;
      break;
    case WORD1:
    case PRONOUN: Noun(); getEword(); AfterNoun();
      break;
    default: Syntax_Error2("<After-Subject");
    }
}

//RE:<afterNoun>  ::=  <be> #gen(“DESCRIPTION”)#  #gen(“TENSE”)# PERIOD | DESTINATION  #gen(“TO”)# <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | OBJECT #gen(“OBJECT”)# <afterObject>
// i.e. Done by:Akash Nemade
void AfterNoun()
{
  cout << "Processing <After-Noun>" << endl;
  switch (next_token())
    {
    case IS :
    case WAS : Be(); gen("DESCRIPTION"); gen("TENSE"); match(PERIOD); cout << "Matched PERIOD" << endl; 
      break;
    case DESTINATION : match(DESTINATION); cout << "Matched DESTINATION" << endl; gen("TO"); Verb(); getEword(); gen("ACTION"); Tense(); gen("TENSE"); match(PERIOD); cout << "Matched PERIOD" << endl; 
      break;
    case OBJECT : match(OBJECT); cout << "Matched OBJECT" << endl; gen("OBJECT"); AfterObject(); 
      break;
    default : Syntax_Error2("After-Noun");
    }
}

//RE:<afterObject> ::= <verb>  #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | <noun>  #getEword# DESTINATION  #gen(“TO”)# <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)#PERIOD                                                     
// i.e. Done by:Akash Nemade                                                                                                                
void AfterObject()
{
  cout << "Processing <After-Object>" << endl;
  switch(next_token())
    {
    case WORD2: Verb(); getEword(); gen("ACTION"); Tense(); gen("TENSE"); match(PERIOD); cout << "Matched PERIOD" << endl;
      break;
    case WORD1:
    case PRONOUN: Noun(); getEword(); match(DESTINATION); cout << "Matched DESTINATION" << endl; gen("TO"); Verb(); getEword(); gen("ACTION"); Tense();
      gen("TENSE"); match(PERIOD); cout << "Matched PERIOD" << endl; 
      break;
    default: Syntax_Error2("After-Object");
    }
}


//RE:<noun>  ::= WORD1 | PRONOUN                                                                                                    
// i.e. Done by:Riya Patel                                                                                                        
void Noun()
{
  cout << "Processing <Noun>" << endl;
  switch(next_token())
    {
    case WORD1: match(WORD1); cout << "Matched WORD1" << endl;
      break;
    case PRONOUN: match(PRONOUN); cout << "Matched PRONOUN" << endl;
      break;
    default: Syntax_Error2("Noun");
    }
}

//RE:<verb>  ::= WORD2                                                                                                                       
// i.e. Done by:Riya Patel             
void Verb()
{
  cout << "Processing <Verb>" << endl;
  match(WORD2);
  cout << "Matched WORD2" << endl;
}

//RE:<be>  ::= IS | WAS                                                                                                           
// i.e. Done by:Eric Perez                                                                                                    
void Be(){
  cout << "Processing <Be>" << endl;
  switch(next_token())
    {
    case IS: match(IS); cout << "Matched IS" << endl;
      break;
    case WAS: match(WAS); cout << "Matched WAS" << endl;
      break;
    default: Syntax_Error2("BE");
    }
}

//RE:<tense>  ::= VERBPAST  | VERBPASTNEG | VERB | VERBNEG                                                                       
// i.e. Done by:Eric Perez
void Tense(){
  cout << "Processing <Tense>" << endl;
  switch(next_token())
    {
    case VERBPAST: match(VERBPAST); cout << "Matched VERBPAST" << endl;
      break;
    case VERBPASTNEG: match(VERBPASTNEG); cout << "Matched VERBPASTNEG" << endl;
      break;
    case VERB: match(VERB); cout << "Matched VERB" << endl;
      break;
    case VERBNEG: match(VERBNEG); cout << "Matched VERBNEG" << endl;
      break;
    default: Syntax_Error2("TENSE");
    }
}
// ** Be sure to put the corresponding grammar rule above each function                                                                      

// ** Done by: Eric Perez                                                                                                                                                       
void Syntax_Error1(tokentype thetoken)
{
  cout << "SYNTAX ERROR: Expected " << tokenName[thetoken] << " but found " << saved_lexeme <<endl;
  error1 = true; 
  errors << "SYNTAX ERROR: Expected " << tokenName[thetoken] << " but found " << saved_lexeme <<endl;
  exit(1);  
}

// ** Done by: Eric Perez                                                                                                                                                      
void Syntax_Error2(string func)
{
  cout << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in parser function " << func << endl;
  error2 = true;
  errors << "SYNTAX ERROR: unexpected " << saved_lexeme << " found in parser function " << func << endl;
  exit(1);
}

///////////////////////////Scanner-Functions///////////////////////////////
///////////////////////////////////////////////////////////////////////////
// ** Done by: Eric Perez                                                                                                                 
// ** RE: (vowel | vowel n | consonant vowel | consonant vowel n |consonant-pair vowel | consonant-pair vowel n)^+   
bool word(string s) {
  int state = 0;
  int charpos = 0;

  while (s[charpos] != '\0') {

    /************************/
    /*    State 0 block     */
    /************************/
    if (state == 0 &&
	(s[charpos] == 'a' || tolower(s[charpos]) == 'e' || tolower(s[charpos]) == 'i' || s[charpos] == 'o' ||
	 s[charpos] == 'u'))
      state = 1;
    else if (state == 0 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' ||
			    s[charpos] == 'm' || s[charpos] == 'n' || s[charpos] == 'p' || s[charpos] == 'r'))
      state = 2;
    else if (state == 0 && (s[charpos] == 'c'))
      state = 3;
    else if (state == 0 && (s[charpos] == 'd' || s[charpos] == 'j' || s[charpos] == 'w' || s[charpos] == 'y' ||
			    s[charpos] == 'z'))
      state = 4;
    else if (state == 0 && (s[charpos] == 's'))
      state = 5;
    else if (state == 0 && (s[charpos] == 't'))
      state = 6;

    /************************/
    /*    State 1 block     */
    /************************/
    else if (state == 1 &&
	     (s[charpos] == 'a' || tolower(s[charpos]) == 'e' || tolower(s[charpos]) == 'i' || s[charpos] == 'o' ||
	      s[charpos] == 'u'))
      state = 1;
    else if (state == 1 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' ||
			    s[charpos] == 'm' || s[charpos] == 'p' || s[charpos] == 'r'))
      state = 2;
    else if (state == 1 && (s[charpos] == 'c'))
      state = 3;
    else if (state == 1 && (s[charpos] == 'd' || s[charpos] == 'j' || s[charpos] == 'w' || s[charpos] == 'y' ||
			    s[charpos] == 'z'))
      state = 4;
    else if (state == 1 && (s[charpos] == 's'))
      state = 5;
    else if (state == 1 && (s[charpos] == 't'))
      state = 6;
    else if (state == 1 && (s[charpos] == 'n'))
      state = 7;

    /************************/
    /*    State 2 block     */
    /************************/
    else if (state == 2 && (s[charpos] == 'y'))
      state = 4;
    else if (state == 2 &&
	     (s[charpos] == 'a' || tolower(s[charpos]) == 'e' || tolower(s[charpos]) == 'i' || s[charpos] == 'o' ||
	      s[charpos] == 'u'))
      state = 1;

    /************************/
    /*    State 3 block     */
    /************************/
    else if (state == 3 && (s[charpos] == 'h'))
      state = 4;

    /************************/
    /*    State 4 block     */
    /************************/
    else if (state == 4 &&
	     (s[charpos] == 'a' || tolower(s[charpos]) == 'e' || tolower(s[charpos]) == 'i' || s[charpos] == 'o' ||
	      s[charpos] == 'u'))
      state = 1;

    /************************/
    /*    State 5 block     */
    /************************/
    else if (state == 5 &&
	     (s[charpos] == 'a' || tolower(s[charpos]) == 'e' || tolower(s[charpos]) == 'i' || s[charpos] == 'o' ||
	      s[charpos] == 'u'))
      state = 1;
    else if (state == 5 && (s[charpos] == 'h'))
      state = 4;

    /************************/
    /*    State 6 block     */
    /************************/
    else if (state == 6 &&
	     (s[charpos] == 'a' || tolower(s[charpos]) == 'e' || tolower(s[charpos]) == 'i' || s[charpos] == 'o' ||
	      s[charpos] == 'u'))
      state = 1;
    else if (state == 6 && (s[charpos] == 's'))
      state = 5;

    /************************/
    /*    State 7 block     */
    /************************/
    else if (state == 7 &&
	     (s[charpos] == 'a' || tolower(s[charpos]) == 'e' || tolower(s[charpos]) == 'i' || s[charpos] == 'o' ||
	      s[charpos] == 'u'))
      state = 1;
    else if (state == 7 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' || s[charpos] == 'k' ||
			    s[charpos] == 'm' || s[charpos] == 'n' || s[charpos] == 'p' || s[charpos] == 'r'))
      state = 2;
    else if (state == 7 && (s[charpos] == 'c'))
      state = 3;
    else if (state == 7 && (s[charpos] == 'd' || s[charpos] == 'j' || s[charpos] == 'w' || s[charpos] == 'y' ||
			    s[charpos] == 'z'))
      state = 4;
    else if (state == 7 && (s[charpos] == 's'))
      state = 5;
    else if (state == 7 && (s[charpos] == 't'))
      state = 6;
    else
      return (false);
    charpos++;
  }//end of while

  // where did I end up????
  if (state == 1 || state == 7) return (true);  // end in a final state
  else return (false);
}


// ** Done by:Eric Perez
bool period(string s) {
  int state = 0;
  int charpos = 0;

  while (s[charpos] != '\0') {
    if (state == 0 && (s[charpos] == '.'))
      state = 1;
    else
      return (false);
    charpos++;
  }

  if (state == 1) return (true);
  else return (false);
}
//Done by: Akash Nemade, Riya Patel
int scanner(tokentype &a, string &w) {

  // ** Grab the next word from the file via fin
  bool reserved = false;

  fin >> w;
  if (w == "eofm")
    return 0;
  if (period(w))
    a = PERIOD;
  else if (word(w)) {
    for (int i = 0; i < 30; i++) {
      if (w == reservedWords[i][0]) {
	a = reservedWordsType[i];
	reserved = true;
	break;
      }
    }
    if (!reserved) {
      if (w[w.length() - 1] == 'I' || w[w.length() - 1] == 'E')
	a = WORD2;
      else
	a = WORD1;
    }
  } else a = ERROR;
  return 1;

  

}//the end


