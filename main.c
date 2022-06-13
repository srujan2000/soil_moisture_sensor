volatile char *AD_MUX = (char*)0x7C; //ADC Multiplexer Selection Registers
volatile char *ADC_SRB = (char*)0x7B;// ADC Control and Status Register B
volatile char *ADC_SRA = (char*)0x7A;// ADC Control and Status Register A
volatile short *ADC_LH = (short*)0x78;// ADC data register


volatile char *ddr_k = (char*)0x107;
volatile char *port_k = (char*)0x108;
volatile char *ddr_l = (char*)0x10A;
volatile char *port_l = (char*)0x10B;

void setup() {
  Serial.begin(9600);
  *ddr_k = 0x03;
  *ddr_l = 0x01;
  *port_l = 0x00;
  adc_read();
  init_timer();
}

void init_timer(){
  volatile char *TCCR1_A = (char*)0x80;
  volatile char *TCCR1_B = (char*)0x81;
  volatile short *TCNT_1 = (short*)0x84;
  volatile short *OCR1_A = (short*)0x88;
  volatile char *TIMSK_1 = (char*)0x6F;

  *TCCR1_A = 0;
  *TCCR1_B = 0x0C;
  *TCNT_1  = 0;
   
   *OCR1_A  = 60000;
   *TIMSK_1 = 0x03;
}

ISR(TIMER1_COMPA_vect){
  *port_k = 0x01; //turn on the sensor
  *ADC_SRA |= 0x40; //start conversion 
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
  *AD_MUX =  0xC0;//internal 2.56 ref and ADC0 is selected
  *ADC_SRA = 0xDF;//ADC Enable, interrupt enable,prescalar-128.
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
