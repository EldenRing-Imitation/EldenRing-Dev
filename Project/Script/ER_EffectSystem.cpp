#include "pch.h"
#include "ER_EffectSystem.h"

#include <thread>
#include <chrono>

#include "ER_ActionScript_Rio.h"
#include "ER_ActionScript_Aya.h"

ER_EffectSystem::ER_EffectSystem()
{
}

ER_EffectSystem::~ER_EffectSystem()
{
}

void ER_EffectSystem::init()
{
}

void ER_EffectSystem::progress()
{
	for (int i = 0; i < m_vParticleLifeTime.size(); ++i) {
		m_vParticleLifeTime[i] -= DT;
		if (m_vParticleLifeTime[i] < 0.f) {
			DestroyObject(m_vDeleteParticles[i]);
			m_vDeleteParticles[i] = nullptr;
			m_vDeleteParticles.erase(m_vDeleteParticles.begin() + i);
			m_vParticleLifeTime.erase(m_vParticleLifeTime.begin() + i);
		}
	}
}

void ER_EffectSystem::SpawnHitEffect(CGameObject* _attacker, CGameObject* _hitter, CGameObject* _projectile, Vec3 _pos, Vec3 _dir)
{
	Vec3 hitEffectPos;	//��Ʈ�� ����Ʈ ���� ��ġ
	Vec3 effectMoveDir;	//����Ʈ ��ġ ����
	Vec3 hitEffectDir;	//����Ʈ�� �ٶ󺸴� ����
	float yRot;			//����Ʈ�� ���� ����(y����)

	//����ü�� ���� ��
	if (_projectile != nullptr) {
		hitEffectPos = _hitter->Transform()->GetWorldPos();
		hitEffectPos.y += 1.5f;

		effectMoveDir = _projectile->Transform()->GetRelativePos() - _hitter->Transform()->GetRelativePos();
		effectMoveDir.y = 0.f;
		effectMoveDir.Normalize();

		hitEffectPos = hitEffectPos + (effectMoveDir * 0.5f);

		hitEffectDir = _projectile->Transform()->GetRelativeRot();
		yRot = Rad2Deg(hitEffectDir.y);

		yRot += 180.f;
		if (yRot > 270.f) {
			yRot = yRot - 270.f;
			yRot = yRot - 90.f;
		}

		hitEffectDir.y = Deg2Rad(yRot);
	}
	//����ü�� ���� ��
	else {

	}
	

	if (_attacker->GetScript<ER_ActionScript_Rio>() != nullptr) {
		thread t(&ER_EffectSystem::SpawnRioHitEffect, this, hitEffectPos, hitEffectDir, effectMoveDir);
		t.detach();
	}
	else if (_attacker->GetScript<ER_ActionScript_Aya>() != nullptr) {
		Vec3 hitPosByAya = _hitter->Transform()->GetWorldPos();
		hitPosByAya.y += 1.5f;
		thread t(&ER_EffectSystem::SpawnAyaHitEffect, this, hitPosByAya);
		t.detach();
	}
}

void ER_EffectSystem::SpawnSkillHitEffect(CGameObject* _attacker, CGameObject* _hitter, const tSkill_Info* _skillInfo, CGameObject* _projectile,  Vec3 _pos, Vec3 _dir)
{
	Vec3 hitEffectPos;	//��Ʈ�� ����Ʈ ���� ��ġ
	Vec3 effectMoveDir;	//����Ʈ ��ġ ����
	Vec3 hitEffectDir;	//����Ʈ�� �ٶ󺸴� ����
	float yRot;			//����Ʈ�� ���� ����(y����)

	//����ü�� ���� ��
	if (_projectile != nullptr) {
		hitEffectPos = _hitter->Transform()->GetWorldPos();
		hitEffectPos.y += 1.5f;

		effectMoveDir = _projectile->Transform()->GetRelativePos() - _hitter->Transform()->GetRelativePos();
		effectMoveDir.y = 0.f;
		effectMoveDir.Normalize();

		hitEffectPos = hitEffectPos + (effectMoveDir * 0.5f);

		hitEffectDir = _projectile->Transform()->GetRelativeRot();
		yRot = Rad2Deg(hitEffectDir.y);

		yRot += 180.f;
		if (yRot > 270.f) {
			yRot = yRot - 270.f;
			yRot = yRot - 90.f;
		}

		hitEffectDir.y = Deg2Rad(yRot);
	}
	//����ü�� ���� ��
	else {

	}


	//���� ��ų
	if (_skillInfo->strName == L"Rio_W" || _skillInfo->strName == L"Rio_W2") {
		thread t(&ER_EffectSystem::SpawnRioHitEffect, this, hitEffectPos, hitEffectDir, effectMoveDir);
		t.detach();
		thread t1(&ER_EffectSystem::SpawnRioWHitEffect, this, hitEffectPos, hitEffectDir, effectMoveDir, _hitter);
		t1.detach();
	}
	else if (_skillInfo->strName == L"Rio_R") {
		thread t(&ER_EffectSystem::SpawnRioRHitEffect, this, hitEffectPos, hitEffectDir, effectMoveDir);
		t.detach();
	}
	//�ƾ� ��ų
	else if (_skillInfo->strName != L"Aya_R" && _skillInfo->strName.substr(0, 4) == L"Aya_") {
		Vec3 hitPosByAya = _hitter->Transform()->GetWorldPos();
		hitPosByAya.y += 1.5f;
		thread t(&ER_EffectSystem::SpawnAyaHitEffect, this, hitPosByAya);
		t.detach();
	}
}

void ER_EffectSystem::SpawnRioHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir)
{
	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(Vec3(0.f, _dir.y, 0.f));

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent02 = onew(CGameObject);
	AddComponents(dummyParent02, _TRANSFORM);

	dummyParent02->Transform()->SetRelativeRot(Vec3(0.f, _dir.y, 0.f));

	SpawnGameObject(dummyParent02, _pos, L"Effect");

	//��Ʈ ����Ʈ1(�ٶ�) ����
	CGameObject* windHitEffect = onew(CGameObject);
	AddComponents(windHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	windHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	windHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"wWind.png");
	windHitEffect->Animator2D()->CreateAnimation(L"wWind", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 15);
	windHitEffect->Animator2D()->Play(L"wWind", true);

	dummyParent01->AddChild(windHitEffect);
	
	//��Ʈ �̺�Ʈ2(Ÿ��) ����
	CGameObject* attackHitEffect = onew(CGameObject);
	AddComponents(attackHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	attackHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	attackHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"rioHit2_Orange.png");
	attackHitEffect->Animator2D()->CreateAnimation(L"rioHit2_Orange", animAtlas, Vec2(0.f, 0.f), Vec2(1024.f, 1024.f), Vec2(1024.f, 1024.f), 1, 15);
	attackHitEffect->Animator2D()->Play(L"rioHit2_Orange", true);

	dummyParent02->AddChild(attackHitEffect);

	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.25f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	//1.0f�� ���� ũ��, 3.0f�� ������ ũ��
	float startScale = 1.0f;
	float endScale = 3.0f;
	float scaleIncreasing = (endScale - startScale) / exeCount;

	//startMovePos�� ���� ũ��, 1.2f�� ���� �� ũ��
	float startMovePos = 0.f;
	float endMovePos = 0.5f;
	float moveIncreasing = (endMovePos - startMovePos) / exeCount;

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		//ũ�� ����
		if (endScale >= startScale) {
			startScale += scaleIncreasing;
		}
		windHitEffect->Transform()->SetRelativeScale(Vec3(startScale/1.f, startScale/1.f, startScale/1.f));
		attackHitEffect->Transform()->SetRelativeScale(Vec3(startScale/2.f, startScale / 2.f, startScale/2.f));

		//ȸ�� ����
		Vec3 rot = windHitEffect->Transform()->GetRelativeRot();
		rot.z += exeCount / 500.f;
		windHitEffect->Transform()->SetRelativeRot(rot);

		//�����̱�
		Vec3 pos = dummyParent01->Transform()->GetRelativePos();
		_effectMoveDir.y = 0.f;
		_effectMoveDir.Normalize();
		Vec3 resultPos = pos + (_effectMoveDir * moveIncreasing);
		dummyParent01->Transform()->SetRelativePos(resultPos);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
	DestroyObject(dummyParent02);
}

void ER_EffectSystem::SpawnAyaHitEffect(Vec3 _pos)
{
	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	//��Ʈ ����Ʈ1(��) ����
	CGameObject* gunHitEffect = onew(CGameObject);
	AddComponents(gunHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	gunHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	gunHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"Fx_ShootGlowSE_04.png");
	gunHitEffect->Animator2D()->CreateAnimation(L"Fx_ShootGlowSE_04", animAtlas, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 64.f), 4, 12);
	gunHitEffect->Animator2D()->Play(L"Fx_ShootGlowSE_04", true);

	dummyParent01->AddChild(gunHitEffect);

	//������ ���� ����
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.20f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
}

void ER_EffectSystem::SpawnRioWHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir, CGameObject* _hitter)
{
	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));

	SpawnGameObject(dummyParent01, L"Effect");

	//maskWind ����
	CGameObject* maskWind = onew(CGameObject);
	AddComponents(maskWind, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	maskWind->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	maskWind->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	maskWind->Transform()->SetRelativeScale(maskWind->Transform()->GetRelativeScale() * 4.f);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"wMaskWind.png");
	maskWind->Animator2D()->CreateAnimation(L"wMaskWind", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 15);
	maskWind->Animator2D()->Play(L"wMaskWind", true);

	dummyParent01->AddChild(maskWind);

	//������ �ð����� ����
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 1.f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		//��ġ ����
		Vec3 hitterPos = _hitter->Transform()->GetRelativePos();
		dummyParent01->Transform()->SetRelativePos(hitterPos);

		//ȸ�� ����
		Vec3 rot = maskWind->Transform()->GetRelativeRot();
		rot.z += exeCount / 2000.f;
		maskWind->Transform()->SetRelativeRot(rot);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
}

void ER_EffectSystem::SpawnRioRHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir)
{//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(Vec3(0.f, _dir.y, 0.f));

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent02 = onew(CGameObject);
	AddComponents(dummyParent02, _TRANSFORM);

	dummyParent02->Transform()->SetRelativeRot(Vec3(0.f, _dir.y, 0.f));

	SpawnGameObject(dummyParent02, _pos, L"Effect");

	//��Ʈ ����Ʈ1(�ٶ�) ����
	CGameObject* windHitEffect = onew(CGameObject);
	AddComponents(windHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	windHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	windHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"wWind.png");
	windHitEffect->Animator2D()->CreateAnimation(L"wWind", animAtlas, Vec2(0.f, 0.f), Vec2(100.f, 100.f), Vec2(100.f, 100.f), 1, 15);
	windHitEffect->Animator2D()->Play(L"wWind", true);

	dummyParent01->AddChild(windHitEffect);

	//��Ʈ �̺�Ʈ2(Ÿ��) ����
	CGameObject* attackHitEffect = onew(CGameObject);
	AddComponents(attackHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	attackHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	attackHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"rioHit2_Orange.png");
	attackHitEffect->Animator2D()->CreateAnimation(L"rioHit2_Orange", animAtlas, Vec2(0.f, 0.f), Vec2(1024.f, 1024.f), Vec2(1024.f, 1024.f), 1, 15);
	attackHitEffect->Animator2D()->Play(L"rioHit2_Orange", true);

	dummyParent02->AddChild(attackHitEffect);

	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.25f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	//1.0f�� ���� ũ��, 3.0f�� ������ ũ��
	float startScale = 1.0f;
	float endScale = 5.0f;
	float scaleIncreasing = (endScale - startScale) / exeCount;

	//startMovePos�� ���� ��ġ, endMovePos�� ������ ��ġ
	float startMovePos = 0.f;
	float endMovePos = 1.0f;
	float moveIncreasing = (endMovePos - startMovePos) / exeCount;

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		//ũ�� ����
		if (endScale >= startScale) {
			startScale += scaleIncreasing;
		}
		windHitEffect->Transform()->SetRelativeScale(Vec3(startScale / 1.f, startScale / 1.f, startScale / 1.f));
		attackHitEffect->Transform()->SetRelativeScale(Vec3(startScale / 2.f, startScale / 2.f, startScale / 2.f));

		//ȸ�� ����
		Vec3 rot = windHitEffect->Transform()->GetRelativeRot();
		rot.z += exeCount / 500.f;
		windHitEffect->Transform()->SetRelativeRot(rot);

		//�����̱�
		Vec3 pos = dummyParent01->Transform()->GetRelativePos();
		_effectMoveDir.y = 0.f;
		_effectMoveDir.Normalize();
		Vec3 resultPos = pos + (_effectMoveDir * moveIncreasing);
		dummyParent01->Transform()->SetRelativePos(resultPos);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
	DestroyObject(dummyParent02);
}

void ER_EffectSystem::AddDeleteParticles(CGameObject* _obj, CGameObject* _parentObj)
{
	lock_guard<mutex> lockGuard(m_aFuncMutex);

	if (_obj->ParticleSystem() == nullptr)
		assert(false);

	CParticleSystem* Particle = _obj->ParticleSystem();
	tParticleModule particle_data = Particle->GetParticleInfo();
	m_vParticleLifeTime.push_back(particle_data.MaxLifeTime + 0.1f);
	particle_data.MinLifeTime = 0.f;
	particle_data.MaxLifeTime = 0.f;
	particle_data.SpawnRate = 0;
	Particle->SetParticleInfo(particle_data);

	if(_parentObj != nullptr)
		m_vDeleteParticles.push_back(_parentObj);
	else 
		m_vDeleteParticles.push_back(_obj);
}
