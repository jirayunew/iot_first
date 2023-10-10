int line = 0;
float x = 0.0;
float y_sin = 0.0;
float y_cos = 1.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  x = x + 1;
  y_sin = 0.5 * y_sin + 0.5 * sin(2 * PI * x / 50.0);
  y_cos = 0.5 * y_cos + 0.5 * cos(2 * PI * x / 50.0);
  Serial.printf("%d %f %f\n",line, y_sin, y_cos);
  delay(10);

}
