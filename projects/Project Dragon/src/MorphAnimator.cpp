#include <MorphAnimator.h>

#include <Timer.h>
#include <iostream>
AData MorphAnimator::GetAnimData()
{
	return m_AnimData;
}

void MorphAnimator::LoadFrame(std::string filePath, glm::vec4 color)
{
	m_AnimData.m_Frames.push_back(ObjLoader::LoadFrame(filePath, color));
}

void MorphAnimator::SendToVao()
{
	
		//Get our indexes from our animation Data
		int F1 = m_Animations[m_AnimData.m_ActiveAnimation].m_CurrentFrame;
		int F2 = m_Animations[m_AnimData.m_ActiveAnimation].m_NextFrame;

		if (m_Animations[m_AnimData.m_ActiveAnimation].m_ShouldSwitchFrames)
		{

			uint32_t slot = 0;
			m_vao->AddVertexBuffer(m_AnimData.m_Frames[F1].m_Pos, { BufferAttribute(slot, 3,
				GL_FLOAT, false, NULL,NULL) });

			slot = 1;
			m_vao->AddVertexBuffer(m_AnimData.m_Frames[F1].m_Col, { BufferAttribute(slot, 3,
			GL_FLOAT, false, NULL,NULL) });

			slot = 2;
			m_vao->AddVertexBuffer(m_AnimData.m_Frames[F1].m_Normal, { BufferAttribute(slot, 3,
			GL_FLOAT, false, NULL,NULL) });

			slot = 3;
			m_vao->AddVertexBuffer(m_AnimData.m_Frames[F1].m_UV, { BufferAttribute(slot, 2,
			GL_FLOAT, false, NULL,NULL) });

			slot = 4;
			m_vao->AddVertexBuffer(m_AnimData.m_Frames[F2].m_Pos, { BufferAttribute(slot, 3,
				GL_FLOAT, false, NULL,NULL) });

			slot = 5;
			m_vao->AddVertexBuffer(m_AnimData.m_Frames[F2].m_Col, { BufferAttribute(slot, 3,
			GL_FLOAT, false,NULL,NULL) });

			slot = 6;
			m_vao->AddVertexBuffer(m_AnimData.m_Frames[F2].m_Normal, { BufferAttribute(slot,  3,
			GL_FLOAT, false, NULL,NULL) });

			m_Animations[m_AnimData.m_ActiveAnimation].m_ShouldSwitchFrames = false;
		}
	
	
}

void MorphAnimator::Update()
{


	//m_Animations[m_AnimData.m_ActiveAnimation].m_Timer += Timer::dt;

	
		m_AnimData.t += Timer::dt * m_Animations[m_AnimData.m_ActiveAnimation].m_TimeForFrame;


		if (m_AnimData.t >= 1.f)
		{
			m_Animations[m_AnimData.m_ActiveAnimation].m_ShouldSwitchFrames = true; //for the vao update


			//updates our frames

			int newCurFrame = m_Animations[m_AnimData.m_ActiveAnimation].m_NextFrame;
			int newNextFrame = m_Animations[m_AnimData.m_ActiveAnimation].m_NextFrame + 1;
			if (newNextFrame > m_Animations[m_AnimData.m_ActiveAnimation].m_LastFrame)
			{
				newNextFrame = m_Animations[m_AnimData.m_ActiveAnimation].m_FirstFrame;
			}
			if (newCurFrame > m_Animations[m_AnimData.m_ActiveAnimation].m_LastFrame)
				newCurFrame = m_Animations[m_AnimData.m_ActiveAnimation].m_FirstFrame;

			m_Animations[m_AnimData.m_ActiveAnimation].m_CurrentFrame = newCurFrame;
			m_Animations[m_AnimData.m_ActiveAnimation].m_NextFrame = newNextFrame;

			//resets timers
			m_AnimData.t = 0.f;
			m_Animations[m_AnimData.m_ActiveAnimation].m_Timer = 0.f;

		}
	


	SendToVao();
}

void MorphAnimator::SetActiveAnimation(int index)
{
	m_AnimData.m_ActiveAnimation = index;
}

void MorphAnimator::AddNewAnimation(int firstframe, int lastframe, float timeperframe)
{
	Animation newAnimation;
	newAnimation.m_FirstFrame = firstframe;
	newAnimation.m_LastFrame = lastframe;
	newAnimation.m_TimeForFrame = timeperframe;

	newAnimation.m_CurrentFrame = firstframe;
	newAnimation.m_NextFrame = firstframe + 1;



	m_Animations.push_back(newAnimation);

}

void MorphAnimator::SetVAO(VertexArrayObject::sptr vao)
{
	m_vao = vao;
}

VertexArrayObject::sptr MorphAnimator::GetVAO()
{
	return m_vao;
}

void MorphAnimator::SetT(float newt)
{
	m_AnimData.t = newt;
}
