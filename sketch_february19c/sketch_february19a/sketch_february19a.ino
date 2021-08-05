int valorPOT;
int porcent;

void setup() {
// put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
// put your main code here, to run repeatedly:
    valorPOT = analogRead(34);
    porcent = map(valorPOT, 0, 4095, 0, 100);
    Serial.println(porcent);
    delay(500);
}
