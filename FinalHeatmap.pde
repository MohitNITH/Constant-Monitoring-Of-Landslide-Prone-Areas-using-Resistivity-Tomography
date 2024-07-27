import processing.serial.*;

Serial myPort;
float[][] data = new float[8][8]; // Declare data at the global scope

void setup() {
  size(800, 600);
  colorMode(HSB, 1);
  
  // Replace with the appropriate serial port for your Arduino
  String portName = "COM14";
  myPort = new Serial(this, portName, 115200);
}

void draw() {
  background(0);
  drawHeatmap(100, 100, 600, 400);
}

void drawHeatmap(int x, int y, int w, int h) {
  float minValue = Float.MAX_VALUE;
  float maxValue = Float.MIN_VALUE;
  
  // Find the minimum and maximum values in the data array
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      float value = data[i][j]; // Can access data here
      minValue = min(minValue, value);
      maxValue = max(maxValue, value);
    }
  }
  
  float range = maxValue - minValue;
  
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      float value = data[i][j]; // Can access data here
      if (range != 0) {
        float colorValue = (value - minValue) / range;
        fill(colorValue, 1, 1);
      } else {
        fill(0.5, 1, 1); // Default color if all values are the same
      }
      rect(x + j * w / 8, y + i * h / 8, w / 8, h / 8);
    }
  }
}

void serialEvent(Serial myPort) {
  String line = myPort.readStringUntil('\n');
  if (line != null) {
    line = trim(line);
    String[] values = split(line, ',');
    if (values.length == 3) {
      int electrode = int(values[0]);
      float resistivity = float(values[2]);
      data[electrode / 8][electrode % 8] = resistivity; // Can access data here
    }
  }
}
