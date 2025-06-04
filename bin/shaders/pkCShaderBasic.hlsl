/***************************************************************************************
* Constant Buffer Structures
***************************************************************************************/

#pragma kernel CS_Main

RWTexture2D<float4> outputTexture : register(u0);
// needs to create/set all the data to the shader
// how do you create the cells?
// takes the index of a cell inside of the cell crid
[numthreads(1920, 1080, 1)]
void CS_Main(uint3 id : SV_DispatchThreadID)
{
  // code for each cell to perform
}