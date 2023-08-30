void setColorWithIntensity(int color[3], float intensity)
{
  currentColor[0] = (int)(color[0] * intensity);
  currentColor[1] = (int)(color[1] * intensity);
  currentColor[2] = (int)(color[2] * intensity);
  currentColor[0] = constrain(currentColor[0], 0, 255);
  currentColor[1] = constrain(currentColor[1], 0, 255);
  currentColor[2] = constrain(currentColor[2], 0, 255);
  fadeInProgress = false;
  analogWrite(REDPIN, currentColor[0]);
  analogWrite(GREENPIN, currentColor[1]);
  analogWrite(BLUEPIN, currentColor[2]);
}
