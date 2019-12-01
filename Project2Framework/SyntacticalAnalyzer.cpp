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
#include <string>
#include "SyntacticalAnalyzer.h"

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

	bool found = false;
	while(t!= EOF_T)
	{
		if(t == LPAREN_T)
		{
			found = true;
			int totalErrors = program();
		}
		t = lex->GetToken();
	}

}

void SyntacticalAnalyzer::write_to_lst(const string & msg){

}

void SyntacticalAnalyzer::write_to_p2(const string & msg) {

}
void SyntacticalAnalyzer::report_error(const string & msg){

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

