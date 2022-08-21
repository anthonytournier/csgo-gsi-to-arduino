void setFadeColorWithIntensity(int color[3], float intensity, int speed)
{
  fadeToColor[0] = (int)(color[0] * intensity);
  fadeToColor[1] = (int)(color[1] * intensity);
  fadeToColor[2] = (int)(color[2] * intensity);
  fadeToColor[0] = constrain(fadeToColor[0], 0, 255);
  fadeToColor[1] = constrain(fadeToColor[1], 0, 255);
  fadeToColor[2] = constrain(fadeToColor[2], 0, 255);
  fadeSpeed = speed;
  fadeInProgress = true;
}
