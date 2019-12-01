/*******************************************************************************
* File name:                                                                   *
* Project: CS 460 Project 2 Fall 2019                                          *
* Author(s):                                                                   *
* Date:                                                                        *
* Description: This file contains                                              *
*******************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include "SyntacticalAnalyzer.h"

/*
TODO:
Finish constructer
Add statments to put rules into a file per function
finish action and more_tokens maps
*/

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	string DBGfilename(filename);
	DBGfilename.erase(DBGfilename.size()-3);//removes .ss extension
	string LSTfilename = DBGfilename;
	string P2filename  = DBGfilename;
	DBGfilename += "2.dbg";
	LSTfilename += "2.lst";
	P2filename  += ".P2";

	dbg.open(DBGfilename,std::ofstream::out); //first open .dbg file
	if(dbg.is_open()){

		lst.open(LSTfilename,std::ofstream::out); //and on..
		if(lst.is_open()){
			write_to_lst("Input file: " + string(filename) + "\n");
			p2.open(P2filename,std::ofstream::out);
			if(p2.is_open()){
				//all files open.
			} else{
				report_error("Error Opening " + string(P2filename) + "\n");
			}
		} else{
			report_error("Error opening " + string(LSTfilename) + "\n");
		}
	} else{
		cout << "Error opening " + string(DBGfilename) + "\n";
	}



	lex = new LexicalAnalyzer (filename);
	token_type t;
	t = lex->GetToken();
	
	/*	Unsure about this, will discuss
	bool found = false;
	while(t!= EOF_T || !found)
	{
		if(t == LPAREN_T)
		{
			found = true;
			int totalErrors = program();
		}
		t = lex->GetToken();
	}
	*/

}

void SyntacticalAnalyzer::write_to_lst(const string & msg){
	if(lst.is_open()){
		lst << msg;
	} else{
		report_error("Error Writing to .lst: " + msg);}

}

void SyntacticalAnalyzer::write_to_p2(const string & msg){
	if(p2.is_open()){
		p2 << msg;
	} else{
		report_error("Error Writing to .p2: " + msg);}

}
void SyntacticalAnalyzer::report_error(const string & msg){
	if(dbg.is_open()){
		dbg << msg;
	} else{
		cout << "Error Writing to debug: " + msg;
	}

}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
}

int SyntacticalAnalyzer::program ()
{
	token = lex->GetToken();
	//Rule 1
	p2 << "Using Rule 1" <<  endl;
	int errors = 0;
	if (lex->GetTokenName(token) ==  "LPAREN_T")
		token = lex->GetToken(); // yo Jacob here (don't worry only comments) in my notes I have that we should
	else				 // get the first token in the constructor since rule 1 hasn't been used until we
		errors++;		//  find a LPAREN_T, also this looks great for if the file is good, but i'm		
	errors += define();		// not seeing how it will continue if it is not.
	if (lex->GetTokenName(token) ==  "LPAREN_T")
		token = lex->GetToken();
	else
		errors++;
	errors += more_defines();
	if (lex->GetTokenName(token) ==  "EOF_T")
		token = lex->GetToken();
	else
		errors++;
	return errors;
}

int SyntacticalAnalyzer::more_defines ()
{
	int errors = 0;
	if (lex->GetTokenName(token) ==  "IDENT_T") //Rule 3
	{
		cout << "Using Rule 3" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		if (lex->GetTokenName(token) ==  "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else					// Rule 2
	{
		cout << "Using Rule 2" << endl;
		errors += define();
		if (lex->GetTokenName(token) ==  "LPAREN_T")
			token = lex->GetToken();
		else
			errors++;
		errors += more_defines();
	}
	return errors;
}

int SyntacticalAnalyzer::define ()
{
	//Rule 4
	cout << "Using Rule 4" << endl;
	int errors = 0;
	if (lex->GetTokenName(token) ==  "DEFINE_T")
		token = lex->GetToken();
	else
		errors++;
	if (lex->GetTokenName(token) ==  "LPAREN_T")
		token = lex->GetToken();
	else
		errors++;
	if (lex->GetTokenName(token) ==  "IDENT_T")
		token = lex->GetToken();
	else
		errors++;
	errors += param_list();
	if (lex->GetTokenName(token) ==  "RPAREN_T")
		token = lex->GetToken();
	else
		errors++;
	errors += stmt();
	errors += stmt_list();
	if (lex->GetTokenName(token) ==  "RPAREN_T")
		token = lex->GetToken();
	else
		errors++;
	return errors;
}

int SyntacticalAnalyzer::stmt_list ()
{
	int errors = 0;
	if (lex->GetTokenName(token) ==  "RPAREN_T") 		// Rule 6
	{
		cout << "Using Rule 6" << endl;
		return errors;
	}
	else							// Rule 5
	{
		cout << "Using Rule 5" << endl;
		errors += stmt();
		errors += stmt_list();
	}
	return errors;
}

int SyntacticalAnalyzer::stmt ()
{
	int errors = 0;
	if (lex->GetTokenName(token) ==  "IDENT_T")		// Rule 8
	{
		cout << "Using Rule 8" << endl;
		token = lex->GetToken();
		return errors;
	}
	else if (lex->GetTokenName(token) == "LPAREN_T")	// Rule 9
	{
		cout << "Using Rule 9" << endl;
		token = lex->GetToken();
		errors += action();
		if (lex->GetTokenName(token) ==  "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else							// Rule 7
	{
		cout << "Using Rule 7" << endl;
		errors += literal();
	}

	return errors;
}

int SyntacticalAnalyzer::literal ()
{
	int errors = 0;
	cout << "This: " << lex->GetTokenName(token) << endl;
	if (lex->GetTokenName(token) ==  "NUMLIT_T")		// Rule 10
	{
		cout << "Using Rule 10" << endl;
		token = lex->GetToken();
	}
	else if (lex->GetTokenName(token) ==  "STRLIT_T")	// Rule 11
	{
		cout << "Using Rule 11" << endl;
		token = lex->GetToken();
	}
	else if (lex->GetTokenName(token) ==  "SQUOTE_T")	// Rule 12
	{
		cout << "Using Rule 12" << endl;
		token = lex->GetToken();
		errors += quoted_lit();
	}
	else
		errors++;
	return errors;
}

int SyntacticalAnalyzer::quoted_lit ()
{
	int errors = 0;
	cout << "Using Rule #" << endl;	//ADD NUMBER HERE
	errors += any_other_token();
	return errors;
}

int SyntacticalAnalyzer::more_tokens ()
{
	int errors = 0;
	if (lex->GetTokenName(token) ==  "RPAREN_T") 		// Rule 15
	{
		cout << "Using Rule 15" << endl;
		return errors;
	}
	else							// Rule 14
	{
		cout << "Using Rule 14" << endl;
		errors += any_other_token();
		errors += more_tokens();
	}
	return errors;
}

int SyntacticalAnalyzer::param_list ()
{
	int errors = 0;
	if (lex->GetTokenName(token) ==  "RPAREN_T") 		// Rule 17
	{
		cout << "Using Rule 17" << endl;
		return errors;
	}
	else if (lex->GetTokenName(token) == "IDENT_T")	// Rule 16
	{
		cout << "Using Rule 16" << endl;
		token = lex->GetToken();
		errors += param_list();
	}
	else
		errors++;
	return errors;
}

int SyntacticalAnalyzer::else_part ()
{
	int errors = 0;
	if (lex->GetTokenName(token) ==  "RPAREN_T") 		// Rule 19
	{
		cout << "Using Rule 19" << endl;
		return errors;
	}
	else 							// Rule 18
	{
		cout << "Using Rule 18" << endl;
		errors += stmt();
	}
	return errors;
}

int SyntacticalAnalyzer::stmt_pair ()
{
	int errors = 0;
	if (lex->GetTokenName(token) ==  "RPAREN_T") 		// Rule 21
	{
		cout << "Using Rule 21" << endl;
		return errors;
	}
	else if (lex->GetTokenName(token) == "LPAREN_T")	// Rule 20
	{
		cout << "Using Rule 20" << endl;
		token = lex->GetToken();
		errors += stmt_pair_body();
	}
	else
		errors++;
	return errors;
}

int SyntacticalAnalyzer::stmt_pair_body ()
{
	int errors = 0;
	if (lex->GetTokenName(token) == "ELSE_T")			// Rule 23
	{
		cout << "Using Rule 23" << endl;
		errors += stmt();
		if (lex->GetTokenName(token) == "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else							// ADD RULE #
	{
		cout << "Using Rule #" << endl;
		errors += stmt();
		errors += stmt();
		if (lex->GetTokenName(token) == "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
		errors += stmt_pair();
	}
	return errors;
}

int SyntacticalAnalyzer::action ()
{
	//  ADD RULE NUMBERS
	int errors = 0;
	/*
	if (lex->GetTokenName(token) == "NEWLINE_T")	// RULE 49
	{
		cout << "Using Rule 49" << endl;
		return errors;
	}
	
	// if the token is in the stmtOnlyList, run
	
		errors += stmt();
		if the token is in the doublestmtlist, run
			errors += stmt();
		
	

	static map<string, int> otherTokenMap = {
						{"IDENT_T", 51}, {"NUMLIT_T", 52}, {"STRLIT_T", 53}, {"LISTOP_T", 54}, {"IF_T", 55},
						{"DISPLAY_T", 56}, {"NEWLINE_T", 57}, {"LISTOP1_T", 58}, {"AND_T", 59}, {"OR_T", 60},
						{"", 61}, {"", 62}, {"", 63}, {"", 64}, {"", 65}, 
						{"", 66}, {"", 67}, {"", 68}, {"", 69}, {"", 70}, 
						{"", 71}, {"", 72}, {"", 73}, {"", 74}, {"", 75}, 
						{"", 76}, {"", 77}, {"", 78}, {"", 80}, {"", 81},  
						};
	
	//stmtOlist = {""}
	
	
	*/
	cout << "Using an Action Rule" << endl;



	return errors;
}

int SyntacticalAnalyzer::any_other_token ()
{
	int errors = 0;
	
	if (lex->GetTokenName(token) == "LPAREN_T")			// RULE 50
	{
		cout << "Using Rule 50" << endl;
		token = lex->GetToken();
		errors += more_tokens();
		if (lex->GetTokenName(token) == "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else if (lex->GetTokenName(token) == "SQUOTE_T")		// RULE 79
	{
		cout << "Using Rule 79" << endl;
		token = lex->GetToken();
		errors += any_other_token();
	}
	else
	{
		/*
		static map<string, int> otherTokenMap = {
							{"IDENT_T", 51}, {"NUMLIT_T", 52}, {"STRLIT_T", 53}, {"LISTOP_T", 54}, {"IF_T", 55},
							{"DISPLAY_T", 56}, {"NEWLINE_T", 57}, {"LISTOP1_T", 58}, {"AND_T", 59}, {"OR_T", 60},
							{"", 61}, {"", 62}, {"", 63}, {"", 64}, {"", 65}, 
							{"", 66}, {"", 67}, {"", 68}, {"", 69}, {"", 70}, 
							{"", 71}, {"", 72}, {"", 73}, {"", 74}, {"", 75}, 
							{"", 76}, {"", 77}, {"", 78}, {"", 80}, {"", 81},  
							};
			
		if (otherTokenMap.count(lex->GetTokenName(token)) == 0)
			errors++;
		else
		{
			// print RULE is otherTokenMap.find(token)
		}
		*/
		cout << "Using Rule ANY_OTHER_TOKEN" << endl;
	}
						
	return errors;
}