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
#include "SyntacticalAnalyzer.h"

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
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::more_defines ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::define ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::stmt_list ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::stmt ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::literal ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::quoted_lit ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::more_tokens ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::param_list ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::else_part ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::stmt_pair ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::stmt_pair_body ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::action ()
{
	int errors = 0;
	return errors;
}

int SyntacticalAnalyzer::any_other_token ()
{
	int errors = 0;
	return errors;
}

