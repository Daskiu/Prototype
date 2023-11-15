const int buttonQ = 2; // Ajusta los pines según tu configuración
const int buttonW = 3;
const int buttonE = 4;
const int buttonR = 5;

void setup() {
  Serial.begin(9600);
  pinMode(buttonQ, INPUT_PULLUP);
  pinMode(buttonW, INPUT_PULLUP);
  pinMode(buttonE, INPUT_PULLUP);
  pinMode(buttonR, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(buttonQ) == LOW) {
    Serial.print("q");
  }
  if (digitalRead(buttonW) == LOW) {
    Serial.print("w");
  }
  if (digitalRead(buttonE) == LOW) {
    Serial.print("e");
  }
  if (digitalRead(buttonR) == LOW) {
    Serial.print("r");
  }
  delay(100); // Ajusta según tus necesidades y considera usar interrupciones
}
