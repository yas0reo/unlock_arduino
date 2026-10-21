// Le code minimal

#include <Arduino.h>
#include <MicroOscSlip.h>
#include <Bounce2.h>
#include <Chrono.h>

MicroOscSlip<128> monOsc(&Serial);
Bounce2::Button bouton;
Chrono chronoPot;

void setup()
{
    Serial.begin(115200);
    bouton.attach(2, INPUT_PULLUP);
    bouton.setPressedState(LOW);
    pinMode(3, OUTPUT);
}

void loop()
{
    bouton.update();
    if (bouton.pressed())
    { // Bouton vient d’être appuyé
        monOsc.sendInt("/valider", 1);
    }
    if (chronoPot.hasPassed(20))
    {                        // SI LE CHRONO DÉPASSE 20 MILLISECONDES
        chronoPot.restart(); // REPARTIR LE CHRONO

        // LE 2 CI_BAS CORRESPOND À LA BROCHE ANALOGIQUE A2
        int valeur = analogRead(2); // LECTURE DE LA TENSION DE LA BROCHE A2 ENTRE 0 ET 1023

        monOsc.sendInt("/tourner", valeur); // ENVOYER LA VALEUR
    }
}

// FONCTION APPELÉE LORSQU’UN MESSAGE OSC EST REÇU
void traiterMessageOsc(MicroOscMessage &message)
{
    // SI L’ADRESSE DU MESSAGE EST "/lumiere" ET QUE LE MESSAGE CONTIENT UN ARGUMENT ENTIER ("i") :
    if (message.checkOscAddressAndTypeTags("/lumiere", "i"))
    {

        int valeur = message.nextAsInt(); // RÉCUPÉRER L’ENTIER ("i")

        digitalWrite(3, valeur); // ASSIGNER LA VALEUR A LA DEL
    }
}