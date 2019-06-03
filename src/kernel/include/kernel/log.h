#pragma once

class KernelLogImpl {
 public:
  void Clear();
  void Newline();
  void ChangeForegroundColor(char c);
  void ChangeBackgroundColor(char c);
  void Print(char c);
};

class KernelLog {
 private:
  int statLoggedStatements = 0;
  KernelLogImpl* impl;

 public:
  void Init(KernelLogImpl* impl);
  void Log(char* str);
  void ChangeColor(char*);
};