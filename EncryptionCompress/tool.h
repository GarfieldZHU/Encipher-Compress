#ifndef _TOOL_H_
#define _TOOL_H_
#define _CRT_SECURE_NO_WARNINGS
#include "tool.h"
#include <stdio.h>
#include "EcpCmr.h"

typedef enum{ true, false } bool;
void input_tips();
FILE *input_file();
void output_to_file(char *res, int len, char option);
#endif