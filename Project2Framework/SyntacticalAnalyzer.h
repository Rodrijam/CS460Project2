/*******************************************************************************
* File name:                                                                   *
* Project: CS 460 Project 2 Fall 2019                                          *
* Author(s):                                                                   *
* Date:                                                                        *
* Description: This file contains                                              *
*******************************************************************************/

#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:

	ofstream lst;
	ofstream p2;

	void write_to_lst(const string & msg);
	void write_to_p2(const string & msg) ;

	LexicalAnalyzer * lex;
	token_type token;
	int program ();
	int more_defines ();
	int define ();
	int stmt_list ();
	int stmt ();
	int literal ();
	int quoted_lit ();
	int more_tokens ();
	int param_list ();
	int else_part ();
	int stmt_pair ();
	int stmt_pair_body ();
	int action ();
	int any_other_token ();
};
	
#endif
