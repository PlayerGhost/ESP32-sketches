int botao = 18;

void setup() {
// put your setup code here, to run once:
    pinMode(2, OUTPUT);
    pinMode(botao, INPUT_PULLUP);
}

void loop() {
// put your main code here, to run repeatedly:
    if(digitalRead(botao) == LOW){
        digitalWrite(2, HIGH);
    }else{
        digitalWrite(2, LOW);
    }
}
