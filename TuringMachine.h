#ifndef _TURING_MACHINE_H
#define _TURING_MACHINE_H

#include <deque>
#include <vector>

using namespace std;

class TuringMachine
{
public:

    struct Exception
    {
        virtual const char *type() const
        {
            return "Unknown exception";
        }
    };

    struct ProgramException : public Exception
    {
        virtual const char *type() const
        {
            return "Programm exception";
        }
    };

    struct BadTapeException : public Exception
    {
        virtual const char *type() const
        {
            return "Bad tape exception";
        }
    };

    struct ProgramFormatException : public Exception
    {
        virtual const char *type() const
        {
            return "Program format exception";
        }
    };

    struct IOException : public Exception
    {
        virtual const char *type() const
        {
            return "IO Exception";
        }
    };

    TuringMachine(const char *programFile);
    TuringMachine(const char *programFile, const char *tapeStr);

    void nextTransition();
    void print() const;

    bool isHalted() const { return halted; }

private:

    void readProgram(const char *programFile);

    class Tape
    {
    public:
    
      static const int LEFT = -1, RIGHT = 1, STOP = 0;
        static const char BLANK;   

        Tape() : pos(0) { }
        Tape(const char *str);

	char read() const
	{
            return tape[pos];
	}

	void write(char symbol)
	{
	    tape[pos] = symbol;
	}

	void move(int dir);
        void print() const;

    private:
        
        int pos;
        deque<char> tape;
    };

    struct Transition
    {
        char state[256],
	     newState[256],
             symbol,
	     newSymbol;
	int  tapeDir;
    };

    vector<Transition> transitions;
    Tape        tape;
    const char *state;
    bool        halted;
};

#endif
