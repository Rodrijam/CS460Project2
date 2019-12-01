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
	lex = new LexicalAnalyzer (filename);
	token_type t;
	int totalErrors = program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
}

int SyntacticalAnalyzer::program ()
{
	//Rule 1
	int errors = 0;
	if (token ==  "LPAREN_T")
		token = lex->GetToken();
	else
		errors++;						
	errors += define();
	if (token ==  "LPAREN_T")
		token = lex->GetToken();
	else
		errors++;
	errors += more_defines();
	if (token ==  "EOF_T")
		token = lex->GetToken();
	else
		errors++;
	return errors;
}

int SyntacticalAnalyzer::more_defines ()
{
	int errors = 0;
	if (token ==  "IDENT_T") //Rule 3
	{
		token = lex->GetToken();
		errors += stmt_list();
		if (token ==  "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else					// Rule 2
	{
		errors += define();
		if (token ==  "LPAREN_T")
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
	int errors = 0;
	if (token ==  "DEFINE_T")
		token = lex->GetToken();
	else
		error++;
	if (token ==  "LPAREN_T")
		token = lex->GetToken();
	else
		error++;
	if (token ==  "IDENT_T")
		token = lex->GetToken();
	else
		error++;
	error += param_list();
	if (token ==  "RPAREN_T")
		token = lex->GetToken();
	else
		error++;
	errors += stmt();
	errors += stmt_list();
	if (token ==  "RPAREN_T")
		token = lex->GetToken();
	else
		errors++;
	return errors;
}

int SyntacticalAnalyzer::stmt_list ()
{
	int errors = 0;
	if (token ==  "RPAREN_T") 		// Rule 6
		return errors;
	else							// Rule 5
	{
		errors += stmt();
		errors += stmt_list();
	}
	return errors;
}

int SyntacticalAnalyzer::stmt ()
{
	int errors = 0;
	if (token ==  "IDENT_T")		// Rule 8
	{
		token = lex->GetToken();
		return errors;
	}
	else if (token == "LPAREN_T")	// Rule 9
	{
		token = lex->GetToken();
		errors += action();
		if (token ==  "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else							// Rule 7
		errors += literal();

	return errors;
}

int SyntacticalAnalyzer::literal ()
{
	int errors = 0;
	if (token ==  "NUMLIT_T")		// Rule 10
		token = lex->GetToken();
	else if (token ==  "STRLIT_T")	// Rule 11
		token = lex->GetToken();
	else if (token ==  "SQUOTE_T")	// Rule 12
	{
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
	errors += any_other_token();
	return errors;
}

int SyntacticalAnalyzer::more_tokens ()
{
	int errors = 0;
	if (token ==  "RPAREN_T") 		// Rule 15
		return errors;
	else							// Rule 14
	{
		errors += any_other_token();
		errors += more_tokens();
	}
	return errors;
}

int SyntacticalAnalyzer::param_list ()
{
	int errors = 0;
	if (token ==  "RPAREN_T") 		// Rule 17
		return errors;
	else if (token == "IDENT_T")	// Rule 16
	{
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
	if (token ==  "RPAREN_T") 		// Rule 19
		return errors;
	else 							// Rule 18
		errors += stmt();
	return errors;
}

int SyntacticalAnalyzer::stmt_pair ()
{
	int errors = 0;
	if (token ==  "RPAREN_T") 		// Rule 21
		return errors;
	else if (token == "LPAREN_T")	// Rule 20
	{
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
	if (token == "ELSE_T")			// Rule 23
	{
		errors += stmt();
		if (token == "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else
	{
		errors += stmt();
		errors += stmt();
		if (token == "RPAREN_T")
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
	vector
	if (token == 0
	return errors;
}

int SyntacticalAnalyzer::any_other_token ()
{
	int errors = 0;
	
	if (token == "LPAREN_T")			// RULE 50
	{
		token = lex->GetToken();
		errors += more_tokens();
		if (token == "RPAREN_T")
			token = lex->GetToken();
		else
			errors++;
	}
	else if (token == "SQUOTE_T")		// RULE 79
	{
		token = lex->GetToken();
		errors += any_other_token();
	}
	else
	{
		static map<token, int> otherTokenMap = {
							{"IDENT_T", 51}, {"NUMLIT_T", 52}, {"STRLIT_T", 53}, {"LISTOP_T", 54}, {"IF_T", 55},
							{"DISPLAY_T", 56}, {"NEWLINE_T", 57}, {"LISTOP1_T", 58}, {"AND_T", 59}, {"OR_T", 60},
							{"", 61}, {"", 62}, {"", 63}, {"", 64}, {"", 65}, 
							{"", 66}, {"", 67}, {"", 68}, {"", 69}, {"", 70}, 
							{"", 71}, {"", 72}, {"", 73}, {"", 74}, {"", 75}, 
							{"", 76}, {"", 77}, {"", 78}, {"", 80}, {"", 81},  
							};
			
		if (otherTokenMap.count(token) == 0)
			errors++;
		else
		{
			// print RULE is otherTokenMap.find(token)
		}					
	}
						
	return errors;
}

