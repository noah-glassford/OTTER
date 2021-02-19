#pragma once
//This is the ECS component that will handle animations, get updated through the AnimationSystem class
#include <VertexArrayObject.h>
#include <GLM/glm.hpp>
#include <ObjLoader.h>

struct AData//animation data
{
	std::vector<Frame> m_Frames;//contains position,color,normal & uv. This class basically just exists to organize that and
	float t = 1.f;//passed into the shader
	int m_ActiveAnimation;//index for the vector of Animation
};
//Create a new object of this type when you want to add a new animation to the entity
struct Animation //contains num of frames, first frame and last frame
{
	int m_FirstFrame;
	int m_LastFrame;
	float m_TimeForFrame;
	int m_NumFrames;

	//indexes for the send to VAO function
	int m_CurrentFrame;
	int m_NextFrame;

	bool m_ShouldSwitchFrames;

	float m_Timer;//timer for this animations specifically, can be seperate from T
};

//this is an object that has all the animation data
//anything that has to be animated will have this component
class MorphAnimator
{
public:
	
	MorphAnimator() //constructor
	{
		m_vao = VertexArrayObject::Create();
	}
	AData GetAnimData();
	//adds a frame to the animation
	void LoadFrame(std::string filePath, glm::vec4 color);

	void SetT(float newt);



	//Sends all the data to the vao
	void SendToVao();

	//this will update t, doesn't do the drawing. Rendering is done is RenderingSystem.cpp
	void Update();

	void SetActiveAnimation(int index);

	//void 

	void AddNewAnimation(int firstframe, int lastframe, float timeperframe);//pushes back a new animation into m_Animations

	void SetVAO(VertexArrayObject::sptr vao);
	VertexArrayObject::sptr GetVAO();




private:
	AData m_AnimData;
	VertexArrayObject::sptr m_vao; //this is used to render, making this a glorified mesh component
	std::vector<Animation> m_Animations;

};

