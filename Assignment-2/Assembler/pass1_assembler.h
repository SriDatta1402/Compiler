#include <bits/stdc++.h>
#include "functions_assembler.h"
using namespace std;
void pass1_assembler(){

    fseek(progamFile, 0, SEEK_SET);

    int words;

    char *args[MAXW];

    char *line = NULL, temp[MAXS];
    size_t len = 0;

    // store addresses

    // read first line
    getline(&line, &len, progamFile);
    strcpy(temp, line);
    int flagged = 0;
    if(f2(line[0],' '))
    flagged = 1;
    if(flagged){
        words = read_Line(temp, 1, args," ");
        LABEL = "";
        OPCODE = args[1];
    }else{
        words = read_Line(temp, 0, args," ");
        LABEL = args[0];
        OPCODE = args[1];
    }
    flagged = 0;
    if(strcmp(args[1], "START") == 0)
    flagged = 1;
    if(flagged){
        OPERAND = args[2];
        STARTADDR = hExToInt(OPERAND);
        PROGNAME = LABEL;
        LOCCTR = STARTADDR;

        // write line to intermediate file
        fprintf(intermediateFile, "%04X\t%s", LOCCTR, line);

        // read next input line
        getline(&line, &len, progamFile);
        strcpy(temp, line);
        if (f2(line[0],' ')==0)
        flagged = 0;
        if (flagged){
            words = read_Line(temp, 1, args," ");
            //LABEL = NULL;
            OPCODE = args[1];
        }else{
            words = read_Line(temp, 0, args," ");
            LABEL = args[0];
            OPCODE = args[1];
        }
    }else{
        LOCCTR = 0;
    }

    symtab *base = new symtab;
    symtab_list[PROGNAME] = base;
    while (f1(OPCODE,"END")==0){
        if (f2(line[0],'.')==0){ // check if not a comment
            if (f1(OPCODE,"CSECT")){
                LOCCTR = literals_Output(LOCCTR, intermediateFile, base);
                base->length = LOCCTR;
                base = new symtab;
                symtab_list[(string)LABEL] = base;
                LOCCTR = 0;
            }else if (f1(LABEL,"")==0){ // check if symbol in label
                if (base->search_symtab(LABEL)!=""){
                    cout<<"Error: Duplicate Symbol."<<LABEL<<endl;
                    exit(0);
                }else{
                    base->insert_symtab(LABEL, LOCCTR);
                }
            }

            extended = false;

            if (f2(OPCODE[0],'+')){
                extended = true;
                OPCODE = OPCODE.substr(1);
            }

            if (words > 2){
                OPERAND = args[2];
                if (f2(OPERAND[0],'=')){
                    literal_pool.insert(OPERAND);
                }
            }

            // write line to intermediate file
            if (f1(OPCODE,"EXTDEF")|| f1(OPCODE,"EXTREF") || f1(OPCODE,"LTORG")){
                fprintf(intermediateFile, "    \t%s", line);
            }else{
                fprintf(intermediateFile, "%04X\t%s", LOCCTR, line);
            }

            if(f(incr_LOCCTR(),1)){
                if (f1(OPCODE,"EXTREF")){
                    char *operand = &OPERAND[0];
                    int size = read_Line(operand, 0, args, ",");
                    int i=0;
                    while(i < size){
                        base->extref.insert(args[i]);
                        i++;
                    }
                }else if (f1(OPCODE,"EXTDEF")){
                    OPERAND = args[2];
                    char *operand = &OPERAND[0];
                    int size = read_Line(operand, 0, args, ",");
                    int i=0;
                    while(i < size){
                        base->extdef.insert(args[i]);
                        i++;
                    }
                }else if (f1(OPCODE,"LTORG")){
                    LOCCTR = literals_Output(LOCCTR, intermediateFile, base);
                }else if (f1(OPCODE,"EQU")){
                }else if (f1(OPCODE,"CSECT")){
                }else{
                    cout<<"ERROR: INVALID OPERATION CODE."<<endl;
                    exit(0);
                }
            }
            
        }else{
            fprintf(intermediateFile, "    \t%s", line);
        }

        getline(&line, &len, progamFile);
        strcpy(temp, line);

        if(f2(line[0],' ')){
            words = read_Line(temp, 1, args," ");
            LABEL = "";
            OPCODE = args[1];
        }else{
            words = read_Line(temp, 0, args," ");
            LABEL = args[0];
            OPCODE = args[1];
        }
    }

    fprintf(intermediateFile, "    \t%s\n", line);

    LOCCTR = literals_Output(LOCCTR, intermediateFile, base);

    PROGLEN = LOCCTR - STARTADDR;

    base->length = LOCCTR;

    cout<<"PASS 1 DONE"<<endl;
}

