int botao = 18;
int botao1 = 19;

void setup() {
// put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(2, OUTPUT);
    pinMode(botao, INPUT_PULLUP);
    pinMode(botao1, INPUT_PULLUP);
}

void loop() {
// put your main code here, to run repeatedly:
    if(digitalRead(botao) == LOW){
        Serial.println("P1 - Cima");
    }else if(digitalRead(botao1) == LOW){
        Serial.println("P1 - Baixo");
    }
}
