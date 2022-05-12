/* Copyright (C) 2022 Alain Dube
 * All rights reserved.
 *
 * Projet Domotique
 * Ecole du Web
* Cours Systèmes embarqués (c)2022
 *  
    @file     MyDHT.h
    @author   Mira Paquin
    @version  1.1 20/04/2022 
    @description
      Utilisation du DHT22 pour lire la température

    platform = ESP32
    OS = Arduino
    Langage : C++

    Historique des versions
        Version    Date       Auteur       Description
        1.1        22/05/11  Mira Paquin   Première version du logiciel

**/
#define MYDHT_H

#define DHTPIN 17
#define DHTTYPE DHT22

class MyDHT {
    private:
        MyDHT *dht;
    public:
        MyDHT();
        ~MyDHT() { };
        void init();
        float getTemperature();
};