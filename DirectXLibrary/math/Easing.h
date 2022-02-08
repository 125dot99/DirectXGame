#pragma once
namespace gamelib
{
//イージング関数
namespace easing 
{
//低速から高速
float EaseInSine(float x);
float EaseInCubic(float x);
float EaseInQuint(float x);
float EaseInCirc(float x);
float EaseInElastic(float x);
float EaseInQuad(float x);
float EaseInQuart(float x);
float EaseInExpo(float x);
float EaseInBack(float x);
float EaseInBounce(float x);

//高速から低速
float EaseOutSine(float x);
float EaseOutCubic(float x);
float EaseOutQuint(float x);
float EaseOutCirc(float x);
float EaseOutElastic(float x);
float EaseOutQuad(float x);
float EaseOutQuart(float x);
float EaseOutExpo(float x);
float EaseOutBack(float x);
float EaseOutBounce(float x);

//低速から高速し低速
float EaseInOutSine(float x);
float EaseInOutCubic(float x);
float EaseInOutQuint(float x);
float EaseInOutCirc(float x);
float EaseInOutElastic(float x);
float EaseInOutQuad(float x);
float EaseInOutQuart(float x);
float EaseInOutExpo(float x);
float EaseInOutBack(float x);
float EaseInOutBounce(float x);
} // namespace easing
} // namespace gamelib