// Branchements
const byte BPCalibre = 2; // Bouton de calibrage
const byte HP[] = {3, 4}; //Haut-parleurs
const byte rec = 5; // Bouton record
const byte USTrig[] = {6,8}; // Capteur ultrasons 1
const byte USEcho[] = {7,9}; // Capteur ultrasons 1
// Variables
int rang[] = {0,0};
int rangp[] = {0,0};
int dmax[] = {500,500}; // distance maximale détectée par les capteurs
byte d = 75; // durée de base d'une note
// boucle :
byte boucle[250]; // tableau qui stocke la boucle
bool finboucle = 0; // se met à 1 une seule fois quand le bouton rec est relaché
bool boucleplein = 0;
byte index = 0; // index de la boucle
byte indexfinal = 250; // Derniere note de la boucle

int signal(byte n) {  // On définit une fonction qui mesure la distance à l'aide du capteur à Ultrasons....................................................
  digitalWrite(USTrig[n], HIGH);
  delayMicroseconds(10);
  digitalWrite(USTrig[n], LOW);
  unsigned long temps = pulseIn(USEcho[n],  HIGH);
  int distance = temps / 58.8235;
  return distance; // Renvoie une distance en Cm.
}
byte electro1(byte rang, byte HP) { // fonction pour jouer un son électrique aigu.........................................
  if (rang >= 90 ) {
    tone(HP, 659, d); // Mi 4
  }
  else if (rang >= 80) {
    tone(HP, 587, d); // Ré 4
  }
  else if (rang >= 70) {
    tone(HP, 523, d); // Do 3
  }
  else if (rang >= 60) {
    tone(HP, 494, d); // Si 3
  }
  else if (rang >= 50) {
    tone(HP, 440, d); // La 3
  }
  else if (rang >= 40) {
    tone(HP, 392, d); // Sol 3
  }
  else if (rang >= 30) { 
    tone(HP, 349, d); // Fa 3
  }
  else if (rang >= 20) {
    tone(HP, 330, d); // Mi 3
  }
  else if (rang >= 10) {
    tone(HP, 294, d); // Ré 3
  }
  else {
    tone(HP, 262, d); // Do 3
  }
  delay(10);
  return 0;
}
byte electro2(byte rang, byte HP) { // fonction pour jouer un son électrique grave.........................................
  if (rang >= 90 ) {
    tone(HP, 247, d); // Si 
  }
  else if (rang >= 80) {
    tone(HP, 220, d); // La
  }
  else if (rang >= 70) {
    tone(HP, 196, d); // Sol
  }
  else if (rang >= 60) {
    tone(HP, 175, d); // Fa
  }
  else if (rang >= 50) {
    tone(HP, 165, d); // Mi
  }
  else if (rang >= 40) {
    tone(HP, 147, d); // Ré
  }
  else if (rang >= 30) {
    tone(HP, 131, d); // Do
  }
  else if (rang >= 20) {
    tone(HP, 123, d); // Si
  }
  else if (rang >= 10) {
    tone(HP, 110, d); // La
  }
  else {
    tone(HP, 98, d); // Sol
  }
  delay(10);
  return 0;
}

void setup() {
  pinMode(BPCalibre, INPUT_PULLUP); // défini les boutons en "input"
  pinMode(rec, INPUT_PULLUP); // défini les boutons en "input"
  pinMode(HP[0], OUTPUT);// Définition des hauts parleurs
  pinMode(HP[1], OUTPUT);// Définition des hauts parleurs
  pinMode(USTrig[0], OUTPUT); // Trig1
  pinMode(USTrig[1], OUTPUT); // Trig1
  pinMode(USEcho[0], INPUT);// Echo 1
  pinMode(USEcho[1], INPUT);// Echo 2
  digitalWrite(USTrig[0], LOW);
  digitalWrite(USTrig[1], LOW);
  Serial.begin(9600);
  dmax[0] = signal(0) - 2; // Calibrage auto au démarrage
  dmax[1] = signal(1) - 2; // Calibrage auto au démarrage
  tone(HP[1], 523, 50);       // BIP
  delay(50);      // BIP
  tone(HP[1], 392, 50);      // BIP
  delay(50);      // BIP
  tone(HP[1], 330, 50);      // BIP
  delay(50);      // BIP
  tone(HP[1], 262, 100);      // BIP
  delay(100);      // BIP
  tone(HP[0], 330, 50);      // BIP
  delay(50);      // BIP
  tone(HP[0], 392, 50);      // BIP
  delay(50);      // BIP
  tone(HP[0], 523, 150);      // BIP
  delay(250);
}


void loop() {
  if (digitalRead(BPCalibre) == LOW) {    // Calibre la distance maximale.................................................................................
    for (byte n = 0 ; n < 2; n++ ) {
      dmax[n] = signal(n) - 2; // On enlève 2 cm à cette distance pour que le "mur" ne soit pas repéré.
      tone(HP[n], 800, 100); // "Bip"
      delay(100);
      Serial.println("Calibre a ");
      Serial.println(dmax[n]);
      memset(boucle,0,250);// vide la boucle
      boucleplein = 0; //défini la boucle comme vide
    }
  }
  int Distance[] = {signal(0), signal(1)} ; // On mesure la distance avec la fonction signal.......................................................................
  for (byte n = 0; n < 2; n++) {
    if ( Distance[n] >= dmax[n] ) { // Si la distance est supérieure à dmax, on ne joue aucun son
      Serial.print(n) ;
      Serial.print(" : Trop eloigne...");
      Serial.println(Distance[n]);
      rang[n] = 0;
      delay(10);
    }
    else { // Sinon, joue la note correspondante
      Serial.print("Distance = ");
      Serial.println(Distance[n]); //affiche la distance mesurée (en mètres)
      rang[n] = Distance[n] * 100 / dmax[n];
      if ((rang[n] - 5 <= rangp[n]) && (rang[n] + 5 >= rangp[n])) { // on compare la distance mesurée avec la distance précédente pour éviter les changements mineurs
        switch (n) {
          case 0 :
            electro1(rangp[n], HP[n]);
            break;
          case 1 :
            electro2(rangp[n], HP[n]);
            break ; 
        }
      }
      else {
        switch (n) {
          case 0 :
            electro1(rang[n], HP[n]);
            break;
          case 1 :
            electro2(rang[n], HP[n]);
            break ;
        }
        rangp[n] = rang[n]; // on enregistre le rang de cette distance en tant que rang précèdent pour pouvoir comparer les deux
      }
    }
    if (digitalRead(rec) == 0) { // On enregistre une boucle lorsque le bouton est appuyé
      if (finboucle == 0) { // s'exécute une fois au début de la boucle (quand elle n'est pas "en cours")
        index = 0;
      }
      boucle[index] = rang[0];
      Serial.print("REC :  [");
      Serial.print(index);
      Serial.print("] -> ");
      Serial.println(boucle[index]);
      index++;
      finboucle = 1; // la boucle est "en cours"
      boucleplein = 1; // la boucle contient qqch
      if (index == 250) { // Revient à zéro lorsque la boucle est à la dernière case.
        index = 0; 
        tone(HP[1],223,200); // Signal sonore
      }
    }
    else if (finboucle == 1) { // s'exécute une fois lorsque le bouton rec est relaché
      finboucle = 0; // la boucle n'est plus "en cours"
      indexfinal = index; // La case actuelle est définie comme "finale"
      index = 0; // On revient à la 1ère case
      Serial.print("INDEX FINAL de la boucle =");
      Serial.println(indexfinal);
    }
    else if (boucleplein == 1) { // Joue la boucle enregistrée (si elle existe)
      if (boucle[index] == 0) { 
        delay(20); // Joue les silences
      }
      else {
      electro1(boucle[index],HP[0]);
      }
      Serial.print("PLAY : [");
      Serial.print(index);
      Serial.print("] == ");
      Serial.println(boucle[index]);
      index ++;
      if (index > indexfinal) {
        index = 0; // Revient à zéro lorsque la boucle est à la dernière case.
      }
    }
  }
}