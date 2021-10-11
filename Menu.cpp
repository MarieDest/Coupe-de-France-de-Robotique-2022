#include <Arduino.h>
#include "Menu.h"
#include "Programme Principal.h"

//amélioration possible revenir en arrière dans le menu avec l'axe Y.

extern LiquidCrystal lcd(29, 27, 25, 23, 17, 16);
int xAxisPin = A12; // define X pin of Joystick 
int zAxisPin = 44; // define Z pin of Joystick 
int MenuEtage1 = 1; // valeur associée au menu 1 = Test 2= Homologation 3= Programme principal
int MenuEtage2 = 0; 
int CurrentEtage = 1;
bool ComeBack = LOW;
bool change = LOW;
int xVal = 0;
int zVal = 0;
unsigned long previousTime = 0;
unsigned long elapsedTime = 0;
bool ChangeEtage = LOW;
bool MenuChoisi = LOW;
int color = 1;
bool setup_Couleur_OK = LOW;
byte Fleche_Haut[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000,
  B00000,
};
byte Carre_Vide[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};
String MenuComplet[4] [3]={
  {"Test","Homologation","Programme principal"},
  {"1M","360deg",},
  {"Fermé","Ouvert","Dynamique"},
  {"qualif","finales",}
}; 
String Couleur[1][2] = {
  {"JAUNE","VIOLET"},
};

void Set_Menu_Value(){
  xVal = get_X_Val();
  if (CurrentEtage == 1){
	  MenuEtage1 = Set_Menu_Value(MenuEtage1);
  }
  else if (CurrentEtage ==2) {
	  MenuEtage2 = Set_Menu_Value(MenuEtage2);
  }
  else {
	  lcd.clear();
  }
  
}

int Set_Menu_Value(int Etage){
  if (xVal > 1000 && Etage == 1 && change == LOW ) {
	Etage = Etage+1;
	SetFleche(Etage);
    change = HIGH;
    previousTime=millis();
  }
  if (xVal >1000 && Etage ==2 && change == LOW){
    Etage = Etage+1;
	SetFleche(Etage);
    change = HIGH;
    previousTime=millis();
    
  }
  if (xVal < 50 && Etage == 3 && change == LOW ) {
	Etage = Etage -1;
	SetFleche(Etage);
    change = HIGH;
    previousTime=millis();
  }
  if (xVal <50 && Etage ==2 && change == LOW){
    Etage = Etage -1;
	SetFleche(Etage);
    change = HIGH;
    previousTime=millis();
  }
  return Etage;
};



void Select_Menu_Value(){
  zVal = get_Z_Val();
  PrintDebug();
  if (CurrentEtage == 1){
    Select_Menu_Value_Etage1();  
  }
  else if(CurrentEtage == 2){
	  if (ChangeEtage) {
		  elapsedTime = millis() - previousTime;
		  if (elapsedTime > 2000) {
			  SetupSecMenu();
			  ChangeEtage = LOW;
			  MenuEtage2 = 1;
		  }
	  }
	  
    Select_Menu_Value_Etage2();
  }
  else {
	  
	  lcd.clear();
	  MenuChoisi = HIGH;
	  delay(2000); 
  }
  SetChange();
 
  
}

void SetChange(){
  elapsedTime=millis()- previousTime;
  if (change == HIGH &&  elapsedTime > 500 ){
    change = LOW ; 
    }
}
void SetCurrentEtage(){
  previousTime = millis();
  CurrentEtage = CurrentEtage +1 ;
  ChangeEtage = HIGH;

}
void Select_Menu_Value_Etage1(){

  if (zVal == 0 ) {
    lcd.clear();
    
    lcd.setCursor(0, 1);
    lcd.print("OK : ");
    lcd.print(MenuComplet[0][MenuEtage1 - 1]);
    
    if (MenuEtage1 > 3 || MenuEtage1 < 0 ) lcd.print("Fail");
	SetCurrentEtage();
  }
  
}
void Select_Menu_Value_Etage2(){
  
  if (zVal == 0 ) {
    // on nettoit la 2eme ligne avant d'écrire
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("OK : ");
    lcd.print(MenuComplet[MenuEtage1][MenuEtage2 - 1]);
    
    if (MenuEtage2 > 3 || MenuEtage2 < 0 ) lcd.print("Fail");
	SetCurrentEtage();
	
  }
}
void setup_Menu(){
  lcd.begin(16, 2); 
  
  pinMode(zAxisPin, INPUT_PULLUP); // initialize the port to pull-up input 
  
  // création/initialisation des caractères byte(0) pour la fleche et byte(1) pour le carre vide
  lcd.createChar(0,Fleche_Haut);
  lcd.createChar(1,Carre_Vide);
  
 
  //création du menu principal la fleche commence sur le test
  lcd.setCursor(0, 0);   
  lcd.print(MenuComplet[0][0].substring(0,4)+"  "+MenuComplet[0][1].substring(0,4)+"  "+MenuComplet[0][2].substring(0,4));
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  Serial.begin(9600);
}
void SetupSecMenu() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(MenuComplet[MenuEtage1][0].substring(0, 4) + "  " + MenuComplet[MenuEtage1][1].substring(0, 4) + "  " + MenuComplet[MenuEtage1][2].substring(0, 4));
	lcd.setCursor(0, 1);
	lcd.write(byte(0));
}

void PrintDebug(){
  Serial.print("X: ");
  Serial.print(xVal);
  Serial.print("  Z: ");
  Serial.print(zVal);
  Serial.print("      MenuEtage1:  ");
  Serial.print(MenuEtage1);
  Serial.print("      MenuEtage2:  ");
  Serial.print(MenuEtage2);
  Serial.print("\n");
}

int get_X_Val(){
  return analogRead(xAxisPin);
}
int get_Z_Val(){
  return digitalRead(zAxisPin);
}
void SetFleche(int Etage) {
	lcd.setCursor(0, 1);
	lcd.print("                ");
	lcd.setCursor(0, 1);
	lcd.setCursor(((Etage - 1) * 6), 1);
	lcd.write(byte(0));
}

void ChoisirCouleur() {
	if (setup_Couleur_OK == LOW) {
		setup_Couleur();
		setup_Couleur_OK = HIGH;
	}
	Set_Couleur();
	select_Couleur();
}
void setup_Couleur() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(Couleur[0][0] + "  " + Couleur[0][1]);
	lcd.setCursor(0, 1);
	lcd.write(byte(0));
}
void Set_Couleur() {
	xVal = get_X_Val();
	color = Set_Menu_Value(color);
}
void select_Couleur() {
	zVal = get_Z_Val();
	if (zVal == 0) {
		lcd.clear();
		lcd.setCursor(0, 1);
		lcd.print("COULEUR =  ");
		lcd.print(Couleur[0][color-1]);
	
	COULEUR = color-1;
	Serial.print("COULEUR = " + COULEUR);
	delay(1000);
	lcd.clear();
	
	}
	SetChange();
}