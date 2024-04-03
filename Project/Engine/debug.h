#pragma once
#include "CSingleton.h"

enum DebugLogFlags_
{
    // Event types

    DebugLogFlags_None = 0,
    DebugLogFlags_Input = 1 << 0,         // �Է� �α� : ���콺, Ű���� �Է� ��
    DebugLogFlags_Collision = 1 << 1,     // �浹 �α� : ��ü �浹 ���� ( �浹 ����, �浹 ���� )
    DebugLogFlags_Command = 1 << 2,       // ��� �α� : ���� ���� ( �̵�, ������ ����, ��ų ��� �� )
    DebugLogFlags_Process = 1 << 3,       // ���� �α� : �Լ� ���� ���� ( �ִϸ��̼� ���, ������ ���/���� �� )
    DebugLogFlags_Info = 1 << 4,          // ���� �α� : ���� ���� ��ȭ/��� ���� ( ������ �߻�, ���� ����/���� �� )
    DebugLogFlags_All = DebugLogFlags_Input | DebugLogFlags_Collision | DebugLogFlags_Command | DebugLogFlags_Process | DebugLogFlags_Info
};

class DebugContext
    : public CSingleton<DebugContext>
{
    SINGLE(DebugContext);
private:
    DebugLogFlags_  DebugLogFlags;
    vector<string>  DebugLogBuf;

public:
    const vector<string>& GetDebugLogs() { return DebugLogBuf; }

    void AddLog(const char* _log);
    void ClearLog() { if (!DebugLogBuf.empty()) DebugLogBuf.clear(); }

    DebugLogFlags_* GetDebugFlags() { return &DebugLogFlags; }
    void SetDebugFlags(DebugLogFlags_ _flags) { DebugLogFlags = _flags; }
};

void DebugLog(const char* type, const char* obj, const char* func, const char* ex);

#define DEBUG_LOG(type,obj,func,ex)      DebugLog(type,obj,func,ex)
#define DEBUG_LOG_INPUT(obj,func,ex)     do { if (*(DebugContext::GetInst()->GetDebugFlags()) & DebugLogFlags_Input) DEBUG_LOG("Input",obj,func,ex); } while (0)
#define DEBUG_LOG_COLLISION(obj,func,ex) do { if (*(DebugContext::GetInst()->GetDebugFlags()) & DebugLogFlags_Collision)    DEBUG_LOG("Collision",obj,func,ex); } while (0)
#define DEBUG_LOG_COMMAND(obj,func,ex)   do { if (*(DebugContext::GetInst()->GetDebugFlags()) & DebugLogFlags_Command)    DEBUG_LOG("Command",obj,func,ex); } while (0)
#define DEBUG_LOG_PROCESS(obj,func,ex)   do { if (*(DebugContext::GetInst()->GetDebugFlags()) & DebugLogFlags_Process)      DEBUG_LOG("Process",obj,func,ex); } while (0)
#define DEBUG_LOG_INFO(obj,func,ex)      do { if (*(DebugContext::GetInst()->GetDebugFlags()) & DebugLogFlags_Info)  DEBUG_LOG("Info",obj,func,ex); } while (0)