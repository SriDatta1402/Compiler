#include <bits/stdc++.h>
#include "functions.h"
using namespace std;

//perform the pass 1 
void pass1()
{
    ifstream fin;
    fin.open("input.txt");

    ofstream fout;
    fout.open("intermediate.txt");

    bool ifcomment = false;
    ifcomment = R_input_Line(&fin);

    if (OPCODE == "START")
    {
        LOCCTR = (convert_Int_To_Hex(Conv_Hex_String_to_Int(OPERAND)));
        START_ADDR = (convert_Int_To_Hex(Conv_Hex_String_to_Int(OPERAND)));
        W_Intermed_li(LOCCTR, &fout);
        ifcomment = R_input_Line(&fin);
    }
    else
    {
        START_ADDR = convert_Int_To_Hex(0);
	LOCCTR = convert_Int_To_Hex(0);    
    }

    while (OPCODE != "END")
    {
        if (ifcomment == false)
        {
            if (LABEL != "")
            {
                if (SYMTAB.find(LABEL) != SYMTAB.end())
                {
                    cout << "ERROR:DUPLICATE SYMBOL" << endl;
                    exit(0);
                }
                else
                {
                    //store add of LABEL in SYMTAB
                    SYMTAB[LABEL] = LOCCTR;
                }
            }
            W_Intermed_li(LOCCTR, &fout);

            //incr the LOCCTR
            Increment_LOCCTR(&LOCCTR);
        }
        else
        {
            string comment = LABEL;
            LABEL = "";
            W_Intermed_li(comment, &fout);
        }

        //read next_line
        ifcomment = R_input_Line(&fin);
    }

    W_Intermed_li("", &fout);

    //cal program_length
    PROG_LEN = convert_Int_To_Hex(Conv_Hex_String_to_Int(LOCCTR) - Conv_Hex_String_to_Int(START_ADDR));

    fout.close();
    fin.close();
}


