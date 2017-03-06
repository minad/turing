#include "TuringMachine.h"
#include <iostream>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

using namespace std;

const char TuringMachine::Tape::BLANK = '#';

TuringMachine::TuringMachine(const char *program)
   : state("INIT"), halted(false)
{
    readProgram(program);
}

TuringMachine::TuringMachine(const char *program, const char *tapeStr)
   : tape(tapeStr), state("INIT"), halted(false)
{
    readProgram(program);
}

void TuringMachine::nextTransition()
{
    if (halted || state == "HALT")
    {
        halted = true;
	return;
    }

    bool programError = true;
    for (vector<Transition>::iterator i = transitions.begin();
         i != transitions.end(); ++i)
    {
        //cout << i->state << ',' << i->symbol << ',' << state << ',' << tape.read() <<endl;
        if (!strcmp(i->state, state))
	{
	    if (i->symbol == tape.read())
            {
                state = i->newState;
	        tape.write(i->newSymbol);
		tape.move(i->tapeDir);
		return;
	    }
	    programError = false;
	}
    }

    if (programError)
        throw ProgramException();
    else
        throw BadTapeException();
}

void TuringMachine::print() const
{
    tape.print();
    cout << "\nState: " << state << endl;
}

void TuringMachine::readProgram(const char *programFile)
{
    FILE *fp = fopen(programFile, "r");
    if (!fp)
        throw IOException();
    while (!feof(fp))
    {
        char dir;
        Transition t;
        if (fscanf(fp, "%s %c %s %c %c", t.state, &t.symbol, t.newState, &t.newSymbol, &dir) == 0)
	    throw new ProgramFormatException();
        switch (dir)
        {
        case 'L': t.tapeDir = Tape::LEFT;  break;
        case 'R': t.tapeDir = Tape::RIGHT; break;
        case 'S': t.tapeDir = Tape::STOP;  break;
        }
        transitions.push_back(t);
    }
    fclose(fp);
}

TuringMachine::Tape::Tape(const char *str)
  : pos(0)
{
    for (const char *p = str; *p != '\0'; ++p)
        tape.push_back(*p);
    if (tape.size() == 0)
      tape.push_back('#');
}

void TuringMachine::Tape::move(int dir)
{
    pos += dir;
    if (pos >= (int)tape.size())
        tape.push_back(BLANK);
    else if (pos < 0)
    {
        tape.push_front(BLANK);
        pos = 0;
    }
}

void TuringMachine::Tape::print() const
{
  //for (int i = pos - 40; i < pos + 40; ++i)
    for(int i = 0; i < tape.size(); ++i)
    {
      //if (pos >= (int)tape.size() || pos < 0)
      //     cout << BLANK;
      //	else
      if (i == pos) cout << "\033[0;40m\033[1;37m" << tape[i] << "\033[0m";
      else cout << tape[i];

    }        
    //cout << "\n                                        ^";
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "usage: turing <program-file> [tape]" << endl;
        exit(EXIT_FAILURE);
    }

    try
    {
        TuringMachine machine(argv[1], argc >= 3 ? argv[2] : "");

        while (!machine.isHalted())
        {
	  //cout << "\033[H\033[J";
	    machine.print();
            machine.nextTransition();
        }
    }
    catch (const TuringMachine::Exception &e)
    { 
        cerr << "Exception caught: " << e.type() << endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
