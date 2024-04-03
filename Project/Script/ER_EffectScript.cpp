#include "pch.h"
#include "ER_EffectScript.h"

#include <chrono>
#include <thread>

ER_EffectScript::ER_EffectScript()
	: CScript((UINT)SCRIPT_TYPE::ER_EFFECTSCRIPT)
	, m_fTime(0.f)
{
}

ER_EffectScript::~ER_EffectScript()
{
}

void ER_EffectScript::EffectRotation(CGameObject* _targetObj, Vec3 _startScale, Vec3 _endScale, bool _rotDirection, float _endTime, CGameObject* _dummpyParent)
{
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	float exeCount = _endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	float scaleIncreasing = (_endScale.x - _startScale.x) / exeCount;

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= _endTime * 1000.f)
			break;

		//ũ�� ����
		if (_endScale.x >= _startScale.x) {
			_startScale.x += scaleIncreasing;
			_startScale.y += scaleIncreasing;
			_startScale.z += scaleIncreasing;
		}
		_targetObj->Transform()->SetRelativeScale(_startScale);

		//ȸ�� ����
		Vec3 rot = _targetObj->Transform()->GetRelativeRot();
		if(_rotDirection)
			rot.z += exeCount / 100.f;
		else
			rot.z -= exeCount / 100.f;
		_targetObj->Transform()->SetRelativeRot(rot);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	if (_dummpyParent != nullptr)
		DestroyObject(_dummpyParent);
	else {
		DestroyObject(_targetObj);
	}
}

void ER_EffectScript::SpawnAnimationEffect(CGameObject* _targetObj, float _endTime, CGameObject* _dummpyParent)
{
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	float exeCount = _endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= _endTime * 1000.f)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	if (_dummpyParent != nullptr)
		DestroyObject(_dummpyParent);
	else {
		DestroyObject(_targetObj);
	}
}

void ER_EffectScript::begin()
{
}

void ER_EffectScript::tick()
{
}

void ER_EffectScript::SaveToLevelFile(FILE* _File)
{
}

void ER_EffectScript::LoadFromLevelFile(FILE* _FILE)
{
}