cbuffer cbView : register(b0)
{
    matrix View;
};

cbuffer cbProjection : register(b1)
{
    matrix Projection;
};

cbuffer cbWorld : register(b2)
{
    matrix World;
};

[numThreads(16, 16, 1)]
void GS(uint3 id : SV_DispatchThreadID)
{
    
}