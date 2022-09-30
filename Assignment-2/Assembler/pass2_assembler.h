#include <bits/stdc++.h>
#include "pass1_assembler.h"
using namespace std;
void pass2_assembler(){

    fseek(intermediateFile, 0, SEEK_SET);

    char *args[MAXW];
    int words;

    OPCODE="";
    LABEL="";
    OPERAND="";

    char *line = NULL, temp[MAXS], addr[MAXS];
    size_t len = 0;

    fscanf(intermediateFile, "%[^\t]s", addr);
    getline(&line, &len, intermediateFile);

    if (f2(line[0],' ')){
        string_Copy(temp, line);
        words = read_Line(temp, 1, args," ");
        LABEL = "";
        OPCODE = args[1];
    }else{
        string_Copy(temp, line);
        words = read_Line(temp, 0, args," ");
        LABEL = args[0];
        OPCODE = args[1];
    }

    bool first_sect = true;
    queue<modrec> modification_records;

    if (f1(OPCODE,"START")){
        fprintf(listFile, "%s%s", addr, line);

        fscanf(intermediateFile, "%[^\t]s", addr);
        getline(&line, &len, intermediateFile);
        string_Copy(temp, line);
        if (f2(line[0],' ')){
            words = read_Line(temp, 1, args," ");
            LABEL = "";
            OPCODE = args[1];
        }else{
            words = read_Line(temp, 0, args," ");
            LABEL = args[0];
            OPCODE = args[1];
        }
    }

    symtab *base = symtab_list[PROGNAME];

    fprintf(objectFile, "H%-6s%06X%06X\n", PROGNAME.c_str(), STARTADDR, base->length);

    char firstaddr[MAXS] = "";
    sprintf(firstaddr, "%0X", STARTADDR);
    char record[MAXS] = "";

    while(true){
        char objcode[MAXS];
        string_Copy(objcode, "");
        if (f2(line[1],'.')==0){ 
            if (f1(OPCODE,"CSECT")){
                base = symtab_list[LABEL];

                if (strlen(record) > 0){
                    fprintf(objectFile, "T%06X%02X%s\n", hExToInt(firstaddr),(int)strlen(record) / 2, record);
                    string_Copy(record, "");
                }

                while (!modification_records.empty()){
                    modrec rec = modification_records.front();
                    modification_records.pop();
                    fprintf(objectFile, "M%06X%02X%s%s\n", rec.addr, rec.length, rec.sign ? "+" : "-", rec.symbol.c_str());
                }

                fprintf(objectFile, "E");
                if (first_sect){
                    fprintf(objectFile, "E%06X", 0);
                    first_sect = false;
                }
                fprintf(objectFile, "\n\n\n");

                char* label = &LABEL[0];
                fprintf(objectFile, "H%-6s%06X%06X\n", label, 0, base->length);
            }

            if (words > 2){
                OPERAND = args[2];
            }

            extended = false;

            if (f2(OPCODE[0],'+')){
                extended = true;
                OPCODE=OPCODE.substr(1);
            }

            if (op_code *info = search_optab(OPCODE)){
                map <char,int> bits;
                bits['n']=0;
                bits['i']=0;
                bits['b']=0;
                bits['e']=0;
                bits['p']=0;
                bits['x']=0;
                int operand_value = 0;

                if (f(info->format,2)){
                    strcat(objcode, info->op_addr.c_str());
                    char* operand = &OPERAND[0];
                    int size = read_Line(operand, 0, args, ",");
                    strcat(objcode, register_list[args[0]].c_str());
                    if (f(size,2)){
                        strcat(objcode, register_list[args[1]].c_str());
                    }else{
                        strcat(objcode, "0");
                    }
                }else if (f(info->format,3) && !extended){
                    if (len > 1 && f2(OPERAND[OPERAND.length() - 1],'X') && f2(OPERAND[OPERAND.length() - 2],',')){
                        bits['x'] = 1;
                        OPERAND[OPERAND.length() - 2] = '\0';
                    }

                    if (words > 2){
                        if (f2(OPERAND[0],'#')){
                            bits['n'] = 0;
                            OPERAND = OPERAND.substr(1);
                        }else{
                            bits['n'] = 1;
                        }

                        if (f2(OPERAND[0],'@')){
                            bits['i'] = 0;
                            OPERAND = OPERAND.substr(1);
                        }else{
                            bits['i'] = 1;
                        }

                        if (!isdigit(OPERAND[0])){
                            string sym = base->search_symtab(OPERAND);
                            operand_value = (int)strtol(sym.c_str(), NULL, 16) - hExToInt(addr) - 3;
                            if (operand_value < 0){
                                operand_value += 1 << 12;
                            }
                            bits['p'] = 1;
                        }else{
                            bits['p'] = 0;
                            char * operand = &OPERAND[0]; 
                            operand_value = (int)strtol(operand, NULL, 10);
                        }
                    }else{
                        bits['n'] = 1;
                        bits['i'] = 1;
                    }

                    int num_objcode = hExToInt(info->op_addr.c_str()) * pow(16, 4);
                    num_objcode |= operand_value;
                    num_objcode |= ((bits['n'] << 17) + (bits['i'] << 16) + (bits['x'] << 15) + (bits['b'] << 14) + (bits['p'] << 13) + (bits['e'] << 12));
                    sprintf(objcode, "%06X", num_objcode);
                }else if(f(info->format,3) && extended){
                    if (words > 2){
                        if (OPERAND.length() > 1 && f2(OPERAND[OPERAND.length() - 1],'X') && f2(OPERAND[OPERAND.length() - 2],',')){
                            bits['x'] = 1;
                            OPERAND[OPERAND.length() - 2] = '\0';
                        }

                        if (f2(OPERAND[0],'#')){
                            bits['n'] = 0;
                            OPERAND = OPERAND.substr(1);
                        }else{
                            bits['n'] = 1;
                        }

                        if (f2(OPERAND[0],'@')){
                            bits['i'] = 0;
                            OPERAND = OPERAND.substr(1);
                        }else{
                            bits['i'] = 1;
                        }

                        bits['e'] = 1;

                        modification_records.push({hExToInt(addr) + 1, 5, true, OPERAND});
                    }
                    int num_objcode = hExToInt(info->op_addr.c_str()) * pow(16, 6);
                    num_objcode |= ((bits['n'] << 17) + (bits['i'] << 16) + (bits['x'] << 15) + (bits['b'] << 14) + (bits['p'] << 13) + (bits['e'] << 12)) << 8;
                    sprintf(objcode, "%08X", num_objcode);
                }
            }
            else if (f1(OPCODE,"BYTE")){
                if (f2(OPERAND[0],'C')){
                    int c;
                    for (int i = 2; i < OPERAND.length() - 1; i++){
                        int c = OPERAND[i];
                        char temp[2];
                        sprintf(temp, "%0X", c);
                        strcat(objcode, temp);
                    }
                }else if (f2(OPERAND[0],'X')){
                    char* operand = &OPERAND[0];
                    strcat(objcode, operand+2);
                    objcode[strlen(objcode) - 1] = '\0';
                }
            }else if (f1(OPCODE,"WORD")){
                char* operand = &OPERAND[0]; 
                sprintf(objcode, "%06X", (int)strtol(operand, NULL, 10));
                int size = read_Line(operand, 0, args, "+");
                if (f(size,2)){
                    modification_records.push({hExToInt(addr), 6, true, (string)args[0]});
                    modification_records.push({hExToInt(addr), 6, true, (string)args[1]});
                }else{
                    char* operand = &OPERAND[0];
                    size = read_Line(operand, 0, args, "-");
                    if (f(size,2)){
                        modification_records.push({hExToInt(addr), 6, true, (string)args[0]});
                        modification_records.push({hExToInt(addr), 6, false, (string)args[1]});
                    }else{
                        modification_records.push({hExToInt(addr), 6, true, (string)args[0]});
                    }
                }
            }else if (f1(OPCODE,"EXTREF")){
                fprintf(objectFile, "R");
                char* operand = &OPERAND[0];
                int size = read_Line(operand, 0, args, ",");
                int i = 0; 
                while(i < size){
                    fprintf(objectFile, "%-6s", args[i]);
                    i++;
                }
                fprintf(objectFile, "\n");
            }else if (f1(OPCODE,"EXTDEF")){
                fprintf(objectFile, "D");
                char* operand = &OPERAND[0];
                int size = read_Line(operand, 0, args, ",");
                int i = 0;
                while(i < size){
                    fprintf(objectFile, "%-6s%06X", args[i], hExToInt(base->search_symtab(args[i]).c_str()));
                    i++;
                }
                fprintf(objectFile, "\n");
            }else if (f1(OPCODE,"LTORG")){
            }else if (f1(OPCODE,"EQU")){
                char* operand = &OPERAND[0];
                int size = read_Line(operand, 0, args, "+");
                if (f(size,2)){
                    string sym = base->search_symtab(args[0]);
                    int val = hExToInt(sym.c_str());
                    sym = base->search_symtab(args[1]);
                    val += hExToInt(sym.c_str());
                    sprintf(addr, "%04X", val);
                }else{
                    char* operand = &OPERAND[0];
                    size = read_Line(operand, 0, args, "-");
                    if (f(size,2)){
                        string sym = base->search_symtab(args[0]);
                        int val = hExToInt(sym.c_str());
                        sym = base->search_symtab(args[1]);
                        val -= hExToInt(sym.c_str());
                        sprintf(addr, "%04X", val);
                    }else if (f1(OPERAND,"*")==0){
                        string sym = base->search_symtab(args[0]);
                        int val = hExToInt(sym.c_str());
                        sprintf(addr, "%04X", val);
                    }
                }
            }

            if (f2(LABEL[0],'*')){
                char* label = &LABEL[0];
                read_Line(label, 0, args, "=");
                OPERAND = args[1];
                if (f2(OPERAND[0],'C')){
                    int c;
                    int i = 2;
                    while(i < OPERAND.length() - 1){
                        int c = OPERAND[i];
                        char temp[2];
                        sprintf(temp, "%0X", c);
                        strcat(objcode, temp);
                        i++;
                    }
                }
                else if (f2(OPERAND[0],'X')){
                    char* operand = &OPERAND[0];
                    strcat(objcode, operand + 2);
                    objcode[strlen(objcode) - 1] = '\0';
                }
            }

            if (strlen(record) + strlen(objcode) > 60 || f1(OPCODE,"RESW") || f1(OPCODE,"RESB")){
                if (strlen(record) > 0){
                    fprintf(objectFile, "T%06X%02X%s\n", hExToInt(firstaddr),(int)strlen(record) / 2, record);
                }
                string_Copy(record, "");
            }

            if (f(strlen(record),0)){
                string_Copy(firstaddr, addr);
            }
            strcat(record, objcode);

            line[strlen(line) - 1] = '\0';
            fprintf(listFile, "%s%-26s\t%s\n", addr, line, objcode);
        }else{
            fprintf(listFile, "\t%s", line);
        }

        if (fscanf(intermediateFile, "%[^\t]s", addr) == -1){
            break;
        }

        getline(&line, &len, intermediateFile);
        string_Copy(temp, line);
        words = read_Line(temp, 0, args," ");
        LABEL = args[0];
        OPCODE = args[1];

        while (f2(LABEL[0],' ') || f2(LABEL[0],'\t')){
            LABEL=LABEL.substr(1);
        }
    }

    if (strlen(record) > 1){
        fprintf(objectFile, "T%06X%02X%s\n", hExToInt(firstaddr),(int)strlen(record) / 2, record);
    }

    base = symtab_list[PROGNAME];

    while (!modification_records.empty()){
        modrec rec = modification_records.front();
        modification_records.pop();
        fprintf(objectFile, "M%06X%02X%s%s\n", rec.addr, rec.length, rec.sign ? "+" : "-", rec.symbol.c_str());
    }

    fprintf(objectFile, "E");

    if (first_sect){
        fprintf(objectFile, "%06X", 0);
        first_sect = false;
    }

    fprintf(objectFile, "\n\n\n");

    cout<<"PASS 2 DONE"<<endl;
}

