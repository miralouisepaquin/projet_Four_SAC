/*
 * MyDHT.cpp
 *
 *  Created on: 20/04/2022
 *      Author: Mira Paquin
 */
#include "MyDHT.h"
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 17
#define DHTTYPE DHT22


MyDHT::MyDHT(){};
//Initialisation du DHT avec la pin utilisé et le type du DHT22.        
void MyDHT::init(){ 
    DHT dht(DHTPIN, DHTTYPE);
    dht.begin();
};
//Permet la lecture de la température en celsius        
float MyDHT::getTemperature(){ 
    DHT dht(DHTPIN, DHTTYPE);
    float temperature = dht.readTemperature();
    return temperature;
};


