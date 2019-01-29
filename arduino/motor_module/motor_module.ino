    /*  Arduino DC Motor Control - PWM | H-Bridge | L298N  -  Example 01
        by Dejan Nedelkovski, www.HowToMechatronics.com
    */
    #define enA 9
    #define in1 6
    #define in2 7
    #define button 4
    #define pin 2
    int rotDirection = 0;
    int pressed = false;
    int pwmOutput = 80;
    int flag = 1;
    int counter_opto = 0;
    int rotation = 0;
    void setup() {
      Serial.begin(9600);
      pinMode(enA, OUTPUT);
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      pinMode(button, INPUT);
      // Set initial rotation direction
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      pinMode(pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin), incerment_counter, RISING); 
    }
    void loop() {
      // send data only when you receive data:
        if (Serial.available() > 0) {
          pwmOutput = Serial.read();
          Serial.print("Rcv msg");
        }
        if(flag==1)
        {
        pwmOutput = pwmOutput + 10;
          if (pwmOutput>255)
          {
            pwmOutput = 255;
            flag = 0;
          }
        }
        else
        {
         pwmOutput = pwmOutput - 10;
          if (pwmOutput<80)
          {
          pwmOutput = 80;
          flag = 1;
          }
        }
        //Serial.print(pwmOutput);
        //Serial.print("\n");
      analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
      // Read button - Debounce
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        if(counter_opto>20)
        {
        rotation++;
        Serial.print(rotation);
        counter_opto = 0;
        Serial.print("\n");
        }

      /*
      if (digitalRead(button) == true) {
        pressed = !pressed;
      }
      while (digitalRead(button) == true);
      delay(20);
      // If button is pressed - change rotation direction
      if (pressed == true  & rotDirection == 0) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        rotDirection = 1;
        delay(20);
      }
      // If button is pressed - change rotation direction
      if (pressed == false & rotDirection == 1) {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        rotDirection = 0;
        delay(20);
      }
      */
      delay(10);
    }

    void incerment_counter()
    {
    counter_opto++;  
    }
