#include "Programme Principal.h"
#include "Homologation.h"
#include "Tests.h"
#include "Menu.h"

String Programme_a_lancer[3][3] = {
	{"Test_1M","Test_360_degres",""},
	{"homologation_statique_ferme","homologation_statique_ouvert","homologation_dynamique"},
	{"programme_principal_qualification","programme_principal_finale",""}
};
String Menu_str = "";
void setup() {
  // put your setup code here, to run once:
  setup_Menu();
  setup_Moteur();

}

void loop() {
	// put your main code here, to run repeatedly:
	if (MenuChoisi == LOW) {
		Set_Menu_Value();
		Select_Menu_Value();
	}

	if (MenuEtage1 == 3 && MenuChoisi) {
		if (COULEUR == -1) {
			ChoisirCouleur();
			Serial.print(COULEUR);
		}
	}
	delay(2000);
	// lancement du programme déterminé dans le menu avant. 
	switch (MenuEtage1) {
	case 1 : 
		if (MenuEtage2 == 1) { Test_1M(); }
		else if (MenuEtage2 == 2) { Test_360_degres(); }
		break;
	case 2 : 
		if (MenuEtage2 == 1) { homologation_statique_ferme(); }
		else if (MenuEtage2 == 2) { homologation_statique_ouvert(); }
		else { homologation_dynamique(); }
		break;
	case 3 : 
		if (MenuEtage2 == 1) { programme_principal_qualification(); }
		else if (MenuEtage2 == 2) { programme_principal_finale(); }
		break;
	}
	
}