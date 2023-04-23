#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include "cmdExec.h"

using namespace std;

string cmdOutput(const char *cmd)
{
    char buffer[MAX_BUFF_SIZE];
    string result = "";
    
    // opening the cmd dir with read permissions
    FILE *pipe = popen(cmd, "r");
    // return string(cmd);
    try
    {
        if (pipe == NULL){
            result = "Command not found\n";
        }
        else
        {
            while (!feof(pipe))
            {
                if (fgets(buffer, MAX_BUFF_SIZE, pipe) != NULL)
                {
                    result += buffer;
                }
            }
        }
        
    }
    catch (...)
    {
        pclose(pipe);
        return result;
    }
    pclose(pipe);
    return result;
}
