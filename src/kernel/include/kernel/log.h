#pragma once

class KernelLogImpl {
 public:
  virtual void Clear() = 0;
  virtual void Newline() = 0;
  virtual void ChangeForegroundColor(char c) = 0;
  virtual void ChangeBackgroundColor(char c) = 0;
  virtual void Print(char c) = 0;
};

class KernelLog {
 private:
  int statLoggedStatements = 0;
  KernelLogImpl* impl;

 public:
  void Init(KernelLogImpl* impl);
  void Log(const char* str);
  void Log(char c);
  void ChangeForegroundColor(char c);
  void ChangeBackgroundColor(char c);
};

extern KernelLog g_log;