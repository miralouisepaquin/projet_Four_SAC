 * 
  * @return int 0 : Si rien   NbreSec : Nbre de dixiemes de secondes que le bouton est appuye et relaché   Négatif:si le bouton est appuyé mais non relaché
 */
int MyButton::checkMyButton() {
    //Vérification si l'usager a appuyé sur le bouton 
    if (touchRead(buttonGPIO) < toucheButtonSensibilite) {
        if (!toucheButtonActif) {
            toucheButtonActif = true;
            toucheButtonActifStart = (unsigned long)(esp_timer_get_time()) / 100000;
            toucheButtonActifEnd = 0;
            return (0);
        }
        toucheButtonActifEnd = (unsigned long)(esp_timer_get_time()) / 100000;
        toucheButtonActifEnd -= toucheButtonActifStart;
        return (-toucheButtonActifEnd);
    }
    else {
        if(toucheButtonActif) {
            toucheButtonActif = false;
            return (toucheButtonActifEnd);
        }
    }
    return (0);
    }