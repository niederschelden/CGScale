function pfadZeichnen(){
    if (debug) console.log("Zeichne Pfad " + i );
    //ab 10 KG Gewicht soll ein grauer strich gezeichnet werden
    if( 0 < xalt && 0 < yalt && 9 < gewicht){
        if (debug) console.log(xalt + " " + yalt + "  nach " + xcoord + " " + ycoord);
        ctx.beginPath();
        ctx.lineWidth = 4;
        ctx.strokeStyle = 'grey';
        ctx.moveTo(xalt,yalt);
        ctx.lineTo(xcoord,ycoord);
        ctx.stroke();
        }
}

function punktZeichnen(){
    if (debug) console.log("Zeichne Punkt " + i );
    //ab 10 KG Gewicht soll ein roter Punkt gezeichnet werden
    if( 3 < xalt && 3 < yalt ){
        cty.clearRect((xalt - 4), (yalt - 4), 8, 8);
        }
    if (1 < xcoord && 1 < ycoord && 9 < gewicht){
        cty.beginPath();
        cty.fillStyle = 'red';
        cty.arc (xcoord, ycoord, 4, 0, 2 * Math.PI);
        cty.fill();
    }
}

async function readJson(){
    if (debug) console.log("Lese JSON " + i );
    //die funktion holt coords.json und wartet dann bis die Daten vorliegen
    const response = await fetch("coords.json");
    if (!response.ok) {
        const message = `An error has occured: ${response.status}`;
        throw new Error(message);
      }    
    const messwerte = await response.json();
    return messwerte;
}

function processMesswerte(messDaten) { 
        if (debug) console.log("Bearbeite Messwerte " + i );
        //messdaten in coordinaten umrechnen
        let VL = messDaten.VL; 
        let VR = messDaten.VR;  
        let HL = messDaten.HL;  
        let HR = messDaten.HR;
        
        //Werte für Simulation beim debuggen mit einer statischen coords.json
        if (debug) {
            HR = HR + (Math.random() * 6) - 3;
            VL = VL + (Math.random() * 6) - 3;
            VR = VR + (Math.random() * 6) - 3;
            HL = HL + (Math.random() * 6) - 3;
            }
        
        gewicht = VL + VR + HL + HR;
        let Hi = HL + HR;
        let Re = VR + HR;
        //den Punkt vom letzten Messwert sichern
        xalt = xcoord;
        yalt = ycoord;
        //den Punkt für den neuen Messwert sichern
        xcoord = Math.round(Re /  gewicht * breite);
        ycoord = Math.round(Hi / gewicht * hoehe);
        }

function anzeigeArt(){
    if (output == true)
        output = false;
    else if (output == false) {
        ctx.clearRect(0, 0, breite, hoehe);
        output = true;
    }

}