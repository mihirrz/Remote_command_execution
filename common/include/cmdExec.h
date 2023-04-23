#ifndef CMDEXEC_H
#define CMDEXEC_H

const int PORT=5001;
const int MAX_BUFF_SIZE=1024;

#include <string>

std::string cmdOutput(const char* param);

#endif