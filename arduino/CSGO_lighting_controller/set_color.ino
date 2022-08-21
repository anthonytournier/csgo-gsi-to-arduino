void setFadeColor(int color[3], int speed)
{
  fadeToColor[0] = color[0];
  fadeToColor[1] = color[1];
  fadeToColor[2] = color[2];
  fadeSpeed = speed;
  fadeInProgress = true;
}

void setColor(int color[3])
{
  analogWrite(REDPIN, color[0]);
  analogWrite(GREENPIN, color[1]);
  analogWrite(BLUEPIN, color[2]);
  fadeInProgress = false;
  setCurrentColor(color);
}

void setColorForFade(int color[3])
{
  analogWrite(REDPIN, color[0]);
  analogWrite(GREENPIN, color[1]);
  analogWrite(BLUEPIN, color[2]);
  setCurrentColor(color);
}

void setColorhealth(int color[3])
{
  analogWrite(REDPIN, color[0]);
  analogWrite(GREENPIN, health);
  analogWrite(BLUEPIN, color[2]);
  fadeInProgress = false;
  setCurrentColor(color);
}
