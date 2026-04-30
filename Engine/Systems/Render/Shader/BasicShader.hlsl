float4 main(float4 pos : POSITION) : SV_POSITION
{
    return pos;
}

struct PSInput
{
    float4 position : SV_POSITION;
};

PSInput VSMain(float4 position : POSITION)
{
    PSInput result;
    result.position = position;
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return float4(1,0,0,1);
}