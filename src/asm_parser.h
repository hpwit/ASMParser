#pragma once
#ifndef __ASM_PARSER
#define __ASM_PARSER
#include <stdio.h>
using namespace std;
#include <string>
#include <stdlib.h>
#include <memory>
#include <vector>
// #include <iostream>
// #include <functional>
#include <Arduino.h>
#include "esp_heap_caps.h"
#include "string_function.h"
#include "asm_parser_LMbin.h"
#include "asm_external.h"

class opRegister
{
public:
  static result_parse_operande parse(std::string s)
  {
    error_message_struct error;
    result_parse_operande res;
    char *endptr = NULL;
    error.error = 0;
    s = trim(s);
    if (s.at(0) == 'a')
    {

      string s2 = s.substr(1, s.size());
      if (s2.size() > 0)
      {
        int value = strtol(s2.c_str(), &endptr, 10);
        if (*endptr == 0)
        {
          if (value >= 0 and value <= 15)
          {
            res.value = value;
            res.error = error;
            return res;
          }
        }
      }
    }
    error.error_message = string_format("Unknown register %s\n", s.c_str());
    error.error = 1;
    res.error = error;
    return res;
  }
};

class opHex
{
public:
  static result_parse_operande parse(std::string s, uint32_t min, uint32_t max)
  {
    error_message_struct error;
    result_parse_operande res;
    char *endptr = NULL;
    error.error = 0;
    s = trim(s);

    if (s.size() > 0)
    {
      int value = strtol(s.c_str(), &endptr, 10);
      if (*endptr == 0)
      {
        if (value >= min and value <= max)
        {
          res.value = value;
          res.error = error;
          return res;
        }
      }
    }

    error.error_message = string_format("incorrect value %s should be between %d and %d\n", s.c_str(), min, max);
    error.error = 1;
    res.error = error;
    return res;
  }
};

class opNumber
{
public:
  static result_parse_operande parse(std::string s, int min, int max)
  {
    error_message_struct error;
    result_parse_operande res;
    char *endptr = NULL;
    error.error = 0;
    s = trim(s);

    if (s.size() > 0)
    {
      int value = strtol(s.c_str(), &endptr, 10);
      if (*endptr == 0)
      {
        if (value >= min and value <= max)
        {
          res.value = value;
          res.error = error;
          return res;
        }
      }
    }

    error.error_message = string_format("incorrect value %s should be between %d and %d\n", s.c_str(), min, max);
    error.error = 1;
    res.error = error;
    return res;
  }
};

class oplabel
{
public:
  static result_parse_operande parse(std::string s)
  {
    error_message_struct error;
    result_parse_operande res;
    char *endptr = NULL;
    error.error = 0;
    s = trim(s);

    if (s.size() > 0)
    {
      res.label = s;
      res.error = error;
      return res;
    }

    error.error_message = string_format("label %s is not valid\n", s.c_str());
    error.error = 1;
    res.error = error;
    return res;
  }
};

result_parse_operande operandeParse(string s, operandeType optype)
{

  if (optype == operandeType::registers)
  {
    return opRegister::parse(s);
  }
  if (optype == operandeType::floatregisters)
  {
  }

  if (optype == operandeType::boolregisters)
  {
  }

  if (optype == operandeType::l0_255)
  {
    return opNumber::parse(s, 0, 255);
  }

  if (optype == operandeType::lm32_95)
  {
    return opNumber::parse(s, -32, 95);
  }

  if (optype == operandeType::lm2048_2047)
  {
    return opNumber::parse(s, -2048, 2047);
  }

  if (optype == operandeType::l0_15)
  {
    return opNumber::parse(s, 0, 15);
  }

  if (optype == operandeType::l0_32760)
  {
    return opNumber::parse(s, 0, 32760);
  }

  if (optype == operandeType::l0_240)
  {
    return opNumber::parse(s, 0, 240);
  }

  if (optype == operandeType::l0_31)
  {
    return opNumber::parse(s, 0, 31);
  }

  if (optype == operandeType::lm128_127)
  {
    return opNumber::parse(s, -128, 127);
  }

  if (optype == operandeType::lm32768_32512)
  {
    return opNumber::parse(s, -32768, 35512);
  }

  if (optype == operandeType::l1_31)
  {
    return opNumber::parse(s, 1, 31);
  }

  if (optype == operandeType::l7_22)
  {
    return opNumber::parse(s, 7, 22);
  }

  if (optype == operandeType::l0_60)
  {
    return opNumber::parse(s, 0, 60);
  }

  if (optype == operandeType::lm64_m4)
  {
    return opNumber::parse(s, -64, -4);
  }

  if (optype == operandeType::l0_510)
  {
    return opNumber::parse(s, 0, 510);
  }

  if (optype == operandeType::lm8_7)
  {
    return opNumber::parse(s, -8, 7);
  }

  if (optype == operandeType::l0_1020)
  {
    return opNumber::parse(s, 0, 1020);
  }
  if (optype == operandeType::lm1_15)
  {
    return opNumber::parse(s, -1, 15);
  }
  if (optype == operandeType::l0_FFFFFFFF)
  {
    return opHex::parse(s, 0, 0xFFFFFFFF);
  }

  if (optype == operandeType::label)
  {
    return oplabel::parse(s);
  }
}

result_parse_line parseOperandes(string str, int nboperande, operandeType *optypes, int size, uint32_t (*createbin)(uint32_t *val))
{
  result_parse_line result;

  uint32_t values[4];
  result.error.error = 0;
  result.align = false;
  result.size = size;
  vector<string> operandes = split(str, ",");
  if (operandes.size() == nboperande)
  {
    for (int i = 0; i < nboperande; i++)
    {
      result_parse_operande res = operandeParse(operandes[i], optypes[i]);
      if (res.error.error)
      {
        result.error.error = res.error.error;
        result.error.error_message = result.error.error_message + res.error.error_message;
      }
      else
      {
        if (optypes[i] == operandeType::label)
        {
          result.name = res.label;
        }
        values[i] = res.value;
      }
    }
    if (result.error.error == 0)
    {
      if (createbin)
      {
        result.bincode = createbin(values);
        // result.size=size;
      }
    }
  }
  else
  {
    result.error.error = 1;
    result.error.error_message = string_format("Error:expected %d arguments got %d\n", nboperande, operandes.size());
  }
  result.op = opCodeType::standard;
  return result;
}

int findLabel(string s, vector<result_parse_line> *asm_parsed)
{
  int res = -1;
  for (int i = 0; i < (*asm_parsed).size(); i++)
  {
    if ((*asm_parsed)[i].op == opCodeType::label || (*asm_parsed)[i].op == opCodeType::data_label || (*asm_parsed)[i].op == opCodeType::number_label)
    {
      if (trim((*asm_parsed)[i].name).compare(trim(s)) == 0)
      {
        return i;
      }
    }
  }
  return res;
}

int findFunction(string s, vector<result_parse_line> *asm_parsed)
{
  int res = -1;
  for (int i = 0; i < (*asm_parsed).size(); i++)
  {
    if ((*asm_parsed)[i].op == opCodeType::function_declaration)
    {
      if ((*asm_parsed)[i].name.compare(s) == 0)
      {
        return i;
      }
    }
  }
  return res;
}

result_parse_line parseline(line sp, vector<result_parse_line> *asm_parsed)
{

  if (sp.opcde.find(":") != -1)
  {
    result_parse_line res;
    res.error.error = 0;
    res.op = opCodeType::label;
    res.size = 0;
    res.align = false;
    res.name = trim(sp.opcde.substr(0, sp.opcde.find(":")));
    if (findLabel(res.name, asm_parsed) != -1)
    {
      res.error.error = 1;
      res.error.error_message = string_format("label %s is already declared in line %d\n", res.name.c_str(), (*asm_parsed)[findLabel(res.name, asm_parsed)].line);
    }

    return res;
  }
  if (sp.opcde.compare("add") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_add, 3, bin_add);
  }
    if (sp.opcde.compare("sub") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_sub, 3, bin_sub);
  }
      if (sp.opcde.compare("quou") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_quou, 3, bin_quou);
  }
  if (sp.opcde.compare("add.n") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_add, 2, bin_add_n);
  }
  if (sp.opcde.compare("addi") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_addi, 3, bin_addi);
  }
  if (sp.opcde.compare("addi.n") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_addi_n, 2, bin_addi_n);
  }
  if (sp.opcde.compare("and") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_and, 3, bin_and);
  }
  if (sp.opcde.compare("bnez") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 2, op_bnez, 3, bin_bnez);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_bnez;

    return ps;
  }
  if (sp.opcde.compare("j") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 1, op_j, 3, bin_j);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_j;

    return ps;
  }
    if (sp.opcde.compare("blt") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 3, op_blt, 3, bin_blt);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_blt;

    return ps;
  }
     if (sp.opcde.compare("bge") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 3, op_bge, 3, bin_bge);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_bge;

    return ps;
  }
     if (sp.opcde.compare("beq") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 3, op_beq, 3, bin_beq);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_beq;

    return ps;
  }
     if (sp.opcde.compare("bne") == 0)
  {

    result_parse_line ps = parseOperandes(sp.operandes, 3, op_bne, 3, bin_bne);
    ps.op = opCodeType::jump;
    ps.calculateOfssetJump = jump_bne;

    return ps;
  }

  if (sp.opcde.compare("l32i") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_l32i, 3, bin_l32i);
  }
  if (sp.opcde.compare("l32i.n") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_l32i_n, 2, bin_l32i_n);
  }
  if (sp.opcde.compare("l32r") == 0)
  {
    result_parse_line ps = parseOperandes(sp.operandes, 2, op_l32r, 3, bin_l32r);
    ps.op = opCodeType::jump_32aligned;
    ps.calculateOfssetJump = jump_l32r;
    return ps;
  }

    if (sp.opcde.compare("call8") == 0)
  {
    result_parse_line ps = parseOperandes(sp.operandes, 1, op_call8, 3, bin_call8);
    ps.op = opCodeType::jump_32aligned;
    ps.calculateOfssetJump = jump_call8;
    return ps;
  }


  if (sp.opcde.compare("mov") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_mov, 3, bin_mov);
  }
    if (sp.opcde.compare("mull") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_mov, 3, bin_mull);
  }
  if (sp.opcde.compare("mov.n") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_mov_n, 2, bin_mov_n);
  }

  if (sp.opcde.compare("movi") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_movi, 3, bin_movi);
  }
  if (sp.opcde.compare("movi.n") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_movi_n, 2, bin_movi_n);
  }
  if (sp.opcde.compare("s32i") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_s32i, 3, bin_s32i);
  }
  if (sp.opcde.compare("s32i.n") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_s32i_n, 2, bin_s32i_n);
  }
  if (sp.opcde.compare("s8i") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_s8i, 3, bin_s8i);
  }
  if (sp.opcde.compare("l8ui") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_l8ui, 3, bin_l8ui);
  }
    if (sp.opcde.compare("s16i") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_s16i, 3, bin_s16i);
  }
  if (sp.opcde.compare("l16si") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_l16si, 3, bin_l16si);
  }
  if (sp.opcde.compare("slli") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_slli, 3, bin_slli);
  }
    if (sp.opcde.compare("remu") == 0)
  {
    return parseOperandes(sp.operandes, 3, op_remu, 3, bin_remu);
  }
  if (sp.opcde.compare("entry") == 0)
  {
    return parseOperandes(sp.operandes, 2, op_entry, 3, bin_entry);
  }
  if (sp.opcde.compare("nop.n") == 0)
  {
    return parseOperandes(sp.operandes, 0, NULL, 2, bin_nop_n);
  }
  if (sp.opcde.compare("nop") == 0)
  {
    return parseOperandes(sp.operandes, 0, NULL, 3, bin_nop);
  }

  if (sp.opcde.compare("callx8") == 0)
  {
    return parseOperandes(sp.operandes, 1, op_callx8, 3, bin_callx8);
  }

  if (sp.opcde.compare("retw.n") == 0)
  {
    return parseOperandes(sp.operandes, 0, NULL, 2, bin_retw_n);
  }
if (sp.opcde.compare(".bytes") == 0)
{
  char *endptr = NULL;
int value = strtol(sp.operandes.c_str(), &endptr, 10);
if (*endptr == 0)
{
  result_parse_line ps;
    ps.op = opCodeType::number;
    result_parse_line *ps1 = &(*asm_parsed)[(*asm_parsed).size() - 1];
      if (ps1->op == opCodeType::label)
      {
        ps1->size = value;
        ps1->op = opCodeType::number_label;
        ps1->align = true;
        ps.error.error = 0;
        ps.size = value;
        ps.name = sp.operandes;
        
        
        return ps;
      }
}
else
{
  result_parse_line ps;
  ps.error.error=1;
  ps.error.error_message="Not Valid size for bytes";
  return ps;

}

}
  if (sp.opcde.compare(".word") == 0)
  {
    result_parse_line ps;
    ps.op = opCodeType::number;
    result_parse_line *ps1 = &(*asm_parsed)[(*asm_parsed).size() - 1];
      if (ps1->op == opCodeType::label)
      {
        ps1->size = 4;
        ps1->op = opCodeType::number_label;
        ps1->align = true;
        ps.error.error = 0;
        ps.size = 4;
        ps.name = sp.operandes;
        printf(".woprd %s\n\r",ps.name.c_str());
        //ps.name = ps.name + '\n' + '\0';
      }
      return ps;
    //return parseOperandes(sp.operandes, 1, op_word, 4, bin_word);
  }

  if (sp.opcde.compare(".align") == 0)
  {
    result_parse_line ps;
    ps.error.error = 0;
    ps.align=false;
    ps.op=opCodeType::not_known;
    ps.size = 0;
    return ps;
  }

  if (sp.opcde.compare(".global") == 0)
  {
    result_parse_line ps;
    ps = parseOperandes(sp.operandes, 1, op_global, 0, NULL);
  //printf("operandes %s\r\n",ps.name.c_str());
    ps.op = opCodeType::function_declaration;
    return ps;
  }

  if (sp.opcde.compare("callExt") == 0)
  {
    result_parse_line ps;
    ps = parseOperandes(sp.operandes, 2, op_movExt, 0, bin_movExt);
    if (ps.error.error != 0)
    {
      return ps;
    }
    int i = findLink(ps.name, externalType::function);
    if (i == -1)
    {
      ps.error.error = 1;
      ps.error.error_message = string_format("External variable %s not found\n", ps.name.c_str());
    }
    else
    {
      // string debugsav=ps.debugtxt;
      int savbin = ps.bincode;
      ps = parseOperandes(string_format("a%d,a2,%d", ps.bincode, i * 4), 3, op_l32i, 3, bin_l32i);
      ps.debugtxt = "call ext function";
      ps.op = opCodeType::external_call;
      (*asm_parsed).push_back(ps);
      ps = parseOperandes(string_format("a%d", savbin), 1, op_callx8, 3, bin_callx8);
      // ps.debugtxt=debugsav;
      ps.op = opCodeType::external_call;
    }
    // ps.op=opCodeType::external_call;
    return ps;
  }

  if (sp.opcde.compare("movExt") == 0)
  {
    result_parse_line ps;
    ps = parseOperandes(sp.operandes, 2, op_movExt, 0, bin_movExt);
    // ps.op=opCodeType::external_call;
    int i = findLink(ps.name, externalType::value);
    if (i == -1)
    {
      ps.error.error = 1;
      ps.error.error_message = string_format("External variable %s not found\n", ps.name.c_str());
    }
    else
    {
      // string debugsav=ps.debugtxt;
      ps = parseOperandes(string_format("a%d,a2,%d", ps.bincode, i * 4), 3, op_l32i, 3, bin_l32i);
      // ps.debugtxt=debugsav;
      ps.op = opCodeType::external_var;
    }
    return ps;
  }

  if (sp.opcde.compare(".text") == 0)
  {
    result_parse_line ps;
    sp.operandes = trim(sp.operandes);

    if (sp.operandes.size() > 0)
    {

      ps.op = opCodeType::data;
      result_parse_line *ps1 = &(*asm_parsed)[(*asm_parsed).size() - 1];
      if (ps1->op == opCodeType::label)
      {
        ps1->size = 4;
        ps1->op = opCodeType::data_label;
        ps1->align = true;
        ps.error.error = 0;
        ps.size = sp.operandes.size() + 2;
        ps.name = sp.operandes;
        ps.name = ps.name + '\n' + '\0';
      }
    }
    else
    {
      ps.error.error = 1;
      ps.error.error_message = "no valuable string";
    }
    return ps;
  }
  if (sp.opcde.compare(".type") == 0)
  {
    result_parse_line ps;
    ps.op=opCodeType::not_known;
    ps.error.error = 0;
    ps.align=false;
    ps.size = 0;
    return ps;
  }

  result_parse_line res;
  error_message_struct err;
  err.error = 1;
  err.error_message = string_format("Opcode %s not found", sp.opcde.c_str());
  res.error = err;
  return res;
}

line splitOpcodeOperande(string s)
{
  line res;
  s = trim(s);

  if (s.size() < 2)
  {
    res.error = 1;
    return res;
  }
  res.error = 0;
  // string s=trim(s2);
  string opcode = trim(s.substr(0, s.find_first_of(" ")));
  string operandes;
  if (s.find_first_of(" ") < s.size())
  {
    operandes = s.substr(s.find_first_of(" "), s.size());
  }
  else
  {
    operandes = "";
  }
  res.opcde = opcode;
  res.operandes = operandes;
  return res;
}

error_message_struct parseASM(vector<string> *_lines, vector<result_parse_line> *asm_parsed)
{
  vector<string> lines = *_lines;
  error_message_struct main_error;
  main_error.error = 0;
  main_error.error_message = "";
  printf("Parsing %d lines ... ", lines.size());
  for (int i = 0; i < lines.size(); i++)
  {
    line res = splitOpcodeOperande(lines[i]);
    if (!res.error)
    {
      result_parse_line re_sparse = parseline(res, asm_parsed);
      re_sparse.debugtxt = lines[i];
      re_sparse.line = i + 1;
      (*asm_parsed).push_back(re_sparse);
      if (re_sparse.error.error)
      {
        main_error.error = 1;
        main_error.error_message += string_format("line:%d %s\n", i, re_sparse.error.error_message.c_str());
      }
    }
  }
  printf("Done.\r\n");
  return main_error;
}

error_message_struct parseASM(string s, vector<result_parse_line> *asm_parsed)
{
  vector<string> lines = split(s, "\n");
  return parseASM(&lines, asm_parsed);
}

void createAddress(vector<result_parse_line> *asm_parsed)
{
  uint32_t add_instr = 0;
  uint32_t add_data = 0;
  vector<result_parse_line>::iterator it = (*asm_parsed).begin();
  for (it; it < (*asm_parsed).end(); it++)
  {
   
    if(it->op != opCodeType::data && it->op != opCodeType::number)
    {
      // printf("%s %s\n\r",it->debugtxt.c_str(), it->name.c_str());
      // result_parse_line  *re_sparse=&asmParsed[i];
      it->address = add_instr;
      if (it->align == true)
      {
        // printf("%s %s %d\n\r",it->debugtxt.c_str(), it->name.c_str(),add_instr & 3);
        if ((add_instr & 3) != 0)
        {
          result_parse_line add_op;
         
          if ((add_instr & 3) == 2)
          {
            add_op.size = 2;
            add_op.bincode = bin_nop_n(0);
          }
          if ((add_instr & 3) == 1)
          {
            add_op.size = 3;
            add_op.bincode = bin_nop(0);
          }
          if ((add_instr & 3) == 3)
          {
            add_op.size = 5;
            add_op.bincode = bin_nop(0);
          }
          add_op.address = add_instr;
          add_op.debugtxt = string_format("nop filler %d",add_op.size);

          add_op.error.error = 0;
          it->address = add_instr + add_op.size;
          //printf("function not aligned\t %s \t %s\n", it->debugtxt.c_str(), it->name.c_str());
          (*asm_parsed).insert(it, add_op);
          it++;
          add_instr=add_instr + add_op.size;
         // add_instr += 4 - (add_instr & 3);
        }
      }
      add_instr += it->size;
    }
    else  
    {
        //we suppose that above it's the label
      it->address = add_data;
      vector<result_parse_line>::iterator prev_it = it - 1;
      prev_it->bincode = add_data;
      add_data += it->size;
    }
  }
}
void dumpmem(uint32_t *dump)
{
  printf("\n%s", "                   ");
  for (int i = 0; i < 8; i++)
  {
    printf("-----");
  }
  printf("\n%s", "                   ");
  for (int i = 0; i < 8; i++)
  {
    printf(" %02x |", i);
  }
  printf("\n");
  printf("%s", "                   ");
  for (int i = 0; i < 8; i++)
  {
    printf("-----");
  }
  for (int j = 0; j < 30; j++)
  {
    printf("\nline:%2d %8x: ", j, dump);
    for (int i = 0; i < 2; i++)
    {
      printf(" %02x | %02x | %02x | %02x |", ((*dump)) & 0xFF, ((*dump) >> 8) & 0xFF, ((*dump) >> 16) & 0xFF, ((*dump) >> 24) & 0xFF);
      dump++;
    }
  }
  printf("\n\n\n");
}

void printparsdAsm(uint32_t start_address, vector<result_parse_line> *asm_parsed)
{
  for (int i = 0; i < (*asm_parsed).size(); i++)
  {
    result_parse_line re_sparse = (*asm_parsed)[i];
    if (re_sparse.error.error)
    {
      printf("%8x \t %s\t %s\n", re_sparse.address + start_address, re_sparse.debugtxt.c_str(), re_sparse.error.error_message.c_str());
    }
    else
    {
      if (re_sparse.op == opCodeType::label)
      {
        printf("%8x \t <%s>:\n", re_sparse.address + start_address, re_sparse.name.c_str());
      }
      else
      {
        printf("%8x \t %6x\t %s\n", re_sparse.address + start_address, re_sparse.bincode, re_sparse.debugtxt.c_str());
      }
    }
  }
}

void flagLabel32aligned(vector<result_parse_line> *asm_parsed)
{
  printf("Flag label(s) to align ... ");
  uint32_t add = 0;
  vector<result_parse_line>::iterator it = (*asm_parsed).begin();
  for (it; it < (*asm_parsed).end(); it++)
  {
    if (it->op == opCodeType::jump_32aligned || it->op == opCodeType::function_declaration)
    {
      int i = findLabel(it->name, asm_parsed);
      if (i != -1)
      {
        result_parse_line *parse_line = &(*asm_parsed)[i];
        parse_line->align = true;
      }
    }
  }
  printf("Done.\r\b");
}

error_message_struct calculateJump(vector<result_parse_line> *asm_parsed)
{
  printf("Calculating jumps ... ");
  error_message_struct error;
  error.error = 0;
  error.error_message = "";
  for (int i = 0; i < (*asm_parsed).size(); i++)
  {
    result_parse_line *parse_line = &(*asm_parsed)[i];
    if ((parse_line->op == opCodeType::jump) || (parse_line->op == opCodeType::jump_32aligned))
    {
      int index = findLabel(parse_line->name, asm_parsed);
      if (index != -1)
      {
        if (parse_line->calculateOfssetJump)
        {
          parse_line->bincode = parse_line->calculateOfssetJump(parse_line->bincode, parse_line->address, (*asm_parsed)[index].address);
        }
        else
        {
          error.error = 1;
          error.error_message += string_format("Error:No method to calcuylate jump offset for %s\n", parse_line->debugtxt.c_str());
        }
      }
      else
      {
        error.error = 1;
        error.error_message += string_format("line %d: label %s not found\n", parse_line->line, parse_line->name.c_str());
      }
    }
  }
  printf("Done.\r\n");
  return error;
}

void createAbsoluteJump(uint8_t *exec, vector<result_parse_line> *asm_parsed, uint32_t address)
{
  for (int i = 0; i < (*asm_parsed).size(); i++)
  {
    if ((*asm_parsed)[i].op == opCodeType::data_label || (*asm_parsed)[i].op == opCodeType::number_label)
    {
      uint32_t content = (*asm_parsed)[i + 1].address + address;
      (*asm_parsed)[i].bincode = content;
      uint32_t *new_adr = (uint32_t *)exec + (*asm_parsed)[i].address / 4;
      // printf("new content %x atr adress %x\n",content,(uint32_t)new_adr);
      memcpy(new_adr, &content, 4);
    }
    
  }
}

executable createBinary(vector<result_parse_line> *asm_parsed)
{

  executable exe;
  exe.error.error = 0;

  for (int i = 0; i < asm_parsed->size(); i++)
  {
    if ((*asm_parsed)[i].op == opCodeType::function_declaration)
    {
      int index = findLabel((*asm_parsed)[i].name, asm_parsed);
      if (index == -1)
      {
        exe.error.error = 1;
        exe.error.error_message = string_format("Impossible to find start function :%s", (*asm_parsed)[i].name.c_str());
        return exe;
      }
      else
      {
        globalcall gc;
        gc.name = (*asm_parsed)[i].name;
        gc.address = (*asm_parsed)[index].address;
        exe.functions.push_back(gc);
      }
    }
  }

  if (exe.functions.size() == 0)
  {
    exe.error.error = 1;
    exe.error.error_message = "No global start function found";
    return exe;
  }
  int data_size = -1;
  int last_one = -1;
  for (int i = 0; i < asm_parsed->size(); i++)
  {
    if ((*asm_parsed)[i].op == opCodeType::data || (*asm_parsed)[i].op == opCodeType::number)
    {
      last_one = i;
    }
  }
  if (last_one >= 0)
  {
    data_size = (*asm_parsed)[last_one].size + (*asm_parsed)[last_one].address;
  }
  uint32_t intr_size = (*asm_parsed)[(*asm_parsed).size() - 1].address + (*asm_parsed)[(*asm_parsed).size() - 1].size;
  printf("Creation of an %d bytes binary and %d bytes data\n", intr_size, data_size);

  uint8_t *val_tmp = (uint8_t *)malloc((intr_size / 4) * 4 + 4);
  uint8_t *data;
  if (data_size > 0)
  {
    data = (uint8_t *)malloc((data_size / 4) * 4 + 4);
  }

  uint32_t *exec = (uint32_t *)heap_caps_aligned_alloc(1, (intr_size / 4) * 4 + 4, MALLOC_CAP_32BIT);

  for (int i = 0; i < (*asm_parsed).size(); i++)
  {
    if ((*asm_parsed)[i].op == opCodeType::data || (*asm_parsed)[i].op == opCodeType::number)
    {
      memcpy(data + (*asm_parsed)[i].address, (*asm_parsed)[i].name.c_str(), (*asm_parsed)[i].size);
    }
    else
    {
      memcpy(val_tmp + (*asm_parsed)[i].address, &(*asm_parsed)[i].bincode, (*asm_parsed)[i].size);
    }
  }
  createAbsoluteJump(val_tmp, asm_parsed, (uint32_t)data);
  memcpy(exec, val_tmp, (intr_size / 4) * 4 + 4);
  free(val_tmp);

  // exe.start_function = (uint32_t)(exec + (*asm_parsed)[index].address / 4);
  Serial.printf("%d start function(s) found:\r\n", exe.functions.size());
  for (int i = 0; i < exe.functions.size(); i++)
  {
    exe.functions[i].address = (uint32_t)(exec + exe.functions[i].address / 4);
    Serial.printf("%2d: %s\t%x\r\n", i, exe.functions[i].name.c_str(), exe.functions[i].address);
  }
  exe.start_program = exec;
  exe.data = data;

  return exe;
}

executable createExectutable(vector<string> *lines, bool display)
{
  vector<result_parse_line> asm_parsed;
  executable exec;
  error_message_struct err = parseASM(lines, &asm_parsed);
  if (err.error == 0)
  {
    flagLabel32aligned(&asm_parsed);
    createAddress(&asm_parsed);
    err = calculateJump(&asm_parsed);

    if (err.error == 0)
    {

      exec = createBinary(&asm_parsed);

      if (exec.error.error == 0)
      {
        // createAbsoluteJump(exec.start_program,&asm_parsed);
        if (display == true)
        {
          printparsdAsm((uint32_t)exec.start_program, &asm_parsed);
          // dumpmem(exec.start_program);
        }

        replaceExternal("start_program", (void *)exec.start_program);
        uint32_t dd = (uint32_t)createExternalLinks();
        exec.links = dd;
        exec.error.error = 0;
      }
      return exec;
    }
    else
    {
      exec.error = err;
      return exec;
    }
  }
  else
  {

    exec.error = err;
    exec.error.error = 1;
    return exec;
  }
}

executable createExectutable(string script, bool display)
{
  vector<result_parse_line> asm_parsed;
  executable exec;
  error_message_struct err = parseASM(script, &asm_parsed);
  if (err.error == 0)
  {
    flagLabel32aligned(&asm_parsed);
    createAddress(&asm_parsed);
    err = calculateJump(&asm_parsed);

    if (err.error == 0)
    {

      exec = createBinary(&asm_parsed);

      if (exec.error.error == 0)
      {
        // createAbsoluteJump(exec.start_program,&asm_parsed);
        if (display == true)
        {
          printparsdAsm((uint32_t)exec.start_program, &asm_parsed);
          // dumpmem(exec.start_program);
        }

        replaceExternal("start_program", (void *)exec.start_program);
        uint32_t dd = (uint32_t)createExternalLinks();
        exec.links = dd;
        exec.error.error = 0;
      }
      return exec;
    }
    else
    {
      exec.error = err;
      return exec;
    }
  }
  else
  {

    exec.error = err;
    exec.error.error = 1;
    return exec;
  }
}

executable createExectutable(string script)
{
  return createExectutable(script, false);
}
void executeBinaryAsm(uint32_t *j, uint32_t *c)
{
  printf("Executing asm code ...\n");
  asm volatile("l32i a10,%1,0\n\t"
               "l32i a15,%0,0\n\t"
               "callx8 a15"
               : : "r"(j), "r"(c)
               :);
  printf("Execution done\n");
  // free(exec);
}

error_message_struct executeBinary(string function,executable ex)
{
  error_message_struct res;
  res.error=0;
   for (int i = 0; i < ex.functions.size(); i++)
  {
    if(ex.functions[i].name.compare(function)==0)
    {
      printf("address of function %s :%x\n",ex.functions[i].name.c_str(), ex.functions[i].address);
      executeBinaryAsm(&ex.functions[i].address, &ex.links);
      //freeBinary(ex);
      return res;
    }
  }
  res.error=1;
  res.error_message=string_format("Impossible to execute %s: not found\n",function);
  return res;
}

error_message_struct executeBinary(executable ex)
{

return executeBinary(ex.functions[0].name,ex);

}
void freeBinary(executable ex)
{
  if (ex.start_program != NULL)
    heap_caps_aligned_free(ex.start_program);

  ex.start_program=NULL;

  if (ex.data != NULL)
    free(ex.data);
  ex.data=NULL;
}
#endif