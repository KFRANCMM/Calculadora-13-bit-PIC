#include <18f4620.h>
#use delay(clock=16M)
#fuses XT, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG

void outputResultado13b(int16 res);   //Funcion para enviar resultado a los puertos A,B,E de los pines correspondientes

void main(void){
      
      setup_oscillator(OSC_normal);
      setup_adc(NO_ANALOGS);
      
       //Activacion de entradas y salidas correspondientes en los puertos
      SET_TRIS_A(0x00);
      SET_TRIS_E(0x08);
      SET_TRIS_B(0xF0); 
      SET_TRIS_C(0xFF);
      SET_TRIS_D(0xFF);
      
      unsigned int8 numeroUno = 0;               // Se puede reemplazar esto al leerlo directamente y operarlo    
      unsigned int8 numeroDos = 0;     
      int16 resultado = 0;                       //Variable resultado para almacenar resultados de operacion
      unsigned int8 banderasOperaciones = 0;     //Variable usada para uso de banderas y control de operaciones
    
      //Declaracion de bits a partir de BanderasOperaciones
      #bit banderaError = banderasOperaciones.2;
      #bit banderaContador = banderasOperaciones.3;
      #bit suma = banderasOperaciones.4;
      #bit resta = banderasOperaciones.5;
      #bit multiplicacion = banderasOperaciones.6;
      #bit division = banderasOperaciones.7;
      
      //Inicalización de variables
      int contador = 50;
      
      while(true){
         //Inicializacion bandera error
         banderaError = 0;
         
         //Lectura de operandos
         numeroUno = input_c();
         numeroDos = input_d();
         
         //Evaluación de boton presionado y activado de bandera correspondiente 
         //a operacion a realizar
         
         if(input(PIN_B4)){
            suma = 1;
            resta = 0;
            multiplicacion = 0;
            division = 0;
         } 
         else if(input(PIN_B5)){
            suma = 0;
            resta = 1;
            multiplicacion = 0;
            division = 0;
         } 
         else if(input(PIN_B6)){
            suma = 0;
            resta = 0;
            multiplicacion = 1;
            division = 0;           
         } 
         else if(input(PIN_B7)){
             suma = 0;
            resta = 0;
            multiplicacion = 0;
            division = 1;
         }
         
         //Realización de calculo a partir de la operacion activa
         if(suma){
            resultado = (long)numeroUno + (long)numeroDos;
         } 
         else if(resta){
            resultado = (long)numeroUno - (long)numeroDos;
         } 
         else if(multiplicacion){
            resultado = (long)numeroUno * (long)numeroDos;     
         } 
         else if(division){
             if(numeroDos != 0){
               resultado = (long)numeroUno / (long)numeroDos;
               outputResultado13b(resultado);
               
            } 
            else{
               banderaError = 1;  //Bandera error Activa
               
               delay_ms(10);
               contador ++;
               if(contador == 50){
                  if(banderaContador == 1){
                     outputResultado13b(8191);
                      banderaContador = 0;
                     contador = 0;
                    
                  }
                  else{
                     outputResultado13b(0);
                     banderaContador = 1;
                     contador = 0;
                  }       
              }       
            }
         }
         
         //Muestra de variable resultado solo si no se enecuentra activo el error de division.
         if(banderaError == 0)
            outputResultado13b(resultado);
   }

}
       

void outputResultado13b(int16 res){
   OUTPUT_A(res);
   OUTPUT_B(res>>6);
   OUTPUT_E(res>>10);
}

