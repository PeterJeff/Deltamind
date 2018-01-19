#include "AnimationManager.h"
#include <fstream>


Animation * AnimationManager::LoadAnimation(const char * _animationFilePath, const char* _bindposeFilePath)
{
	Animation toPush;
	std::ifstream reader;
	reader.open(_animationFilePath, std::ios_base::binary);
	if (reader.is_open())
	{
		reader.read((char*)&toPush.duration, sizeof(toPush.duration));
		int length;
		reader.read((char*)&length, sizeof(length));
		for (int i = 0; i < length; ++i)
		{
			keyframe temp;
			reader.read((char*)&temp.endTime, sizeof(temp.endTime));
			int jointNum;
			reader.read((char*)&jointNum, sizeof(jointNum));
			for (int index = 0; index < jointNum; ++index)
			{
				animJoint joint;
				reader.read((char*)&joint.parent_index, sizeof(joint.parent_index));
				for (int almostThere = 0; almostThere < 4; ++almostThere)
					reader.read((char*)&joint.transform.m[almostThere], sizeof(joint.transform.m[almostThere]));
				temp.joints.push_back(joint);
			}
			toPush.keyframes.push_back(temp);
		}
	}
	reader.close();
	toPush.animID = (unsigned int)animations.size();
	toPush.bPose = LoadBindpose(_bindposeFilePath);
	animations.push_back(toPush);
	return &animations[animations.size() - 1];
}

bindpose * AnimationManager::LoadBindpose(const char * _bindposeFilePath)
{
	for (size_t i = 0; i < bindPoses.size(); ++i)
	{
		if (strcmp(bindPoses[i].filePath, _bindposeFilePath))
			return &bindPoses[i];
	}
	bindpose toPush;
	std::ifstream reader;
	reader.open(_bindposeFilePath, std::ios_base::binary);
	int len;
	reader.read((char*)&len, sizeof(len));
	for (int i = 0; i < len; ++i)
	{
		animJoint alright;
		reader.read((char*)&alright.parent_index, sizeof(alright.parent_index));
		for (int almostThere = 0; almostThere < 4; ++almostThere)
			reader.read((char*)&alright.transform.m[almostThere], sizeof(alright.transform.m[almostThere]));

		XMStoreFloat4x4(&alright.transform, XMMatrixInverse(&XMMatrixDeterminant(XMLoadFloat4x4(&alright.transform)), XMLoadFloat4x4(&alright.transform)));
		toPush.joints.push_back(alright);
	}
	reader.close();
	toPush.filePath = _bindposeFilePath;
	bindPoses.push_back(toPush);
	return &bindPoses[bindPoses.size()-1];
}

AnimationManager::AnimationManager()
{
}


AnimationManager::~AnimationManager()
{
}

unsigned int AnimationManager::AddElement(const char * _animationFilePath, const char * _bindposeFilePath)
{
	return LoadAnimation(_animationFilePath, _bindposeFilePath)->animID;
}

Animation * AnimationManager::GetElement(const unsigned int _id)
{
	for (size_t i = 0; i < animations.size(); ++i)
	{
		if (animations[i].animID == _id)
			return &animations[i];
	}
	return nullptr;
}