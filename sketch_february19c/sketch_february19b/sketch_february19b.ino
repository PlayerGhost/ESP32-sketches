int valorPOT;

int flag = 0;
int flag1 = 0;

int porcent;

int P1Cima = 18;
int P1Baixo = 19;

void setup() {
// put your setup code here, to run once:
    Serial.begin(9600);

    pinMode(P1Cima, INPUT_PULLUP);
    pinMode(P1Baixo, INPUT_PULLUP);
}

void loop() {
// put your main code here, to run repeatedly:
    valorPOT = analogRead(34);
    porcent = map(valorPOT, 0, 4095, 0, 100);

    Serial.print("Player 2: ");
    Serial.println(porcent);

    if(digitalRead(P1Cima) == LOW && flag == 0){
        Serial.println("P1 - Cima");
        flag = 1;
    }else if(digitalRead(P1Cima) == HIGH){
        flag = 0;
    }
    
    if(digitalRead(P1Baixo) == LOW && flag1 == 0){
        Serial.println("P1 - Baixo");
        flag1 = 1;
    }else if(digitalRead(P1Baixo) == HIGH){
        flag1 = 0;
    }

    delay(200);
}
