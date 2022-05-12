/* Copyright (C) 2022 Alain Dube
 * All rights reserved.111
 *
 * Projet Stone SAC
 * Ecole du Web
 * Cours Systèmes embarqués (c)2022
 *
    @file     main.cpp
    @author   Alain Dubé
    @version  1.4 22/08/15
    @description
      Gestion de démarrage et automatisation d'un four pour séchage de bois.
      Utilisation du DHT22 pour gestion de la température.
      Si le bouton démarrer est appuyer, le four sèche selon les informations du type de bois entré.

    platform = ESP32
    OS : Arduino
    Langage : C++

    Historique des versions
        Version    Date       Auteur       Description
        1.1        22/08/15  Alain        Première version du logiciel
        1.2        22/04/29  Mira Paquin  Ajout d'un lecteur de température
        1.3        22/05/05  Mira Paquin  Ajout de la logique pour le four
        1.4        22/05/07  Mira Paquin  Ménage de code pour projet final

    Fonctionnalités implantées
        Lecture des evénements envoyés par l'écran
        Envoyer une commande à l'écran
        Lire la tempérture du DHT22 et l'afficher sur le MyStone
 * */

#include <Arduino.h>
#include "DHT.h"

#define RXD2 18
#define TXD2 19
#define BAUD_RATE 115200
int temperatureMin=22;//température min requise pour séchage
int tempsSechageMax=20;//temps de séchage du bois

#include <iostream>

#include "MyStone.h"
MyStone *myStone;

#include "MyDHT.h"
MyDHT *myDht = new MyDHT();
float temperature=0.0; //température du four

int tempsSechage=0; //temps de séchage du four
char bufferTemperature[20];//buffer pour la température
char bufferTemps[5];//buffer pour le temps de séchage
bool demarrage=0; //bool pour le démarrage
char boutonAppuyerIni[10];//buffer pour le nom du bouton appuyer
char bufferSechageMax[5];//buffer pour le temps de séchage du bois
char bufferTemperatureMin[5];//buffer pour la température min requise pour séchage

bool visible=true;
bool invisible=false;

std::string intToHexa(int value){
  char buffer[10];
  sprintf(buffer , "0x%4X", value);
  return (buffer);
};

std::string charToString(char str, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), str);
  return (buffer);
};

std::string intToString(int value, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), value);
  return (buffer);
};



//Thread qui permet de LOOPER et lire le contenu du port serie
//avec l'aide du la fonction getValidsDatasIfExists
void readStoneData() {
  datasRead rd = myStone->getValidsDatasIfExists();
  //std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
  switch(rd.id){

      case 0x0002: { //Version
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string stoneVersion = rd.name;
          std::cout << "Version : " <<  stoneVersion.c_str() << "\n";

          //std::this_thread::sleep_for(std::chrono::milliseconds(10));
          break;
          }
      case 0x1001: { //button
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string buttonName = rd.name;
          strcpy(boutonAppuyerIni,rd.name);
          std::cout << "button : " <<  buttonName.c_str() << "\n";
          //std::this_thread::sleep_for(std::chrono::milliseconds(10));
          break;
          }
      }

  if(rd.id<0) std::cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type<< ")\n";
}


void setup() {
  
  Serial.begin(9600);

  myDht->init();//Démarre la lecture du sensor de tempétature DHT22

  Serial.println("Stone serial Txd is on pin: "+String(TXD2));
  Serial.println("Stone serial Rxd is on pin: "+String(RXD2));
  myStone = new MyStone(115200, SERIAL_8N1, RXD2, TXD2);

  //affichage du splash pour 5 sec
  delay(2000);
  if(myStone)myStone->changePage("home_page");
  delay(2000);
  if(myStone)myStone->changePage("sac");
  //initialisation des label selon les données de temps et température
  myStone->setLabel("lblf01","Érable");
  myStone->setLabel("lblf02","0");
  sprintf(bufferSechageMax,"%d",tempsSechageMax);
  myStone->setLabel("lblf06",bufferSechageMax);
  sprintf(bufferTemperatureMin,"%d",temperatureMin);
  myStone->setLabel("lblf07",bufferTemperatureMin);
}

void loop() { 
  readStoneData();  

  //initialisation de mon bool demarrage si btn01 appuyer
  if(strcmp(boutonAppuyerIni,"btn01")==0){
    demarrage=1;
    myStone->setTextButton("btn01","Séchage en cours");
  }
      temperature = myDht->getTemperature();//lecture de la température
      
      if(demarrage == 1){     //démarre juste si btn01 appuyer                
          if(tempsSechage <= tempsSechageMax){            
            if((temperature >= (temperatureMin-(temperatureMin*1.1)))&&(temperature <= (temperatureMin+(temperatureMin*1.1)))){
                tempsSechage++; //ajout d'une seconde
            }
          }  
        //affichage du temps de séchage actuelle               
        sprintf(bufferTemps, "%02d", tempsSechage); 
        myStone->setLabel("lblf02",bufferTemps); 
        delay(500);   
        //réinitialisation des variable lorsque le temps de séchage est atteint
        if(tempsSechage >= tempsSechageMax){
                demarrage=0;
                tempsSechage=0;
                myStone->setTextButton("btn01","Démarrage");
                strcpy(boutonAppuyerIni,"");
          }              
      }
      //affichage de la température actuelle
      sprintf(bufferTemperature, "%2.1f cel", temperature);
      myStone->setLabel("lblf04",bufferTemperature);
      Serial.println(bufferTemperature);
      delay(500);      
  }