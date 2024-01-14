#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTexture.h>

#include <Engine\CParticleSystem.h>

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("##ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)
	, m_bParticleInfoUI(false)
	, m_iCurTab(0)
{
	SetName("ParticleSystem");
}

ParticleSystemUI::~ParticleSystemUI()
{
}

int ParticleSystemUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	if (ImGui::Button("EditParticle##ParticleSystemWindow"))
	{
		m_bParticleInfoUI = true;
		
		// �ӽ� �����
		m_tTempParticleData = GetTarget()->ParticleSystem()->GetParticleData();
	}

	if (m_bParticleInfoUI)
		render_ParticleInfoWindow();

	return TRUE;
}

void ParticleSystemUI::render_ParticleInfoWindow()
{
	tParticleModule mParticle = GetTarget()->ParticleSystem()->GetParticleData();
	
	if (ImGui::Begin("Particle_Editor", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking))
	{
		render_TabBar(ImGuiTabBarFlags_None);

		ImGui::BeginGroup();

		ImGui::Text("Module Check ");
		ImGui::SameLine();
		
		switch (m_iCurTab)
		{
		case (UINT)PARTICLE_MODULE::PARTICLE_SPAWN:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN], "##PARTICLE_SPAWN", (UINT)PARTICLE_MODULE::PARTICLE_SPAWN);

			// ImGui::Text("Max ParticleCount	: ");
			// ImGui::SameLine();
			// ImGui::SliderInt("##MaxParticleCount", &mParticle.iMaxParticleCount, 0, 3000, "%d");
			
			ImGui::Text("SpawnRate			: ");
			ImGui::SameLine();
			ImGui::SliderInt("##SpawnRate", &mParticle.SpawnRate, 0, 3000, "%d");


			Vec4    vSpawnColor;		// ������ ����
			Vec4	vSpawnScaleMin;		// ������ �ּ� ũ�����
			Vec4	vSpawnScaleMax;		// ������ �ִ� ũ�����
			Vec3	vBoxShapeScale;		// �������� ũ��
			float	fSphereShapeRadius;	// ����Ÿ�� ��, ������ ����
			int		SpawnShapeType;		// 0 : BOX, 1 : Sphere
			int		Space;				// ��ƼŬ ������Ʈ ��ǥ�� ( 0 : World,  1 : Local)
			float   MinLifeTime;		// �ּ� ����
			float   MaxLifeTime;		// �ִ� ����
			

			break;
		}
		case (UINT)PARTICLE_MODULE::COLOR_CHANGE:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE], "##COLOR_CHANGE", (UINT)PARTICLE_MODULE::COLOR_CHANGE);

			ImGui::Text("COLOR_CHANGE");

			Vec4	vStartColor;		// �ʱ� ����
			Vec4	vEndColor;			// ���� ����

			break;
		}
		case (UINT)PARTICLE_MODULE::SCALE_CHANGE:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE], "##SCALE_CHANGE", (UINT)PARTICLE_MODULE::SCALE_CHANGE);

			ImGui::Text("SCALE_CHANGE");

			float	StartScale;			// �ʱ� ����
			float	EndScale;			// ���� ����	

			break;
		}
		case (UINT)PARTICLE_MODULE::ADD_VELOCITY:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY], "##ADD_VELOCITY", (UINT)PARTICLE_MODULE::ADD_VELOCITY);

			ImGui::Text("ADD_VELOCITY");

			Vec4	vVelocityDir;
			int     AddVelocityType;	// 0 : From Center, 1: To Center, 2 : Fixed Direction	
			float	OffsetAngle;
			float	Speed;
			int     addvpad;

			break;
		}
		case (UINT)PARTICLE_MODULE::DRAG:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG], "##DRAG", (UINT)PARTICLE_MODULE::DRAG);

			ImGui::Text("DRAG");

			float	StartDrag;
			float	EndDrag;

			break;
		}
		case (UINT)PARTICLE_MODULE::NOISE_FORCE:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE], "##NOISE_FORCE", (UINT)PARTICLE_MODULE::NOISE_FORCE);

			ImGui::Text("NOISE_FORCE");

			float	fNoiseTerm;		// ���� �� ���� ����
			float	fNoiseForce;	// ���� �� ũ��

			break;
		}
		case (UINT)PARTICLE_MODULE::RENDER:
		{
			OnOffBtn(mParticle.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER], "##RENDER", (UINT)PARTICLE_MODULE::RENDER);

			ImGui::Text("RENDER");

			int		VelocityAlignment;	// 1 : �ӵ����� ���(�̵� �������� ȸ��) 0 : ��� ����
			int		VelocityScale;		// 1 : �ӵ��� ���� ũ�� ��ȭ ���, 0 : ��� ����	
			float   vMaxSpeed;			// �ִ� ũ�⿡ �����ϴ� �ӷ�
			Vec4	vMaxVelocityScale;	// �ӷ¿� ���� ũ�� ��ȭ�� �ִ�ġ
			int		renderpad;

			break;
		}
		default:
			break;
		}

		ImGui::EndGroup();

		GetTarget()->ParticleSystem()->SetParticleInfo(mParticle);

		if (ImGui::Button("   OK   ##ParticleEditorDoneBtn"))
		{
			m_tTempParticleData = mParticle;
			m_bParticleInfoUI = false;
		}

		ImGui::SameLine();

		if (ImGui::Button(" Cancle ##ParticleEditorCancleBtn"))
		{
			GetTarget()->ParticleSystem()->SetParticleInfo(m_tTempParticleData);
			m_bParticleInfoUI = false;
		}

		ImGui::End();
	}
}

void ParticleSystemUI::render_TabBar(ImGuiTabBarFlags _flags)
{
	if (ImGui::BeginTabBar("ParticleEditorTabBar", _flags))
	{
		if (ImGui::BeginTabItem("SPAWN"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::PARTICLE_SPAWN; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Spawn");
		
		if (ImGui::BeginTabItem("COLOR"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::COLOR_CHANGE; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Color Change Effect");
		
		if (ImGui::BeginTabItem("SCALE"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::SCALE_CHANGE; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Scale Change Effect");
		
		if (ImGui::BeginTabItem("VELOCITY"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::ADD_VELOCITY; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Add Velocity Effect");
		
		if (ImGui::BeginTabItem("DRAG"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::DRAG; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Drag Effect");
		
		if (ImGui::BeginTabItem("NOISE"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::NOISE_FORCE; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle RandomSpawn Effect");
		
		if (ImGui::BeginTabItem("RENDER"))
		{
			m_iCurTab = (UINT)PARTICLE_MODULE::RENDER; ImGui::EndTabItem();
		}
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Set Particle Render Option");

		ImGui::EndTabBar();
	}
}

void ParticleSystemUI::OnOffBtn(int& _ModuleCheck, const char* _Lavel, UINT _id)
{
	bool bOn = false;
	int  BtnCol = 0;

	string strName = "OFF";

	if (0 < _ModuleCheck)
	{
		bOn = true;
		BtnCol = 2;
		strName = "ON";
	}
	strName += _Lavel;

	ImGui::PushID(_id);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(BtnCol / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(BtnCol / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(BtnCol / 7.0f, 0.8f, 0.8f));

	if (ImGui::Button(strName.c_str()))
	{
		if (bOn)
			_ModuleCheck = false;
		else
			_ModuleCheck = true;
	}

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}