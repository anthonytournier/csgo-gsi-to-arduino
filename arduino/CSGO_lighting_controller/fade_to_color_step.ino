void fadeToColorStep(int color[])
{
  if (!isArrayEquals(currentColor, color)){
    if (currentColor[0] > color[0]){
      currentColor[0] -= fadeSpeed;
      currentColor[0] = constrain(currentColor[0], color[0], 255);
    }
    else if (currentColor[0] < color[0]){
      currentColor[0] += fadeSpeed;
      currentColor[0] = constrain(currentColor[0], 0, color[0]);
    }
    if (currentColor[1] > color[1]){
      currentColor[1] -= fadeSpeed;
      currentColor[1] = constrain(currentColor[1], color[1], 255);
    }
    else if (currentColor[1] < color[1]){
      currentColor[1] += fadeSpeed;
      currentColor[1] = constrain(currentColor[1], 0, color[1]);
    }
    if (currentColor[2] > color[2]){
      currentColor[2] -= fadeSpeed;
      currentColor[2] = constrain(currentColor[2], color[2], 255);
    }
    else if (currentColor[2] < color[2]){
      currentColor[2] += fadeSpeed;
      currentColor[2] = constrain(currentColor[2], 0, color[2]);
    }
    setColorForFade(currentColor);
  }
  else{
    fadeInProgress = false;
  }
}
