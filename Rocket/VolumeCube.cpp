#include "VolumeCube.h"
#include "Pipeline.h"

VolumeCube::VolumeCube() :
	Volume()
{
	mScale = { 5.0f,5.0f,5.0f };

}

void VolumeCube::Draw()
{
	Pipeline::mCommandList->DrawInstanced(36, 1, 0, 0);
}
