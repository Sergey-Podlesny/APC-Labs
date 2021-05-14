#include <dos.h>
#include <conio.h>
#include <stdio.h>

int FEsig;
int quit;
int blinkType;

void interrupt far newint09(...);
void interrupt(far* oldint09)(...);

void inicialize(){
	FEsig = 0;
	quit = 0;
	blinkType = 0;
	_disable();
	oldint09 = getvect(0x09);
	setvect(0x09, newint09);
	_enable();
}

void deinicialize(){
	setvect(0x09, oldint09);
}

void interrupt newint09(...){
	unsigned char temp = inp(0x60);
	if (temp == 0xFE)
		FEsig = 1;
	if (temp == 0x02)  //нажата ли 1
		blinkType = 1;
	if (temp == 0x01)  //нажата ли Esc
		quit = 1;
	printf("\t%x ", temp);
	oldint09();
}

void ChangeIndicator(unsigned char mask){
	do{
		if (quit == 1)  return;
		FEsig = 0;
		while ((inp(0x64) & 0x02) != 0x00);//проверяем входной регистр на незанятость
		outp(0x60, 0xED); //отправляем код, для управления индикаторами через, 60h
	} while (FEsig == 1);
	do{
		if (quit == 1)  return;
		FEsig = 0;
		while ((inp(0x64) & 0x02) != 0x00);
		outp(0x60, mask); //отправляем маску, в соответствии с которой должны загорется индикаторы, через 60h
	} while (FEsig == 1);
}

void main(){
	inicialize();
	while (quit == 0){
		if (blinkType == 1){
			for(int i = 0;i<3;i++){
				ChangeIndicator(0x04);
				delay(200);
			}
			blinkType = 0;
		}
	}
	deinicialize();
}