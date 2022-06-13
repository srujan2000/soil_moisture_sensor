volatile char *AD_MUX = (char*)0x7C; 
volatile char *ADC_SRB = (char*)0x7B;
volatile char *ADC_SRA = (char*)0x7A;
volatile short *ADC_LH = (short*)0x78;


volatile char *ddr_k = (char*)0x107;
volatile char *port_k = (char*)0x108;
volatile char *ddr_l = (char*)0x10A;
volatile char *port_l = (char*)0x10B;

void setup() {
  Serial.begin(9600);
  *ddr_k = 0x03;
  *ddr_l = 0x01;
  *port_l = 0x00;
  init_timer();
}

void init_timer(){
  volatile char *TCCR1_A = 0x80;
  volatile char *TCCR1_B = 0x81;
  volatile short *TCNT_1 = 0x84;
  volatile short *OCR1_A = 0x88;
  volatile char *TIMSK_1 = 0x6F;

  *TCCR1_A = 0;
  *TCCR1_B = 0x0C;
  *TCNT_1  = 0;
   
   *OCR1_A  = 60000;
   *TIMSK_1 = 0x03;
}

ISR(TIMER1_COMPA_vect){
  *port_k = 0x01; //turn on the sensor
  adc_read();
  delay1();
  *port_k = 0x00; //turn off the sensor
}

ISR(ADC_vect){
  Serial.println(*ADC_LH,DEC);
  if(*ADC_LH>400){  //check if soil is dry 
    *port_l = 0x01; //turn on dc pump
    delay2();
    *port_l = 0x00; // turn off dc pump
  }
}

void adc_read(){
  *AD_MUX =  0xC0;
  *ADC_SRA = 0xDF;
}

void delay1(){
  volatile long i;
  for(i=0;i<20000;i++);
}

void delay2(){
  volatile long i;
  for(i=0;i<500000;i++);
}

void loop() {
  // put your main code here, to run repeatedly:

}
