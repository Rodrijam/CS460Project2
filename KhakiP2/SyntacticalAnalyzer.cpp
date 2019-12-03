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
	LSTfilename += ".lst";
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
	token = lex->GetToken();
	int totalErrors = program ();
	
	/*	Unsure about this, since there are isn't a function to put back tokens, creating a vector to hold tokens already
	bool found = false;	//found seems like a bad approach outside the scope of this class. 
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
	write_to_p2("Using Rule 1\n");
	int errors = 0;
	if (lex->GetTokenName(token) ==  "LPAREN_T")
		token = lex->GetToken(); 	// yo Jacob here (don't worry only comments) in my notes I have that we should
	else						 	// get the first token in the constructor since rule 1 hasn't been used until we
		errors++;					//  find a LPAREN_T, also this looks great for if the file is good, but i'm		
	errors += define();				// not seeing how it will continue if it is not.
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
		write_to_p2("Using Rule 3\n");
		token = lex->GetToken();
		errors += stmt_list();
		if (lex->GetTokenName(token) ==  "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else					// Rule 2
	{
		write_to_p2("Using Rule 2\n");
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
	write_to_p2("Using Rule 4\n");
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
		write_to_p2("Using Rule 6\n");
		return errors;
	}
	else							// Rule 5
	{
		write_to_p2("Using Rule 5\n");
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
		write_to_p2("Using Rule 8\n");
		token = lex->GetToken();
		return errors;
	}
	else if (lex->GetTokenName(token) == "LPAREN_T")	// Rule 9
	{
		write_to_p2("Using Rule 9\n");
		token = lex->GetToken();
		errors += action();
		if (lex->GetTokenName(token) ==  "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else							// Rule 7
	{
			
		//errors += stmt();
		
		int nerrors = literal();
		if (nerrors == 0)
			write_to_p2("Using Rule 7\n");
		else
		{
			errors = errors + nerrors + stmt();
		}
		
	}

	return errors;
}

int SyntacticalAnalyzer::literal ()
{
	int errors = 0;
	if (lex->GetTokenName(token) ==  "NUMLIT_T")		// Rule 10
	{
		write_to_p2("Using Rule 10\n");
		token = lex->GetToken();
	}
	else if (lex->GetTokenName(token) ==  "STRLIT_T")	// Rule 11
	{
		write_to_p2("Using Rule 11\n");
		token = lex->GetToken();
	}
	else if (lex->GetTokenName(token) ==  "SQUOTE_T")	// Rule 12
	{
		write_to_p2("Using Rule 12\n");
		token = lex->GetToken();
		errors += quoted_lit();
	}
	else
	{
		string mesg = "Error with: " + lex->GetTokenName(token) + string("\n");
		token = lex->GetToken();
		write_to_lst(mesg);
		errors++;
	}
	return errors;
}

int SyntacticalAnalyzer::quoted_lit ()
{
	int errors = 0;
	write_to_p2("Using Rule 13\n");
	errors += any_other_token();
	return errors;
}

int SyntacticalAnalyzer::more_tokens ()
{
	int errors = 0;
	if (lex->GetTokenName(token) ==  "RPAREN_T") 		// Rule 15
	{
		write_to_p2("Using Rule 15\n");
		return errors;
	}
	else							// Rule 14
	{
		write_to_p2("Using Rule 14\n");
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
		write_to_p2("Using Rule 17\n");
		return errors;
	}
	else if (lex->GetTokenName(token) == "IDENT_T")	// Rule 16
	{
		write_to_p2("Using Rule 16\n");
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
		write_to_p2("Using Rule 19\n");
		return errors;
	}
	else 							// Rule 18
	{
		write_to_p2("Using Rule 18\n");
		errors += stmt();
	}
	return errors;
}

int SyntacticalAnalyzer::stmt_pair ()
{
	int errors = 0;
	if (lex->GetTokenName(token) ==  "RPAREN_T") 		// Rule 21
	{
		write_to_p2("Using Rule 21\n");
		return errors;
	}
	else if (lex->GetTokenName(token) == "LPAREN_T")	// Rule 20
	{
		write_to_p2("Using Rule 20\n");
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
		token = lex->GetToken();
		write_to_p2("Using Rule 23\n");
		errors += stmt();
		if (lex->GetTokenName(token) == "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else												// RULE 22
	{
		write_to_p2("Using Rule 22\n");
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
	int errors = 0;
	string currToken = lex->GetTokenName(token);
	
	if (currToken == "NEWLINE_T")	// RULE 49
	{
		token = lex->GetToken();
		write_to_p2("Using Rule 49\n");
		return errors;
	}	
	else if (currToken == "COND_T") 	// RULE 25
	{
		token = lex->GetToken();
		write_to_p2("Using Rule 25\n");
		if (lex->GetTokenName(token) == "LPAREN_T")
		{
			token = lex->GetToken();;
			errors += stmt_pair_body();
		}
		else
			errors++;
		return errors;
	}
	
	static map<string, int> stmtStMap = 	{
											{"IF_T", 24}, {"LISTOP1_T", 26}, {"LISTOP2_T", 27}, {"NOT_T",30},
											{"NUMBERP_T",31}, {"LISTP_T",32}, {"ZEROP_T",33}, {"NULLP_T",34}, {"STRINGP_T",35},
											{"MINUS_T",37}, {"DIV_T",38}, {"MODULO_T",40}, {"ROUND_T",41}, {"DISPLAY_T",48} 
											};

	static map<string, int> stmtLSTMap = {
						{"AND_T", 28}, {"OR_T", 29}, {"PLUS_T", 36}, {"MULT_T", 39}, {"EQUALTO_T", 42},
						{"GT_T", 43}, {"LT_T", 44}, {"GTE_T", 45}, {"LTE_T", 46}, {"IDENT_T", 47},  
						};
											
	if (stmtStMap.count(currToken) == 1)
	{
		string ruleNum = to_string(stmtStMap.at(currToken));
		token = lex->GetToken();
		errors += stmt();
		if (currToken == "LISTOP2_T" || currToken == "MODULO_T" || currToken == "IF_T")
		{
			errors += stmt();
			if (currToken == "IF_T")
				errors += else_part();
			string mesg = "Using Rule " + ruleNum + string("\n");
			write_to_p2(mesg);
		}
		else if (currToken == "MINUS_T" || currToken == "DIV_T")
		{
			errors += stmt_list();
			string mesg = "Using Rule " + ruleNum + string("\n");
			write_to_p2(mesg);			
		}
		else
		{
			string mesg = "Using Rule " + ruleNum + string("\n");
			write_to_p2(mesg);		
		}
		return errors;
	}		
	else if (stmtLSTMap.count(currToken) == 1)
	{
		token = lex->GetToken();
		errors += stmt_list();
		string mesg = "Using Rule " + to_string(stmtLSTMap.at(currToken)) + string("\n");
		write_to_p2(mesg);	
	}	
	else
	{
		errors++;
		token = lex->GetToken();
		errors += action();
		
	}
	return errors;
}

int SyntacticalAnalyzer::any_other_token ()
{
	int errors = 0;
	
	if (lex->GetTokenName(token) == "LPAREN_T")			// RULE 50
	{
		write_to_p2("Using Rule 50\n");
		token = lex->GetToken();
		errors += more_tokens();
		if (lex->GetTokenName(token) == "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else if (lex->GetTokenName(token) == "SQUOTE_T")		// RULE 79
	{
		write_to_p2("Using Rule 79\n");
		token = lex->GetToken();
		errors += any_other_token();
	}
	else
	{
		
		static map<string, int> otherTokenMap = {
							{"IDENT_T", 51}, {"NUMLIT_T", 52}, {"STRLIT_T", 53}, {"LISTOP_T", 54}, {"IF_T", 55},
							{"DISPLAY_T", 56}, {"NEWLINE_T", 57}, {"LISTOP1_T", 58}, {"AND_T", 59}, {"OR_T", 60},
							{"NOT_T", 61}, {"DEFINE_T", 62}, {"NUMBERP_T", 63}, {"LISTP_T", 64}, {"ZEROP_T", 65}, 
							{"NULLP_T", 66}, {"STRINGP_T", 67}, {"PLUS_T", 68}, {"MINUS_T", 69}, {"DIV_T", 70}, 
							{"MULT_T", 71}, {"MODULO_T", 72}, {"ROUND_T", 73}, {"EQUALTO_T", 74}, {"GT_T", 75}, 
							{"LT_T", 76}, {"GTE_T", 77}, {"LTE_T", 78}, {"COND_T", 80}, {"ELSE_T", 81},  
							};
			
		if (otherTokenMap.count(lex->GetTokenName(token)) == 0)
			errors++;
		else
		{
			string mesg = "Using Rule " + to_string(otherTokenMap.at(lex->GetTokenName(token))) + string("\n");
			write_to_p2(mesg);
			token = lex->GetToken();
		}
	}			
	return errors;
}
