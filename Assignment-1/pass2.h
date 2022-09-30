#include <bits/stdc++.h>
#include "pass1.h"
using namespace std;
//perform the pass 2 
void pass2()
{
    ifstream fin;
    fin.open("intermediate.txt");

    ofstream fout;
    fout.open("object.txt");
    int char_counter = 0;

    bool ifcomment = false;

    string START_ADDR = convert_Int_To_Hex(0);
    string LOCCTR = convert_Int_To_Hex(0);

    ifcomment = R_Intermediate_li(&LOCCTR, &fin);

    string prevLABEL = LABEL;
    string prevOPERAND = OPERAND;

    if (OPCODE == "START")
    {
        START_ADDR = (convert_Int_To_Hex(Conv_Hex_String_to_Int(OPERAND)));
        ifcomment = R_Intermediate_li(&LOCCTR, &fin);
    }

    //Writing Header_Record
    string HEADER = "H^";
    HEADER += prevLABEL;
    int i=0;
    while(i < 6 - prevLABEL.length())
    {
        HEADER += " ";
        i++;
    }
    HEADER += "^";
    HEADER += LOCCTR;
    HEADER += "^";
    HEADER += PROG_LEN;
    W_Obj_li(HEADER, &fout);

    //Initialising Text_Record
    string TEXT = "T^";
    TEXT += LOCCTR;
    TEXT += "^";
    TEXT += "  ";

    string OPERAND_ADDR;
    string OBJECT_CODE;

    while (OPCODE != "END")
    {
        if (ifcomment == false)
        {
            if (OPTAB.find(OPCODE) != OPTAB.end())
            {
                if (OPERAND != "")
                {
                    if (SYMTAB.find(OPERAND) != SYMTAB.end())
                    {
                        OPERAND_ADDR = SYMTAB[OPERAND];

                        //if OPCODE is LDCH or STCH we will add 8000 hex value to operand address 
                        if (OPCODE == "LDCH" || OPCODE == "STCH")
                        {
                            OPERAND_ADDR = convert_Int_To_Hex(Conv_Hex_String_to_Int("008000") + Conv_Hex_String_to_Int(OPERAND_ADDR));
                        }
                    }
                    else
                    {
                        OPERAND_ADDR = convert_Int_To_Hex(0);
                        cout << "ERROR:UNDEFINED SYMBOL" << endl;
                        exit(0);
                    }
                }
                else
                {   
                    //if OPERAND is empty OPERAND_ADDRESS will be 0000
                    OPERAND_ADDR = convert_Int_To_Hex(0);
                }
                //will make object code from OPCODE and OPERAND ADDRESS of the instruction
                OBJECT_CODE = ASSM_OBJ_COD(OPERAND_ADDR);
            }
            else if (OPCODE == "BYTE" || OPCODE == "WORD")
            {
                if (OPCODE == "WORD")
                {
                    OBJECT_CODE = convert_Int_To_Hex(Conv_Str_To_Int(OPERAND));
                }
                else
                {
                    if (OPERAND[0] == 'C')
                    {
                        OBJECT_CODE = "";
                        int i =2;
                        while(i < OPERAND.length() - 1)
                        {
                            OBJECT_CODE += convert_Int_To_HexLen2(OPERAND[i]);
                            i++;
                        }
                    }
                    else
                    {
                        OBJECT_CODE = "";
                        int i = 2;
                        while(i < OPERAND.length() - 1)
                        {
                            OBJECT_CODE += OPERAND[i];
                            i++;
                        }
                    }
                }
            }

            //check that if object code fits the text record of not and also break it if OPCODE is RESW or RESB
            if (OBJECT_CODE.length() + TEXT.length() > TEXT_RECORD_LEN + char_counter+2 || ((OPCODE == "RESW" || OPCODE == "RESB") && TEXT.length() > 11))
            {
                //finalizing text record
                string len = convert_Int_To_HexLen2((TEXT.length() - 11-char_counter) / 2);
                TEXT[9] = len[0];
                TEXT[10] = len[1];
                W_Obj_li(TEXT, &fout);

                //initializing next text record
                TEXT = "T^";
                TEXT += LOCCTR;
                TEXT += "^";
                TEXT += "  ";
                char_counter=0;
            }

            if (TEXT.length() == 11)
            {
                //Updating LOCCTR to text record
                TEXT = "T^";
                TEXT += LOCCTR;
                TEXT += "^";
                TEXT += "  ";
            }

            if (OPCODE != "RESW" && OPCODE != "RESB")
            {
                //writing object code to text record
                TEXT += "^";
                TEXT +=OBJECT_CODE;
                char_counter++;
            }
        }

        //read next line
        ifcomment = R_Intermediate_li(&LOCCTR, &fin);
    }

    string len = convert_Int_To_HexLen2((TEXT.length() - 11-char_counter) / 2);
    TEXT[9] = len[0];
    TEXT[10] = len[1];
    W_Obj_li(TEXT, &fout);

    //writing end record
    string endRecord = "E^";
    endRecord += START_ADDR;
    W_Obj_li(endRecord, &fout);

    fout.close();
    fin.close();
}

