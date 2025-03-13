int ThermistorPin1 = 0;
int ThermistorPin2 = 1;

// int ThermistorPin1 = 0;
// int ThermistorPin2 = 1;

int HEATER_PIN = 1;

struct Temperature {
    float c;
    float f;
};

void setup() {
    pinMode(HEATER_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    Temperature t1 = getFloorTemperature();
    Temperature t2 = getRoomTemperature();

    Serial.print("Floor Temperature: "); 
    Serial.print(t1.f);
    Serial.print(" F; ");
    Serial.print(t1.c);
    Serial.println(" C");   

    Serial.print("Room Temperature: "); 
    Serial.print(t2.f);
    Serial.print(" F; ");
    Serial.print(t2.c);
    Serial.println(" C");   

    if (t1.c > 24) {  // Виправлено: використовуємо t1.c замість Tc
        turnHeater(LOW);
    } else {
        turnHeater(HIGH);
    }

    delay(500);
}

void turnHeater(int value) {
    digitalWrite(HEATER_PIN, value);
}

Temperature getTemperature(int pin, float R1) {
    float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

    int Vo = analogRead(pin);
    if (Vo == 0) return {0, 32};  // Уникнення ділення на нуль
    
    float R2 = R1 * (1023.0 / (float)Vo - 1.0);
    float logR2 = log(R2);
    float T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
    float Tc = T - 273.15;
    float Tf = (Tc * 9.0) / 5.0 + 32.0;

    return {Tc, Tf};
}

Temperature getRoomTemperature() {
    return getTemperature(ThermistorPin2, 10000);
}

Temperature getFloorTemperature() {
    return getTemperature(ThermistorPin1, 10000);
}
