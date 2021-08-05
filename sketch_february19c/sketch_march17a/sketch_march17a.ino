#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN    21
#define RST_PIN   22

#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16

//Authentication
MFRC522::MIFARE_Key key;

//Authentication status code
MFRC522::StatusCode status;

//Pins
MFRC522 mfrc522(SS_PIN, RST_PIN); 

int tiltPin = 2;
int tiltStatus;

boolean isTilted;
boolean isPlaying = false;

String player = "";

void setup() {
    Serial.begin(9600);
    SPI.begin();
    
    mfrc522.PCD_Init();

    pinMode(tiltPin, INPUT);
}

void loop() {
    if(isPlaying){
        tiltStatus = digitalRead(tiltPin);

        if(tiltStatus == HIGH){
            if(!isTilted){
                isTilted = true;

                Serial.println(player + " - BAIXO");
            }
        } else {
            if (isTilted) {
                isTilted = false;

                Serial.println(player + " - CIMA");
            }
        }
    } else {
        //call menu 
        int option = menu();

        if(option == 0) 
            registerPlayer();
        else if(option == 1) 
            selectPlayer();
        else
            Serial.println(F("opcao incorreta!"));

            return;

        
    }

    //Instructs the PICC when in the ACTIVE state to go to a "STOP" state
    mfrc522.PICC_HaltA(); 

    //Stop the encryption of the PCD
    mfrc522.PCD_StopCrypto1();
}

int menu(){
    Serial.println(F("\nEscolha uma opcao:"));
    Serial.println(F("0 - Registrar jogadores"));
    Serial.println(F("1 - Escolher quem vai jogar\n"));

    //Waits user interaction
    while(!Serial.available()){};
    
    //Retrieves the chosen option
    int option = (int)Serial.read();

    while(Serial.available()) {
        if(Serial.read() == '\n') break; 
        Serial.read();
    }

    return (option-48); //Subtract 48 from read value, 48 is the zero from ascii table
}

void registerPlayer(){
    Serial.println(F("\nAproxime um cartao"));

    //waiting the card approach
    while (!mfrc522.PICC_IsNewCardPresent()) {}

    // Select a card
    while (!mfrc522.PICC_ReadCardSerial()) {}

    //Prints thecnical details from of the card/tag
    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));

    Serial.println(F("\nA qual jogador esse cartao pertence?"));
    Serial.println(F("1 - Jogador 1"));
    Serial.println(F("2 - Jogador 2\n"));

    while(!Serial.available()){};

    int playerOption = (int)Serial.read();

    while(Serial.available()) {
        if(Serial.read() == '\n') break; 
            Serial.read();
    }

    playerOption = playerOption - 48;

    String data;

    if(playerOption == 1){
        data = "Player 1";
    }

    else if(playerOption == 2){
        data = "Player 2";
    }

    //Prepare the key - all keys are set to FFFFFFFFFFFFh
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    byte buffer[MAX_SIZE_BLOCK] = {0};
    byte block;
    byte dataSize;

    dataSize = sizeof(data);

    data.getBytes(buffer, dataSize);

    //Void positions that are left in the buffer will be filled with whitespace
    for(byte i = dataSize; i < MAX_SIZE_BLOCK; i++)
    {
        buffer[i] = ' ';
    }
    
    block = 1; //The block to operate

    //Authenticates the block to operate
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                        block, &key, &(mfrc522.uid));

    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));

        return;
    }
    
    //Writes in the block
    status = mfrc522.MIFARE_Write(block, buffer, MAX_SIZE_BLOCK);

    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        
        return;
    }else{
        Serial.println(F("Jogador registrado com sucesso!"));
    }

    return;
}

void selectPlayer(){
    Serial.println(F("\nAproxime um cartao"));

    //waiting the card approach
    while (!mfrc522.PICC_IsNewCardPresent()) {}

    // Select a card
    while (!mfrc522.PICC_ReadCardSerial()) {}

    //Prints the technical details of the card/tag
    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); 
    
    //Prepare the key - all keys are set to FFFFFFFFFFFFh
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
    
    //Buffer for read data
    byte buffer[SIZE_BUFFER] = {0};
    
    //The block to operate
    byte block = 1;
    byte size = SIZE_BUFFER; //Authenticates the block to operate

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
       
    /* if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Authentication failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));

        return;
    }*/

    //Read data from block
    status = mfrc522.MIFARE_Read(block, buffer, &size);

    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        
        return;
    }

    player = (char*)buffer;

    Serial.println("\nO " + player + " ira jogar");

    isPlaying = true;

    return;
}